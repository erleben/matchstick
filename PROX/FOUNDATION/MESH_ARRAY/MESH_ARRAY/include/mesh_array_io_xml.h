#ifndef MESH_ARRAY_IO_XML_H
#define MESH_ARRAY_IO_XML_H

#include <mesh_array_t4mesh.h>
#include <mesh_array_vertex_attribute.h>

#include <string>

namespace mesh_array
{
  template<typename T>
  void read_xml(
                std::string const & filename
                , T4Mesh & mesh
                , VertexAttribute<T,T4Mesh> & X
                , VertexAttribute<T,T4Mesh> & Y
                , VertexAttribute<T,T4Mesh> & Z
                );
  
  template<typename T>
  void write_xml(
                 std::string const & filename
                 , T4Mesh const & mesh
                 , VertexAttribute<T,T4Mesh> const & X
                 , VertexAttribute<T,T4Mesh> const & Y
                 , VertexAttribute<T,T4Mesh> const & Z
                 );
  
} // namespace mesh_array

//MESH_ARRAY_IO_XML_H
#endif
