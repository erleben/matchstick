#ifndef GEOMETRY_OVERLAP_TRIANGLE_TRIANGLE_H
#define GEOMETRY_OVERLAP_TRIANGLE_TRIANGLE_H

#include <types/geometry_triangle.h>
#include <types/geometry_plane.h>

#include <tiny_vector_functions.h>

namespace geometry
{
  namespace detail
  {

    template< typename V>
    inline bool SAT_line( V const  & a0
                         , V const & a1
                         , V const & b0
                         , V const & b1
                         , V const & n
                         )
    {
      using std::max;
      using std::min;

      typedef typename V::real_type     T;

      T const da0 = tiny::inner_prod(n, a0);
      T const da1 = tiny::inner_prod(n, a1);
      T const db0 = tiny::inner_prod(n, b0);
      T const db1 = tiny::inner_prod(n, b1);

      T const a_min = min(da0, da1);
      T const a_max = max(da0, da1);
      T const b_min = min(db0, db1);
      T const b_max = max(db0, db1);

      if( (a_max < b_min) || (a_min > b_max) )
        return true;

      return false;
    }

  } // end of namespace detail

  template< typename V>
  inline bool overlap_triangle_triangle(
                                        Triangle<V> const & A
                                        , Triangle<V> const & B
                                        )
  {
    typedef typename V::value_traits VT;
    typedef typename V::real_type     T;

    V const & A0 = A.point(0);
    V const & A1 = A.point(1);
    V const & A2 = A.point(2);

    V const & B0 = B.point(0);
    V const & B1 = B.point(1);
    V const & B2 = B.point(2);

    // All vertices of B is on the same side of A
    {
      Plane<V>    const plane    = make_plane(A);

      T const b0 = get_signed_distance(B0, plane);
      T const b1 = get_signed_distance(B1, plane);
      T const b2 = get_signed_distance(B2, plane);

      if ( b0 > VT::zero() && b1 > VT::zero() && b2 > VT::zero()  )
        return false;

      if ( b0 < VT::zero() && b1 < VT::zero() && b2 < VT::zero()  )
        return false;
    }

    // All vertices of A is on the same side of B
    {
      Plane<V>    const plane    = make_plane(B);

      T const a0 = get_signed_distance(A0, plane);
      T const a1 = get_signed_distance(A1, plane);
      T const a2 = get_signed_distance(A2, plane);

      if ( a0 > VT::zero() && a1 > VT::zero() && a2 > VT::zero()  )
        return false;

      if ( a0 < VT::zero() && a1 < VT::zero() && a2 < VT::zero()  )
        return false;
    }

    // None of the face normals was a separating axis, now we test the edges

    // let edge0 be vertices 0 and 1
    // let edge1 be vertices 0 and 2
    // let edge2 be vertices 1 and 2

    // Ae0 v Be0
    V const ae0be0 = tiny::cross(A1-A0, B1-B0);
    if (detail::SAT_line(A1, A2, B1, B2, ae0be0))
      return false;

    // Ae0 v Be1
    V const ae0be1 = tiny::cross(A1-A0, B2-B0);
    if (detail::SAT_line(A1, A2, B0, B1, ae0be1))
      return false;

    // Ae0 v Be2
    V const ae0be2 = tiny::cross(A1-A0, B2-B1);
    if (detail::SAT_line(A1, A2, B0, B1, ae0be2))
      return false;

    // Ae1 v Be0
    V const ae1be0 = tiny::cross(A2-A0, B1-B0);
    if (detail::SAT_line(A1, A2, B1, B2, ae1be0))
      return false;

    // Ae1 v Be1
    V const ae1be1 = tiny::cross(A2-A0, B2-B0);
    if (detail::SAT_line(A1, A2, B1, B2, ae1be1))
      return false;

    // Ae1 v Be2
    V const ae1be2 = tiny::cross(A2-A0, B2-B1);
    if (detail::SAT_line(A1, A2, B0, B1, ae1be2))
      return false;

    // Ae2 v Be0
    V const ae2be0 = tiny::cross(A2-A1, B1-B0);
    if (detail::SAT_line(A0, A2, B1, B2, ae2be0))
      return false;

    // Ae2 v Be1
    V const ae2be1 = tiny::cross(A2-A1, B2-B0);
    if (detail::SAT_line(A0, A2, B1, B2, ae2be1))
      return false;

    // Ae2 v Be2
    V const ae2be2 = tiny::cross(A2-A1, B2-B1);
    if (detail::SAT_line(A0, A2, B0, B2, ae2be2))
      return false;

    return true;
  }
  
}// namespace geometry

// GEOMETRY_OVERLAP_TRIANGLE_TRIANGLE_H
#endif
