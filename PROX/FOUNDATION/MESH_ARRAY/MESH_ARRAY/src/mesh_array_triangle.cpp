#include <mesh_array_constants.h>
#include <mesh_array_triangle.h>

namespace mesh_array
{
  
  Triangle::Triangle()
  : m_i(UNASSIGNED())
  , m_j(UNASSIGNED())
  , m_k(UNASSIGNED())
  , m_idx(UNASSIGNED())
  {}
  
  Triangle::~Triangle(){}
  
  Triangle::Triangle(size_t const & i, size_t const & j, size_t const & k, size_t const & idx) 
  : m_i(i)
  , m_j(j)
  , m_k(k)
  , m_idx(idx)
  {}
  
  Triangle::Triangle(Triangle const & o) 
  {
    *this = o;
  }
  
  bool Triangle::operator==(Triangle const & o) 
  { 
    return (m_i == o.m_i &&
            m_j == o.m_j &&
            m_k == o.m_k &&
            m_idx == o.m_idx);
  }
  
  Triangle & Triangle::operator=(Triangle const & o)
  { 
    if (this != &o)
    {
      m_i   = o.m_i;
      m_j   = o.m_j;
      m_k   = o.m_k;
      m_idx = o.m_idx;
    }
    return *this; 
  }
  
  size_t const & Triangle::idx() const 
  {
    return m_idx; 
  }
  
  size_t const & Triangle::i() const 
  {
    return m_i;   
  }
  
  size_t const & Triangle::j() const 
  {
    return m_j;   
  }
  
  size_t const & Triangle::k() const 
  {
    return m_k;   
  }
  
  bool Triangle::has_vertex(size_t const & idx) const
  {
    return ( m_i == idx || m_j == idx || m_k == idx);
  } 

} // end namespace mesh_array
