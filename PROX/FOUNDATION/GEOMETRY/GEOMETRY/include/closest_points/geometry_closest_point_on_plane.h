#ifndef GEOMETRY_CLOSEST_POINT_ON_PLANE_H
#define GEOMETRY_CLOSEST_POINT_ON_PLANE_H

#include <types/geometry_plane.h>

#include <tiny_is_finite.h>
#include <tiny_is_number.h>

#include <cassert>

namespace geometry
{

  template<typename V>
  inline V closest_point_on_plane(V const & p, Plane<V> const & P)
  {
    typedef typename V::real_type T;
    
    V const & n = P.n();
    T const & w = P.w();
    
    V const   q = p - n * inner_prod( n, (p - n*w) );

    assert( is_number(q(0)) || !"closest_point_on_line(): NaN encountered");
    assert( is_finite(q(0)) || !"closest_point_on_line(): Inf encountered");
    assert( is_number(q(1)) || !"closest_point_on_line(): NaN encountered");
    assert( is_finite(q(1)) || !"closest_point_on_line(): Inf encountered");
    assert( is_number(q(2)) || !"closest_point_on_line(): NaN encountered");
    assert( is_finite(q(2)) || !"closest_point_on_line(): Inf encountered");

    return q;
  }
  
}// namespace geometry

// GEOMETRY_CLOSEST_POINT_ON_PLANE_H
#endif