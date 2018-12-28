#include <prox_engine.h>
#include <prox_engine_data.h>
#include <prox_compute_structure_map_constant.h>
#include <prox_compute_structure_map_from_rotational_sweep.h>

#include <util_string_helper.h>
#include <util_config_file.h>
#include <util_log.h>

#include <mesh_array.h>

namespace prox
{

  typedef tiny::MathTypes<float> MT;
  typedef MT::vector3_type       V;
  typedef MT::quaternion_type    Q;
  typedef MT::value_traits       VT;
  typedef MT::real_type          T;
  
  Engine::Engine()
  {
    m_data = new EngineData();
    
    assert( m_data || !"Engine(): Internal error, null pointer");
  }
  
  Engine::Engine(
                 std::string const & solver
                 , std::string const & normal_solver
                 , std::string const & friction_solver
                 , std::string const & time_stepper
                 , std::string const & r_factor_strategy
                 )
  {
    m_data = new EngineData();
    
    assert( m_data || !"Engine(): internal error, null pointer");
    
    set_parameter(PARAM_SOLVER,          solver             );
    set_parameter(PARAM_NORMAL_SOLVER,   normal_solver      );
    set_parameter(PARAM_FRICTION_SOLVER, friction_solver    );
    set_parameter(PARAM_TIME_STEPPER,    time_stepper       );
    set_parameter(PARAM_R_FACTOR,        r_factor_strategy  );
  }
  
  Engine::~Engine()
  {
    if (m_data)
    {
      m_data->clear();
      delete m_data;
    }
  }
  
  bool Engine::simulate( float const & dt )
  {
    using std::min;
    
    assert( m_data    || !"Engine::simulate(): Internal error, null pointer");
    assert( dt > 0.0f || !"Engine::simulate(): It does not make sense to take zero time step");
    
    
    float dt_left     = dt;
    float script_time = m_data->m_time;
    
    while(dt_left > 0.0f)
    {
      float const ddt = min( m_data->m_time_step, dt_left );
      
      script_time += ddt;
      for (size_t i = 0u; i < m_data->m_all_scripted_bodies.size(); ++i)
      {
        
        size_t const body_idx = m_data->m_all_scripted_bodies[i];
        
        EngineData::rigid_body_type & body = m_data->m_bodies[body_idx];
        
        EngineData::ScriptedMotion * motion = m_data->m_motion_callbacks.at(body_idx);
        
        motion->update(script_time, body);
      }
      
      m_data->step_simulation( ddt );
      
      dt_left -= ddt;
    }
    
    m_data->m_time += dt;
    
    return true;
  }
  
  void Engine::clear()
  {
    assert( m_data || !"Engine::clear(): Internal error, null pointer");
    if (m_data)
      m_data->clear();
  }

  size_t Engine::create_rigid_body( std::string const & name )
  {
    assert( m_data || !"Engine::create_rigid_body(): Internal error, null pointer");
    EngineData::rigid_body_type B = EngineData::rigid_body_type();
    B.set_name( name );
    m_data->m_bodies.push_back(B);
    return m_data->m_bodies.size()-1;
  }
  
  void Engine::set_rigid_body_position(  size_t const & body_idx
                                       , float const & x
                                       , float const & y
                                       , float const & z
                                       )
  {
    assert( m_data                                         || !"Engine::set_rigid_body_position(): Null pointer");
    assert( body_idx < m_data->m_bodies.size()             || !"Engine::set_rigid_body_position(): No such rigid body");
    assert( (is_number(x) && is_number(y) && is_number(z)) || !"Engine::set_rigid_body_position(): NaN or inf value");
    
    m_data->m_bodies[ body_idx ].set_position( EngineData::V::make( x, y, z ) );
  }
  
  void Engine::set_rigid_body_orientation(  size_t const & body_idx
                                          , float const & Qs
                                          , float const & Qx
                                          , float const & Qy
                                          , float const & Qz
                                          )
  {
    assert( m_data || !"Engine::set_rigid_body_orientation(): Internal error: null pointer");
    assert( body_idx < m_data->m_bodies.size() || !"Engine::set_rigid_body_orientation(): Internal error: no such rigid body");
    assert( (is_number(Qs) && is_number(Qx) && is_number(Qy) && is_number(Qz)) || !"Engine::set_rigid_body_orientation(): Internal error: NaN or inf value");
    
    EngineData::MT::quaternion_type Q;
    
    Q.real()    = Qs;
    Q.imag()(0) = Qx;
    Q.imag()(1) = Qy;
    Q.imag()(2) = Qz;
    
    m_data->m_bodies[ body_idx ].set_orientation( Q );
  }
  
  void Engine::set_rigid_body_velocity(
                                       size_t const & body_idx
                                       , float const & vx
                                       , float const & vy
                                       , float const & vz
                                       )
  {
    assert( m_data || !"Engine::set_rigid_body_velocity(): Internal error: null pointer");
    assert( body_idx < m_data->m_bodies.size() || !"Engine::set_rigid_body_velocity(): Internal error: no such rigid body");
    assert( (is_number(vx) && is_number(vy) && is_number(vz)) || !"Engine::set_rigid_body_velocity(): Internal error: NaN or inf value");
    
    m_data->m_bodies[ body_idx ].set_velocity( EngineData::V::make( vx, vy, vz ) );
  }
  
  void Engine::set_rigid_body_spin(
                                   size_t const & body_idx
                                   , float const & wx
                                   , float const & wy
                                   , float const & wz
                                   )
  {
    assert( m_data || !"Engine::set_rigid_body_spin(): Internal error: null pointer");
    assert( body_idx < m_data->m_bodies.size() || !"Engine::set_rigid_body_spin(): Internal error: no such rigid body");
    assert( (is_number(wx) && is_number(wy) && is_number(wz)) || !"Engine::set_rigid_body_spin(): Internal error: NaN or inf value");
    
    m_data->m_bodies[ body_idx ].set_spin( EngineData::V::make( wx, wy, wz ) );
  }
  
  void Engine::set_rigid_body_mass( size_t const & body_idx, float const & mass)
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_idx < m_data->m_bodies.size() || !"internal error: no such rigid body");
    assert( is_number(mass) || !"internal error: NaN or inf value");
    
    m_data->m_bodies[ body_idx ].set_mass( mass );
  }
  
  void Engine::set_rigid_body_inertia(
                                      size_t const & body_idx
                                      , float const & Ixx
                                      , float const & Iyy
                                      , float const & Izz
                                      )
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_idx < m_data->m_bodies.size() || !"internal error: no such rigid body");
    assert( (is_number(Ixx) && is_number(Iyy) && is_number(Izz)) || !"internal error: NaN or inf value");
    assert( ((0 <= Ixx) && (0 <= Iyy) && (0 <= Izz)) || !"internal error: negative value");
    
    EngineData::MT::matrix3x3_type I = EngineData::MT::matrix3x3_type::make(
                                                                            Ixx, 0.0f, 0.0f,
                                                                            0.0f, Iyy, 0.0f,
                                                                            0.0f, 0.0f, Izz
                                                                            );
    
    m_data->m_bodies[ body_idx ].set_inertia_bf( I );
  }
  
  void Engine::set_rigid_body_active( size_t const & body_idx, bool const & active )
  {
    //not used
  }
  
  void Engine::set_rigid_body_fixed( size_t const & body_idx, bool const & fixed )
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_idx < m_data->m_bodies.size() || !"internal error: no such rigid body");
    
    m_data->m_bodies[ body_idx ].set_fixed( fixed );
  }
  
  void Engine::set_rigid_body_material( size_t const & body_idx, size_t const & material_idx)
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_idx < m_data->m_bodies.size() || !"internal error: no such rigid body");
    
    m_data->m_bodies[ body_idx ].set_material_idx( material_idx );
  }
  
  void Engine::connect_collision_geometry(size_t body_idx, size_t geometry_index )
  {
    assert( m_data                             || !"internal error: null pointer");
    assert( body_idx < m_data->m_bodies.size() || !"internal error: no such rigid body");
    
    EngineData::rigid_body_type & body     = m_data->m_bodies[ body_idx ];
    EngineData::geometry_type   & geometry = m_data->m_narrow.get_geometry(geometry_index);
    
    body.set_geometry_idx( geometry_index );
    
    if( geometry.has_shape())
    {
      narrow::make_kdop_bvh( m_data->m_narrow.params(), body, geometry);
    }
  }
  
  size_t Engine::create_material( std::string const & name )
  {
    
    assert( m_data || !"internal error: null pointer");
    m_data->m_materials.push_back( name );
    size_t idx = m_data->m_materials.size() - 1u;
    
    // 2018-12-08 Kenny: m_number_of_materials is a #define in prox_engine_data.h
    assert( idx < m_number_of_materials || !"internal error: more materials created than is allocated space for in material table");
    return idx;
  }
  
  void Engine::create_material_property(  size_t const & first_idx
                                        , size_t const & second_idx)
  {
    assert( m_data || !"internal error: null pointer");
    assert( first_idx  < m_number_of_materials || !"internal error: index excedes allocated space for in material table");
    assert( second_idx < m_number_of_materials || !"internal error: index excedes allocated space for in material table");
    
    
    bool exist = m_data->m_exist_property[first_idx][second_idx];
    
    assert( !exist || !"internal error: material was already created?");
    
    EngineData::property_type P = EngineData::property_type();
    
    m_data->m_properties[first_idx][second_idx] = P;
    m_data->m_properties[second_idx][first_idx] = P;
    
    m_data->m_exist_property[first_idx][second_idx] = true;
    
    m_data->m_property_counter++;
  }

  void Engine::set_coefficients_of_anisotropic_friction(
                                                        size_t const & first_idx
                                                        , size_t const & second_idx
                                                        , float const & mu_x
                                                        , float const & mu_y
                                                        , float const & mu_z
                                                        )
  {
    assert( m_data || !"internal error: null pointer");
    assert( first_idx  < m_number_of_materials || !"internal error: index excedes allocated space for in material table");
    assert( second_idx < m_number_of_materials || !"internal error: index excedes allocated space for in material table");
    assert( (is_number(mu_x) && is_number(mu_y) && is_number(mu_z)) || !"internal error: NaN or inf value");
    
    m_data->m_properties[first_idx][second_idx].set_coefficients_of_anisotropic_friction( mu_x, mu_y, mu_z );
    m_data->m_properties[second_idx][first_idx].set_coefficients_of_anisotropic_friction( mu_x, mu_y, mu_z );
  }

  void Engine::set_coefficient_of_isotropic_friction(
                                                     size_t const & first_idx
                                                     , size_t const & second_idx
                                                     , float const & mu
                                                     )
  {
    assert( m_data || !"internal error: null pointer");
    assert( first_idx  < m_number_of_materials || !"internal error: index excedes allocated space for in material table");
    assert( second_idx < m_number_of_materials || !"internal error: index excedes allocated space for in material table");
    assert( is_number(mu) || !"internal error: NaN or inf value");

    m_data->m_properties[first_idx][second_idx].set_coefficient_of_isotropic_friction( mu );
    m_data->m_properties[second_idx][first_idx].set_coefficient_of_isotropic_friction( mu );
  }

  void Engine::set_material_structure_map(
                                          size_t const & body_idx
                                          , float const & rot_x
                                          , float const & rot_y
                                          , float const & rot_z
                                          , float const & ref_x
                                          , float const & ref_y
                                          , float const & ref_z
                                          , float const & dir_x
                                          , float const & dir_y
                                          , float const & dir_z
                                          )
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_idx  < m_data->m_bodies.size() || !"internal error: index excedes allocated space for in material table");
    assert( (is_number(dir_x) && is_number(dir_y) && is_number(dir_z)) || !"internal error: NaN or inf value");


    EngineData::rigid_body_type       & body     = m_data->m_bodies[ body_idx ];
    size_t                      const & geo_idx  = body.get_geometry_idx();
    EngineData::geometry_type         & geometry = m_data->m_narrow.get_geometry(geo_idx);
    mesh_array::T4Mesh          const & mesh     = geometry.m_mesh;

    mesh_array::VertexAttribute<V,mesh_array::T4Mesh> structure_map;

    V const n = tiny::unit( V::make(rot_x, rot_y, rot_z) );
    V const r = tiny::unit( V::make(ref_x, ref_y, ref_z) );
    V const s = tiny::unit( V::make(dir_x, dir_y, dir_z) );

    compute_structure_map_from_rotational_sweep<MT>(
                                                    mesh
                                                    , geometry.m_X0
                                                    , geometry.m_Y0
                                                    , geometry.m_Z0
                                                    , n   // rotation axis
                                                    , r   // reference point
                                                    , s   // structure direction at reference point
                                                    , structure_map
                                                    );

    body.set_structure_map(structure_map);



  }

  void Engine::set_material_structure_map(
                                          size_t const & body_idx
                                          , float const & dir_x
                                          , float const & dir_y
                                          , float const & dir_z
                                          )
  {
    assert( m_data                                                     || !"internal error: null pointer"                                       );
    assert( body_idx  < m_data->m_bodies.size()                        || !"internal error: index excedes allocated space for in material table");
    assert( (is_number(dir_x) && is_number(dir_y) && is_number(dir_z)) || !"internal error: NaN or inf value"                                   );

    EngineData::rigid_body_type       & body     = m_data->m_bodies[ body_idx ];
    size_t                      const & geo_idx  = body.get_geometry_idx();
    EngineData::geometry_type         & geometry = m_data->m_narrow.get_geometry(geo_idx);
    mesh_array::T4Mesh          const & mesh     = geometry.m_mesh;

    mesh_array::VertexAttribute<V,mesh_array::T4Mesh> structure_map;

    V const s = tiny::unit( V::make(dir_x, dir_y, dir_z) );

    compute_structure_map_constant<MT>(
                                       mesh
                                       , geometry.m_X0
                                       , geometry.m_Y0
                                       , geometry.m_Z0
                                       , s
                                       , structure_map
                                       );
    body.set_structure_map(structure_map);
  }

  void Engine::get_material_structure_map_size(size_t const & body_idx, size_t & N)
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_idx  < m_data->m_bodies.size() || !"internal error: index excedes allocated space for in material table");

    EngineData::rigid_body_type       & body     = m_data->m_bodies[ body_idx ];
    mesh_array::VertexAttribute<V,mesh_array::T4Mesh> const & structure_map = body.m_structure_map;
    N = structure_map.size();
  }

  void Engine::get_material_structure_map_data(size_t const & body_idx, size_t * indices, float *  data)
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_idx  < m_data->m_bodies.size() || !"internal error: index excedes allocated space for in material table");

    EngineData::rigid_body_type       & body     = m_data->m_bodies[ body_idx ];
    size_t                      const & geo_idx  = body.get_geometry_idx();
    EngineData::geometry_type         & geometry = m_data->m_narrow.get_geometry(geo_idx);
    mesh_array::T4Mesh          const & mesh     = geometry.m_mesh;

    mesh_array::VertexAttribute<V,mesh_array::T4Mesh> const & structure_map = body.m_structure_map;
    for ( size_t idx = 0u; idx < mesh.vertex_size(); ++idx)
    {
      mesh_array::Vertex const vertex = mesh.vertex( idx );
      V const & s = structure_map(vertex);
      indices[idx]      = vertex.idx();
      data[idx*3u     ] = s(0);
      data[idx*3u + 1u] = s(1);
      data[idx*3u + 2u] = s(2);
    }
  }

  void Engine::set_material_structure_map(size_t const & body_idx, size_t const & N, size_t * indices, float *  data)
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_idx  < m_data->m_bodies.size() || !"internal error: index excedes allocated space for in material table");

    EngineData::rigid_body_type       & body     = m_data->m_bodies[ body_idx ];
    size_t                      const & geo_idx  = body.get_geometry_idx();
    EngineData::geometry_type         & geometry = m_data->m_narrow.get_geometry(geo_idx);
    mesh_array::T4Mesh          const & mesh     = geometry.m_mesh;

    mesh_array::VertexAttribute<V,mesh_array::T4Mesh> structure_map;

    structure_map.bind(mesh);

    for ( size_t i = 0u; i < N; ++i)
    {
      size_t const idx = indices[i];
      float const & sx = data[idx*3u     ];
      float const & sy = data[idx*3u + 1u];
      float const & sz = data[idx*3u + 2u];
      mesh_array::Vertex const vertex = mesh.vertex( idx );
      structure_map(vertex) = V::make(sx,sy,sz);
    }

  }

  void Engine::set_coefficient_of_restitution(  size_t const & first_idx
                                              , size_t const & second_idx
                                              , float const & e
                                              )
  {
    assert( m_data || !"internal error: null pointer");
    assert( first_idx  < m_number_of_materials || !"internal error: index excedes allocated space for in material table");
    assert( second_idx < m_number_of_materials || !"internal error: index excedes allocated space for in material table");
    assert( is_number(e) || !"internal error: NaN or inf value");
    
    m_data->m_properties[first_idx][second_idx].set_coefficient_of_restitution( e );
    m_data->m_properties[second_idx][first_idx].set_coefficient_of_restitution( e );
  }
  /// materials
  
  /// collision geometries
  size_t Engine::create_collision_geometry( std::string const & name )
  {
    assert( m_data || !"internal error: null pointer");
    
    size_t const gid = m_data->m_narrow.create_geometry();

    m_data->m_geometry_names.push_back( name );
    
    return gid;
  }

  void Engine::set_box_shape(  size_t const & geometry_index
                             , float const & width
                             , float const & height
                             , float const & depth
                             )
  {
    assert( m_data || !"internal error: null pointer");
    assert( (is_number(width) && is_number(height) && is_number(depth)) || !"internal error: NaN or inf value");
    
    EngineData::geometry_type & geometry = m_data->m_narrow.get_geometry(geometry_index);
    
    mesh_array::T3Mesh                                 surface;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_X;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_Y;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_Z;

    mesh_array::make_box<MT>(
                             width
                             , height
                             , depth
                             , surface
                             , surface_X
                             , surface_Y
                             , surface_Z
                             );

    m_data->make_tetramesh_shape_from_surface(
                                              geometry
                                              , surface
                                              , surface_X
                                              , surface_Y
                                              , surface_Z
                                              );

  }

  void Engine::set_capsule_shape(  size_t const & geometry_index
                                 , float const & radius
                                 , float const & height
                                 )
  {
    assert( m_data      || !"internal error: null pointer");
    assert( (is_number(radius) && is_number(height)) || !"internal error: NaN or inf value");
    
    EngineData::geometry_type & geometry = m_data->m_narrow.get_geometry(geometry_index);
    
    mesh_array::T3Mesh                                 surface;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_X;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_Y;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_Z;
    
    size_t const segments = 12u;
    size_t const slices   = 12u;
    
    mesh_array::make_capsule<MT>(
                                 radius
                                 , height
                                 , slices
                                 , segments
                                 , surface
                                 , surface_X
                                 , surface_Y
                                 , surface_Z
                                 );
    
    m_data->make_tetramesh_shape_from_surface(
                                              geometry
                                              , surface
                                              , surface_X
                                              , surface_Y
                                              , surface_Z
                                              );
    
  }

  void Engine::set_cone_shape(  size_t const & geometry_index
                              , float const & radius
                              , float const & height
                              )
  {
    assert( m_data      || !"internal error: null pointer");
    assert( (is_number(radius) && is_number(height)) || !"internal error: NaN or inf value");
    
    EngineData::geometry_type & geometry = m_data->m_narrow.get_geometry(geometry_index);
    
    mesh_array::T3Mesh                                 surface;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_X;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_Y;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_Z;
    
    size_t const slices = 12u;
    
    mesh_array::make_cone<MT>(
                              radius
                              , height
                              , slices
                              , surface
                              , surface_X
                              , surface_Y
                              , surface_Z
                              );
    
    m_data->make_tetramesh_shape_from_surface(
                                              geometry
                                              , surface
                                              , surface_X
                                              , surface_Y
                                              , surface_Z
                                              );
  }

  void Engine::set_convex_shape(  size_t const & geometry_index
                                , size_t const & N
                                , float const * coordinates
                                )
  {
    assert( m_data      || !"internal error: null pointer");
    assert( coordinates || !"internal error: null pointer");
    
    EngineData::geometry_type & geometry = m_data->m_narrow.get_geometry(geometry_index);
    
    std::vector<V> p;
    p.resize(N);
    for(size_t i = 0u; i < N; ++i)
    {
      p[i](0) = coordinates[3u*i+0u];
      p[i](1) = coordinates[3u*i+1u];
      p[i](2) = coordinates[3u*i+2u];
    }
    

    mesh_array::T3Mesh                                 surface;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_X;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_Y;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_Z;

    mesh_array::make_convex<MT>(p
                                , surface
                                , surface_X
                                , surface_Y
                                , surface_Z
                                );


    m_data->make_tetramesh_shape_from_surface(
                                              geometry
                                              , surface
                                              , surface_X
                                              , surface_Y
                                              , surface_Z
                                              );
  }

  void Engine::set_cylinder_shape(  size_t const & geometry_index
                                  , float const & radius
                                  , float const & height
                                  )
  {
    assert( m_data      || !"internal error: null pointer");
    assert( (is_number(radius) && is_number(height)) || !"internal error: NaN or inf value");
    
    EngineData::geometry_type & geometry = m_data->m_narrow.get_geometry(geometry_index);
    
    mesh_array::T3Mesh                                 surface;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_X;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_Y;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_Z;
    
    size_t const slices = 20u;
    
    mesh_array::make_cylinder<MT>(
                                  radius
                                  , height
                                  , slices
                                  , surface
                                  , surface_X
                                  , surface_Y
                                  , surface_Z
                                  );
    
    m_data->make_tetramesh_shape_from_surface(
                                              geometry
                                              , surface
                                              , surface_X
                                              , surface_Y
                                              , surface_Z
                                              );
  }

  void Engine::set_ellipsoid_shape(  size_t const & geometry_index
                                   , float const & sx
                                   , float const & sy
                                   , float const & sz
                                   )
  {
    
    assert( m_data      || !"internal error: null pointer");
    assert( (is_number(sx) && is_number(sy) && is_number(sz)) || !"internal error: NaN or inf value");
    
    EngineData::geometry_type & geometry = m_data->m_narrow.get_geometry(geometry_index);
    
    mesh_array::T3Mesh                                 surface;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_X;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_Y;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_Z;
    
    size_t const segments = 12u;
    size_t const slices   = 12u;
    
    mesh_array::make_ellipsoid<MT>(
                                   sx
                                   , sy
                                   , sz
                                   , slices
                                   , segments
                                   , surface
                                   , surface_X
                                   , surface_Y
                                   , surface_Z
                                   );
    
    m_data->make_tetramesh_shape_from_surface(
                                              geometry
                                              , surface
                                              , surface_X
                                              , surface_Y
                                              , surface_Z
                                              );
  }

  void Engine::set_sphere_shape(  size_t const & geometry_index
                                , float const & radius
                                )
  {
    assert( m_data || !"internal error: null pointer");
    assert( is_number(radius) || !"internal error: NaN or inf value");
    
    EngineData::geometry_type & geometry = m_data->m_narrow.get_geometry(geometry_index);
    
    mesh_array::T4Mesh                                 volume;
    mesh_array::VertexAttribute<T, mesh_array::T4Mesh> volume_X;
    mesh_array::VertexAttribute<T, mesh_array::T4Mesh> volume_Y;
    mesh_array::VertexAttribute<T, mesh_array::T4Mesh> volume_Z;

    mesh_array::T3Mesh                                 surface;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_X;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_Y;
    mesh_array::VertexAttribute<T, mesh_array::T3Mesh> surface_Z;

    size_t const segments = 12u;
    size_t const slices   = 12u;

    mesh_array::make_sphere<MT>(
                                radius
                                , slices
                                , segments
                                , surface
                                , surface_X
                                , surface_Y
                                , surface_Z
                                );

    m_data->make_tetramesh_shape_from_surface(
                                              geometry
                                              , surface
                                              , surface_X
                                              , surface_Y
                                              , surface_Z
                                              );
  }

  void Engine::set_tetramesh_shape(  size_t const & geometry_index
                                   , size_t const & N
                                   , size_t const & K
                                   , size_t const * vertices
                                   , size_t const * tetrahedra
                                   , float  const * coordinates
                                   )
  {
    assert( m_data || !"internal error: null pointer");
    
    EngineData::geometry_type & geometry = m_data->m_narrow.get_geometry(geometry_index);
    
    assert( vertices    || !"internal error: null pointer");
    assert( tetrahedra  || !"internal error: null pointer");
    assert( coordinates || !"internal error: null pointer");
    
    mesh_array::T4Mesh mesh;
    mesh_array::VertexAttribute<T,mesh_array::T4Mesh> X;
    mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Y;
    mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Z;
    
    mesh_array::convert(N, K, vertices, tetrahedra, coordinates, mesh, X, Y,Z );

    if(kdop::SelectContactPointAlgorithm::is_using_closest_point())
    {
      mesh_array::shrink<MT>(
                             VT::numeric_cast(0.99)
                             , mesh
                             , X
                             , Y
                             , Z
                             );
    }

    geometry.set_shape(
                       mesh
                       , X
                       , Y
                       , Z
                       );
  }
  
  float Engine::get_collision_envelope()
  {
    assert( m_data || !"internal error: null pointer");
    
    return m_data->m_narrow.params().get_envelope();
  }
  
  float Engine::get_time_step()
  {
    assert( m_data || !"internal error: null pointer");
    
    return m_data->m_time_step;
  }
  
  float Engine::get_time()
  {
    assert( m_data || !"internal error: null pointer");
    
    return m_data->m_time;
  }
  
  std::string Engine::get_material_name(size_t const & material_index)
  {
    assert( m_data || !"internal error: null pointer");
    return m_data->m_materials[ material_index ];
  }
  
  size_t Engine::get_number_of_materials()
  {
    assert( m_data || !"internal error: null pointer");
    return m_data->m_materials.size();
  }
  
  void Engine::get_material_indices( size_t * index_array )
  {
    assert( m_data || !"internal error: null pointer");
    for (size_t i = 0; i < m_data->m_materials.size() ; ++i)
    {
      index_array[i] = i;
    }
  }
  
  size_t Engine::get_number_of_properties()
  {
    assert( m_data || !"internal error: null pointer");
    return m_data->m_property_counter;
  }
  
  void Engine::get_material_property_indices(  size_t * first_index_array
                                             , size_t * second_index_array
                                             )
  {
    assert( m_data || !"internal error: null pointer");
    
    size_t p = 0;
    for (size_t first_idx = 0; first_idx < m_number_of_materials; ++first_idx)
    {
      for (size_t second_idx = 0; second_idx < m_number_of_materials; ++second_idx)
      {
        if (m_data->m_exist_property[first_idx][second_idx])
        {
          first_index_array[p]  = first_idx;
          second_index_array[p] = second_idx;
          
          ++p;
        }
      }
    }
  }

  void Engine::get_coefficients_of_anisotropic_friction( size_t const & first_index
                                                        , size_t const & second_index
                                                        , float & mu_x
                                                        , float & mu_y
                                                        , float & mu_z
                                                        )
  {
    
    assert( m_data || !"internal error: null pointer");
    assert( first_index  < m_number_of_materials || !"internal error: index excedes allocated space for in material table");
    assert( second_index < m_number_of_materials || !"internal error: index excedes allocated space for in material table");
    
    V const mu = m_data->m_properties[first_index][second_index].get_coefficients_of_anisotropic_friction();

    mu_x = mu(0);
    mu_y = mu(1);
    mu_z = mu(2);
  }

  void Engine::get_coefficient_of_isotropic_friction( size_t const & first_index
                                                     , size_t const & second_index
                                                     , float & mu
                                                     )
  {

    assert( m_data || !"internal error: null pointer");
    assert( first_index  < m_number_of_materials || !"internal error: index excedes allocated space for in material table");
    assert( second_index < m_number_of_materials || !"internal error: index excedes allocated space for in material table");

    mu = m_data->m_properties[first_index][second_index].get_coefficient_of_isotropic_friction();
  }

  float Engine::get_coefficient_of_restitution( size_t const & first_index
                                               , size_t const & second_index
                                               )
  {
    assert( m_data || !"internal error: null pointer");
    assert( first_index  < m_number_of_materials || !"internal error: index excedes allocated space for in material table");
    assert( second_index < m_number_of_materials || !"internal error: index excedes allocated space for in material table");
    
    return m_data->m_properties[first_index][second_index].get_coefficient_of_restitution();
  }
  
  size_t Engine::get_number_of_geometries()
  {
    assert( m_data || !"internal error: null pointer");
    return m_data->m_narrow.size();
  }
  
  void Engine::get_geometry_indices( size_t * index_array )
  {
    assert( m_data || !"internal error: null pointer");
    
    for (size_t i = 0; i < m_data->m_narrow.size() ; ++i)
    {
      index_array[i] = i;
    }
  }
  
  std::string Engine::get_geometry_name(size_t const & geometry_index)
  {
    assert( m_data || !"internal error: null pointer");
    
    return m_data->m_geometry_names[ geometry_index ];
  }

  void Engine::get_tetramesh_shape(  size_t const & geometry_index
                                   , size_t       & N
                                   , size_t       & K
                                   )
  {
    assert( m_data || !"internal error: null pointer");
    
    EngineData::geometry_type & geometry = m_data->m_narrow.get_geometry(geometry_index);

    N = geometry.m_mesh.vertex_size();
    K = geometry.m_mesh.tetrahedron_size();
  }
  
  void Engine::get_tetramesh_shape(  size_t const & geometry_index
                                   , size_t * vertices
                                   , size_t * tetrahedra
                                   , float  * coordinates
                                   )
  {
    assert( m_data || !"internal error: null pointer");
    
    EngineData::geometry_type & geometry = m_data->m_narrow.get_geometry(geometry_index);
    
    size_t const N = geometry.m_mesh.vertex_size();
    size_t const K = geometry.m_mesh.tetrahedron_size();
    
    for (size_t m = 0u; m < N; ++m)
    {
      mesh_array::Vertex const v = geometry.m_mesh.vertex(m);
      
      vertices[m] = v.idx();
      
      coordinates[3u*m+0u] = geometry.m_X0(v);
      coordinates[3u*m+1u] = geometry.m_Y0(v);
      coordinates[3u*m+2u] = geometry.m_Z0(v);
    }
    
    for (size_t k = 0u; k < K; ++k)
    {
      mesh_array::Tetrahedron const t = geometry.m_mesh.tetrahedron(k);
      
      tetrahedra[4u*k+0u] = t.i();
      tetrahedra[4u*k+1u] = t.j();
      tetrahedra[4u*k+2u] = t.k();
      tetrahedra[4u*k+3u] = t.m();
    }
  }

  size_t Engine::get_number_of_rigid_bodies()
  {
    assert( m_data || !"internal error: null pointer");
    return m_data->m_bodies.size();
  }
  
  void Engine::get_rigid_body_indices( size_t * index_array )
  {
    assert( m_data || !"internal error: null pointer");
    for (size_t i = 0; i < m_data->m_bodies.size() ; ++i)
    {
      index_array[i] = i;
    }
  }
  
  std::string Engine::get_rigid_body_name( size_t const & body_index )
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_index < m_data->m_bodies.size() || !"internal error: no such rigid body");
    
    return m_data->m_bodies[ body_index ].get_name();
  }
  
  void Engine::get_rigid_body_position( size_t const & body_index
                                       , float & x
                                       , float & y
                                       , float & z
                                       )
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_index < m_data->m_bodies.size() || !"internal error: no such rigid body");
    
    EngineData::V pos = m_data->m_bodies[ body_index ].get_position();
    
    assert( (is_number(pos[0]) && is_number(pos[1]) && is_number(pos[2])) || !"internal error: NaN or inf value");
    
    x = pos[0];
    y = pos[1];
    z = pos[2];
  }
  
  void Engine::get_rigid_body_orientation( size_t const & body_index
                                          , float & Qs
                                          , float & Qx
                                          , float & Qy
                                          , float & Qz
                                          )
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_index < m_data->m_bodies.size() || !"internal error: no such rigid body");
    
    EngineData::MT::quaternion_type Q = m_data->m_bodies[ body_index ].get_orientation();
    
    assert( (is_number(Q.real()) && is_number(Q.imag()(0)) && is_number(Q.imag()(1)) && is_number(Q.imag()(2))) || !"internal error: NaN or inf value");
    
    Qs = Q.real();
    Qx = Q.imag()(0);
    Qy = Q.imag()(1);
    Qz = Q.imag()(2);
  }
  
  void Engine::get_rigid_body_velocity( size_t const & body_index
                                       , float & x
                                       , float & y
                                       , float & z
                                       )
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_index < m_data->m_bodies.size() || !"internal error: no such rigid body");
    
    EngineData::V vel = m_data->m_bodies[ body_index ].get_velocity();
    
    assert( (is_number(vel[0]) && is_number(vel[1]) && is_number(vel[2])) || !"internal error: NaN or inf value");
    
    x = vel[0];
    y = vel[1];
    z = vel[2];
  }
  
  void Engine::get_rigid_body_spin( size_t const & body_index
                                   , float & x
                                   , float & y
                                   , float & z
                                   )
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_index < m_data->m_bodies.size() || !"internal error: no such rigid body");
    
    EngineData::V spin = m_data->m_bodies[ body_index ].get_spin();
    
    assert( (is_number(spin[0]) && is_number(spin[1]) && is_number(spin[2])) || !"internal error: NaN or inf value");
    
    x = spin[0];
    y = spin[1];
    z = spin[2];
  }
  
  void Engine::get_rigid_body_inertia( size_t const & body_index
                                      , float & xx
                                      , float & yy
                                      , float & zz
                                      )
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_index < m_data->m_bodies.size() || !"internal error: no such rigid body");
    
    EngineData::MT::matrix3x3_type in = m_data->m_bodies[ body_index ].get_inertia_bf();
    
    assert( (is_number(in(0,0)) && is_number(in(1,1)) && is_number(in(2,2))) || !"internal error: NaN or inf value");
    
    // BF -> WCS??
    xx = in(0,0);
    yy = in(1,1);
    zz = in(2,2);
  }
  
  float Engine::get_rigid_body_mass( size_t const & body_index )
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_index < m_data->m_bodies.size() || !"internal error: no such rigid body");
    
    float mass = m_data->m_bodies[ body_index ].get_mass();
    
    assert( is_number(mass) || !"internal error: NaN or inf value");
    
    return mass;
  }
  
  bool Engine::get_rigid_body_active( size_t const & body_index )
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_index < m_data->m_bodies.size() || !"internal error: no such rigid body");
    
    return true;
  }
  
  bool Engine::get_rigid_body_fixed( size_t const & body_index )
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_index < m_data->m_bodies.size() || !"internal error: no such rigid body");
    
    return m_data->m_bodies[ body_index ].is_fixed();
  }
  
  size_t Engine::get_rigid_body_material( size_t const & body_index )
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_index < m_data->m_bodies.size() || !"internal error: no such rigid body");
    
    return m_data->m_bodies[ body_index ].get_material_idx();
  }
  
  size_t Engine::get_rigid_body_collision_geometry(size_t const & body_index)
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_index < m_data->m_bodies.size() || !"internal error: no such rigid body");
    
    return m_data->m_bodies[ body_index ].get_geometry_idx();
  }
  

  void Engine::get_rigid_body_bounding_box(
                                           size_t const & body_index
                                           , float & min_x
                                           , float & min_y
                                           , float & min_z
                                           , float & max_x
                                           , float & max_y
                                           , float & max_z
                                           )
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_index < m_data->m_bodies.size() || !"internal error: no such rigid body");
    
    return m_data->m_bodies[ body_index ].get_box(min_x, min_y, min_z, max_x, max_y, max_z);
  }
  
  void Engine::set_gravity_up(
                              float const & x
                              , float const & y
                              , float const & z
                              )
  {
    assert( m_data || !"internal error: null pointer");
    
    m_data->m_gravity.up() = V::make(x,y,z);
  }
  
  void Engine::set_gravity_acceleration( float const & acceleration )
  {
    assert( m_data         || !"internal error: null pointer");
    assert(acceleration>=0 || !"set_gravity_acceleration(): acceleartion damping must be non-negative");
    
    m_data->m_gravity.acceleration() = acceleration;
  }
  
  void Engine::set_damping_parameters( float const & linear, float const & angular )
  {
    assert( m_data    || !"internal error: null pointer");
    assert(linear>=0  || !"set_damping_parameters(): linear damping must be non-negative");
    assert(angular>=0 || !"set_damping_parameters(): angular damping must be non-negative");
    
    m_data->m_damping.linear()  = linear;
    m_data->m_damping.angular() = angular;
  }
  
  void Engine::get_gravity_up(
                              float & x
                              , float & y
                              , float & z
                              )
  {
    assert( m_data || !"internal error: null pointer");
    
    x = m_data->m_gravity.up()(0);
    y = m_data->m_gravity.up()(1);
    z = m_data->m_gravity.up()(2);
  }
  
  void Engine::get_gravity_acceleration( float & acceleration )
  {
    assert( m_data || !"internal error: null pointer");
    
    acceleration = m_data->m_gravity.acceleration();
  }
  
  void Engine::get_damping_parameters( float & linear, float & angular )
  {
    assert( m_data || !"internal error: null pointer");
    
    linear  = m_data->m_damping.linear();
    angular = m_data->m_damping.angular();
  }

  size_t Engine::get_number_of_pin_forces() const
  {
    assert( m_data || !"internal error: null pointer");
    
    return m_data->m_pin_forces.size();
  }
  
  void Engine::get_pin_force_indices( size_t * index_array )
  {
    assert( m_data || !"internal error: null pointer");
    
    assert(index_array || !"internal error: null pointer");
    
    for( size_t i = 0; i < m_data->m_pin_forces.size();++i)
    {
      index_array[i] = m_data->m_pin_forces[i].get_idx();
    }
  }
  
  void Engine::get_pin_tau(size_t const & force_idx, float & tau )
  {
    assert( m_data || !"internal error: null pointer");
    
    tau = m_data->m_pin_forces[force_idx].tau();
  }
  
  void Engine::get_pin_target(size_t const & force_idx, float  & x, float  & y, float  & z )
  {
    assert( m_data || !"internal error: null pointer");
    
    x = m_data->m_pin_forces[force_idx].target()(0);
    y = m_data->m_pin_forces[force_idx].target()(1);
    z = m_data->m_pin_forces[force_idx].target()(2);
  }
  
  void Engine::get_pin_anchor(size_t const & force_idx, float  & x, float  & y, float  & z )
  {
    assert( m_data || !"internal error: null pointer");
    
    x = m_data->m_pin_forces[force_idx].anchor()(0);
    y = m_data->m_pin_forces[force_idx].anchor()(1);
    z = m_data->m_pin_forces[force_idx].anchor()(2);
  }
  
  size_t Engine::create_pin_force() const
  {
    assert( m_data || !"internal error: null pointer");
    
    size_t const idx = m_data->m_force_callbacks.size();
    
    m_data->m_pin_forces[idx] = prox::Pin<EngineData::MT>();
    
    m_data->m_pin_forces[idx].set_idx(idx);
    
    m_data->m_force_callbacks.push_back( &m_data->m_pin_forces[idx] );
    
    return m_data->m_pin_forces.size() - 1;
  }
  
  void Engine::set_pin_tau(size_t const & force_idx, float const & tau )
  {
    assert( m_data || !"internal error: null pointer");
    
    m_data->m_pin_forces[force_idx].tau() = tau;
  }
  
  void Engine::set_pin_target(size_t const & force_idx, float const & x, float const & y, float const & z )
  {
    assert( m_data || !"internal error: null pointer");
    
    m_data->m_pin_forces[force_idx].target()(0) = x;
    m_data->m_pin_forces[force_idx].target()(1) = y;
    m_data->m_pin_forces[force_idx].target()(2) = z;
  }
  
  void Engine::set_pin_anchor(size_t const & force_idx, float const & x, float const & y, float const & z )
  {
    assert( m_data || !"internal error: null pointer");
    
    m_data->m_pin_forces[force_idx].anchor()(0) = x;
    m_data->m_pin_forces[force_idx].anchor()(1) = y;
    m_data->m_pin_forces[force_idx].anchor()(2) = z;
  }
  
  size_t Engine::get_number_of_connected_forces(size_t const & body_index)
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_index < m_data->m_bodies.size() || !"internal error: no such rigid body");
    
    return m_data->m_bodies[body_index].get_force_callbacks().size();
  }
  
  void Engine::get_connected_force_indices(  size_t const & body_index, size_t * index_array )
  {
    assert( m_data || !"internal error: null pointer");
    assert( body_index < m_data->m_bodies.size() || !"internal error: no such rigid body");
    
    size_t const N = m_data->m_bodies[body_index].get_force_callbacks().size();
    
    for(size_t i =0u; i< N; ++i)
    {
      index_array[i] = m_data->m_bodies[body_index].get_force_callbacks()[i]->get_idx();
    }
  }
  
  void Engine::connect_force( size_t body_idx, size_t force_idx )
  {
    assert( m_data                                       || !"internal error: null pointer");
    assert( body_idx < m_data->m_bodies.size()           || !"internal error: no such rigid body");
    assert( force_idx < m_data->m_force_callbacks.size() || !"internal error: no such force callback");
    
    m_data->m_bodies[body_idx].get_force_callbacks().push_back( m_data->m_force_callbacks[force_idx]  );
  }
  
  void Engine::connect_scripted_motion( size_t const & body_idx, size_t const & motion_idx )
  {
    assert( m_data                                        || !"connect_scripted_motion(): Data was null");
    assert( body_idx < m_data->m_bodies.size()            || !"connect_scripted_motion(): No such rigid body exist");
    assert( !(m_data->m_bodies[ body_idx ].is_scripted()) || !"connect_scripted_motion(): Rigid body is already scripted");
    assert( m_data->find_motion(motion_idx)!=0            || !"connect_scripted_motion(): No such motion exist");
    
    m_data->m_bodies[ body_idx ].set_scripted( true );
    
    m_data->m_all_scripted_bodies.push_back(body_idx);
    
    EngineData::ScriptedMotion * motion = m_data->find_motion(motion_idx);
    
    m_data->m_motion_callbacks[body_idx] = motion;
  }
  
  void Engine::set_scripted_key_position(
                                         size_t const & motion_index
                                         , float const & time
                                         , float const & x
                                         , float const & y
                                         , float const & z
                                         )
  {
    assert( m_data || !"internal error: null pointer");
    
    std::map<size_t, EngineData::KeyframeMotion>::iterator lookup = m_data->m_keyframe_motions.find(motion_index);
    
    assert( lookup  != m_data->m_keyframe_motions.end() || !"set_scripted_key_position(): Motion did not exist");
    
    lookup->second.add_position(time, x, y, z);
  }
  
  void Engine::set_scripted_key_orientation(
                                            size_t const & motion_index
                                            , float const & time
                                            , float const & qs
                                            , float const & qx
                                            , float const & qy
                                            , float const & qz
                                            )
  {
    assert( m_data || !"internal error: null pointer");
    
    std::map<size_t, EngineData::KeyframeMotion>::iterator lookup = m_data->m_keyframe_motions.find(motion_index);
    
    assert( lookup  != m_data->m_keyframe_motions.end() || !"set_scripted_key_position(): Motion did not exist");
    
    lookup->second.add_orientation(time, qs, qx, qy, qz);
  }
  
  bool Engine::has_scripted_motion( size_t const & body_index )
  {
    assert( m_data || !"has_scripted_motion(): data null pointer");
    assert( body_index < m_data->m_bodies.size() || !"has_scripted_motion(): no such rigid body");
    
    return m_data->m_bodies[ body_index ].is_scripted();
  }
  
  size_t Engine::get_scripted_motion( size_t const & body_index )
  {
    assert( m_data || !"get_scripted_motion(): data null pointer");
    assert( body_index < m_data->m_bodies.size() || !"get_scripted_motion(): no such rigid body");
    assert( m_data->m_bodies[ body_index ].is_scripted() || !"get_scripted_motion(): rigid body was not scripted");
    
    std::map<size_t, EngineData::ScriptedMotion * > ::iterator lookup = m_data->m_motion_callbacks.find(body_index);
    
    assert(lookup!=m_data->m_motion_callbacks.end() || !"get_scripted_motion(): No scripted motion was connected to this body");
    
    size_t const motion_idx = lookup->second->m_index;
    
    return motion_idx;
  }
  
  size_t Engine::create_key_frame_scripted_motion()
  {
    EngineData::KeyframeMotion motion;
    
    motion.m_index = EngineData::ScriptedMotion::get_next_index();
    
    ++(EngineData::ScriptedMotion::get_next_index());
    
    m_data->m_keyframe_motions[motion.m_index] = motion;
    
    return motion.m_index;
  }
  
  size_t Engine::create_oscilation_scripted_motion()
  {
    EngineData::OscillationMotion motion;
    
    motion.m_index = EngineData::ScriptedMotion::get_next_index();
    
    ++(EngineData::ScriptedMotion::get_next_index());
    
    m_data->m_oscillation_motions[motion.m_index] = motion;
    
    return motion.m_index;
  }
  
  void Engine::set_scripted_oscilation_paramters(
                                                 size_t const & motion_index
                                                 , float const & amplitude
                                                 , float const & frequency
                                                 , float const & phase
                                                 , float const & dir_x
                                                 , float const & dir_y
                                                 , float const & dir_z
                                                 , float const & ref_x
                                                 , float const & ref_y
                                                 , float const & ref_z
                                                 )
  {
    assert( m_data || !"set_scripted_oscilation_paramters(): data was null pointer");
    
    std::map<size_t, EngineData::OscillationMotion>::iterator lookup = m_data->m_oscillation_motions.find(motion_index);
    
    assert( lookup  != m_data->m_oscillation_motions.end() || !"set_scripted_oscilation_paramters(): Motion did not exist");
    
    assert(amplitude > 0.0f || !"set_scripted_oscilation_paramters(): Amplitude must be positive");
    assert(frequency > 0.0f || !"set_scripted_oscilation_paramters(): frequency must be positive");
    
    lookup->second.m_amplitude = amplitude;
    lookup->second.m_frequency = frequency;
    lookup->second.m_phase     = phase;
    
    assert(( dir_x!=0.0f || dir_y!=0.0f || dir_z!= 0.0f) || !"set_scripted_oscilation_paramters(): direction was zero vector");
    
    lookup->second.m_direction = tiny::unit( V::make(dir_x, dir_y, dir_z) );
    lookup->second.m_origin    = V::make(ref_x,ref_y,ref_z);
  }
  
  size_t Engine::get_number_of_scripted_motions()
  {
    assert( m_data || !"set_scripted_oscilation_paramters(): data was null pointer");
    
    return m_data->m_oscillation_motions.size() + m_data->m_keyframe_motions.size();
  }
  
  void Engine::get_scripted_motion_indices( size_t * index_array )
  {
    assert( m_data || !"get_scripted_motion_indices(): data was null pointer");
    
    size_t count = 0;
    
    {
      std::map<size_t, EngineData::OscillationMotion>::const_iterator oscilation = m_data->m_oscillation_motions.begin();
      std::map<size_t, EngineData::OscillationMotion>::const_iterator end        = m_data->m_oscillation_motions.end();
      
      for(;oscilation!=end;++oscilation,++count)
      {
        index_array[count] = oscilation->second.m_index;
      }
    }
    {
      std::map<size_t, EngineData::KeyframeMotion>::const_iterator keyframe = m_data->m_keyframe_motions.begin();
      std::map<size_t, EngineData::KeyframeMotion>::const_iterator end      = m_data->m_keyframe_motions.end();
      
      for(;keyframe!=end;++keyframe,++count)
      {
        index_array[count] = keyframe->second.m_index;
      }
    }
  }
  
  bool Engine::is_scripted_motion_keyframe( size_t const & motion_index )
  {
    assert( m_data || !"is_scripted_motion_keyframe(): data was null pointer");
    
    std::map<size_t, EngineData::KeyframeMotion>::iterator lookup = m_data->m_keyframe_motions.find(motion_index);
    
    return lookup  != m_data->m_keyframe_motions.end();
  }
  
  bool Engine::is_scripted_motion_oscilation( size_t const & motion_index )
  {
    assert( m_data || !"is_scripted_motion_oscilation(): data was null pointer");
    
    std::map<size_t, EngineData::OscillationMotion>::iterator lookup = m_data->m_oscillation_motions.find(motion_index);
    
    return lookup  != m_data->m_oscillation_motions.end();
  }
  
  void Engine::get_scripted_oscilation_paramters(
                                                 size_t const & motion_index
                                                 , float & amplitude
                                                 , float & frequency
                                                 , float & phase
                                                 , float & dir_x
                                                 , float & dir_y
                                                 , float & dir_z
                                                 )
  {
    assert( m_data || !"get_scripted_oscilation_paramters(): data was null pointer");
    
    std::map<size_t, EngineData::OscillationMotion>::iterator lookup = m_data->m_oscillation_motions.find(motion_index);
    
    assert( lookup  != m_data->m_oscillation_motions.end() || !"get_scripted_oscilation_paramters(): motion was not oscilation scripted motion");
    
    amplitude = lookup->second.m_amplitude;
    frequency = lookup->second.m_frequency;
    phase     = lookup->second.m_phase;
    dir_x     = lookup->second.m_direction(0);
    dir_y     = lookup->second.m_direction(1);
    dir_z     = lookup->second.m_direction(2);
  }
  
  size_t Engine::get_number_of_key_frame_positions( size_t const & motion_index)
  {
    assert( m_data || !"get_number_of_key_frame_positions(): data was null pointer");
    
    std::map<size_t, EngineData::KeyframeMotion>::iterator lookup = m_data->m_keyframe_motions.find(motion_index);
    
    assert( lookup  != m_data->m_keyframe_motions.end() || !"get_number_of_key_frame_positions(): motion was not keyframe scripted motion");
    
    return lookup->second.m_positions.size();
  }
  
  size_t Engine::get_number_of_key_frame_orientations( size_t const & motion_index)
  {
    assert( m_data || !"get_number_of_key_frame_orientations(): data was null pointer");
    
    std::map<size_t, EngineData::KeyframeMotion>::iterator lookup = m_data->m_keyframe_motions.find(motion_index);
    
    assert( lookup  != m_data->m_keyframe_motions.end() || !"get_number_of_key_frame_positions(): motion was not keyframe scripted motion");
    
    return lookup->second.m_orientations.size();
  }
  
  void Engine::get_key_frame_positions(
                                       size_t const & motion_index
                                       , float * time_array
                                       , float * x_array
                                       , float * y_array
                                       , float * z_array
                                       )
  {
    assert( m_data || !"get_key_frame_positions(): data was null pointer");
    
    std::map<size_t, EngineData::KeyframeMotion>::iterator lookup = m_data->m_keyframe_motions.find(motion_index);
    
    assert( lookup  != m_data->m_keyframe_motions.end() || !"get_number_of_key_frame_positions(): motion was not keyframe scripted motion");
    
    std::vector<EngineData::KeyPosition>::const_iterator key = lookup->second.m_positions.begin();
    std::vector<EngineData::KeyPosition>::const_iterator end = lookup->second.m_positions.end();
    
    size_t count = 0u;
    for(;key!=end;++key,++count)
    {
      time_array[count] = key->m_time;
      x_array[count] = key->m_value(0);
      y_array[count] = key->m_value(1);
      z_array[count] = key->m_value(2);
    }
  }
  
  void Engine::get_key_frame_orientations(
                                          size_t const & motion_index
                                          , float * time_array
                                          , float * qs_array
                                          , float * qx_array
                                          , float * qy_array
                                          , float * qz_array
                                          )
  {
    assert( m_data || !"get_key_frame_orientations(): data was null pointer");
    
    std::map<size_t, EngineData::KeyframeMotion>::iterator lookup = m_data->m_keyframe_motions.find(motion_index);
    
    assert( lookup  != m_data->m_keyframe_motions.end() || !"get_key_frame_orientations(): motion was not keyframe scripted motion");
    
    std::vector<EngineData::KeyOrientation>::const_iterator key = lookup->second.m_orientations.begin();
    std::vector<EngineData::KeyOrientation>::const_iterator end = lookup->second.m_orientations.end();
    
    size_t count = 0u;
    for(;key!=end;++key,++count)
    {
      time_array[count] = key->m_time;
      qs_array[count]   = key->m_value.real();
      qx_array[count]   = key->m_value.imag()(0);
      qy_array[count]   = key->m_value.imag()(1);
      qz_array[count]   = key->m_value.imag()(2);
    }
  }

  bool Engine::compute_raycast(
                               float const & p_x
                               , float const & p_y
                               , float const & p_z
                               , float const & ray_x
                               , float const & ray_y
                               , float const & ray_z
                               , size_t & body_idx
                               , float & hit_x
                               , float & hit_y
                               , float & hit_z
                               , float & distance
                               )
  {
    assert( m_data || !"compute_raycast(): data was null pointer");

    return m_data->compute_raycast(
                                   p_x, p_y, p_z,
                                   ray_x, ray_y, ray_z,
                                   body_idx,
                                   hit_x, hit_y, hit_z,
                                   distance
                                   );
  }

}// namespace prox
