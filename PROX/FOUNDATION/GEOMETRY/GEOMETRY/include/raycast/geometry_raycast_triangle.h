#ifndef GEOMETRY_COMPUTE_RAYCAST_TRIANGLE_H
#define GEOMETRY_COMPUTE_RAYCAST_TRIANGLE_H

#include <types/geometry_triangle.h>
#include <types/geometry_plane.h>
#include <types/geometry_ray.h>
#include <raycast/geometry_raycast_plane.h>

#include <cmath>
#include <vector>

namespace geometry
{

  template<typename V>
  inline bool compute_raycast_triangle(
                                   Ray<V> const & ray
                                   , Triangle<V> const & triangle
                                   , V & hit
                                   , typename V::real_type & length
                                   , bool const & only_front_face = false
                                   )
  {
    typedef typename V::value_traits VT;
    typedef typename V::real_type     T;

    length = VT::infinity();
    hit    = V::zero();

    Plane<V> const & plane = make_plane( triangle );

    V p     = V::zero();
    T t     = VT::infinity();

    bool const hit_plane = compute_raycast_plane(ray, plane, p, t, only_front_face );

    if (!hit_plane)
      return false;

    bool const inside = inside_triangle( p, triangle );

    if(!inside)
      return false;

    hit    = p;
    length = t;

    return true;
  }


}// namespace geometry

// GEOMETRY_COMPUTE_RAYCAST_TRIANGLE_H
#endif
