#ifndef KDOP_SINGLE_TRAVERSAL_H
#define KDOP_SINGLE_TRAVERSAL_H

#include <kdop_tree.h>

#include <geometry.h>

#include <mesh_array.h>

namespace kdop
{
  namespace details
  {



    /**
     *
     * @param should_flip            By default normals point from shape (being object A) towards
     *                               tetrahedral mesh ( being object B). However, if order is reversed then
     *                               setting this flag to true will flip the normals.
     */
    template< typename V, size_t K, typename T, typename S>
    inline void traversal(
                            geometry::DOP<T, K> const & shape_dop
                          , S const & shape
                          , size_t const & node_idx
                          , SubTree<T,K> const & branch
                          , mesh_array::T4Mesh const & mesh
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & X
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Y
                          , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Z
                          , mesh_array::TetrahedronAttribute<mesh_array::TetrahedronSurfaceInfo,mesh_array::T4Mesh> const & surface_map
                          , geometry::ContactsCallback<V> & callback
                          , bool const & should_flip
                          )
    {
      using namespace mesh_array;

      Node<T,K> const & node = branch.m_nodes[node_idx];

      if(!geometry::overlap_dop_dop(node.m_volume, shape_dop))
        return;

      bool const is_leaf = node.is_leaf();

      if(is_leaf)
      {
        Tetrahedron const tet = mesh.tetrahedron( node.m_start );

        V const p0 = V::make( X( tet.i() ), Y( tet.i() ), Z( tet.i() ) );
        V const p1 = V::make( X( tet.j() ), Y( tet.j() ), Z( tet.j() ) );
        V const p2 = V::make( X( tet.k() ), Y( tet.k() ), Z( tet.k() ) );
        V const p3 = V::make( X( tet.m() ), Y( tet.m() ), Z( tet.m() ) );

        geometry::Tetrahedron<V> const tetrahedron = geometry::make_tetrahedron(p0, p1, p2,p3);

        std::vector<bool> surface(4u, false);

        surface[0] = surface_map( tet ).m_i;
        surface[1] = surface_map( tet ).m_j;
        surface[2] = surface_map( tet ).m_k;
        surface[3] = surface_map( tet ).m_m;

        contacts_shape_tetrahedron(shape, tetrahedron, surface, callback, should_flip);
      }
      else
      {

        for(size_t child = node.m_start; child <= node.m_end; ++child)
        {
            traversal<V,K,T,S>(
                                shape_dop
                               , shape
                               , child
                               , branch
                               , mesh
                               , X
                               , Y
                               , Z
                               , surface_map
                               , callback
                               , should_flip
                               );
        }

      }

    }

  } // end of namespace details




  /**
   *
   * @param should_flip            By default normals point from shape (being object A) towards
   *                               tetrahedral mesh ( being object B). However, if order is reversed then
   *                               setting this flag to true will flip the normals.
   */
  template< typename V, size_t K, typename T, typename S>
  inline void single_traversal(
                                 S const & shape
                               , Tree<T,K> const & tree
                               , mesh_array::T4Mesh const & mesh
                               , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & X
                               , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Y
                               , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Z
                               , mesh_array::TetrahedronAttribute<mesh_array::TetrahedronSurfaceInfo,mesh_array::T4Mesh> const & surface_map
                               , geometry::ContactsCallback<V> & callback
                               , bool const & should_flip = false
                               )
  {
    geometry::DOP<T,K> const shape_dop = geometry::convert<K,V>( shape );

    if(!overlap_dop_dop(tree.m_root, shape_dop))
      return;

    size_t const C = tree.branches().size();

    for( size_t a = 0u; a < C; ++a)
    {
      SubTree<T,K> const & branch = tree.branches()[a];

      details::traversal<V,K,T,S>(
                                    shape_dop
                                  , shape
                                  , 0
                                  , branch
                                  , mesh
                                  , X
                                  , Y
                                  , Z
                                  , surface_map
                                  , callback
                                  , should_flip
                                  );
    }
  }
  
}// namespace kdop

// KDOP_SINGLE_TRAVERSAL_H
#endif
