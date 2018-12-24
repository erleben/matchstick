#ifndef GEOMETRY_MAKE_INTERSECTION_H
#define GEOMETRY_MAKE_INTERSECTION_H

#include <types/geometry_line.h>
#include <types/geometry_plane.h>

#include <tiny_is_finite.h>
#include <tiny_is_number.h>

#include <cassert>
#include <cmath>  // needed for std::fabs

namespace geometry
{


  template<typename V>
  inline V make_intersection(Line<V> const & L, Plane<V> const & P)
  {
    using std::fabs;
    
    typedef typename V::real_type    T;
    typedef typename V::value_traits VT;
    
    V const & o       = L.point();
    V const & d       = L.direction();
    V const & n       = P.normal();
    T const & w       = P.offset();
    
    //
    // A line is given as all points
    //
    //     p(t) = o + t*d for all t
    //
    // For all points on the line that are on the plane we have
    //
    //     n*p(t) - w = 0
    //
    // Substitution yiedls
    //
    //     n*o + t*n*d - w = 0
    //
    //  we find t-value as
    //
    //     t =  ( w - n*o ) / n*d
    //
    //
    
    T const   n_dot_d = inner_prod( n, d );

    assert( is_number(n_dot_d)         || !"make_intersection(): NaN encountered");
    assert( is_finite(n_dot_d)         || !"make_intersection(): Inf encountered");
    assert(fabs( n_dot_d) > VT::zero() || !"make_intersection(): line was parallel withe plane");


    T const   n_dot_o = inner_prod( n, o );

    assert( is_number(n_dot_o)         || !"make_intersection(): NaN encountered");
    assert( is_finite(n_dot_o)         || !"make_intersection(): Inf encountered");

    T const   t       = (w - n_dot_o) / n_dot_d;

    assert( is_number(t)               || !"make_intersection(): NaN encountered");
    assert( is_finite(t)               || !"make_intersection(): Inf encountered");
    V const   p       = o + t*d;
    
    assert( is_number(p(0))               || !"make_intersection(): NaN encountered");
    assert( is_finite(p(0))               || !"make_intersection(): Inf encountered");
    assert( is_number(p(1))               || !"make_intersection(): NaN encountered");
    assert( is_finite(p(1))               || !"make_intersection(): Inf encountered");
    assert( is_number(p(2))               || !"make_intersection(): NaN encountered");
    assert( is_finite(p(2))               || !"make_intersection(): Inf encountered");

    return p;
  }
  
  template<typename V>
  inline Line<V> make_intersection( Plane<V> const & A,  Plane<V> const & B)
  {
    typedef typename V::value_traits   VT;

    V const D = cross( A.normal(), B.normal() );

    assert( inner_prod(D,D) > VT::zero() || !"make_intersection(): error planes are coplanar");
    assert( is_number(D(0))               || !"make_intersection(): NaN encountered");
    assert( is_finite(D(0))               || !"make_intersection(): Inf encountered");
    assert( is_number(D(1))               || !"make_intersection(): NaN encountered");
    assert( is_finite(D(1))               || !"make_intersection(): Inf encountered");
    assert( is_number(D(2))               || !"make_intersection(): NaN encountered");
    assert( is_finite(D(2))               || !"make_intersection(): Inf encountered");

    V const d = unit( D );

    assert( is_number(d(0))               || !"make_intersection(): NaN encountered");
    assert( is_finite(d(0))               || !"make_intersection(): Inf encountered");
    assert( is_number(d(1))               || !"make_intersection(): NaN encountered");
    assert( is_finite(d(1))               || !"make_intersection(): Inf encountered");
    assert( is_number(d(2))               || !"make_intersection(): NaN encountered");
    assert( is_finite(d(2))               || !"make_intersection(): Inf encountered");

    // Get a point on plane A
    V const pA = A.normal()*A.offset();

    assert( is_number(pA(0))               || !"make_intersection(): NaN encountered");
    assert( is_finite(pA(0))               || !"make_intersection(): Inf encountered");
    assert( is_number(pA(1))               || !"make_intersection(): NaN encountered");
    assert( is_finite(pA(1))               || !"make_intersection(): Inf encountered");
    assert( is_number(pA(2))               || !"make_intersection(): NaN encountered");
    assert( is_finite(pA(2))               || !"make_intersection(): Inf encountered");

    // Direction on plane A towards plane B
    V const r = unit(cross( A.normal(), d ));

    assert( is_number(r(0))               || !"make_intersection(): NaN encountered");
    assert( is_finite(r(0))               || !"make_intersection(): Inf encountered");
    assert( is_number(r(1))               || !"make_intersection(): NaN encountered");
    assert( is_finite(r(1))               || !"make_intersection(): Inf encountered");
    assert( is_number(r(2))               || !"make_intersection(): NaN encountered");
    assert( is_finite(r(2))               || !"make_intersection(): Inf encountered");

    // Now we find the intersection point between the line on plane A with origin pA and direction r and plane B
    //
    // Since line (pA, r) is on plane A then the intersection point with B will be a point both on plane A and B.
    //
    Line<V> const L(pA, r);
    
    V const o = make_intersection(L, B);

    assert( is_number(o(0))               || !"make_intersection(): NaN encountered");
    assert( is_finite(o(0))               || !"make_intersection(): Inf encountered");
    assert( is_number(o(1))               || !"make_intersection(): NaN encountered");
    assert( is_finite(o(1))               || !"make_intersection(): Inf encountered");
    assert( is_number(o(2))               || !"make_intersection(): NaN encountered");
    assert( is_finite(o(2))               || !"make_intersection(): Inf encountered");

    return make_line(o, d, FROM_DIRECTION() );
  }

  template<typename V>
  inline V make_intersection(Plane<V> const & A, Plane<V> const & B, Plane<V> const & C)
  {
    Line<V> const L = make_intersection(A,B);
    return make_intersection(L,C);
  }
  
}// namespace geometry

// GEOMETRY_MAKE_INTERSECTION_H
#endif