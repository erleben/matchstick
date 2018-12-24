#ifndef PROCEDURAL_CREATE_GEOMETRY_HANDLE_OBJ_H
#define PROCEDURAL_CREATE_GEOMETRY_HANDLE_OBJ_H

#include <content.h>

#include <procedural_factory_types.h>
#include <procedural_types.h>
#include <procedural_noise.h>

#include <tiny_math_types.h>
#include <tiny_coordsys_functions.h>
#include <mesh_array.h>
#include <mass.h>
#include <util_string_helper.h>

#include <string>

namespace procedural
{

  template<typename MT>
  inline GeometryHandle<MT> create_geometry_handle_obj(
                                                content::API * engine
                                                , std::string const & rel_file_name
                                                , typename MT::real_type const & scale  = typename MT::value_traits::one()
                                                , bool const & blind_copy = false
                                                , mesh_array::TetGenSettings tetset  = mesh_array::tetgen_default_settings()
                                                )
  {
    using std::min;

    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type Q;
    typedef typename MT::value_traits    VT;

    static size_t counter = 0u;

    std::string const geom_name = "obj_" + util::to_string( counter++ );

    detail::MeshData<MT> data;

    mesh_array::read_obj(rel_file_name, data.m_mesh, data.m_X, data.m_Y, data.m_Z);

    if(!blind_copy)
    {
      T max_x = VT::zero();
      T min_x = VT::zero();

      T max_y = VT::zero();
      T min_y = VT::zero();

      T max_z = VT::zero();
      T min_z = VT::zero();

      mesh_array::compute_extents<MT>( data.m_mesh , data.m_X , min_x , max_x );
      mesh_array::compute_extents<MT>( data.m_mesh , data.m_Y , min_y , max_y );
      mesh_array::compute_extents<MT>( data.m_mesh , data.m_Z , min_z , max_z );

      T const x_scale = VT::one()/(max_x-min_x);
      T const y_scale = VT::one()/(max_y-min_y);
      T const z_scale = VT::one()/(max_z-min_z);

      T const uniform_scale = min( min( x_scale, y_scale ), z_scale ) * scale;

      mesh_array::scale<MT>(
                            uniform_scale
                            , uniform_scale
                            , uniform_scale
                            , data.m_mesh
                            , data.m_X
                            , data.m_Y
                            , data.m_Z
                            );
    }

    mass::Properties<T> props_mf = mass::compute_mesh(VT::one(), data.m_mesh.triangle_size(), &data );
    mass::Properties<T> props_bf = mass::translate_to_body_frame(props_mf);
    mass::Properties<T> props    = mass::rotate_to_body_frame(props_mf);

    // Change geometry from model space to body space
    if(!blind_copy)
    {

      V const d = - V::make(props_mf.m_x, props_mf.m_y, props_mf.m_z);
      Q const R =   Q(props.m_Qs,props.m_Qx,props.m_Qy,props.m_Qz);

      mesh_array::translate<MT>( d, data.m_mesh, data.m_X, data.m_Y, data.m_Z);
      mesh_array::rotate<MT>(conj(R), data.m_mesh, data.m_X, data.m_Y, data.m_Z);
    }

    if(blind_copy)
    {
      tetset.m_maximum_volume = 0;
      tetset.m_quality_ratio = 0;
      tetset.m_suppress_splitting = true;
    }

    mesh_array::T4Mesh volume;
    mesh_array::VertexAttribute<T, mesh_array::T4Mesh> volX;
    mesh_array::VertexAttribute<T, mesh_array::T4Mesh> volY;
    mesh_array::VertexAttribute<T, mesh_array::T4Mesh> volZ;

    mesh_array::tetgen(data.m_mesh, data.m_X, data.m_Y, data.m_Z, volume, volX, volY, volZ, tetset);

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

  template<typename MT>
  inline GeometryHandle<MT> create_geometry_handle_obj(
                                                       content::API * engine
                                                       , std::string const & rel_file_name
                                                       , typename MT::real_type const & scale_x
                                                       , typename MT::real_type const & scale_y
                                                       , typename MT::real_type const & scale_z
                                                       , mesh_array::TetGenSettings tetset = mesh_array::tetgen_default_settings()
                                                       )
  {
    using std::min;

    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type Q;
    typedef typename MT::value_traits    VT;

    static size_t counter = 0u;

    std::string const geom_name = "nonuniform_obj_" + util::to_string( counter++ );

    typename detail::MeshData<MT> data;

    mesh_array::read_obj(rel_file_name, data.m_mesh, data.m_X, data.m_Y, data.m_Z);

    mesh_array::scale<MT>(
                            scale_x
                          , scale_y
                          , scale_z
                          , data.m_mesh
                          , data.m_X
                          , data.m_Y
                          , data.m_Z
                          );

    mass::Properties<T> props_mf = mass::compute_mesh(VT::one(), data.m_mesh.triangle_size(), &data );
    mass::Properties<T> props_bf = mass::translate_to_body_frame(props_mf);
    mass::Properties<T> props    = mass::rotate_to_body_frame(props_mf);

    // Change geometry from model space to body space
    V const d = - V::make(props_mf.m_x, props_mf.m_y, props_mf.m_z);
    Q const R =   Q(props.m_Qs,props.m_Qx,props.m_Qy,props.m_Qz);

    mesh_array::translate<MT>( d, data.m_mesh, data.m_X, data.m_Y, data.m_Z);
    mesh_array::rotate<MT>(conj(R), data.m_mesh, data.m_X, data.m_Y, data.m_Z);

    mesh_array::T4Mesh volume;
    mesh_array::VertexAttribute<T, mesh_array::T4Mesh> volX;
    mesh_array::VertexAttribute<T, mesh_array::T4Mesh> volY;
    mesh_array::VertexAttribute<T, mesh_array::T4Mesh> volZ;

    mesh_array::tetgen(data.m_mesh, data.m_X, data.m_Y, data.m_Z, volume, volX, volY, volZ, tetset);

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

} //namespace procedural


// PROCEDURAL_CREATE_GEOMETRY_HANDLE_OBJ_H
#endif
