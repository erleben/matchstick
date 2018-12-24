#ifndef MESH_ARRAY_COMPUTE_SURFACE_MAP_H
#define MESH_ARRAY_COMPUTE_SURFACE_MAP_H

#include <mesh_array_t4mesh.h>
#include <mesh_array_vertex_ring_t4mesh.h>
#include <mesh_array_adjacency_info_t4mesh.h>
#include <mesh_array_vertex_attribute.h>
#include <mesh_array_tetrahedron_attribute_t4mesh.h>

namespace mesh_array
{

  class TetrahedronSurfaceInfo
  {
  public:

    bool m_i;   ///<  If true then face opposite vertex i is on the surface
    bool m_j;   ///<  If true then face opposite vertex j is on the surface
    bool m_k;   ///<  If true then face opposite vertex k is on the surface
    bool m_m;   ///<  If true then face opposite vertex m is on the surface

    TetrahedronSurfaceInfo()
    : m_i(false)
    , m_j(false)
    , m_k(false)
    , m_m(false)
    {}

  };

  /**
   * Compute surface map of a tetrahedral mesh.
   *
   * The surface map holds information about what triangle faces
   * that lies on the surface. This is convenient to determine if
   * a tetrahedron is part of the surface of an object and what
   * faces of that tetrahedron are actual triangles on the
   * surface.
   */
  template<typename T>
  inline void compute_surface_map(
                                   T4Mesh const & mesh
                                  , VertexAttribute<T, T4Mesh> const & X
                                  , VertexAttribute<T, T4Mesh> const & Y
                                  , VertexAttribute<T, T4Mesh> const & Z
                                  , TetrahedronAttribute<TetrahedronSurfaceInfo, T4Mesh> & surface_map
                                  )
  {

    VertexRing<T4Mesh>    ring(mesh);
    AdjacencyInfo<T4Mesh> adjacency_info(ring);
    
    surface_map.bind(mesh);

    for ( size_t idx = 0u; idx < mesh.tetrahedron_size(); ++idx)
    {
      size_t const opposite_i  = adjacency_info.i(  idx ) ;
      size_t const opposite_j  = adjacency_info.j(  idx ) ;
      size_t const opposite_k  = adjacency_info.k(  idx ) ;
      size_t const opposite_m  = adjacency_info.m(  idx ) ;
      
      Tetrahedron const tetrahedron = mesh.tetrahedron( idx );

      if ( opposite_i == UNASSIGNED() )
        surface_map(tetrahedron).m_i = true;

      if ( opposite_j == UNASSIGNED() )
        surface_map(tetrahedron).m_j = true;

      if ( opposite_k == UNASSIGNED() )
        surface_map(tetrahedron).m_k = true;

      if ( opposite_m == UNASSIGNED() )
        surface_map(tetrahedron).m_m = true;
    }

  }

}// namespace mesh_array
  
// MESH_ARRAY_COMPUTE_SURFACE_MAP_H
#endif
