#ifndef KDOP_TANDEM_TRAVERSAL_H
#define KDOP_TANDEM_TRAVERSAL_H

#include <kdop_test_pair.h>
#include <kdop_tree.h>
#include <kdop_select_contact_point_algorithm.h>

#include <mesh_array.h>

#include <util_profiling.h>

namespace kdop
{
  namespace details
  {
    
    template< typename V, size_t K, typename T>
    inline void traversal(
                          size_t const & node_idx_A
                          , SubTree<T,K> const & branch_A
                          , mesh_array::T4Mesh const & mesh_A
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & X_A
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Y_A
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Z_A
                          , mesh_array::TetrahedronAttribute<mesh_array::TetrahedronSurfaceInfo,mesh_array::T4Mesh> const & surface_map_A
                          , mesh_array::VertexAttribute<V,mesh_array::T4Mesh> const & Sa
                          , size_t const & node_idx_B
                          , SubTree<T,K> const & branch_B
                          , mesh_array::T4Mesh const & mesh_B
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & X_B
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Y_B
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Z_B
                          , mesh_array::TetrahedronAttribute<mesh_array::TetrahedronSurfaceInfo,mesh_array::T4Mesh> const & surface_map_B
                          , mesh_array::VertexAttribute<V,mesh_array::T4Mesh> const & Sb
                          , geometry::ContactsCallback<V> & callback
                          )
    {
      using namespace mesh_array;
      
      Node<T,K> const & node_A = branch_A.m_nodes[node_idx_A];
      Node<T,K> const & node_B = branch_B.m_nodes[node_idx_B];
      
      if(!geometry::overlap_dop_dop(node_A.m_volume, node_B.m_volume))
        return;
      
      bool const A_is_leaf = node_A.is_leaf();
      bool const B_is_leaf = node_B.is_leaf();
      
      if(A_is_leaf && B_is_leaf)
      {
        PAUSE_TIMER("kdop_tandem_traversal_time");
        RESUME_TIMER("contact_point_generation_time");

        Tetrahedron const & tet_A = mesh_A.tetrahedron( node_A.m_start );
        Tetrahedron const & tet_B = mesh_B.tetrahedron( node_B.m_start );
        
        bool const & surface_Ai = surface_map_A( tet_A ).m_i;
        bool const & surface_Aj = surface_map_A( tet_A ).m_j;
        bool const & surface_Ak = surface_map_A( tet_A ).m_k;
        bool const & surface_Am = surface_map_A( tet_A ).m_m;

        if (
            !surface_Ai &&
            !surface_Aj &&
            !surface_Ak &&
            !surface_Am
            )
        {
          PAUSE_TIMER("contact_point_generation_time");
          RESUME_TIMER("kdop_tandem_traversal_time");
          return;
        }

        bool const & surface_Bi = surface_map_B( tet_B ).m_i;
        bool const & surface_Bj = surface_map_B( tet_B ).m_j;
        bool const & surface_Bk = surface_map_B( tet_B ).m_k;
        bool const & surface_Bm = surface_map_B( tet_B ).m_m;

        if (
            !surface_Bi &&
            !surface_Bj &&
            !surface_Bk &&
            !surface_Bm
            )
        {
          PAUSE_TIMER("contact_point_generation_time");
          RESUME_TIMER("kdop_tandem_traversal_time");
          return; // all faces of B are internal
        }

        V const a0 = V::make( X_A( tet_A.i() ), Y_A( tet_A.i() ), Z_A( tet_A.i() ) );
        V const a1 = V::make( X_A( tet_A.j() ), Y_A( tet_A.j() ), Z_A( tet_A.j() ) );
        V const a2 = V::make( X_A( tet_A.k() ), Y_A( tet_A.k() ), Z_A( tet_A.k() ) );
        V const a3 = V::make( X_A( tet_A.m() ), Y_A( tet_A.m() ), Z_A( tet_A.m() ) );
        
        V const b0 = V::make( X_B( tet_B.i() ), Y_B( tet_B.i() ), Z_B( tet_B.i() ) );
        V const b1 = V::make( X_B( tet_B.j() ), Y_B( tet_B.j() ), Z_B( tet_B.j() ) );
        V const b2 = V::make( X_B( tet_B.k() ), Y_B( tet_B.k() ), Z_B( tet_B.k() ) );
        V const b3 = V::make( X_B( tet_B.m() ), Y_B( tet_B.m() ), Z_B( tet_B.m() ) );

        std::vector<bool> surface_A( 4u, false );
        std::vector<bool> surface_B( 4u, false );
        std::vector<V> structure_A( 4u, V::zero() );
        std::vector<V> structure_B( 4u, V::zero() );


        geometry::Tetrahedron<V> const gtet_A = geometry::make_tetrahedron(a0,a1,a2,a3);
        geometry::Tetrahedron<V> const gtet_B = geometry::make_tetrahedron(b0,b1,b2,b3);

        surface_A[0] = surface_Ai;
        surface_A[1] = surface_Aj;
        surface_A[2] = surface_Ak;
        surface_A[3] = surface_Am;
        
        surface_B[0] = surface_Bi;
        surface_B[1] = surface_Bj;
        surface_B[2] = surface_Bk;
        surface_B[3] = surface_Bm;

        structure_A[0] = Sa( tet_A.i() );
        structure_A[1] = Sa( tet_A.j() );
        structure_A[2] = Sa( tet_A.k() );
        structure_A[3] = Sa( tet_A.m() );

        structure_B[0] = Sb( tet_B.i() );
        structure_B[1] = Sb( tet_B.j() );
        structure_B[2] = Sb( tet_B.k() );
        structure_B[3] = Sb( tet_B.m() );

        SelectContactPointAlgorithm::call_algorithm(gtet_A, gtet_B, callback, surface_A, surface_B, structure_A, structure_B );

        PAUSE_TIMER("contact_point_generation_time");
        RESUME_TIMER("kdop_tandem_traversal_time");

      }
      else if(!A_is_leaf && !B_is_leaf)
      {
        for(size_t a = node_A.m_start; a <= node_A.m_end; ++a)
        {
          for(size_t b = node_B.m_start; b <= node_B.m_end; ++b)
          {
            traversal<V,K,T>(  a, branch_A, mesh_A, X_A, Y_A, Z_A, surface_map_A, Sa
                             , b, branch_B, mesh_B, X_B, Y_B, Z_B, surface_map_B, Sb
                             , callback
                             );
          }
        }
      }
      else if(!A_is_leaf && B_is_leaf)
      {
        for(size_t a = node_A.m_start; a <= node_A.m_end; ++a)
        {
          traversal<V,K,T>(           a, branch_A, mesh_A, X_A, Y_A, Z_A, surface_map_A, Sa
                           , node_idx_B, branch_B, mesh_B, X_B, Y_B, Z_B, surface_map_B, Sb
                           , callback
                           );
        }
      }
      else if( A_is_leaf && !B_is_leaf)
      {
        for(size_t b = node_B.m_start; b <= node_B.m_end; ++b)
        {
          traversal<V,K,T>(  node_idx_A, branch_A, mesh_A, X_A, Y_A, Z_A, surface_map_A, Sa
                           ,          b, branch_B, mesh_B, X_B, Y_B, Z_B, surface_map_B, Sb
                           , callback
                           );
        }
      }
      
    }
    
  }// namespace details

  template< typename V, size_t K, typename T>
  inline void tandem_traversal( TestPair<V,K,T> & work_item  )
  {

    if(!geometry::overlap_dop_dop(work_item.m_tree_a->m_root, work_item.m_tree_b->m_root))
      return;

    size_t const C_A = work_item.m_tree_a->branches().size();
    size_t const C_B = work_item.m_tree_b->branches().size();

    for( size_t a = 0u; a < C_A; ++a)
    {
      SubTree<T,K> const & branch_A = work_item.m_tree_a->branches()[a];

      for( size_t b = 0u; b < C_B; ++b)
      {
        SubTree<T,K> const & branch_B = work_item.m_tree_b->branches()[b];

        details::traversal<V,K,T>(  0
                                  , branch_A
                                  , *(work_item.m_mesh_a)
                                  , *(work_item.m_x_a)
                                  , *(work_item.m_y_a)
                                  , *(work_item.m_z_a)
                                  , *(work_item.m_surface_map_a)
                                  , *(work_item.m_Sa)
                                  , 0
                                  , branch_B
                                  , *(work_item.m_mesh_b)
                                  , *(work_item.m_x_b)
                                  , *(work_item.m_y_b)
                                  , *(work_item.m_z_b)
                                  , *(work_item.m_surface_map_b)
                                  , *(work_item.m_Sb)
                                  , *(work_item.m_callback)
                                  );
      }
    }
  }

  template< typename V, size_t K, typename T>
  inline void tandem_traversal(
                               std::vector< TestPair<V,K,T> > & work_pool
                               )
  {
    if( work_pool.empty() )
      return;

    typedef          TestPair<V,K,T>                  work_item_type;
    typedef          std::vector< work_item_type >    work_pool_type;
    typedef typename work_pool_type::iterator         work_item_iterator;

    START_TIMER("kdop_tandem_traversal_time");
    START_TIMER("contact_point_generation_time");
    PAUSE_TIMER("contact_point_generation_time");

    work_item_iterator end     = work_pool.end();
    work_item_iterator current = work_pool.begin();

    for(;current != end; ++current)
    {
      tandem_traversal<V, K, T>( *current );
    }

    RESUME_TIMER("contact_point_generation_time");
    STOP_TIMER("contact_point_generation_time");
    STOP_TIMER("kdop_tandem_traversal_time");
  }

}// namespace kdop

// KDOP_TANDEM_TRAVERSAL_H
#endif
