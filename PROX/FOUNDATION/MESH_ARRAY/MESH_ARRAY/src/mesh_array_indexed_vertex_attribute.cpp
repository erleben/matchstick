#include <mesh_array_indexed_vertex_attribute.h>
#include <mesh_array_t3mesh.h>
#include <mesh_array_t4mesh.h>

#include <tiny_math_types.h>

namespace mesh_array
{
  template<typename V, typename M>
  IndexedVertexAttribute<V,M>::~IndexedVertexAttribute()
  {
  }
  
  template<typename V, typename M>
  IndexedVertexAttribute<V,M>::IndexedVertexAttribute(size_t const & idx, VertexAttribute<V,M> & data)
  : m_data( data )
  , m_idx( idx )
  {
  }
  
  template<typename V, typename M>
  IndexedVertexAttribute<V,M>::IndexedVertexAttribute(IndexedVertexAttribute<V,M> const & o)
  : m_data( o.m_data )
  , m_idx( o.m_idx )
  {
  }
  
  template<typename V, typename M>
  IndexedVertexAttribute<V,M> const & IndexedVertexAttribute<V,M>::operator=(IndexedVertexAttribute<V,M> const & o)
  {
    if(this != &o)
    {
      this->m_data = o.m_data;
      this->m_idx  = o.m_idx;
    }
    return *this;
  }
  
  template<typename V, typename M>
  M const * IndexedVertexAttribute<V,M>::mesh() const
  {
    return this->m_data.mesh();
  }
  
  template<typename V, typename M>
  void IndexedVertexAttribute<V,M>::bind(M const & m)
  {
    this->m_data.bind(m);
  }
  
  template<typename V, typename M>
  void IndexedVertexAttribute<V,M>::release()
  {
    this->m_data.release();
  }
  
  template<typename V, typename M>
  size_t IndexedVertexAttribute<V,M>::size() const
  {
    return this->m_data.size();
  }
  
  template<typename V, typename M>
  typename V::real_type const & IndexedVertexAttribute<V,M>::operator()(Vertex const & v) const
  {
    return this->m_data(v)[this->m_idx];
  }
  
  template<typename V, typename M>
  typename V::real_type & IndexedVertexAttribute<V,M>::operator()(Vertex const & v)
  {
    return this->m_data(v)[this->m_idx];
  }
  
  template<typename V, typename M>
  typename V::real_type const & IndexedVertexAttribute<V,M>::operator[](Vertex const & v) const
  {
    return this->m_data[v][this->m_idx];
  }
  
  template<typename V, typename M>
  typename V::real_type & IndexedVertexAttribute<V,M>::operator[](Vertex const & v)
  {
    return this->m_data[v][this->m_idx];
  }

  
  typedef tiny::MathTypes<float>  MTf;
  typedef tiny::MathTypes<double> MTd;
  
  template class IndexedVertexAttribute<MTf::vector3_type, T3Mesh>;
  template class IndexedVertexAttribute<MTf::vector3_type, T4Mesh>;
  template class IndexedVertexAttribute<MTd::vector3_type, T3Mesh>;
  template class IndexedVertexAttribute<MTd::vector3_type, T4Mesh>;
  
} // end namespace mesh_array
