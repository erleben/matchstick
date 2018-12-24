#ifndef MESH_ARRAY_IO_POLY_H
#define MESH_ARRAY_IO_POLY_H

#include <mesh_array_t3mesh.h>
#include <mesh_array_vertex_attribute.h>

#include <string>

namespace mesh_array
{
  
  template<typename T>
  void write_poly(
                  std::string const & filename
                  , T3Mesh const & mesh
                  , VertexAttribute<T,T3Mesh> const & X
                  , VertexAttribute<T,T3Mesh> const & Y
                  , VertexAttribute<T,T3Mesh> const & Z
                  );
  
} // namespace mesh_array

//MESH_ARRAY_IO_POLY_H
#endif
