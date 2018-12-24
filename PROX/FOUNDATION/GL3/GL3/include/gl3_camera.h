#ifndef GL3_CAMERA_H
#define GL3_CAMERA_H

#include <gl3_trackball.h>

#include <gl3_glm.h>

namespace gl3
{

  /**
   * Camera Class.
   * The camera class provides functionality for computing the coordinate
   * transformation that brings you from your world space into your local
   * camera coordinate frame.
   */
  class Camera
  : public gl3::Trackball
  {
  public:

    typedef gl3::Trackball base_class;

  protected:

    glm::vec3  m_position;    ///< Viewer Position (Placement of Camera in Model Space.
    glm::vec3  m_target;      ///< Viewer Target, the spot in the model space we are looking at.
    glm::vec3  m_up;          ///< Viwer Up, the direction in model space that is upwards.
    glm::vec3  m_dof;         ///< Direction of Flight, i.e. the direction in model space we are moving forward in....
    glm::vec3  m_right;       ///< Direction to the right in model space.

  protected:

    bool m_orbit_mode;      ///< Boolean flag indicating whether camera is orbiting or rotation duing a mouse move.
    bool m_target_locked;   ///< Boolean flag indicating whether target positions is locked (ie. we allways look at the same spot)
    glm::vec3 m_tmp_position;    ///< Temporaries used for handling trackball.
    glm::vec3 m_tmp_target;      ///< Temporaries used for handling trackball.
    glm::vec3 m_tmp_up;          ///< Temporaries used for handling trackball.

  public:

    glm::vec3 const & up()            const { return m_up;            }
    glm::vec3 const & target()        const { return m_target;        }
    glm::vec3 const & position()      const { return m_position;      }
    glm::vec3 const & right()         const { return m_right;         }
    glm::vec3 const &  dof()          const { return m_dof;           }
    bool            & orbit_mode()          { return m_orbit_mode;    }
    bool      const & orbit_mode()    const { return m_orbit_mode;    }
    bool            & target_locked()       { return m_target_locked; }
    bool      const & target_locked() const { return m_target_locked; }

  public:

    Camera()
    : m_orbit_mode(true)
    , m_target_locked(false)
    {
      init( glm::vec3(0,0,40), glm::vec3(0,0,0), glm::vec3(0,1,0) );
    }

    void init(glm::vec3 const & position, glm::vec3 const & target, glm::vec3 const & up)
    {
      m_position = position;
      m_target   = target;
      m_up       = up;

      //--- First determine transform from local camera frame to world
      //--- coordinate system, i.e.
      //---
      //---     H = | R T |  =   | X Y Z T |=   | right  up' -dof position |
      //---         | 0 1 |      | 0 0 0 1 |    |   0     0    0    1      |
      //---
      //---
      //---   |q|       |p|
      //---   |1|  =  H |1|    or    q = R*p + T
      //---
      m_dof    = glm::normalize( m_target - m_position);
      m_right  = glm::normalize( glm::cross( m_dof , m_up) );
      m_up     = glm::cross( m_right, m_dof);
    }

    float depth(glm::vec3 const & r) const
    {
      return glm::dot( m_dof , (r - m_position) );
    }

    /**
     *
     * @param R  rotation in local camera space
     */
    void rotate(glm::mat4 const & Rc)
    {
      glm::mat4 Rc2w;  // cam space to world space

      Rc2w[0] = glm::vec4( m_right, 0.0f );
      Rc2w[1] = glm::vec4( m_up,    0.0f );
      Rc2w[2] = glm::vec4(-m_dof,   0.0f );
      Rc2w[3] = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );

      glm::mat4 const Rw  = Rc2w * Rc * glm::transpose(Rc2w);

      if(! m_target_locked)
      {
        m_target = glm::vec3(  Rw*glm::vec4((m_target - m_position), 0.0f) ) + m_position;
      }
      m_up = glm::vec3( Rw * glm::vec4(m_up, 0.0f) );

      init(m_position, m_target, m_up);
    }

    void orbit(glm::mat4 const & Rc)
    {
      glm::mat4 Rc2w;  // cam space to world space

      Rc2w[0] = glm::vec4( m_right, 0.0f );
      Rc2w[1] = glm::vec4( m_up,    0.0f );
      Rc2w[2] = glm::vec4(-m_dof,   0.0f );
      Rc2w[3] = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );

      glm::mat4 const Rw  = Rc2w * Rc * glm::transpose(Rc2w);

      m_position =  glm::vec3( Rw*glm::vec4((m_position - m_target),0.0f) ) + m_target;
      m_up       =  glm::vec3( Rw*glm::vec4(m_up,0.0f) );

      init(m_position, m_target, m_up);
    }

    void pan(float const & x, float const & y)
    {
      glm::vec3 const panx = x * m_right;
      glm::vec3 const pany = y * glm::normalize( glm::cross( m_right, m_dof) );

      m_position += panx;
      m_position += pany;

      if(!m_target_locked )
      {
        m_target   += panx;
        m_target   += pany;
      }

      init(m_position, m_target, m_up);
    }

    void dolly(float const & distance)
    {
      m_position += m_dof*distance;

      if( !m_target_locked )
        m_target += m_dof*distance;

      init(m_position, m_target, m_up);
    }

    void mouse_down(float const &  sx, float const & sy)
    {
      m_tmp_position = m_position;
      m_tmp_target   = m_target;
      m_tmp_up       = m_up;
      base_class::mouse_down(sx,sy);
    }

    void mouse_up(float const &  sx, float const & sy)
    {
      base_class::mouse_up(sx,sy);

      init(m_tmp_position, m_tmp_target, m_tmp_up);

      if(m_orbit_mode)
        orbit(  glm::transpose( base_class::get_rotation_matrix() ) );
      else
        rotate( base_class::get_rotation_matrix() );
    }

    void mouse_move(float const &  sx, float const & sy)
    {
      base_class::mouse_move(sx,sy);

      init(m_tmp_position, m_tmp_target, m_tmp_up);

      if(m_orbit_mode)
        orbit(  glm::transpose( base_class::get_rotation_matrix() ) );
      else
        rotate( base_class::get_rotation_matrix() );
    }

  };

} // namespace gl3

// GL3_CAMERA_H
#endif
