#ifndef MESH_ARRAY_NEIGHBORHOODS_H
#define MESH_ARRAY_NEIGHBORHOODS_H

#include <mesh_array_constants.h>

#include <mesh_array_t4mesh.h>
#include <mesh_array_t3mesh.h>
#include <mesh_array_t2mesh.h>

#include <vector>
#include <algorithm>

namespace mesh_array
{

  /**
   * The offset array index into the V2T mapping such that all
   * triangles of vertex i is given by the index range
   *
   *    V2T[ offset(i) ],... V2T[ offset(i+1) ]
   */
  class Neighborhoods
  {
  public:

    std::vector<std::pair<size_t, size_t> > m_V2T;    ///< This map contains node to triangle mapping. Node indices are stored incrementally
    std::vector<size_t>                     m_offset; ///< Offset(i) tells the startig index in the V2T mapping for vertex i's mapping info.

  public:

    void clear()
    {
      m_V2T.clear();
      m_offset.clear();
    }
    
  };

  namespace details
  {

    inline void scan_for_start_offset(
                                      size_t const & V
                                      , Neighborhoods  & info
                                      )
    {
      size_t const N = info.m_V2T.size();

      info.m_offset.resize( V + 1u, UNASSIGNED() );

      info.m_offset[ info.m_V2T[ 0 ].first ] = 0;
      info.m_offset[ V ]                     = N;

      for(size_t n = 1u; n < N; ++n)
      {
        size_t const cur  = info.m_V2T[ n ].first;
        size_t const prev = info.m_V2T[ n - 1u ].first;

        if (prev != cur)
        {
          info.m_offset[cur] = n;
        }
      }
    }

    inline void skip_isolated_nodes(
                                    size_t const & V
                                    , Neighborhoods  & info
                                    )
    {
      for(size_t v = 1u; v < V+1u; ++v)
      {
        size_t const cur = info.m_offset[v];

        if (cur == UNASSIGNED() )
          continue;

        size_t scan = v-1u;
        size_t left = info.m_offset[scan];

        while(left == UNASSIGNED() )
        {
          info.m_offset[scan] = cur;
          --scan;
          left =  info.m_offset[scan];
        }
      }
    }

  }// end namespace details


  inline void compute_neighbors(
                                T4Mesh const & mesh
                                , Neighborhoods & info
                                )

  {
    size_t const V          = mesh.vertex_size();
    size_t const T          = mesh.tetrahedron_size();
    size_t const N          = T*4u;

    info.m_V2T.resize( N );

    for ( size_t idx = 0u; idx < T; ++idx)
    {
      size_t const i  = mesh.tetrahedron(idx).i();
      size_t const j  = mesh.tetrahedron(idx).j();
      size_t const k  = mesh.tetrahedron(idx).k();
      size_t const m  = mesh.tetrahedron(idx).m();

      info.m_V2T[ idx*4     ].first = i;
      info.m_V2T[ idx*4 + 1 ].first = j;
      info.m_V2T[ idx*4 + 2 ].first = k;
      info.m_V2T[ idx*4 + 3 ].first = m;

      info.m_V2T[ idx*4     ].second = idx;
      info.m_V2T[ idx*4 + 1 ].second = idx;
      info.m_V2T[ idx*4 + 2 ].second = idx;
      info.m_V2T[ idx*4 + 3 ].second = idx;
    }

    std::sort( info.m_V2T.begin(), info.m_V2T.end() );
    details::scan_for_start_offset(V,info);
    details::skip_isolated_nodes(V,info);
  }

  inline void compute_neighbors(
                                T3Mesh const & mesh
                                , Neighborhoods & info
                                )

  {
    size_t const V          = mesh.vertex_size();
    size_t const T          = mesh.triangle_size();
    size_t const N          = T*3u;

    info.m_V2T.resize( N );

    for ( size_t idx = 0u; idx < T; ++idx)
    {
      size_t const i  = mesh.triangle(idx).i();
      size_t const j  = mesh.triangle(idx).j();
      size_t const k  = mesh.triangle(idx).k();

      info.m_V2T[ idx*3     ].first = i;
      info.m_V2T[ idx*3 + 1 ].first = j;
      info.m_V2T[ idx*3 + 2 ].first = k;

      info.m_V2T[ idx*3     ].second = idx;
      info.m_V2T[ idx*3 + 1 ].second = idx;
      info.m_V2T[ idx*3 + 2 ].second = idx;
    }
    std::sort( info.m_V2T.begin(), info.m_V2T.end() );
    details::scan_for_start_offset(V,info);
    details::skip_isolated_nodes(V,info);
  }

  inline void compute_neighbors(
                                T2Mesh const & mesh
                                , Neighborhoods & info)
  {
    size_t const V          = mesh.vertex_size();
    size_t const T          = mesh.edge_size();
    size_t const N          = T*2u;


    info.m_V2T.resize( N );
    
    for ( size_t idx = 0u; idx < T; ++idx)
    {
      size_t const i  = mesh.edge(idx).i();
      size_t const j  = mesh.edge(idx).j();
      
      info.m_V2T[ idx*3     ].first = i;
      info.m_V2T[ idx*3 + 1 ].first = j;
      
      info.m_V2T[ idx*3     ].second = idx;
      info.m_V2T[ idx*3 + 1 ].second = idx;
    }
    
    std::sort( info.m_V2T.begin(), info.m_V2T.end() );
    details::scan_for_start_offset(V,info);
    details::skip_isolated_nodes(V,info);
    
  }

}// namespace mesh_array

//MESH_ARRAY_NEIGHBORHOODS_H
#endif
