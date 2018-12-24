#include <mesh_array_tetrahedron_attribute_t4mesh.h>

namespace mesh_array 
{
  
//  template<typename T>
//  TetrahedronAttribute<T,T4Mesh>::TetrahedronAttribute()
//  : m_mesh(0)
//  , m_data()
//  {
//  }
//  
//  template<typename T>
//  TetrahedronAttribute<T,T4Mesh>::~TetrahedronAttribute()
//  {
//    this->release();
//  }
//  
//  template<typename T>
//  TetrahedronAttribute<T,T4Mesh>::TetrahedronAttribute( T4Mesh const & m) 
//  {
//    this->bind( m ); 
//  }
//  
//  template<typename T>
//  TetrahedronAttribute<T,T4Mesh>::TetrahedronAttribute(TetrahedronAttribute<T, T4Mesh  > const & o) 
//  {
//    (*this) = o; 
//  }
//  
//  template<typename T>
//  TetrahedronAttribute<T,T4Mesh> const & TetrahedronAttribute<T,T4Mesh>::operator=(TetrahedronAttribute<T, T4Mesh  > const & o)
//  {
//    if( this != &o)
//    {
//      this->m_mesh = o.m_mesh;
//      this->m_data = o.m_data;
//    }
//    return *this; 
//  }
//  
//  template<typename T>
//  T4Mesh const * TetrahedronAttribute<T,T4Mesh>::mesh() const
//  {
//    return this->m_mesh;
//  }
//  
//  template<typename T>
//  void TetrahedronAttribute<T,T4Mesh>::bind(T4Mesh const & m)
//  {
//    this->m_data.resize(m.tetrahedron_capacity());
//    this->m_mesh = &m;
//  }
//  
//  template<typename T>
//  void TetrahedronAttribute<T,T4Mesh>::release()
//  {
//    this->m_mesh = 0;
//    this->m_data.clear();
//  }
//  
//  template<typename T>
//  size_t TetrahedronAttribute<T,T4Mesh>::size() const 
//  {
//    return this->m_data.size(); 
//  }
//  
//  template<typename T>
//  typename TetrahedronAttribute<T,T4Mesh>::const_reference TetrahedronAttribute<T,T4Mesh>::operator()(Tetrahedron const & t) const  
//  { 
//    return this->m_data[t.idx()];  
//  }
//  
//  template<typename T>
//  typename TetrahedronAttribute<T,T4Mesh>::reference TetrahedronAttribute<T,T4Mesh>::operator()(Tetrahedron const & t)        
//  {
//    return this->m_data[t.idx()];  
//  }
//  
//  template<typename T>
//  typename TetrahedronAttribute<T,T4Mesh>::const_reference TetrahedronAttribute<T,T4Mesh>::operator[](Tetrahedron const & t) const  
//  {
//    return this->operator()(t);
//  }
//  
//  template<typename T>
//  typename TetrahedronAttribute<T,T4Mesh>::reference TetrahedronAttribute<T,T4Mesh>::operator[](Tetrahedron const & t)
//  {
//    return this->operator()(t);
//  }

  template class TetrahedronAttribute<bool, T4Mesh>;
  template class TetrahedronAttribute<float, T4Mesh>;
  template class TetrahedronAttribute<double, T4Mesh>;
  template class TetrahedronAttribute<int, T4Mesh>;
  template class TetrahedronAttribute<unsigned int, T4Mesh>;
  
} // end namespace mesh_array 
