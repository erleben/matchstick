#include <mesh_array_constants.h>
#include <mesh_array_edge.h>

namespace mesh_array
{
  
  Edge::Edge()
  : m_i(UNASSIGNED())
  , m_j(UNASSIGNED())
  , m_idx(UNASSIGNED())
  {}
  
  Edge::~Edge(){}
  
  Edge::Edge(size_t const & i, size_t const & j, size_t const & idx)
  : m_i(i)
  , m_j(j)
  , m_idx(idx)
  {}
  
  Edge::Edge(Edge const & e)
  {
    *this = e;
  }
  
  bool Edge::operator==(Edge const & e)
  { 
    return (m_i == e.m_i &&
            m_j == e.m_j &&
            m_idx == e.m_idx);
  }
  
  Edge & Edge::operator=(Edge const & e)
  { 
    if (this != &e)
    {
      m_i   = e.m_i;
      m_j   = e.m_j;
      m_idx = e.m_idx;
    }
    return *this; 
  }
  
  size_t const & Edge::idx() const 
  {
    return m_idx; 
  }
  
  size_t const & Edge::i() const 
  {
    return m_i;   
  }
  
  size_t const & Edge::j() const 
  {
    return m_j;   
  }

  bool Edge::has_vertex(size_t const & idx) const
  {
    return ( m_i == idx || m_j == idx);
  } 

} // end namespace mesh_array
