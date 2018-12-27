#include <procedural_factory.h>
#include <procedural_factory_types.h>
#include <procedural_types.h>
#include <procedural_noise.h>

#include <content.h>

#include <mesh_array.h>

#include <tiny_math_types.h>
#include <tiny_coordsys_functions.h>
#include <mass.h>
#include <util_string_helper.h>

#include <string>

namespace procedural
{
  
  typedef tiny::MathTypes<float> MTf;

  template<typename MT>
  GeometryHandle<MT> create_geometry_handle_convex(
                                                   content::API * engine
                                                   , std::vector<typename MT::vector3_type> const &  vertices
                                                   )
  {
    typedef typename MT::real_type        T;
    typedef typename MT::vector3_type     V;
    typedef typename MT::quaternion_type  Q;
    typedef typename MT::value_traits     VT;

    static size_t counter = 0u;

    std::string const geom_name = "convex_" + util::to_string( counter++ );

    detail::MeshData<MT> data;

    //fix vertex sequence
    mesh_array::make_convex<MT>(
                                vertices
                                , data.m_mesh
                                , data.m_X
                                , data.m_Y
                                , data.m_Z
                                );

    mass::Properties<T> props_mf = mass::compute_mesh(VT::one(), data.m_mesh.triangle_size(), &data );
    mass::Properties<T> props_bf = mass::translate_to_body_frame(props_mf);
    mass::Properties<T> props    = mass::rotate_to_body_frame(props_bf);

    // Change geometry from model space to body space
    V const d = - V::make(props_mf.m_x, props_mf.m_y, props_mf.m_z);
    Q const R = Q(props.m_Qs,props.m_Qx,props.m_Qy,props.m_Qz);
    mesh_array::translate<MT>( d, data.m_mesh, data.m_X, data.m_Y, data.m_Z);
    mesh_array::rotate<MT>(conj(R), data.m_mesh, data.m_X, data.m_Y, data.m_Z);

    size_t const gid = engine->create_collision_geometry( geom_name );
    size_t const N	 = data.m_mesh.vertex_size();

    std::vector<T> coords;
    coords.resize(3u*N);

    for(size_t i = 0; i<N; ++i)
    {
      mesh_array::Vertex const v = data.m_mesh.vertex( i );
      coords[3*i  ] = data.m_X(v);
      coords[3*i+1] = data.m_Y(v);
      coords[3*i+2] = data.m_Z(v);
    }

    engine->set_convex_shape( gid, N ,&coords[0] );

    return GeometryHandle<MT>(
                              props.m_m
                              , props.m_Ixx
                              , props.m_Iyy
                              , props.m_Izz
                              , V::make(props_mf.m_x, props_mf.m_y, props_mf.m_z)
                              , Q(props.m_Qs,props.m_Qx,props.m_Qy,props.m_Qz)
                              , gid
                              );
  }

  template
  GeometryHandle<MTf> create_geometry_handle_convex(
                                                   content::API * engine
                                                   , std::vector<MTf::vector3_type> const &  vertices
                                                   );

  template<typename MT>
  GeometryHandle<MT> create_geometry_handle_sphere(
                                                   content::API * engine
                                                   , typename MT::real_type const & radius
                                                   )
  {
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type  Q;
    typedef typename MT::value_traits    VT;
    
    static size_t counter = 0u;
    
    std::string const geom_name = "sphere_" + util::to_string( counter++ );
    
    mass::Properties<T> props = mass::compute_sphere(VT::one(), radius);
    
    size_t const gid = engine->create_collision_geometry( geom_name );

    engine->set_sphere_shape( gid, radius );
    
    return GeometryHandle<MT>( props.m_m, props.m_Ixx, props.m_Iyy, props.m_Izz, V::zero(), Q::identity(), gid );
  }

  template
  GeometryHandle<MTf> create_geometry_handle_sphere<MTf>(
                                                         content::API * engine
                                                         , MTf::real_type const & radius
                                                         );
  
  template<typename MT>
  GeometryHandle<MT> create_geometry_handle_cylinder(
                                                    content::API * engine
                                                    , typename MT::real_type const & radius
                                                    , typename MT::real_type const & height
                                                    )
  {
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type  Q;
    typedef typename MT::value_traits    VT;

    static size_t counter = 0u;

    std::string const geom_name = "cylinder_" + util::to_string( counter++ );

    mass::Properties<T> props = mass::compute_cylinder(VT::one(), radius, VT::half()*height);

    size_t const gid = engine->create_collision_geometry( geom_name );

    engine->set_cylinder_shape( gid, radius, height );

    return GeometryHandle<MT>( props.m_m, props.m_Ixx, props.m_Iyy, props.m_Izz, V::zero(), Q::identity(), gid );
  }


  template
  GeometryHandle<MTf> create_geometry_handle_cylinder<MTf>(
                                                          content::API * engine
                                                          , MTf::real_type const & radius
                                                          , MTf::real_type const & height
                                                          );



  template<typename MT>
  GeometryHandle<MT> create_geometry_handle_capsule(
                                                    content::API * engine
                                                    , typename MT::real_type const & radius
                                                    , typename MT::real_type const & height
                                                    )
  {
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type  Q;
    typedef typename MT::value_traits    VT;
    
    static size_t counter = 0u;
    
    std::string const geom_name = "capsule_" + util::to_string( counter++ );

    mass::Properties<T> props = mass::compute_capsule(VT::one(), radius, VT::half()*height);

    size_t const gid = engine->create_collision_geometry( geom_name );

    engine->set_capsule_shape( gid, radius, height );
    
    return GeometryHandle<MT>( props.m_m, props.m_Ixx, props.m_Iyy, props.m_Izz, V::zero(), Q::identity(), gid );
  }
  
  
  template
  GeometryHandle<MTf> create_geometry_handle_capsule<MTf>(
                                                          content::API * engine
                                                          , MTf::real_type const & radius
                                                          , MTf::real_type const & height
                                                          );
  
  
  template<typename MT>
  GeometryHandle<MT> create_geometry_handle_box(
                                                content::API * engine
                                                , typename MT::real_type  const & width
                                                , typename MT::real_type  const & height
                                                , typename MT::real_type  const & depth
                                                )
  {
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type  Q;
    typedef typename MT::value_traits    VT;
    
    static size_t counter = 0u;
    
    std::string const geom_name = "box_" + util::to_string( counter++ );
    
    mass::Properties<T> props = mass::compute_box(VT::one(), width/VT::two(), height/VT::two(), depth/VT::two());
    
    size_t const gid = engine->create_collision_geometry( geom_name );

    engine->set_box_shape( gid, width, height, depth );
    
    return GeometryHandle<MT>( props.m_m, props.m_Ixx, props.m_Iyy, props.m_Izz, V::zero(), Q::identity(), gid );
  }
  
  template
  GeometryHandle<MTf> create_geometry_handle_box<MTf>(
                                                      content::API * engine
                                                      , MTf::real_type  const & width
                                                      , MTf::real_type  const & height
                                                      , MTf::real_type  const & depth
                                                      );
  
  template<typename MT>
  GeometryHandle<MT> create_geometry_handle_tetrahedron(
                                                  content::API * engine
                                                  , typename MT::vector3_type one
                                                  , typename MT::vector3_type two
                                                  , typename MT::vector3_type three
                                                  , typename MT::vector3_type four
                                                  )
  {
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type  Q;
    typedef typename MT::value_traits    VT;
    
    static size_t counter = 0u;
    
    std::string const geom_name = "tetra_" + util::to_string( counter++ );
    
    detail::MeshData<MT> data;
    
    mesh_array::make_tetrahedron<MT>(one, two, three, four, data.m_mesh, data.m_X, data.m_Y, data.m_Z);
    
    mass::Properties<T> props_mf = mass::compute_mesh(VT::one(), data.m_mesh.triangle_size(), &data );
    mass::Properties<T> props_bf = mass::translate_to_body_frame(props_mf);
    mass::Properties<T> props    = mass::rotate_to_body_frame(props_bf);
    

      // Change geometry from model space to body space
    V const d = - V::make(props_mf.m_x, props_mf.m_y, props_mf.m_z);
    Q const R =   Q(props.m_Qs,props.m_Qx,props.m_Qy,props.m_Qz);
    mesh_array::translate<MT>( d, data.m_mesh, data.m_X, data.m_Y, data.m_Z);
    mesh_array::rotate<MT>(conj(R), data.m_mesh, data.m_X, data.m_Y, data.m_Z);
    
    size_t const gid = engine->create_collision_geometry( geom_name );
    size_t const N	 = data.m_mesh.vertex_size();
    
    std::vector<T> coords;
    coords.resize(3u*N);
    
    for(size_t i = 0; i<N; ++i)
    {
      mesh_array::Vertex const v = data.m_mesh.vertex( i );
      coords[3*i  ] = data.m_X(v);
      coords[3*i+1] = data.m_Y(v);
      coords[3*i+2] = data.m_Z(v);
    }
    
    engine->set_convex_shape( gid, N ,&coords[0] );

    return GeometryHandle<MT>(
                              props.m_m
                              , props.m_Ixx
                              , props.m_Iyy
                              , props.m_Izz
                              , V::make(props_mf.m_x, props_mf.m_y, props_mf.m_z)
                              , Q(props.m_Qs,props.m_Qx,props.m_Qy,props.m_Qz)
                              , gid
                              );
  }
  
  template
  GeometryHandle<MTf> create_geometry_handle_tetrahedron<MTf>(
                                                        content::API * engine
                                                        , MTf::vector3_type one
                                                        , MTf::vector3_type two
                                                        , MTf::vector3_type three
                                                        , MTf::vector3_type four
                                                        );
  
  
  template<typename MT>
  GeometryHandle<MT> create_geometry_handle_cuboid(
                                                   content::API * engine
                                                   , typename MT::vector3_type * vertices
                                                   )
  {
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type  Q;
    typedef typename MT::value_traits    VT;
    
    static size_t counter = 0u;
    
    std::string const geom_name = "cuboid_" + util::to_string( counter++ );
    
    detail::MeshData<MT> data;



    //fix vertex sequence
    mesh_array::make_cuboid<MT>(
                                  vertices[0]
                                , vertices[1]
                                , vertices[2]
                                , vertices[3]
                                , vertices[4]
                                , vertices[5]
                                , vertices[6]
                                , vertices[7]
                                , data.m_mesh, data.m_X, data.m_Y, data.m_Z
                                );
    
    mass::Properties<T> props_mf = mass::compute_mesh(VT::one(), data.m_mesh.triangle_size(), &data );
    mass::Properties<T> props_bf = mass::translate_to_body_frame(props_mf);
    mass::Properties<T> props    = mass::rotate_to_body_frame(props_bf);
    
      // Change geometry from model space to body space
    V const d = - V::make(props_mf.m_x, props_mf.m_y, props_mf.m_z);
    Q const R = Q(props.m_Qs,props.m_Qx,props.m_Qy,props.m_Qz);
    mesh_array::translate<MT>( d, data.m_mesh, data.m_X, data.m_Y, data.m_Z);
    mesh_array::rotate<MT>(conj(R), data.m_mesh, data.m_X, data.m_Y, data.m_Z);
    
    size_t const gid = engine->create_collision_geometry( geom_name );
    size_t const N	 = data.m_mesh.vertex_size();
    
    std::vector<T> coords;
    coords.resize(3u*N);
    
    for(size_t i = 0; i<N; ++i)
    {
      mesh_array::Vertex const v = data.m_mesh.vertex( i );
      coords[3*i  ] = data.m_X(v);
      coords[3*i+1] = data.m_Y(v);
      coords[3*i+2] = data.m_Z(v);
    }
    
    engine->set_convex_shape( gid, N ,&coords[0] );

    return GeometryHandle<MT>(
                              props.m_m
                              , props.m_Ixx
                              , props.m_Iyy
                              , props.m_Izz
                              , V::make(props_mf.m_x, props_mf.m_y, props_mf.m_z)
                              , Q(props.m_Qs,props.m_Qx,props.m_Qy,props.m_Qz)
                              , gid
                              );
  }
  
  template
  GeometryHandle<MTf> create_geometry_handle_cuboid<MTf>(
                                                         content::API * engine
                                                         , MTf::vector3_type * vertices
                                                         );


  template<typename MT>
  GeometryHandle<MT> create_geometry_handle_pillar_segment( content::API * engine
                                                           , typename MT::real_type const & bottom_radius
                                                           , typename MT::real_type const & top_radius
                                                           , typename MT::real_type const & height
                                                           , size_t const & slices
                                                           , mesh_array::TetGenSettings tetset
                                                        )
  {
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type Q;
    typedef typename MT::value_traits    VT;
    
    static size_t counter = 0u;
    
    std::string const geom_name = "pillar_segment_" + util::to_string( counter++ );
    
    mesh_array::T3Mesh mesh;
    mesh_array::VertexAttribute<T,mesh_array::T3Mesh> X;
    mesh_array::VertexAttribute<T,mesh_array::T3Mesh> Y;
    mesh_array::VertexAttribute<T,mesh_array::T3Mesh> Z;
    
    mesh_array::make_conical<MT>( bottom_radius, top_radius, height, slices, mesh, X, Y, Z );
    
    mass::Properties<T> props_mf = mass::compute_conical_solid(VT::one(), bottom_radius, top_radius, height);
    mass::Properties<T> props_bf = mass::translate_to_body_frame(props_mf);
    mass::Properties<T> props    = mass::rotate_to_body_frame(props_bf);

    // Change geometry from model space to body space
    V const d = - V::make(props_mf.m_x, props_mf.m_y, props_mf.m_z);
    Q const R = Q(props.m_Qs,props.m_Qx,props.m_Qy,props.m_Qz);
    mesh_array::translate<MT>( d, mesh, X, Y, Z);
    mesh_array::rotate<MT>(conj(R), mesh, X, Y, Z);

    mesh_array::T4Mesh volume;
    mesh_array::VertexAttribute<T, mesh_array::T4Mesh> volX;
    mesh_array::VertexAttribute<T, mesh_array::T4Mesh> volY;
    mesh_array::VertexAttribute<T, mesh_array::T4Mesh> volZ;

    mesh_array::tetgen(mesh, X, Y, Z, volume, volX, volY, volZ, tetset);
  
    size_t const gid = engine->create_collision_geometry( geom_name );
    size_t const N	 = volume.vertex_size();
    size_t const K   = volume.tetrahedron_size();
    
    std::vector<size_t> verts;
    verts.resize(N);
    
    for(size_t i = 0; i<N; ++i)
    {
      verts[ i ] = i;
    }
    
    std::vector<T> coords;
    coords.resize(3u*N);
    
    for(size_t i = 0; i<N; ++i)
    {
      mesh_array::Vertex const v = volume.vertex( i );
      coords[3*i  ] = volX(v);
      coords[3*i+1] = volY(v);
      coords[3*i+2] = volZ(v);
    }
    
    std::vector<size_t> tets;
    tets.resize(4u*K);
    
    for(size_t i = 0; i<K; ++i)
    {
      mesh_array::Tetrahedron const t = volume.tetrahedron( i );
      tets[4*i  ] = t.i();
      tets[4*i+1] = t.j();
      tets[4*i+2] = t.k();
      tets[4*i+3] = t.m();
    }
    
    engine->set_tetramesh_shape(gid, N, K, &verts[0], &tets[0], &coords[0]);
    
    return GeometryHandle<MT>(
                              props.m_m
                              , props.m_Ixx
                              , props.m_Iyy
                              , props.m_Izz
                              , V::make(props_mf.m_x, props_mf.m_y, props_mf.m_z)
                              , Q(props.m_Qs,props.m_Qx,props.m_Qy,props.m_Qz)
                              , gid
                              );
  }
  
  template
  GeometryHandle<MTf> create_geometry_handle_pillar_segment<MTf>( content::API * engine
                                                                 , MTf::real_type const & bottom_radius
                                                                 , MTf::real_type const & top_radius
                                                                 , MTf::real_type const & height
                                                                 , size_t const & slices
                                                                 , mesh_array::TetGenSettings tetset
                                                                 );
  
  template<typename MT>
  size_t create_rigid_body(  content::API * engine
                           , typename MT::vector3_type const & Tb2w
                           , typename MT::quaternion_type const & Qb2w
                           , GeometryHandle<MT> const & geometry
                           , size_t const & mid
                           , typename MT::real_type const & density
                           , bool const fixed
                           , std::string const material_name
                           )
  {
    typedef typename MT::value_traits    VT;
    
    static size_t counter = 0u;
    
    std::string const body_name = "body_" + util::to_string( counter++ );
    
    size_t const rid = engine->create_rigid_body( body_name );
    
      //2011-05-04 Mort: should these be passed as a parameter? Right now I have just hardcoded them to zero;
      //set_rigid_body spin?
    engine->set_rigid_body_velocity( rid, VT::zero(), VT::zero(), VT::zero());



    if (Noise::on() )
    {
      // 2018-12-27 Kenny: The idea of adding a little noise to the world is
      // that it will break perfect symmatries and hence make it easier to
      // generate contact points of type vertex-face instead of having more
      // "singular" types of contact points such as vertex-vertex type.
      typedef typename MT::vector3_type V;

      // 2018-12-27 Kenny: Retrieve the global noise level of the "world". This
      // value is often parsed in from a cfg-file. It is stored as a "global"
      // variable.
      V const noise = V::random(- Noise::scale(), Noise::scale());

      // 2018-12-27 Kenny: The world up direction is usually the y-axis. Hence
      // we add a bit of noise only in x and y directions in order not to
      // create too many penetrations in the direction of gravity.
      engine->set_rigid_body_position( rid
                                      , Tb2w(0)+ noise(0)
                                      , Tb2w(1)
                                      , Tb2w(2)+ noise(2)
                                      );
    }
    else
    {
      engine->set_rigid_body_position( rid, Tb2w(0), Tb2w(1), Tb2w(2) );
    }

    engine->set_rigid_body_orientation( rid, Qb2w.real(), Qb2w.imag()(0), Qb2w.imag()(1), Qb2w.imag()(2) );

    engine->set_rigid_body_mass( rid , geometry.m_m*density );
    
    engine->set_rigid_body_inertia(
                                   rid
                                   , geometry.m_Ixx*density
                                   , geometry.m_Iyy*density
                                   , geometry.m_Izz*density
                                   );
    
    engine->connect_collision_geometry( rid, geometry.m_gid );
    
    engine->set_rigid_body_material( rid, mid );
    
    engine->set_rigid_body_fixed( rid, fixed);
    
    return rid;
  }
  
  template
  size_t create_rigid_body<MTf>(  content::API * engine
                                , MTf::vector3_type const & Tb2w
                                , MTf::quaternion_type const & Qb2w
                                , GeometryHandle<MTf> const & geometry
                                , size_t const & mid
                                , MTf::real_type const & density
                                , bool const fixed
                                , std::string const material_name
                                );
  
  template<typename MT>
  void compute_arch_stone_vertices(
                                    typename MT::real_type const & theta
                                   , typename MT::real_type const & depth
                                   , typename MT::real_type const & r_outer
                                   , typename MT::real_type const & r_inner
                                   , typename MT::vector3_type * vertices
                                   )
  {
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::value_traits    VT;
    
    using std::sin;
    using std::cos;
    
    T const center_height    = (r_outer + r_inner)*0.5f;
    T const max_height       = r_outer*cos( theta*0.5f );
    T const min_height       = r_inner*cos( theta*0.5f );
    T const half_width_inner = r_inner * sin( theta*0.5f );
    T const half_width_outer = r_outer * sin( theta*0.5f );
    T const half_depth       = depth*0.5f;
    
    vertices[0] = V::make( - half_width_inner, min_height-center_height, half_depth );
    vertices[1] = V::make(   half_width_inner, min_height-center_height, half_depth );
    vertices[2] = V::make(   half_width_outer, max_height-center_height, half_depth );
    vertices[3] = V::make( - half_width_outer, max_height-center_height, half_depth );
    
    vertices[4] =  vertices[0] - V::make( VT::zero(), VT::zero(), depth );
    vertices[5] =  vertices[1] - V::make( VT::zero(), VT::zero(), depth );
    vertices[6] =  vertices[2] - V::make( VT::zero(), VT::zero(), depth );
    vertices[7] =  vertices[3] - V::make( VT::zero(), VT::zero(), depth );
  }
  
  template
  void compute_arch_stone_vertices<MTf>(
                                        MTf::real_type const & theta
                                        , MTf::real_type const & depth
                                        , MTf::real_type const & r_outer
                                        , MTf::real_type const & r_inner
                                        , MTf::vector3_type * vertices
                                        );
  
  template<typename MT>
  void compute_body_to_world_transform(
                                         typename MT::vector3_type      const & T_body2model
                                       , typename MT::quaternion_type const & Q_body2model
                                       , typename MT::vector3_type    const & T_model2local
                                       , typename MT::quaternion_type const & Q_model2local
                                       , typename MT::vector3_type    const & T_local2world
                                       , typename MT::quaternion_type const & Q_local2world
                                       , typename MT::vector3_type          & T_body2world
                                       , typename MT::quaternion_type       & Q_body2world
                                       )
  {
    typedef typename MT::coordsys_type    C;

    C const Xb2m  = C::make( T_body2model,  Q_body2model );
    C const Xm2l  = C::make( T_model2local, Q_model2local);
    C const Xl2w  = C::make( T_local2world, Q_local2world);

    C const Xb2l = prod(Xm2l,  Xb2m);
    C const Xb2w = prod(Xl2w,  Xb2l);

    Q_body2world = Xb2w.Q();
    T_body2world = Xb2w.T();
  }
  template
  void compute_body_to_world_transform<MTf>(
                                       MTf::vector3_type const & body_model_translation
                                       ,MTf::quaternion_type const & body_model_orientation
                                       , MTf::vector3_type const & model_local_translation
                                       , MTf::quaternion_type const & model_local_orientation
                                       , MTf::vector3_type const & local_world_translation
                                       , MTf::quaternion_type const & local_world_orientation
                                       , MTf::vector3_type & body_world_translation
                                       , MTf::quaternion_type & body_world_orientation
                                       );
  
  template<typename MT>
  size_t get_material_id(
                         MaterialInfo<typename MT::real_type> info
                         , std::string const material
                         )
  {
    if( material.compare( "Stone" ) == 0 )
      return info.m_stone_mid;
    
    if( material.compare( "Ground" ) == 0)
      return info.m_ground_mid;
    
    if( material.compare( "Cannonball") == 0)
      return info.m_cannonball_mid;
    
    return 0u;
  }
  
  template
  size_t get_material_id<MTf>(
                              MaterialInfo<MTf::real_type> info
                              , std::string const material
                              );
  
  template<typename MT>
  typename MT::real_type get_material_density(
                                              MaterialInfo<typename MT::real_type> info
                                              , std::string const material
                                              )
  {
    typedef typename MT::value_traits    VT;
    
    if ( material.compare( "Stone" ) == 0)
      return info.m_stone_density;
    
    if ( material.compare( "Ground" ) == 0)
      return info.m_ground_density;
    
    if ( material.compare(  "Cannonball" ) == 0)
      return info.m_cannonball_density;
    
    return VT::zero();
  }
  
  template
  MTf::real_type get_material_density<MTf>(
                                           MaterialInfo<MTf::real_type> info
                                           , std::string const material
                                           );
  
} //namespace procedural
