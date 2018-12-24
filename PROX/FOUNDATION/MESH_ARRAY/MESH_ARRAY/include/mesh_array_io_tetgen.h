#ifndef MESH_ARRAY_IO_TETGEN_H
#define MESH_ARRAY_IO_TETGEN_H

#include <mesh_array_t3mesh.h>
#include <mesh_array_t4mesh.h>
#include <mesh_array_vertex_attribute.h>

#include <string>


namespace mesh_array 
{
  template<typename T>
  void read_tetgen(
                   std::string const & filename
                   , T3Mesh  & mesh
                   , VertexAttribute<T,T3Mesh> & X
                   , VertexAttribute<T,T3Mesh> & Y
                   , VertexAttribute<T,T3Mesh> & Z
                   );
  
  template<typename T>
  void read_tetgen(
                   std::string const & filename
                   , T4Mesh  & mesh
                   , VertexAttribute<T,T4Mesh> & X
                   , VertexAttribute<T,T4Mesh> & Y
                   , VertexAttribute<T,T4Mesh> & Z
                   );
  
  template<typename T>
  void write_tetgen(
                    std::string const & filename
                    , T3Mesh  const & mesh
                    , VertexAttribute<T,T3Mesh> const & X
                    , VertexAttribute<T,T3Mesh> const & Y
                    , VertexAttribute<T,T3Mesh> const & Z
                    );
  
  template<typename T>
  void write_tetgen(
                    std::string const & filename
                    , T4Mesh  const & mesh
                    , VertexAttribute<T,T4Mesh> const & X
                    , VertexAttribute<T,T4Mesh> const & Y
                    , VertexAttribute<T,T4Mesh> const & Z
                    );
  
} // end namespace mesh_array

// MESH_ARRAY_IO_TETGEN_H
#endif 
