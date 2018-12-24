#include <mesh_array_triangle_circulator.h>

namespace mesh_array
{
  
  TriangleCirculator<T3Mesh>::TriangleCirculator()
  {
  }
  
  TriangleCirculator<T3Mesh>::~TriangleCirculator()
  {
  }
  
  TriangleCirculator<T3Mesh>::TriangleCirculator(TriangleCirculator const & o) 
  {
    (*this) = o;
  }
  
  TriangleCirculator<T3Mesh>::TriangleCirculator(VertexRing<T3Mesh> const & VR, Vertex const & v) 
  {    
    this->m_owner = &VR;
    
    if(this->m_owner->offset().size()> 0u)
    {
      this->m_cur = this->m_owner->offset()[ v.idx()       ];
      this->m_end = this->m_owner->offset()[ v.idx() + 1u  ];
    }
    else
    {
      this->m_cur = 0u;
      this->m_end = 0u;
    }
  }
  
  bool TriangleCirculator<T3Mesh>::operator==(TriangleCirculator const & o) const 
  { 
    return (this->m_owner == o.m_owner) && (this->m_cur == o.m_cur); 
  }
  
  TriangleCirculator<T3Mesh> const & TriangleCirculator<T3Mesh>::operator=(TriangleCirculator<T3Mesh> const & o)
  {
    if( this != &o)
    {
      this->m_owner = o.m_owner;
      this->m_cur   = o.m_cur;
      this->m_end   = o.m_end;
    }
    return *this; 
  }
  
  Triangle const * TriangleCirculator<T3Mesh>::operator->() const 
  {
    return &(this->operator*());
  }
  
  Triangle * TriangleCirculator<T3Mesh>::operator->()
  {
    return &(this->operator*());
  }
  
  Triangle const & TriangleCirculator<T3Mesh>::operator*() const 
  {
    if(this->m_cur < this->m_end)
    {
      size_t const idx = this->m_owner->V2T()[this->m_cur].second;      
      return this->m_owner->mesh()->triangle( idx );
    }
    
    return this->m_undefined;
  }
  
  Triangle & TriangleCirculator<T3Mesh>::operator*()     
  {
    if(this->m_cur < this->m_end)
    {
      size_t const idx = this->m_owner->V2T()[this->m_cur].second;      
      return const_cast< T3Mesh  * >( this->m_owner->mesh())->triangle( idx );
    }
    
    return this->m_undefined;
  }
  
  TriangleCirculator<T3Mesh> &  TriangleCirculator<T3Mesh>::operator++()
  {
    ++(this->m_cur);
    return (*this);
  }
  
  bool TriangleCirculator<T3Mesh>::operator()() const 
  {
    return this->m_cur < this->m_end; 
  }
  
} // end namespace mesh_array
