#ifndef MESH_ARRAY_VERTEX_RING_T4MESH_H
#define MESH_ARRAY_VERTEX_RING_T4MESH_H

#include <mesh_array_t4mesh.h>
#include <mesh_array_vertex_ring.h>

#include <vector>

namespace mesh_array
{
  
  template <>
  class VertexRing<T4Mesh>
  {
  public:

    template<typename M>
    friend class VertexRingAccessor;
        
  protected:
    
    T4Mesh const * m_mesh;    ///< Mesh holds tetrahedra 2 vertex mapping as is basic property. 
    
    std::vector< std::pair<size_t, size_t > >  m_V2T;         ///< Keeps vertex to tetrahedra mappings.
    std::vector< size_t >                      m_offset;      ///< Offset of i'th vertex into V2T array
        
  public :

    VertexRing();
    
    ~VertexRing();
    
    VertexRing(VertexRing<T4Mesh> const & o);
               
    VertexRing(T4Mesh const & m);
    
    VertexRing<T4Mesh> & operator=(VertexRing<T4Mesh> const& o);
    
  public: 

    T4Mesh const * mesh() const;
    
    void bind(T4Mesh const & m);
    
    void release();
    
    std::vector<size_t> const & offset() const;
    
    std::vector< std::pair<size_t, size_t > > const & V2T() const;
    
  };
  
}//end of namespace mesh_array

// MESH_ARRAY_VERTEX_RING_T4MESH_H
#endif
