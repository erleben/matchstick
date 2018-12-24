#ifndef MESH_ARRAY_IS_POSITIVE_ORIENTATION_H
#define MESH_ARRAY_IS_POSITIVE_ORIENTATION_H

#include <mesh_array_t4mesh.h>
#include <mesh_array_vertex_attribute.h>

namespace mesh_array
{
  
  template<typename T>
  bool is_positive_orientation(
               T4Mesh & mesh
              , VertexAttribute<T,T4Mesh> & X
              , VertexAttribute<T,T4Mesh> & Y
              , VertexAttribute<T,T4Mesh> & Z
              );
  
} // namespace mesh_array

//MESH_ARRAY_IS_POSITIVE_ORIENTATION_H
#endif
