#ifndef MESH_ARRAY_TETRAHEDRON_ATTRIBUTE_T4MESH_H
#define MESH_ARRAY_TETRAHEDRON_ATTRIBUTE_T4MESH_H

#include <mesh_array_tetrahedron_attribute.h>
#include <mesh_array_t4mesh.h>

#include <vector>

namespace mesh_array 
{
  
  template<typename T>
  class TetrahedronAttribute<T,T4Mesh>
  {
  public:
    
    typedef typename std::vector<T>::reference       reference;
		typedef typename std::vector<T>::const_reference const_reference;
    
  protected:
    
    T4Mesh       const * m_mesh;    
    std::vector<T>       m_data;
    
  public:  
    
//    TetrahedronAttribute();
//    
//    ~TetrahedronAttribute();
//    
//    TetrahedronAttribute(T4Mesh const & m);
//    
//    TetrahedronAttribute(TetrahedronAttribute<T,T4Mesh> const & o);
//    
//    TetrahedronAttribute const & operator=(TetrahedronAttribute<T,T4Mesh> const & o);
//    
//  public:
//    
//    T4Mesh const * mesh() const;
//    
//    void bind(T4Mesh const & m);
//    
//    void release();
//    
//    size_t size() const;
//    
//    const_reference operator()(Tetrahedron const & t) const;
//    
//    reference operator()(Tetrahedron const & t);
//    
//    const_reference operator[](Tetrahedron const & t) const;
//    
//    reference operator[](Tetrahedron const & t);
//
    
    
    
    TetrahedronAttribute()
    : m_mesh(0)
    , m_data()
    {
    }
    
    ~TetrahedronAttribute()
    {
      this->release();
    }
    
    TetrahedronAttribute( T4Mesh const & m)
    {
      this->bind( m );
    }
    
    TetrahedronAttribute(TetrahedronAttribute<T, T4Mesh  > const & o)
    {
      (*this) = o;
    }
    
    TetrahedronAttribute<T,T4Mesh> const & operator=(TetrahedronAttribute<T, T4Mesh  > const & o)
    {
      if( this != &o)
      {
        this->m_mesh = o.m_mesh;
        this->m_data = o.m_data;
      }
      return *this;
    }
    
    T4Mesh const * mesh() const
    {
      return this->m_mesh;
    }
    
    void bind(T4Mesh const & m)
    {
      this->m_data.resize(m.tetrahedron_capacity());
      this->m_mesh = &m;
    }
    
    void release()
    {
      this->m_mesh = 0;
      this->m_data.clear();
    }
    
    size_t size() const
    {
      return this->m_data.size();
    }
    
    const_reference operator()(Tetrahedron const & t) const
    {
      return this->m_data[t.idx()];
    }
    
    reference operator()(Tetrahedron const & t)
    {
      return this->m_data[t.idx()];
    }
    
    const_reference operator[](Tetrahedron const & t) const
    {
      return this->operator()(t);
    }
    
    reference operator[](Tetrahedron const & t)
    {
      return this->operator()(t);
    }
    
    
  };
  
  typedef TetrahedronAttribute<float,  T4Mesh> t4_tetrahedron_float_attribute;
  typedef TetrahedronAttribute<int,    T4Mesh> t4_tetrahedron_int_attribute;
  typedef TetrahedronAttribute<size_t, T4Mesh> t4_tetrahedron_uint_attribute;
  typedef TetrahedronAttribute<bool,   T4Mesh>  t4_tetrahedron_bool_attribute;
  
} // end namespace mesh_array 

// MESH_ARRAY_TETRAHEDRON_ATTRIBUTE_T4MESH_H
#endif 
