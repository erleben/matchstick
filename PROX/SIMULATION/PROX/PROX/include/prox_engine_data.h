#ifndef PROX_ENGINE_DATA_H
#define PROX_ENGINE_DATA_H

#include <prox_math_policy.h>
#include <prox_rigid_body.h>
#include <prox_contact_point.h>
#include <prox_matchstick_model.h>
#include <prox_params.h>
#include <prox_force_callbacks.h>
#include <narrow.h>
#include <broad.h>


#include <vector>
#include <map>
#include <cmath>
#include <cassert>

// 2018-12-08 Kenny: Hack to make sure we do not excede the allocated space in material table
#define m_number_of_materials 4u

namespace prox
{
  
	class EngineData
	{
  public:
    
    typedef prox::MathPolicy< float >          MT;
    typedef MT::tiny_types                     TT;
    typedef TT::vector3_type                   V;
    typedef TT::quaternion_type                Q;
    typedef TT::real_type                      T;
    typedef TT::value_traits                   VT;
    typedef TT::matrix3x3_type                 M;

    typedef prox::RigidBody< MT >              rigid_body_type;
    typedef prox::ContactPoint< MT >           contact_type;
    typedef narrow::Geometry< TT >             geometry_type;
    typedef prox::MatchStickModel< MT >        contact_model_type;
    typedef prox::Params< MT >                 params_type;
    typedef prox::ForceCallback<MT>            force_callback;

  public:
    
    typedef broad::System< T >              broad_phase_type;
    typedef narrow::System< TT >            narrow_phase_type;
    
    std::vector< std::string >       m_geometry_names;
    std::vector< std::string >       m_materials;
    std::vector< rigid_body_type  >  m_bodies;
    std::vector< contact_type >      m_contacts;

    broad_phase_type                 m_broad;
    narrow_phase_type                m_narrow;

    size_t                           m_contact_model_counter;
    
    std::vector< std::vector< contact_model_type > > m_contact_models;

    bool                             m_exist_model[m_number_of_materials][m_number_of_materials];
    float                            m_time_step;
    float                            m_time;      ///< Simulated time
    
    params_type                      m_params;

    prox::Gravity<MT>       m_gravity;
    prox::Damping<MT>       m_damping;


    std::vector< force_callback * >    m_force_callbacks;
    std::map< size_t, prox::Pin<MT> >  m_pin_forces;        ///< Container of pin forces. We on
                                                            ///< purpose use a map here to make
                                                            ///< sure pointers to elements stay
                                                            ///< the same when the data structure
                                                            ///< grows. Otherwise we can not use
                                                            ///< vectors of points to these elements.

    mesh_array::TetGenSettings m_tetgen_settings;           ///< Tetget settings

  public:

    class ScriptedMotion
    {
    public:

      /**
       * Get next free available scripted motion index.
       * This is essentially just a global counter allowing us to count the number of all scripted motions.
       *
       * @return   The next avialable unique index value.
       */
      static size_t & get_next_index()
      {
        static size_t value = 0;

        return value;
      }

    public:

      ScriptedMotion(){}
      virtual ~ScriptedMotion(){}

    public:

      size_t m_index;   ///< Unique index identifying this scriped motion.

      virtual void update(T const & time, rigid_body_type & body) = 0;

    };

    template<typename D>
    class Key
    {
    public:

      T m_time;
      D m_value;

    public:

      bool operator==(Key const & key) const
      {
        return m_time == key.m_time;
      }

      bool operator!=(Key const & key) const
      {
        return m_time != key.m_time;
      }

      bool operator<(Key const & key) const
      {
        return m_time < key.m_time;
      }

      bool operator>(Key const & key) const
      {
        return m_time > key.m_time;
      }

      bool operator<=(Key const & key) const
      {
        return m_time <= key.m_time;
      }

      bool operator>=(Key const & key) const
      {
        return m_time >= key.m_time;
      }

    };

    class KeyPosition    : public Key<V> {    };
    class KeyOrientation : public Key<Q> {    };

    class KeyframeMotion : public ScriptedMotion
    {
    public:

      std::vector< KeyPosition >    m_positions;
      std::vector< KeyOrientation > m_orientations;

      void add_position( T const & time, T const & x, T const & y, T const & z )
      {
        KeyPosition key;

        key.m_time  = time;
        key.m_value = V::make(x,y,z);

        m_positions.push_back(key);
        std::sort(m_positions.begin(), m_positions.end());
      }

      void add_orientation( T const & time, T const & qs, T const & qx, T const & qy, T const & qz )
      {
        KeyOrientation key;

        key.m_time  = time;
        key.m_value = Q(qs,qx,qy,qz);

        m_orientations.push_back(key);
        std::sort(m_orientations.begin(), m_orientations.end());
      }

    public:

      void update(T const & time, rigid_body_type & body)
      {
        if (! m_positions.empty())
        {
          size_t const N = m_positions.size();

          V p;
          V v;

          if( m_positions[0].m_time >= time )
          {
            p = m_positions[0].m_value;
            v = V::zero();
          }
          else if( m_positions[N-1].m_time <= time )
          {
            p = m_positions[N-1].m_value;
            v = V::zero();
          }
          else
          {
            size_t key = 1u;

            while (time >= m_positions[key].m_time)
            {
              ++key;
            }

            bool const valid_interval = (m_positions[key-1u].m_time <= time)  &&  (time < m_positions[key].m_time);

            assert( valid_interval || !"Keyframe::update(): could not find time span");

            T const    t = time -  m_positions[key-1u].m_time;
            T const   dt = m_positions[key].m_time -  m_positions[key-1u].m_time;

            V const & p0 = m_positions[key-1u].m_value;

            V const & p1 = m_positions[key].m_value;

            p =  (p1-p0)*(t/dt) + p0;

            v = (p1-p0)/dt;
          }

          body.set_position(p);
          body.set_velocity(v);
        }

        if (! m_orientations.empty())
        {
          size_t const N = m_orientations.size();

          V w;
          Q q;

          if( m_orientations[0].m_time >= time )
          {
            q = m_orientations[0].m_value;
            w = V::zero();
          }
          else if( m_orientations[N-1].m_time <= time )
          {
            q = m_orientations[N-1].m_value;
            w = V::zero();
          }
          else
          {
            size_t key = 1u;

            while (time >= m_orientations[key].m_time)
            {
              ++key;
            }

            bool const valid_interval = (m_orientations[key-1u].m_time <= time)  &&  (time < m_orientations[key].m_time);

            assert( valid_interval || !"Keyframe::update(): could not find time span");

            T const    t = time -  m_orientations[key-1u].m_time;
            T const   dt = m_orientations[key].m_time -  m_orientations[key-1u].m_time;

            Q const & q0 = m_orientations[key-1u].m_value;
            Q const & q1 = m_orientations[key].m_value;
            Q const q_rel  = tiny::prod( q1, tiny::conj(q0));

            T theta = VT::zero();
            V axis  = V::zero();
            tiny::get_axis_angle(q_rel, axis, theta);

            T const speed = theta/dt;
            
            w = axis*speed;
            q =  tiny::slerp(q0, q1, t/dt );
          }
          
          body.set_orientation(q);
          body.set_spin(w);
        }

      }
    };

    class OscillationMotion : public ScriptedMotion
    {
    public:

      T m_amplitude;
      T m_frequency;
      T m_phase;
      V m_direction;

      V m_origin;

    public:

      void update(T const & time, rigid_body_type & body)
      {
        using tiny::unit;
        using std::cos;
        using std::sin;

        V const p =  m_amplitude*sin(m_frequency*time + m_phase)*unit(m_direction) + m_origin;
        V const v =  m_amplitude*m_frequency*cos(m_frequency*time + m_phase)*unit(m_direction);

        body.set_position(p);
        body.set_velocity(v);
      }
      
    };


    std::vector< size_t >                m_all_scripted_bodies;     ///< A collection of all rigid bodies that have a scripted motion
    std::map<size_t, KeyframeMotion>     m_keyframe_motions;
    std::map<size_t, OscillationMotion>  m_oscillation_motions;
    std::map<size_t, ScriptedMotion * >  m_motion_callbacks;        ///< Maps body indices to a motion pointer

    ScriptedMotion * find_motion(size_t const & motion_idx)
    {
      assert( motion_idx < ScriptedMotion::get_next_index() || !"No motion exist with this index value" );

      std::map<size_t, KeyframeMotion>::iterator keyframe = this->m_keyframe_motions.find(motion_idx);

      if(keyframe != this->m_keyframe_motions.end() )
        return &(keyframe->second);

      std::map<size_t, OscillationMotion>::iterator oscillation = this->m_oscillation_motions.find(motion_idx);

      if(oscillation != this->m_oscillation_motions.end() )
        return &(oscillation->second);

      return 0;
    }

  public:
		
		EngineData();
		
		virtual ~EngineData();
		    
		void clear();
    
    void step_simulation(float const & dt);

    bool compute_raycast(
                         float const & p_x
                         , float const & p_y
                         , float const & p_z
                         , float const & ray_x
                         , float const & ray_y
                         , float const & ray_z
                         , size_t & body_index
                         , float & hit_x
                         , float & hit_y
                         , float & hit_z, float & distance
                         );

    void make_tetramesh_shape_from_surface(
                                 geometry_type & geometry
                                 , mesh_array::T3Mesh                                 const & surface
                                 , mesh_array::VertexAttribute<T, mesh_array::T3Mesh> const & surface_X
                                 , mesh_array::VertexAttribute<T, mesh_array::T3Mesh> const & surface_Y
                                 , mesh_array::VertexAttribute<T, mesh_array::T3Mesh> const & surface_Z
                                 );


    void get_total_energy( float & kinetic, float & potential);

	};

} //namespace prox

// PROX_ENGINE_DATA_H
#endif
