#ifndef KDOP_MESH_REORDER_H
#define KDOP_MESH_REORDER_H

#include <mesh_array_t4mesh.h>
#include <mesh_array_constants.h>
#include <mesh_array_adjacency_info_t4mesh.h>
#include <mesh_array_vertex_ring_t4mesh.h>
#include <mesh_array_tetrahedron_attribute_t4mesh.h>
#include <mesh_array_vertex_attribute.h>

#include <algorithm>
#include <cmath>
#include <limits>
#include <queue>

namespace kdop
{
  namespace details
  {

    struct mesh_reorder_breadth_first {};

    struct mesh_reorder_morton {};
    
    inline bool compare_morton_codes( std::pair<mesh_array::Tetrahedron, unsigned long long> a
                                    , std::pair<mesh_array::Tetrahedron, unsigned long long> b
                                    )
    {
      return a.second < b.second;
    }
  }
  
  template<typename T>
  inline void mesh_reorder( mesh_array::T4Mesh const & M_in
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & X_in
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Y_in
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Z_in
                          , mesh_array::T4Mesh & M_out
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> & X_out
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> & Y_out
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> & Z_out
                          , details::mesh_reorder_breadth_first const & /* tag */
                          )
  {
    using namespace mesh_array;
    
    VertexRing<T4Mesh>    ring(M_in);
    AdjacencyInfo<T4Mesh> A(ring);
    
    M_out.clear();
    
    M_out.set_capacity( M_in.vertex_capacity(), M_in.tetrahedron_capacity());
    
    X_out.bind(M_out);
    Y_out.bind(M_out);
    Z_out.bind(M_out);
    
    t4_tetrahedron_bool_attribute seen(M_in);
    for(size_t i = 0u;i<M_in.tetrahedron_size(); ++i)
      seen(  M_in.tetrahedron(i) ) = false;
    
    std::vector<Vertex> lookup;
    lookup.resize( M_in.vertex_size() );
    
    std::queue<size_t> Q;
    Q.push( 0 );   // 2013-05-19 Kenny: Maybe find tetrahedron with smallest coordinate?
    seen(M_in.tetrahedron(0)) = true;
    
    // 2013-05-24 Sarah: Not sure that this is the right solution,
    //            but without it we 'overfill' the output mesh? with
    //            it it seems to still produce the right mesh (at least
    //            for a mesh small enough to manually inspect)

    while(!Q.empty() )
    {
      size_t      const idx = Q.front(); Q.pop();
      Tetrahedron const Tet = M_in.tetrahedron(idx);
      
      size_t const idx_adj_i  = A.i(  idx ) ;
      size_t const idx_adj_j  = A.j(  idx ) ;
      size_t const idx_adj_k  = A.k(  idx ) ;
      size_t const idx_adj_m  = A.m(  idx ) ;
              
      if ( idx_adj_i != UNASSIGNED())
      {
        Tetrahedron const Tet_i = M_in.tetrahedron(idx_adj_i);
        if (!seen(Tet_i))
        {
          seen(Tet_i) = true;
          Q.push(idx_adj_i);
        }
      }
      
      if ( idx_adj_j != UNASSIGNED())
      {
        Tetrahedron const Tet_j = M_in.tetrahedron(idx_adj_j);
        if (!seen(Tet_j))
        {
          seen(Tet_j) = true;
          Q.push(idx_adj_j);
        }
      }
      
      if ( idx_adj_k != UNASSIGNED())
      {
        Tetrahedron const Tet_k = M_in.tetrahedron(idx_adj_k);
        if (!seen(Tet_k))
        {
          seen(Tet_k) = true;
          Q.push(idx_adj_k);
        }
      }
      
      if ( idx_adj_m != UNASSIGNED())
      {
        Tetrahedron const Tet_m = M_in.tetrahedron(idx_adj_m);
        if (!seen(Tet_m))
        {
          seen(Tet_m) = true;
          Q.push(idx_adj_m);
        }
      }
      
      Vertex vi  =   lookup[ Tet.i() ];
      if (vi.idx() == UNASSIGNED())
      {
        vi = M_out.push_vertex();
        lookup[ Tet.i() ] = vi;

        X_out[ vi.idx() ]  = X_in[ Tet.i() ];
        Y_out[ vi.idx() ]  = Y_in[ Tet.i() ];
        Z_out[ vi.idx() ]  = Z_in[ Tet.i() ];
      }
      
      Vertex vj  =   lookup[ Tet.j() ];
      if (vj.idx() == UNASSIGNED())
      {
        vj = M_out.push_vertex();
        lookup[ Tet.j() ] = vj;

        X_out[ vj.idx() ]  = X_in[ Tet.j() ];
        Y_out[ vj.idx() ]  = Y_in[ Tet.j() ];
        Z_out[ vj.idx() ]  = Z_in[ Tet.j() ];
      }
      
      Vertex vk  =   lookup[ Tet.k() ];
      if (vk.idx() == UNASSIGNED())
      {
        vk = M_out.push_vertex();
        lookup[ Tet.k() ] = vk;

        X_out[ vk.idx() ]  = X_in[ Tet.k() ];
        Y_out[ vk.idx() ]  = Y_in[ Tet.k() ];
        Z_out[ vk.idx() ]  = Z_in[ Tet.k() ];
      }
      
      Vertex vm  =   lookup[ Tet.m() ];
      if (vm.idx() == UNASSIGNED())
      {
        vm = M_out.push_vertex();
        lookup[ Tet.m() ] = vm;

        X_out[ vm.idx() ]  = X_in[ Tet.m() ];
        Y_out[ vm.idx() ]  = Y_in[ Tet.m() ];
        Z_out[ vm.idx() ]  = Z_in[ Tet.m() ];
      }
      
      M_out.push_tetrahedron(vi,vj,vk,vm);
    }
  }
  
  template<typename T>
  inline void mesh_reorder( mesh_array::T4Mesh const & M_in
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & X_in
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Y_in
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Z_in
                          , mesh_array::T4Mesh & M_out
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> & X_out
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> & Y_out
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> & Z_out
                          , details::mesh_reorder_morton const & /* tag */
                          )
  {
    using std::min;
    using std::max;
    using namespace mesh_array;
    
    //--- Collect centroids for all tetrahedra ---------------------------------
    //--- and remember min and max centroid coordinates ------------------------
    TetrahedronAttribute<T, T4Mesh> centroids_X(M_in);
    TetrahedronAttribute<T, T4Mesh> centroids_Y(M_in);
    TetrahedronAttribute<T, T4Mesh> centroids_Z(M_in);

    T min_X = std::numeric_limits<T>::max();
    T min_Y = std::numeric_limits<T>::max();
    T min_Z = std::numeric_limits<T>::max();
    T max_X = std::numeric_limits<T>::min();
    T max_Y = std::numeric_limits<T>::min();
    T max_Z = std::numeric_limits<T>::min();
    
    for(size_t i = 0; i < M_in.tetrahedron_size(); ++i)
    {
      Tetrahedron const tet = M_in.tetrahedron(i);

      centroids_X[ tet ] = 0.25 * (
              X_in[ tet.i() ] +
              X_in[ tet.j() ] +
              X_in[ tet.k() ] +
              X_in[ tet.m() ]
      );

      centroids_Y[ tet ] = 0.25 * (
              Y_in[ tet.i() ] +
              Y_in[ tet.j() ] +
              Y_in[ tet.k() ] +
              Y_in[ tet.m() ]
      );

      centroids_Z[ tet ] = 0.25 * (
              Z_in[ tet.i() ] +
              Z_in[ tet.j() ] +
              Z_in[ tet.k() ] +
              Z_in[ tet.m() ]
      );

      min_X = min(min_X, centroids_X[ tet ]);
      min_Y = min(min_Y, centroids_Y[ tet ]);
      min_Z = min(min_Z, centroids_Z[ tet ]);
      max_X = max(max_X, centroids_X[ tet ]);
      max_Y = max(max_Y, centroids_Y[ tet ]);
      max_Z = max(max_Z, centroids_Z[ tet ]);
    }

    //--- Discretize the centroids' coordinates into K-bit integers ------------
    //--- because we need to combine them into one 3K bit integer to sort on ---
    int const K = (sizeof(unsigned long long) * 8) / 3;

    int scale = ((1 << K) - 1) / (max_X - min_X);

    scale = min(scale, static_cast<int>(
                                        (((1 << K) - 1) / (max_Y - min_Y))
                                        )
                );

    scale = min(scale, static_cast<int>(
                                        (((1 << K) - 1) / (max_Z - min_Z))
                                        )
                );

    for(size_t i = 0; i < M_in.tetrahedron_size(); ++i)
    {
      Tetrahedron const tet = M_in.tetrahedron(i);

      centroids_X[ tet ] = static_cast<int>( ((centroids_X[ tet ] - min_X) * scale) );
      centroids_Y[ tet ] = static_cast<int>( ((centroids_Y[ tet ] - min_Y) * scale) );
      centroids_Z[ tet ] = static_cast<int>( ((centroids_Z[ tet ] - min_Z) * scale) );
    }


    //--- Now all centroids' coordinates are integers between 0 and 2^K
    //--- i.e. they're spread out in a 2^K x 2^K x 2^K cube
    //---
    //--- let's interleave them into a long integer per tetrahedron, so we
    //--- can use it to create the Morton ordering
    std::vector< std::pair<Tetrahedron, unsigned long long> > morton_codes(M_in.tetrahedron_size());

    for(size_t i = 0; i < M_in.tetrahedron_size(); ++i)
    {
      Tetrahedron const tet = M_in.tetrahedron(i);
      
      unsigned long long x = (unsigned long long) centroids_X[ tet ];
      unsigned long long y = (unsigned long long) centroids_Y[ tet ];
      unsigned long long z = (unsigned long long) centroids_Z[ tet ];
      unsigned long long morton_code = 0;

      for(unsigned long k = 0; k < K; ++k)
      {
        // interleave the k'th bits: x, y, z
        morton_code |= ((x >> k) & 1) << (3 * k);
        morton_code |= ((y >> k) & 1) << (3 * k + 1);
        morton_code |= ((z >> k) & 1) << (3 * k + 2);
      }

      morton_codes[ i ] = std::make_pair(tet, morton_code);
    }
            
    //--- Sort according to Morton codes ---------------------------------------
    std::sort(
              morton_codes.begin()
              , morton_codes.end()
              , details::compare_morton_codes
              );
    
    //--- Now simply push out tetrahedra and vertices in that order ------------
    M_out.clear();
    M_out.set_capacity(M_in.vertex_capacity(), M_in.tetrahedron_capacity());
    X_out.bind(M_out);
    Y_out.bind(M_out);
    Z_out.bind(M_out);
    
    std::vector<Vertex> lookup(M_in.vertex_size());

    for(size_t i = 0; i < morton_codes.size(); ++i)
    {
      Tetrahedron const tet = morton_codes[i].first;

      Vertex vi = lookup[ tet.i() ];

      if (vi.idx() == UNASSIGNED())
      {
        vi = M_out.push_vertex();
        lookup[ tet.i() ] = vi;
        X_out[ vi.idx() ] = X_in[ tet.i() ];
        Y_out[ vi.idx() ] = Y_in[ tet.i() ];
        Z_out[ vi.idx() ] = Z_in[ tet.i() ];
      }
      
      Vertex vj = lookup[ tet.j() ];

      if (vj.idx() == UNASSIGNED())
      {
        vj = M_out.push_vertex();
        lookup[ tet.j() ] = vj;
        X_out[ vj.idx() ] = X_in[ tet.j() ];
        Y_out[ vj.idx() ] = Y_in[ tet.j() ];
        Z_out[ vj.idx() ] = Z_in[ tet.j() ];
      }
      
      Vertex vk = lookup[ tet.k() ];

      if (vk.idx() == UNASSIGNED())
      {
        vk = M_out.push_vertex();
        lookup[ tet.k() ] = vk;
        X_out[ vk.idx() ] = X_in[ tet.k() ];
        Y_out[ vk.idx() ] = Y_in[ tet.k() ];
        Z_out[ vk.idx() ] = Z_in[ tet.k() ];
      }
      
      Vertex vm = lookup[ tet.m() ];

      if (vm.idx() == UNASSIGNED())
      {
        vm = M_out.push_vertex();
        lookup[ tet.m() ] = vm;
        X_out[ vm.idx() ]  = X_in[ tet.m() ];
        Y_out[ vm.idx() ]  = Y_in[ tet.m() ];
        Z_out[ vm.idx() ]  = Z_in[ tet.m() ];
      }
      
      M_out.push_tetrahedron(vi, vj, vk, vm);
    }
  }
  
  /**
   *
   * A subset of tetrahedra does not necessarily form a contigous chunk  of memory.... this
   * implies we need to re-order the tetrahedra, the same goes for  vertices.... if
   * not we will be jumping randomly around in memory...
   *
   * Properly the best is re-order a tetramesh to make it memory friendly, traversing the mesh in 
   * a breadth first manner
   */
  template<typename T>
  inline void mesh_reorder( mesh_array::T4Mesh const & M_in
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & X_in
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Y_in
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Z_in
                          , mesh_array::T4Mesh & M_out
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> & X_out
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> & Y_out
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> & Z_out
                          )
  {
    //--- Default to breadth-first reordering of mesh --------------------------
    mesh_reorder<T>(
                    M_in
                    , X_in
                    , Y_in
                    , Z_in
                    , M_out
                    , X_out
                    , Y_out
                    , Z_out
                    , details::mesh_reorder_morton()
                    );
  }
  
}// namespace kdop

// KDOP_MESH_REORDER_H
#endif
