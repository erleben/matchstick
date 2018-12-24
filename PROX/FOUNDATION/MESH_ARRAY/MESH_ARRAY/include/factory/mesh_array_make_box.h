#ifndef MESH_ARRAY_MAKE_BOX_H
#define MESH_ARRAY_MAKE_BOX_H

#include <mesh_array_t3mesh.h>
#include <mesh_array_vertex_attribute.h>

namespace mesh_array
{
  
  template<typename MT>
  void make_box(
                typename MT::real_type const & width
                , typename MT::real_type const & height
                , typename MT::real_type const & depth
                , T3Mesh & mesh
                , VertexAttribute<typename MT::real_type,T3Mesh> & X
                , VertexAttribute<typename MT::real_type,T3Mesh> & Y
                , VertexAttribute<typename MT::real_type,T3Mesh> & Z
                );
  
  template<typename MT>
  void make_box(
                typename MT::real_type const & width
                , typename MT::real_type const & height
                , typename MT::real_type const & depth
                , T3Mesh & mesh
                , VertexAttribute<typename MT::vector3_type,T3Mesh> & coords
                );
    
} //namespace mesh_array

// MESH_ARRAY_MAKE_BOX_H
#endif 
