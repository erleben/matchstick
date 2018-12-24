#ifndef MESH_ARRAY_TRIANGLE_H
#define MESH_ARRAY_TRIANGLE_H

#include <mesh_array_constants.h>

namespace mesh_array 
{
 
  class Triangle
  {
  protected:
    
    size_t        m_i;   ///< Index of the vertex i
    size_t        m_j;   ///< Index of the vertex j
    size_t        m_k;   ///< Index of the vertex k
    size_t        m_idx; ///< The triangle own index
    
  public:
    
    Triangle();

    ~Triangle();
    
    Triangle(size_t const & i, size_t const & j, size_t const & k, size_t const & idx);

    Triangle(Triangle const & o);
       
    bool operator==(Triangle const & o);
    
    Triangle & operator=(Triangle const& o);
    
  public:
    
    size_t const & idx() const;
    size_t const & i()   const;
    size_t const & j()   const;
    size_t const & k()   const;
    
  public:

    bool has_vertex(size_t const & idx) const;

    size_t get_local_index(size_t const & global_idx) const
    {
      if( m_i == global_idx )
        return 0u;

      if( m_j == global_idx )
        return 1u;

      if( m_k == global_idx )
        return 2u;

      return UNASSIGNED();
    }

  };
  
} // end namcespace mesh_array

// MESH_ARRAY_TRIANGLE_H
#endif 
