#ifndef NARROW_RAYCAST_H
#define NARROW_RAYCAST_H

#include <narrow_object.h>
#include <narrow_geometry.h>

#include <tiny.h>

#include <geometry.h>

#include <kdop_raycast.h>

namespace narrow
{

  template<typename M>
  inline bool raycast(
                      geometry::Ray<typename M::vector3_type> const & ray
                      , Object<M>                               const & objA
                      , Geometry<M>                             const & geoA
                      , typename M::vector3_type                const & tA
                      , typename M::quaternion_type             const & qA
                      , typename M::vector3_type                      & point
                      , typename M::real_type                         & distance
                      )
  {
    typedef typename M::value_traits    VT;
    typedef typename M::vector3_type    V;
    //typedef typename M::real_type       T;
    //typedef typename M::coordsys_type   C;

    point    = V::zero();
    distance = VT::infinity();

    return kdop::raycast<V, 8>(
                               ray
                               , objA.m_tree
                               , geoA.m_mesh
                               , objA.m_X
                               , objA.m_Y
                               , objA.m_Z
                               , geoA.m_surface_map
                               , point
                               , distance
                               );
  }

} //namespace narrow

// NARROW_RAYCAST_H
#endif
