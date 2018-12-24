#ifndef MESH_ARRAY_TRIANGLE_ATTRIBUTE_T3MESH_H
#define MESH_ARRAY_TRIANGLE_ATTRIBUTE_T3MESH_H

#include <mesh_array_triangle_attribute.h>
#include <mesh_array_t3mesh.h>

#include <vector>

namespace mesh_array 
{
    
  
  template<typename T>
  class TriangleAttribute< T,T3Mesh>
  {
  public:
    
    typedef typename std::vector<T>::reference       reference;
		typedef typename std::vector<T>::const_reference const_reference;
    
  protected:
    
    T3Mesh  const * m_mesh;
    std::vector<T>  m_data;
    
  public:  
    
    TriangleAttribute();
    
    ~TriangleAttribute();
    
    TriangleAttribute(T3Mesh const & m);
    
    TriangleAttribute(TriangleAttribute<T,T3Mesh> const & o);
    
    TriangleAttribute const & operator=(TriangleAttribute<T,T3Mesh  > const & o);
    
  public:
    
    T3Mesh const * mesh() const;
    
    void bind(T3Mesh const & m);
    
    void release();
    
    size_t size() const;
    
    const_reference operator()(Triangle const & t) const;
    
    reference operator()(Triangle const & t);
    
    const_reference operator[](Triangle const & t) const;
    
    reference operator[](Triangle const & t);
    
  }; 
  
  typedef TriangleAttribute<bool,   T3Mesh> t3_triangle_bool_attribute;
  typedef TriangleAttribute<float,  T3Mesh> t3_triangle_float_attribute;
  typedef TriangleAttribute<int,    T3Mesh> t3_triangle_int_attribute;
  typedef TriangleAttribute<size_t, T3Mesh> t3_triangle_uint_attribute;   
  
} // end namespace mesh_array 

// MESH_ARRAY_TRIANGLE_ATTRIBUTE_T3MESH_H
#endif 
