#include <mesh_array_vertex.h>
#include <mesh_array_constants.h>

namespace mesh_array
{
  
  Vertex::~Vertex(){}
  
  Vertex::Vertex()
  : m_idx(UNASSIGNED())
  {}
  
  Vertex::Vertex( size_t const & idx)
  : m_idx(idx)
  {}
  
  Vertex::Vertex(Vertex const & o) 
  : m_idx(o.m_idx)
  {}
  
  bool Vertex::operator==(Vertex const & o) 
  { 
    return (m_idx == o.m_idx);
  }
  
  Vertex & Vertex::operator=(Vertex const & o)
  { 
    if(this != &o)
    {
      m_idx = o.m_idx;  
    }
    return *this; 
  }
  
  size_t const & Vertex::idx() const
  {
    return m_idx;     
  }
  
} // end namespace mesh_array
