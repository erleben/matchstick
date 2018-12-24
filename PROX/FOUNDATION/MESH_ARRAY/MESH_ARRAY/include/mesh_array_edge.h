#ifndef MESH_ARRAY_EDGE_H
#define MESH_ARRAY_EDGE_H

#include <mesh_array_constants.h>

namespace mesh_array 
{
 
  class Edge
  {
  protected:
    
    size_t        m_i;   ///< Index of the vertex i
    size_t        m_j;   ///< Index of the vertex j
    size_t        m_idx; ///< The triangle own index
    
  public:
    
    Edge();

    ~Edge();
    
    Edge(size_t const & i, size_t const & j, size_t const & idx);

    Edge(Edge const & e);
       
    bool operator==(Edge const & e);
    
    Edge & operator=(Edge const& e);
    
  public:
    
    size_t const & idx() const;
    size_t const & i()   const;
    size_t const & j()   const;

  public:

    bool has_vertex(size_t const & idx) const;

    size_t get_local_index(size_t const & global_idx) const
    {
      if( m_i == global_idx )
        return 0u;

      if( m_j == global_idx )
        return 1u;

      return UNASSIGNED();
    }

  };
  
} // end namcespace mesh_array

// MESH_ARRAY_EDGE_H
#endif 
