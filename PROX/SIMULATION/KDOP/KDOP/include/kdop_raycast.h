#ifndef KDOP_RAYCAST_H
#define KDOP_RAYCAST_H

#include <kdop_tree.h>

#include <raycast/geometry_raycast_tetrahedron.h>
#include <raycast/geometry_raycast_dop.h>

#include <mesh_array.h>
#include <mesh_array_t4mesh.h>
#include <mesh_array_vertex_attribute.h>
#include <mesh_array_compute_surface_map.h>

namespace kdop
{
  namespace details
  {

    template< typename V, size_t K>
    inline void raycast(
                          geometry::Ray<V> const & ray
                        , size_t const & node_idx
                        , SubTree<typename V::real_type, K> const & branch
                        , mesh_array::T4Mesh const & mesh
                        , mesh_array::VertexAttribute<typename V::real_type,mesh_array::T4Mesh> const & X
                        , mesh_array::VertexAttribute<typename V::real_type,mesh_array::T4Mesh> const & Y
                        , mesh_array::VertexAttribute<typename V::real_type,mesh_array::T4Mesh> const & Z
                        , mesh_array::TetrahedronAttribute<mesh_array::TetrahedronSurfaceInfo,mesh_array::T4Mesh> const & surface_map
                        , V & hit_point
                        , typename V::real_type & length
                        )
    {
      typedef typename V::real_type    T;
      typedef typename V::value_traits VT;

      T const dop_threshold = VT::numeric_cast(0.01);

      Node<T,K> const & node = branch.m_nodes[node_idx];

      V p = V::zero();
      T t = VT::infinity();

      if(! geometry::compute_raycast_dop( ray, node.m_volume, p, t, dop_threshold))
        return;

      if(t >= length)
        return;

      bool const is_leaf = node.is_leaf();

      if(is_leaf)
      {
        mesh_array::Tetrahedron const mT = mesh.tetrahedron( node.m_start );

        V const p0 = V::make( X(mT.i()), Y(mT.i()), Z(mT.i()) );
        V const p1 = V::make( X(mT.j()), Y(mT.j()), Z(mT.j()) );
        V const p2 = V::make( X(mT.k()), Y(mT.k()), Z(mT.k()) );
        V const p3 = V::make( X(mT.m()), Y(mT.m()), Z(mT.m()) );

        std::vector<bool> surf(4u,false);

        surf[0] = surface_map(mT).m_i;
        surf[1] = surface_map(mT).m_j;
        surf[2] = surface_map(mT).m_k;
        surf[3] = surface_map(mT).m_m;

        V p = V::zero();
        T s = VT::infinity();

        geometry::Tetrahedron<V> const gT = geometry::make_tetrahedron(p0,p1,p2,p3);

        bool const did_hit = geometry::compute_raycast_tetrahedron( ray, gT, p, s, surf);

        if(did_hit)
        {
          hit_point    = s < length ? p : hit_point;
          length       = s < length ? s : length;
        }

      }
      else
      {

        for(size_t idx = node.m_start; idx <= node.m_end; ++idx)
        {
            raycast<V,K>(
                         ray
                         , idx
                         , branch
                         , mesh
                         , X
                         , Y
                         , Z
                         , surface_map
                         , hit_point
                         , length
                         );
        }

      }

    }

  } // end of namespace details

  template< typename V, size_t K>
  inline bool raycast(
                        geometry::Ray<V> const & ray
                      , Tree<typename V::real_type, K> const & tree
                      , mesh_array::T4Mesh const & mesh
                      , mesh_array::VertexAttribute<typename V::real_type,mesh_array::T4Mesh> const & X
                      , mesh_array::VertexAttribute<typename V::real_type,mesh_array::T4Mesh> const & Y
                      , mesh_array::VertexAttribute<typename V::real_type,mesh_array::T4Mesh> const & Z
                      , mesh_array::TetrahedronAttribute<mesh_array::TetrahedronSurfaceInfo,mesh_array::T4Mesh> const & surface_map
                      , V & hit_point
                      , typename V::real_type & length
                      )
  {
    typedef typename V::real_type    T;
    typedef typename V::value_traits VT;

    T const dop_threshold = VT::numeric_cast(0.01);

    hit_point    = V::zero();
    length       = VT::infinity();

    V p = V::zero();
    T t = VT::infinity();

    if(!geometry::compute_raycast_dop( ray, tree.m_root, p, t, dop_threshold))
      return false;

    size_t const C = tree.branches().size();

    for( size_t c = 0u; c < C; ++c)
    {
      SubTree<T,K> const & branch = tree.branches()[c];

      details::raycast<V,K>(
                            ray
                            , 0
                            , branch
                            , mesh
                            , X
                            , Y
                            , Z
                            , surface_map
                            , hit_point
                            , length
                            );
    }

    return length < VT::infinity();
  }
  
}// namespace kdop

// KDOP_RAYCAST_H
#endif
