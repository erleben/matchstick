#ifndef MESH_ARRAY_MAKE_TETRAHEDRON_H
#define MESH_ARRAY_MAKE_TETRAHEDRON_H

#include <mesh_array_t3mesh.h>
#include <mesh_array_vertex_attribute.h>

namespace mesh_array
{

  template<typename MT>
  void make_tetrahedron(
                        typename MT::vector3_type const & p0
                        , typename MT::vector3_type const & p1
                        , typename MT::vector3_type const & p2
                        , typename MT::vector3_type const & p3
                        , T3Mesh & mesh
                        , VertexAttribute<typename MT::real_type,T3Mesh> & X
                        , VertexAttribute<typename MT::real_type,T3Mesh> & Y
                        , VertexAttribute<typename MT::real_type,T3Mesh> & Z
                        );
  
} //namespace mesh_array

// MESH_ARRAY_MAKE_TETRAHEDRON_H
#endif 
