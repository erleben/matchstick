#ifndef MESH_ARRAY_MAKE_CUBOID_H
#define MESH_ARRAY_MAKE_CUBOID_H

#include <mesh_array_t3mesh.h>
#include <mesh_array_vertex_attribute.h>

namespace mesh_array
{
  
  /**
   * The vertices of the eight corners of a cuboid are specified. 
   * The first four vertices are the front face in CCW order. The last four gives
   * the back face vertices in same order as the front face. That is in CW order if
   * one is looking at the front-side of the back face.
   */
  template<typename MT>
  void make_cuboid(
                   typename MT::vector3_type const & v0
                   , typename MT::vector3_type const & v1
                   , typename MT::vector3_type const & v2
                   , typename MT::vector3_type const & v3
                   , typename MT::vector3_type const & v4
                   , typename MT::vector3_type const & v5
                   , typename MT::vector3_type const & v6
                   , typename MT::vector3_type const & v7
                   , T3Mesh & mesh
                   , VertexAttribute<typename MT::real_type,T3Mesh> & X
                   , VertexAttribute<typename MT::real_type,T3Mesh> & Y
                   , VertexAttribute<typename MT::real_type,T3Mesh> & Z
                   );
  
} //namespace mesh_array

// MESH_ARRAY_MAKE_CUBOID_H
#endif 
