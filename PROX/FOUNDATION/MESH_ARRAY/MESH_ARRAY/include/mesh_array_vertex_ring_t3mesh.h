#ifndef MESH_ARRAY_VERTEX_RING_T3MESH_H
#define MESH_ARRAY_VERTEX_RING_T3MESH_H

#include <mesh_array_t3mesh.h>
#include <mesh_array_vertex_ring.h>

#include <vector>

namespace mesh_array
{
  
  /**
   * Triangle mesh array data structure specialized for a static mesh.
   */
  template<>
  class VertexRing<T3Mesh>
  {
  public:
    
    template<typename M>
    friend class VertexRingAccessor;
        
  protected:
    
    T3Mesh   const * m_mesh;    ///< Mesh holds triangle 2 vertex mapping as is basic property. 
    
    std::vector< std::pair<size_t, size_t > >  m_V2T;         ///< Keeps vertex to triangle mappings.
    std::vector< size_t >                      m_offset;     ///< Offset of i'th vertex into V2T array
        
  public :

    VertexRing();
    
    ~VertexRing();
    
    VertexRing(VertexRing<T3Mesh> const & o);
               
    VertexRing(T3Mesh  const & m);
    
    VertexRing<T3Mesh> & operator=(VertexRing<T3Mesh> const& o);
    
  public: 

    T3Mesh const * mesh() const;
    
    void bind(T3Mesh const & m);
    
    void release();
    
    std::vector< size_t > const & offset() const;
    
    std::vector< std::pair<size_t, size_t > > const & V2T() const;
    
  };
  
}//end of namespace mesh_array

// MESH_ARRAY_VERTEX_RING_T3MESH_H
#endif
