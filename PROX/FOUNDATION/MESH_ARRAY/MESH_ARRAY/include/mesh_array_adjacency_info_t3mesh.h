#ifndef MESH_ARRAY_ADJACENCY_INFO_T3MESH_H
#define MESH_ARRAY_ADJACENCY_INFO_T3MESH_H

#include <mesh_array_constants.h>
#include <mesh_array_adjacency_info.h>
#include <mesh_array_vertex_ring_t3mesh.h>

namespace mesh_array 
{  
  
  template<>
  class AdjacencyInfo<T3Mesh>
  {
  protected:
  
    class NeighborInfo
    {
    public:
      
      size_t m_i;  ///< Index of triangle opposing vertex i.
      size_t m_j;  ///< Index of triangle opposing vertex j.
      size_t m_k;  ///< Index of triangle opposing vertex k.
      
    public:
      
      NeighborInfo()
      : m_i(UNASSIGNED())
      , m_j(UNASSIGNED())
      , m_k(UNASSIGNED())
      {}
      
    };
    
    std::vector<NeighborInfo> m_info;
    
    VertexRing<T3Mesh> const * m_owner;
    
  public:
    
    AdjacencyInfo();

    ~AdjacencyInfo();
    
    AdjacencyInfo(AdjacencyInfo const & A);
    
    AdjacencyInfo operator=(AdjacencyInfo const & A);
    
    AdjacencyInfo(VertexRing<T3Mesh> const & VR);
    
  public:
    
    void release();
    
    void bind(VertexRing<T3Mesh> const & VR);
        
    size_t i(size_t const & idx) const;
    size_t j(size_t const & idx) const;
    size_t k(size_t const & idx) const;

  };
  
} // end namespace mesh_array

// MESH_ARRAY_ADJACENCY_INFO_T3MESH_H
#endif 
