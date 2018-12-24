#ifndef MESH_ARRAY_TETRAHEDRON_H
#define MESH_ARRAY_TETRAHEDRON_H

#include <mesh_array_constants.h>

namespace mesh_array 
{
  class Tetrahedron
  {
  protected:
    
    size_t        m_i;   ///< Index of the vertex i
    size_t        m_j;   ///< Index of the vertex j
    size_t        m_k;   ///< Index of the vertex k
    size_t        m_m;   ///< Index of the vertex m
    size_t        m_idx; ///< The tetrahedron own index
    
  public:
    
    Tetrahedron();

    ~Tetrahedron();
    
    Tetrahedron(size_t const & i, size_t const & j, size_t const & k, size_t const & m, size_t const & idx);
        
    Tetrahedron(Tetrahedron const & o);
       
    bool operator==(Tetrahedron const & o);
    
    Tetrahedron & operator=(Tetrahedron const & o);

  public:
    
    size_t const & i()    const;
    size_t const & j()    const;
    size_t const & k()    const;
    size_t const & m()    const;
    size_t const & idx()  const;
    
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

      if( m_m == global_idx )
        return 3u;
      
      return  UNASSIGNED();
    }

  };

} // end namespace mesh_array

// MESH_ARRAY_TETRAHEDRON_H
#endif 
