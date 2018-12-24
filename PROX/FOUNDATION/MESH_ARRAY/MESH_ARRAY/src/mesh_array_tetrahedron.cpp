#include <mesh_array_constants.h>
#include <mesh_array_tetrahedron.h>

namespace mesh_array
{
  
  Tetrahedron::Tetrahedron()
  : m_i(UNASSIGNED())
  , m_j(UNASSIGNED())
  , m_k(UNASSIGNED())
  , m_m(UNASSIGNED())
  , m_idx(UNASSIGNED())
  {
  }
  
  Tetrahedron::~Tetrahedron()
  {
  }
  
  Tetrahedron::Tetrahedron(size_t const & i, size_t const & j, size_t const & k, size_t const & m, size_t const & idx) 
  : m_i(i)
  , m_j(j)
  , m_k(k)
  , m_m(m)
  , m_idx(idx)
  {
  }  
  
  Tetrahedron::Tetrahedron(Tetrahedron const & o) 
  {
    *this = o;
  }
  
  bool Tetrahedron::operator==(Tetrahedron const & o) 
  { 
    return (m_i == o.m_i &&
            m_j == o.m_j &&
            m_k == o.m_k &&
            m_m == o.m_m &&
            m_idx == o.m_idx);
  }
  
  Tetrahedron & Tetrahedron::operator=(Tetrahedron const & o)
  { 
    if(this!= &o)
    {
      m_i   = o.m_i;
      m_j   = o.m_j;
      m_k   = o.m_k;
      m_m   = o.m_m;
      m_idx = o.m_idx;
    }
    return *this; 
  }
  
  size_t const & Tetrahedron::i() const
  {
    return m_i;   
  }
  
  size_t const & Tetrahedron::j() const
  {
    return m_j;   
  }
  
  size_t const & Tetrahedron::k() const
  {
    return m_k;   
  }
  
  size_t const & Tetrahedron::m() const
  {
    return m_m;   
  }
  
  size_t const & Tetrahedron::idx() const
  {
    return m_idx; 
  }
    
  bool Tetrahedron::has_vertex(size_t const & idx) const
  {
    return ( m_i == idx || m_j == idx || m_k == idx || m_m == idx);
  }
  
} // end namespace mesh_array
