#ifndef KDOP_MAKE_TREE_H
#define KDOP_MAKE_TREE_H

#include <kdop_refit_tree.h>
#include <kdop_tree.h>

#include <mesh_array_t4mesh.h>
#include <mesh_array_vertex_attribute.h>

#include <tiny_power2.h>

#include <algorithm>   // Needed for std::min
#include <cassert>     // Needed for assert
#include <cmath>       // Needed for std::ceil and std::floor

namespace kdop
{
  
  namespace details
  {
      
    template<typename T>
    class MeshChunkInfo
    {
    public:
      
      
      size_t m_first;
      size_t m_last;
      
      mesh_array::T4Mesh const & m_mesh;
      
      mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & m_X;
      mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & m_Y;
      mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & m_Z;                
      
    public:
      
      MeshChunkInfo( size_t first
                    , size_t last
                    , mesh_array::T4Mesh const & mesh
                    , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & X
                    , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Y
                    , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Z                
                    )
      : m_first(first)
      , m_last(last)
      , m_mesh(mesh)
      , m_X(X)
      , m_Y(Y)
      , m_Z(Z)
      {}
      
    };
        
    template< typename V, size_t K, typename T>
    inline void make_subtree( size_t const & parent_idx
                            , size_t & free_idx
                            , MeshChunkInfo<T> const & geometry
                            , SubTree<T,K> & branch
                            , size_t & height
                            )
    {
      using namespace details;
      using std::floor;
      using std::max;
      
      size_t const N = geometry.m_last - geometry.m_first + 1u;// number of elements in chunk
      
      if (N > 1) 
      {
        size_t const left_idx  = free_idx;
        size_t const right_idx = free_idx+1;
        free_idx += 2u;

        size_t const nodes = branch.m_nodes.size();

        assert(parent_idx < nodes || !"make_tree: out of bounds");

        Node<T,K> & parent      = branch.m_nodes[parent_idx];
        parent.m_start          = left_idx;
        parent.m_end            = right_idx;
        
        size_t const mid  = floor( (geometry.m_first + geometry.m_last)/2.0 );
        
        MeshChunkInfo<T> const left_geometry(  geometry.m_first, mid, geometry.m_mesh, geometry.m_X, geometry.m_Y, geometry.m_Z );
        MeshChunkInfo<T> const right_geometry( mid+1, geometry.m_last, geometry.m_mesh, geometry.m_X, geometry.m_Y, geometry.m_Z );
 
        assert(left_idx < nodes || !"make_tree: out of bounds");

        Node<T,K> & left_child  = branch.m_nodes[left_idx];
        left_child.m_parent     = parent_idx;

        assert(right_idx < nodes || !"make_tree: out of bounds");

        Node<T,K> & right_child = branch.m_nodes[right_idx];
        right_child.m_parent    = parent_idx;
                
        size_t left_height  = 0u;
        size_t right_height = 0u;

        make_subtree<V,K,T>(left_idx,  free_idx, left_geometry,  branch, left_height);
        make_subtree<V,K,T>(right_idx, free_idx, right_geometry, branch, right_height);

        height = max(left_height, right_height) + 1;
        
      }
      else
      {

        assert(geometry.m_first == geometry.m_last || !"make_subtree(): internal error, can not be a leaf case?" );
        
        Node<T,K> & leaf = branch.m_nodes[parent_idx];
        
        leaf.m_start = geometry.m_first;
        leaf.m_end   = geometry.m_first;
        
        height = 0u;
      }
    }
    
    template< typename V, size_t K, typename T>
    inline void make_subtree( size_t const & parent_idx
                            , size_t & free_idx
                            , size_t first
                            , size_t last
                            , SubTree<T,K> & super_chunk
                            , size_t & height
                            )
    {
      size_t const N = last - first + 1;

      if(N > 1)
      {

        size_t const left_idx = free_idx;
        size_t const right_idx = free_idx + 1;

        free_idx += 2u;
        
        size_t const nodes = super_chunk.m_nodes.size();

        assert(parent_idx < nodes || !"make_tree: out of bounds");

        Node<T,K> & parent = super_chunk.m_nodes[parent_idx];

        parent.m_start = left_idx;
        parent.m_end = right_idx;
        
        size_t const mid  = floor((first + last) / 2.0);
 
        assert(left_idx < nodes || !"make_tree: out of bounds");

        Node<T,K> & left_child = super_chunk.m_nodes[left_idx];

        left_child.m_parent = parent_idx;

        assert(right_idx < nodes || !"make_tree: out of bounds");

        Node<T,K> & right_child = super_chunk.m_nodes[right_idx];

        right_child.m_parent = parent_idx;
                
        size_t left_height  = 0u;
        size_t right_height = 0u;

        make_subtree<V,K,T>(left_idx, free_idx, first, mid, super_chunk, left_height);
        make_subtree<V,K,T>(right_idx, free_idx, mid + 1, last, super_chunk, right_height);

        height = std::max(left_height, right_height) + 1;

      } else {

        assert(first == last || !"make_subtree(): internal error, can not be a leaf case?" );
        
        //--- reference to the covered subtree

        Node<T, K> & leaf = super_chunk.m_nodes[parent_idx];

        leaf.m_start = first;
        leaf.m_end = first;        

        height = 0;
      }
    }
    
  } // end of namespace details
  
  template< typename V, size_t K, typename T>
  inline Tree<T,K> make_tree( size_t const & mem_bytes
                             , mesh_array::T4Mesh const & mesh
                             , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & X
                             , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Y
                             , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Z
                             )
  {
    typedef typename V::value_traits VT;
    
    assert( mem_bytes > 0u || !"make_tree(): must have some memory");
    
    using std::ceil;
    using std::floor;
    using std::min;
    using std::max;
    
    using namespace details;
    
    Tree<T,K> tree;
    
    //--- Assuming we have reordered a T4Mesh then we may now create subsets for
    //--- leaf nodes simply by chopping up the tetrahedra array into chunks. In
    //--- order to do so we much determine how big chunks we need.
    
    size_t const node_bytes = sizeof(Node<T,K> );    
    size_t const N_max      = floor( (mem_bytes / node_bytes)* VT::half() );  // Total number of nodes that fit into memory, divided by two because we want two trees simultaneously in memory.
    
    // However, the maximum number of possible nodes may not be a power of 2,
    // which is needed to create a perfect balanced binary tree.
    // There is no need to have more than is needed for one full BVH.
    size_t const N_perfect  = 
            min(tiny::lower_power2(N_max),
                tiny::upper_power2(mesh.tetrahedron_size() * 2)
            ) - 1;
    size_t const L          = (N_perfect + 1)*VT::half();                     // So if we have a single tree of N nodes (assuming binary balanced tree) then how many leaves will such a tree have? The total number of nodes in a perfect binary tree is N = 2 L  - 1 where L is number of leaf nodes
    size_t const M          = mesh.tetrahedron_size();                        // Total number of tetrahedra
    size_t       C          = ceil( VT::one()*M / L);                         // Total number of chunks to divide the mesh into
    size_t const H          = max(ceil(log(C) / log(L)), 1.0);                // Total number of chunk levels (C could be 1)
    
    
    //--- Create enough branches to hold all subtrees correspodining to --------
    //--- the chuncks of the mesh. ---------------------------------------------
    tree.m_chunk_levels.resize(H);
    tree.branches().resize(C);
    
    //--- Loop over the chunks of the mesh and build subtrees for each chunck --
    for( size_t c = 0u; c < C;++c) 
    {
      SubTree<T,K> & branch = tree.branches()[c];
      
      size_t const first  =  c*L;                             // Index of first tetrahedron in chunk
      size_t const last   =  min( first+L-1u, M - 1u);        // Index of last tetrahedron in chunk
      
      MeshChunkInfo<T> geometry( first, last, mesh, X, Y, Z); // Create a wrapper of all mesh information
      
      branch.m_nodes.resize(N_perfect); // Pre-allocate storge for a full perfect balanced tree

      size_t const root_idx = 0u;
      size_t       free_idx = 1u;

      make_subtree<V,K,T>( root_idx, free_idx, geometry, branch, branch.m_height );  // Now build the sucker!
    }

    //--- Do the same for all higher levels covering the next lower one bottom up
    for(size_t h = H - 1; h >= 1; --h)
    {
      C = ceil(VT::one()*C / L);

      tree.super_chunks(h - 1).resize(C);
      
      for(size_t c = 0; c < C; ++c)
      {
        SubTree<T,K> & super_chunck = tree.super_chunks(h - 1)[c];

        size_t const first = c * L;
        size_t const last = min(first + L - 1, tree.super_chunks(h).size() - 1);

        super_chunck.m_nodes.resize(N_perfect);

        size_t const root_idx = 0u;
        size_t free_idx = 1u;

        make_subtree<V,K,T>(
                              root_idx
                            , free_idx
                            , first
                            , last
                            , super_chunck
                            , super_chunck.m_height
                            );
      }
    }

    refit_tree<V,K,T>(tree, mesh, X, Y, Z );
    
    return tree;
  }
  
}// namespace kdop

// KDOP_MAKE_TREE_H
#endif
