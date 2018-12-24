#ifndef GEOMETRY_COMPUTE_RAYCAST_SPHERE_H
#define GEOMETRY_COMPUTE_RAYCAST_SPHERE_H

#include <types/geometry_sphere.h>
#include <types/geometry_ray.h>

#include <tiny_is_number.h>
#include <tiny_is_finite.h>

#include <cmath> // needed for std::sqrt

namespace geometry 
{

  template<typename V>
  inline bool compute_raycast_sphere(
                                   Ray<V> const & ray
                                   , Sphere<V> const & sphere
                                   , V & hit_point
                                   , typename V::real_type & length
                                   )
  {
    typedef typename V::value_traits VT;
    typedef typename V::real_type    T;

    using std::sqrt;

    length       = VT::infinity();
    hit_point    = V::zero();

    V const & o = sphere.center();
    T const & r = sphere.radius();
    V const & p = ray.origin();
    V const & u = ray.direction();

    T const a = tiny::inner_prod( u,   u   );
    T const b = tiny::inner_prod( p-o, u   )*VT::two();
    T const c = tiny::inner_prod( p-o, p-o ) - r*r;

    assert(a>VT::zero() || !"compute_raycast_sphere(): a-coefficent must be positive");

    // Test that ray origin is out-side sphere
    if (c <= VT::zero())
    {
      return false;
    }

    T const D = b*b - VT::four()*a*c;

    if( D < VT::zero() )
      return false;

    T const sqrt_D = sqrt(D);

    T const r1 = ( -b - sqrt_D ) / ( VT::two()*a );
    T const r2 = ( -b + sqrt_D ) / ( VT::two()*a );

    assert(is_finite(r1) || !"compute_raycast_sphere(): r1 is inf");
    assert(is_number(r1) || !"compute_raycast_sphere(): r1 is nan");
    assert(is_finite(r2) || !"compute_raycast_sphere(): r2 is inf");
    assert(is_number(r2) || !"compute_raycast_sphere(): r2 is nan");

    assert(r1 <= r2 || !"compute_raycast_sphere(): r1 can not be larger than r2");

    if( r1 >= VT::zero() )
    {
      length    = r1;
      hit_point = p + length*u;
      return true;
    }

    if( r2 >= VT::zero())
    {
      length    = r1;
      hit_point = p + length*u;
      return true;
    }

    return false;
  }

} //namespace geometry

// GEOMETRY_COMPUTE_RAYCAST_SPHERE_H
#endif 