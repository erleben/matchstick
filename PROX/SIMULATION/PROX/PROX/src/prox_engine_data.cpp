#include <prox_engine_data.h>
#include <prox_compute_raycast.h>
#include <prox_update_inertia_tensor.h>

#include <util_profiling.h>
#include <util_string_helper.h>
#include <util_log.h>

#include <steppers/prox_bind_stepper.h>

#include <cassert>

namespace prox
{

  EngineData::EngineData()
  : m_geometry_names()
  , m_materials()
  , m_bodies()
  , m_contacts()
  , m_broad()
  , m_narrow()
  , m_contact_model_counter(0u)
  , m_time_step(0.01f)
  , m_time(0.0f)
  , m_params()
  , m_tetgen_settings(mesh_array::tetgen_quality_settings())
  , m_all_scripted_bodies()
  {
    clear();
  }

  EngineData::~EngineData()
  {
    clear();
  }

  void EngineData::clear()
  {
    m_bodies.clear();
    m_contacts.clear();
    m_broad.clear();
    m_narrow.clear();
    m_geometry_names.clear();
    m_materials.clear();
    m_contact_model_counter = 0u;
    m_time      = 0.0f;
    m_time_step = 0.01f;
    m_params = params_type();

    m_contact_models.resize(m_number_of_materials);

    for (size_t i = 0u; i < m_number_of_materials; ++i)
    {
      m_contact_models[i].resize(m_number_of_materials);

      for (size_t j = 0u; j < m_number_of_materials; ++j)
      {
        m_exist_model[i][j] = false;
      }
    }

    m_gravity = prox::Gravity<MT>();
    m_damping = prox::Damping<MT>();

    m_force_callbacks.clear();
    m_pin_forces.clear();

    m_all_scripted_bodies.clear();
    m_motion_callbacks.clear();
    m_keyframe_motions.clear();
    m_oscillation_motions.clear();

    util::Profiling::reset();
  }

  void EngineData::step_simulation(float const & dt)
  {
    typedef prox::StepperBinder< MT > stepper_binder_type;
    
    assert( dt>0.0f         || !"step_simulation(): invalid step size");
    assert( dt<=m_time_step || !"step_simulation(): invalid step size");
    
    stepper_binder_type stepper = prox::bind_stepper< MT >( m_params.stepper_params().stepper() );
    
    stepper( dt, m_bodies, m_contact_models, m_gravity, m_damping, m_params, m_broad, m_narrow, m_contacts, MT() );

    T E_kinetic;
    T E_potential;
    get_total_energy(E_kinetic, E_potential);

    RECORD("dt",   dt          );
    RECORD("Ekin", E_kinetic   );
    RECORD("Epot", E_potential );
  }

  bool EngineData::compute_raycast(
                                   float const & p_x
                                   , float const & p_y
                                   , float const & p_z
                                   , float const & ray_x
                                   , float const & ray_y
                                   , float const & ray_z
                                   , size_t & body_index
                                   , float & hit_x
                                   , float & hit_y
                                   , float & hit_z
                                   , float & hit_distance
                                   )
  {


    V const ray_origin    = V::make(p_x, p_y, p_z);
    V const ray_direction = V::make(ray_x, ray_y, ray_z);

    geometry::Ray<V> ray = geometry::make_ray( ray_origin, ray_direction );

    V point;
    T distance;

    bool const did_hit = prox::compute_raycast<EngineData::MT>(
                                                               ray
                                                               , m_bodies
                                                               , m_narrow
                                                               , body_index
                                                               , point
                                                               , distance
                                                               );

    if (did_hit)
    {
      hit_x        = point(0);
      hit_y        = point(1);
      hit_z        = point(2);
      hit_distance = distance;
    }

    return did_hit;
  }

  void EngineData::make_tetramesh_shape_from_surface(
                                                     geometry_type & geometry
                                                     , mesh_array::T3Mesh                                 const & surface_in
                                                     , mesh_array::VertexAttribute<T, mesh_array::T3Mesh> const & surface_X_in
                                                     , mesh_array::VertexAttribute<T, mesh_array::T3Mesh> const & surface_Y_in
                                                     , mesh_array::VertexAttribute<T, mesh_array::T3Mesh> const & surface_Z_in
                                                     )
  {
    mesh_array::T3Mesh                                 surface   = surface_in;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_X = surface_X_in;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_Y = surface_Y_in;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_Z = surface_Z_in;

    if(kdop::SelectContactPointAlgorithm::is_using_closest_point())
    {
      // 2018-12-27 Kenny code review: For volumetric defined objects we use
      // mesh_array::shrink for surface based defined shapes we use
      // normal-displacements. This is a little inconsistent, one "approach"
      // only should be used.
      std::vector<V> normals;

      mesh_array::compute_vertex_normals<MT>(surface, surface_X, surface_Y, surface_Z, normals);

      T const distance = VT::numeric_cast(-0.01);

      mesh_array::displace_vertices<MT>(surface, surface_X, surface_Y, surface_Z, distance, normals);
    }

    mesh_array::T4Mesh                                 volume;
    mesh_array::VertexAttribute<T, mesh_array::T4Mesh> volume_X;
    mesh_array::VertexAttribute<T, mesh_array::T4Mesh> volume_Y;
    mesh_array::VertexAttribute<T, mesh_array::T4Mesh> volume_Z;

    mesh_array::tetgen(surface
                       , surface_X
                       , surface_Y
                       , surface_Z
                       , volume
                       , volume_X
                       , volume_Y
                       , volume_Z
                       , m_tetgen_settings
                       );

    geometry.set_shape(
                       volume
                       , volume_X
                       , volume_Y
                       , volume_Z
                       );

  }

  void EngineData::get_total_energy( float & kinetic, float & potential)
  {
    kinetic   = 0.0f;
    potential = 0.0f;

    for (size_t i = 1u; i < m_bodies.size(); ++i)
    {
      rigid_body_type const & body = m_bodies[i];

      if(body.is_fixed())
        continue;

      // 2016-06-21 Kenny code review: Hmm, should scripted bodies be
      //            ignore? They can move around and when colliding with
      //            free bodies they will transfer energy. Hence, they
      //            work like an "external" force on the system right now.
      if(body.is_scripted())
        continue;

      float const m  = body.get_mass();
      float const h  = tiny::inner_prod( m_gravity.up(), body.get_position());
      float const v  = tiny::norm( body.get_velocity());

      V const & w    = body.get_spin();
      M const & I_bf = body.get_inertia_bf();
      M const R      = tiny::make( body.get_orientation() );

      M I;
      prox::detail::update_inertia_tensor<MT>( R, I_bf, I );

      T const wIw    = tiny::inner_prod( w , I*w );

      kinetic       += 0.5*(m*v*v + wIw);
      potential     += m_gravity.acceleration() * m * h;

    }
  }

} //namespace prox
