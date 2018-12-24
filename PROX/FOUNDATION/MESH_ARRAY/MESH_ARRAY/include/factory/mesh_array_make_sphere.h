#ifndef MESH_ARRAY_MAKE_SPHERE_H
#define MESH_ARRAY_MAKE_SPHERE_H

#include <mesh_array_t3mesh.h>
#include <mesh_array_vertex_attribute.h>

namespace mesh_array
{
  
  template<typename MT>
  void make_sphere(
                   typename MT::real_type const & radius
                   , size_t const & slices
                   , size_t const & segments
                   , T3Mesh & mesh
                   , VertexAttribute<typename MT::real_type,T3Mesh> & X
                   , VertexAttribute<typename MT::real_type,T3Mesh> & Y
                   , VertexAttribute<typename MT::real_type,T3Mesh> & Z
                   );
  
} //namespace mesh_array

// MESH_ARRAY_MAKE_SPHERE_H
#endif 
