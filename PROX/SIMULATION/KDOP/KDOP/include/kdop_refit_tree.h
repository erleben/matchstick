#ifndef KDOP_REFIT_TREE_H
#define KDOP_REFIT_TREE_H

#include <kdop_tree.h>

#include <types/geometry_direction_table.h>
#include <types/geometry_dop.h>
#include <geometry_make_union.h>

#include <mesh_array_t4mesh.h>
#include <mesh_array_vertex_attribute.h>

#include <cstddef>
#include <vector>

namespace kdop
{
  
  namespace details
  {
  
    template< typename V, size_t K, typename T>
    inline void refit_subtree(
                              SubTree<T,K> & branch
                              , mesh_array::T4Mesh const & mesh
                              , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & X
                              , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Y
                              , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Z                              
                              , geometry::DirectionTable<V,(K/2)> const & DT
                              )
    {     
      using namespace mesh_array;
      
      size_t const N = branch.m_nodes.size();
     
      for(size_t i = 0; i<N; ++i)
      {
        Node<T,K> & node = branch.m_nodes[N-i-1u];
      
        if(node.is_undefined())
          continue;
        
        if(node.is_leaf())
        {
          size_t      const tet_idx = node.m_start;
          Tetrahedron const Tet     = mesh.tetrahedron(tet_idx);
          
          std::vector<V> points;
          points.resize(4);
          
          points[0](0) = X(Tet.i());
          points[0](1) = Y(Tet.i());
          points[0](2) = Z(Tet.i());
          
          points[1](0) = X(Tet.j());
          points[1](1) = Y(Tet.j());
          points[1](2) = Z(Tet.j());
          
          points[2](0) = X(Tet.k());
          points[2](1) = Y(Tet.k());
          points[2](2) = Z(Tet.k());
          
          points[3](0) = X(Tet.m());
          points[3](1) = Y(Tet.m());
          points[3](2) = Z(Tet.m());
          
          node.m_volume = make_dop(points.begin(), points.end(), DT);
          
        }else{
          
          Node<T,K> const & left_child  = branch.m_nodes[node.m_start];
          Node<T,K> const & right_child = branch.m_nodes[node.m_end];
          
          node.m_volume = geometry::make_union( left_child.m_volume, right_child.m_volume );
        }
        
      }
      
    }
    
    template< typename V, size_t K, typename T>
    inline void refit_subtree(
                              SubTree<T,K> & super_chunk
                              , std::vector< SubTree<T, K> > const & chunk_children
                              )
    {     
      using namespace mesh_array;
      
      size_t const N = super_chunk.m_nodes.size();
     
      for(size_t i = 0; i<N; ++i)
      {
        Node<T,K> & node = super_chunk.m_nodes[N - i - 1];

        if(node.is_undefined())
          continue;
        
        if(node.is_leaf())
        {
          SubTree<T, K> const subtree = chunk_children[node.m_start];

          node.m_volume = subtree.m_nodes[0].m_volume;
        }
        else
        {

          Node<T,K> const & left_child  = super_chunk.m_nodes[node.m_start];
          Node<T,K> const & right_child = super_chunk.m_nodes[node.m_end];

          node.m_volume = geometry::make_union( left_child.m_volume, right_child.m_volume );

        }
      }
    }
    
  } // end of namespace details
  
  template< typename V, size_t K, typename T>
  inline void refit_tree(
                              Tree<T,K> & tree
                             , mesh_array::T4Mesh const & mesh
                             , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & X
                             , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Y
                             , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Z
                             )
  {        
    geometry::DirectionTable<V,(K/2)> const DT = geometry::DirectionTableHelper<V,(K/2)>::make();

    size_t C = tree.branches().size();
    
    for( size_t c = 0u; c < C;++c) 
    {
      SubTree<T,K> & branch = tree.branches()[c];
            
      details::refit_subtree<V,K,T>(branch, mesh, X, Y, Z, DT);
    }

    for(size_t h = tree.number_of_levels() - 1; h >= 1; --h)
    {
      C = tree.super_chunks(h - 1).size();
      
      //--- In the first iteration the underlying chunks are the actual ones ---
      //--- in all subsequent ones it's the superchunks (ie chuncks of chuncks)
      for(size_t c = 0; c < C; ++c)
      {
        SubTree<T,K> & super_chunk = tree.super_chunks(h - 1)[c];

        details::refit_subtree<V,K,T>( super_chunk, tree.super_chunks(h) );
      }
    }
    
    //--- Loop over all the subtrees and create the root BV of the whole tree --
    tree.m_root = tree.super_chunks(0)[0].m_nodes[0].m_volume;

    C = tree.super_chunks(0).size();
    for( size_t c = 1u; c < C;++c)
    {      
      tree.m_root = geometry::make_union( tree.m_root, tree.super_chunks(0)[c].m_nodes[0].m_volume );
    }    
  }
  
}// namespace kdop

// KDOP_REFIT_TREE_H
#endif
