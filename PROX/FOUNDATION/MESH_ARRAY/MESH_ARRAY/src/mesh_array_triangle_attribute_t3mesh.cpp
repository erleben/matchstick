#include <mesh_array_triangle_attribute_t3mesh.h>

namespace mesh_array 
{
  
  template<typename T>
  TriangleAttribute<T,T3Mesh>::TriangleAttribute()
  : m_mesh(0)
  , m_data()
  {
  }
  
  template<typename T>
  TriangleAttribute<T,T3Mesh>::~TriangleAttribute()
  {
    this->release();
  }
  
  template<typename T>
  TriangleAttribute<T,T3Mesh>::TriangleAttribute(T3Mesh const & m) 
  { 
    this->bind( m ); 
  }
  
  template<typename T>
  TriangleAttribute<T,T3Mesh>::TriangleAttribute(TriangleAttribute<T,T3Mesh> const & o) 
  {
    (*this) = o; 
  }
  
  template<typename T>
  TriangleAttribute<T,T3Mesh> const & TriangleAttribute<T,T3Mesh>::operator=(TriangleAttribute<T, T3Mesh> const & o)
  {
    if( this != &o)
    {
      this->m_mesh = o.m_mesh;
      this->m_data = o.m_data;
    }
    return *this; 
  }
  
  template<typename T>
  T3Mesh const * TriangleAttribute<T,T3Mesh>::mesh() const
  {
    return this->m_mesh;
  }
  
  template<typename T>
  void TriangleAttribute<T,T3Mesh>::bind(T3Mesh const & m)
  {
    this->release();
    
    this->m_data.resize(m.triangle_capacity());
    this->m_mesh = &m;
  }
  
  template<typename T>
  void TriangleAttribute<T,T3Mesh>::release()
  {
    this->m_mesh = 0;
    this->m_data.clear();
  }
  
  template<typename T>
  size_t TriangleAttribute<T,T3Mesh>::size() const 
  { 
    return this->m_data.size(); 
  }
  
  template<typename T>
  typename TriangleAttribute<T,T3Mesh>::const_reference TriangleAttribute<T,T3Mesh>::operator()(Triangle const & t) const  
  { 
    return this->m_data[t.idx()];  
  }
  
  template<typename T>
  typename TriangleAttribute<T,T3Mesh>::reference TriangleAttribute<T,T3Mesh>::operator()(Triangle const & t)
  {
    return this->m_data[t.idx()];  
  }
  
  template<typename T>
  typename TriangleAttribute<T,T3Mesh>::const_reference TriangleAttribute<T,T3Mesh>::operator[](Triangle const & t) const
  {
    return this->operator()(t);  
  }
  
  template<typename T>
  typename TriangleAttribute<T,T3Mesh>::reference TriangleAttribute<T,T3Mesh>::operator[](Triangle const & t)
  {
    return this->operator()(t);
  }

  template class TriangleAttribute<bool, T3Mesh>;
  template class TriangleAttribute<float, T3Mesh>;
  template class TriangleAttribute<double, T3Mesh>;
  template class TriangleAttribute<int, T3Mesh>;
  template class TriangleAttribute<unsigned int, T3Mesh>;
  
} // end namespace mesh_array 
