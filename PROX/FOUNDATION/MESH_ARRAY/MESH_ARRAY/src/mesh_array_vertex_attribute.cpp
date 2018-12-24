#include <mesh_array_vertex_attribute.h>

#include <tiny.h>

namespace mesh_array 
{
  template class VertexAttribute<float, T3Mesh>;
  template class VertexAttribute<double, T3Mesh>;
  template class VertexAttribute<int, T3Mesh>;
  template class VertexAttribute<unsigned int, T3Mesh>;
  template class VertexAttribute<bool, T3Mesh>;

  template class VertexAttribute< tiny::Vector<3,tiny::float_traits>,  T3Mesh>;
  template class VertexAttribute< tiny::Vector<3,tiny::double_traits>, T3Mesh>;

  template class VertexAttribute<bool, T4Mesh>;
  template class VertexAttribute<float, T4Mesh>;
  template class VertexAttribute<double, T4Mesh>;
  template class VertexAttribute<int, T4Mesh>;
  template class VertexAttribute<size_t, T4Mesh>;

  template class VertexAttribute< tiny::Vector<3,tiny::float_traits>,  T4Mesh>;
  template class VertexAttribute< tiny::Vector<3,tiny::double_traits>, T4Mesh>;
  
} // end namespace mesh_array 
