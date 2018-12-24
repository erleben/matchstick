#ifndef GEOMETRY_COMPUTE_RAYCAST_PLANE_H
#define GEOMETRY_COMPUTE_RAYCAST_PLANE_H

#include <tiny.h>

#include <cmath>

namespace geometry 
{

  /**
   *
   * @param only_front_face       Default value is false. If set to true then
   *                              a hit is only found if ray is hitting from the
   *                              front side of the plane. If hitting from
   *                              the back side then no hit is reported.
   */
  template<typename V>
  inline bool compute_raycast_plane(
                                  Ray<V> const & ray
                                , Plane<V> const & plane
                                , V & hit
                                , typename V::real_type & length
                                , bool const & only_front_face = false
                                )
  {
    typedef typename V::value_traits VT;
    typedef typename V::real_type     T;

    length = VT::infinity();
    hit    = V::zero();

    V const p = ray.origin();
    V const r = ray.direction();
    V const n = plane.normal();
    T const w = plane.offset();

    // Test if ray is parallel with plane
    if (fabs(inner_prod(n,r)) <= tiny::working_precision<T>() )
      return false;

    if (only_front_face && inner_prod(n,r) > VT::zero() )
      return false;

    T const test = ( w - inner_prod(n, p) )  / inner_prod(n,r);

    assert( is_number( test )    || !"compute_ray_plane(): nan");
    assert( is_finite( test )    || !"compute_ray_plane(): inf");

    // Test if the ray hit will be "behind" the ray origin (shooting backwards)
    if (test < VT::zero())
      return false;

    length = test;
    hit    = p + length*r;

    assert( is_number( length )    || !"compute_ray_plane(): nan");
    assert( is_finite( length )    || !"compute_ray_plane(): inf");
    assert( length >= VT::zero()    || !"compute_ray_plane(): non-positive");

    assert( is_number( hit(0) )    || !"compute_ray_plane(): nan");
    assert( is_finite( hit(0) )    || !"compute_ray_plane(): inf");
    assert( is_number( hit(1) )    || !"compute_ray_plane(): nan");
    assert( is_finite( hit(1) )    || !"compute_ray_plane(): inf");
    assert( is_number( hit(2) )    || !"compute_ray_plane(): nan");
    assert( is_finite( hit(2) )    || !"compute_ray_plane(): inf");

    return true;
  }

} //namespace geometry

// GEOMETRY_COMPUTE_RAYCAST_PLANE_H
#endif 