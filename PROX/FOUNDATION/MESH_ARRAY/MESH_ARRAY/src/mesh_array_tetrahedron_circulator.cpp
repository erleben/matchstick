#include <mesh_array_tetrahedron_circulator.h>

namespace mesh_array
{
  
  TetrahedronCirculator<T4Mesh>::TetrahedronCirculator()
  {
  }
  
  TetrahedronCirculator<T4Mesh>::~TetrahedronCirculator()
  {
  }
  
  TetrahedronCirculator<T4Mesh>::TetrahedronCirculator(TetrahedronCirculator const & o) 
  {
    (*this) = o;
  }
  
  TetrahedronCirculator<T4Mesh>::TetrahedronCirculator(VertexRing<T4Mesh> const & VR, Vertex const & v) 
  {    
    this->m_owner = &VR;
    
    if(this->m_owner->offset().size()>0u)
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
  
  bool TetrahedronCirculator<T4Mesh>::operator==(TetrahedronCirculator const & o) const 
  { 
    return (this->m_owner == o.m_owner) && (this->m_cur == o.m_cur); 
  }
  
  TetrahedronCirculator<T4Mesh> const & TetrahedronCirculator<T4Mesh>::operator=(TetrahedronCirculator<T4Mesh> const & o)
  {
    if( this != &o)
    {
      this->m_owner = o.m_owner;
      this->m_cur   = o.m_cur;
      this->m_end   = o.m_end;
    }
    return *this; 
  }
  
  Tetrahedron const * TetrahedronCirculator<T4Mesh>::operator->() const 
  {
    return &(this->operator*());
  }
  
  Tetrahedron * TetrahedronCirculator<T4Mesh>::operator->()
  {
    return &(this->operator*());
  }
  
  Tetrahedron const & TetrahedronCirculator<T4Mesh>::operator*() const 
  {    
    if(this->m_cur < this->m_end)
    {
      size_t const idx = this->m_owner->V2T()[this->m_cur].second;      
      return this->m_owner->mesh()->tetrahedron( idx );
    }
    return this->m_undefined;
  }
  
  Tetrahedron & TetrahedronCirculator<T4Mesh>::operator*()
  {    
    if(this->m_cur < this->m_end)
    {
      size_t const idx = this->m_owner->V2T()[this->m_cur].second;      
      return const_cast< T4Mesh  * >(this->m_owner->mesh())->tetrahedron( idx );
    }
    return this->m_undefined;
  }
  
  TetrahedronCirculator<T4Mesh> & TetrahedronCirculator<T4Mesh>::operator++()
  {
    ++(this->m_cur);    
    return (*this);
  }
  
  bool TetrahedronCirculator<T4Mesh>::operator()() const 
  {
    return this->m_cur < this->m_end; 
  }
  
} // end namespace mesh_array
