#ifndef MESH_ARRAY_VERTEX_H
#define MESH_ARRAY_VERTEX_H

#include <cstddef> // needed for std::size_t

namespace mesh_array 
{  
  
  class Vertex
  {
  protected:
    
    size_t m_idx;      ///< The vertex own index.
    
  public:
    
    Vertex();
    
    ~Vertex();
    
    Vertex(size_t const & idx);
    
    Vertex(Vertex const & o);
    
    bool operator==(Vertex const & o);
    
    Vertex & operator=(Vertex const& o);
    
  public:
      
    size_t const & idx() const;

  };
  
} // end namespace mesh_array

// MESH_ARRAY_VERTEX_H
#endif 
