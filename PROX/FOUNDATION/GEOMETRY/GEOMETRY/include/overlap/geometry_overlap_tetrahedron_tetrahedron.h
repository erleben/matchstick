#ifndef GEOMETRY_OVERLAP_TETRAHEDRON_TETRAHEDRON_H
#define GEOMETRY_OVERLAP_TETRAHEDRON_TETRAHEDRON_H

#include <types/geometry_tetrahedron.h>
#include <types/geometry_plane.h>

#include <tiny_vector_functions.h>

namespace geometry
{
  namespace detail
  {

    template< typename V>
    inline bool SAT_line( V const  & t0_p0
                         , V const & t0_p1
                         , V const & t0_p2
                         , V const & t1_p0
                         , V const & t1_p1
                         , V const & t1_p2
                         , V const & n
                         )
    {
      using std::max;
      using std::min;

      typedef typename V::real_type     T;

      T const t0_d0 = tiny::inner_prod(n, t0_p0);
      T const t0_d1 = tiny::inner_prod(n, t0_p1);
      T const t0_d2 = tiny::inner_prod(n, t0_p2);

      T const t1_d0 = tiny::inner_prod(n, t1_p0);
      T const t1_d1 = tiny::inner_prod(n, t1_p1);
      T const t1_d2 = tiny::inner_prod(n, t1_p2);

      T const t0_min = min( min(t0_d0, t0_d1), t0_d2);
      T const t1_min = min( min(t1_d0, t1_d1), t1_d2);
      T const t0_max = max( max(t0_d0, t0_d1), t0_d2);
      T const t1_max = max( max(t1_d0, t1_d1), t1_d2);

      if( (t0_max < t1_min) || (t0_min > t1_max) )
        return true;

      return false;
    }

  } // end of namespace detail

  template< typename V>
  inline bool overlap_tetrahedron_tetrahedron(
                                              Tetrahedron<V> const & A
                                              , Tetrahedron<V> const & B
                                              )
  {
    typedef typename V::value_traits VT;
    typedef typename V::real_type     T;

    assert( get_signed_volume(A) > VT::zero() || !"overlap_tetrahedron_tetrahedron(): A was left oriented ");
    assert( get_signed_volume(B) > VT::zero() || !"overlap_tetrahedron_tetrahedron(): B was left oriented ");

    V const & A0 = A.point(0);
    V const & A1 = A.point(1);
    V const & A2 = A.point(2);
    V const & A3 = A.point(3);

    V const & B0 = B.point(0);
    V const & B1 = B.point(1);
    V const & B2 = B.point(2);
    V const & B3 = B.point(3);

    for (unsigned int i = 0u; i < 4u; ++i)
    {
      Triangle<V> const triangle = get_opposite_face( i, A );
      Plane<V>    const plane    = make_plane(triangle);

      T const b0 = get_signed_distance(B0, plane);
      T const b1 = get_signed_distance(B1, plane);
      T const b2 = get_signed_distance(B2, plane);
      T const b3 = get_signed_distance(B3, plane);

      if ( b0 > VT::zero() && b1 > VT::zero() && b2 > VT::zero() && b3 > VT::zero() )
        return false;
    }

    for (unsigned int i = 0u; i < 4u; ++i)
    {
      Triangle<V> const triangle = get_opposite_face( i, B );
      Plane<V>    const plane    = make_plane(triangle);

      T const a0 = get_signed_distance(A0, plane);
      T const a1 = get_signed_distance(A1, plane);
      T const a2 = get_signed_distance(A2, plane);
      T const a3 = get_signed_distance(A3, plane);

      if ( a0 > VT::zero() && a1 > VT::zero() && a2 > VT::zero() && a3 > VT::zero() )
        return false;
    }

    // None of the face normals was a separating axis, now we test the edges

    // let edge0 be vertices 0 and 1
    // let edge1 be vertices 0 and 2
    // let edge2 be vertices 0 and 3
    // let edge3 be vertices 1 and 2
    // let edge4 be vertices 1 and 3
    // let edge5 be vertices 2 and 3

    // Ae0 v Be0
    V const ae0be0 = tiny::cross(A1-A0, B1-B0);
    if (detail::SAT_line(A1, A2, A3, B1, B2, B3, ae0be0))
      return false;

    // Ae0 v Be1
    V const ae0be1 = tiny::cross(A1-A0, B2-B0);
    if (detail::SAT_line(A1, A2, A3, B0, B1, B3, ae0be1))
      return false;

    // Ae0 v Be2
    V const ae0be2 = tiny::cross(A1-A0, B3-B0);
    if (detail::SAT_line(A1, A2, A3, B0, B1, B2, ae0be2))
      return false;

    // Ae0 v Be3
    V const ae0be3 = tiny::cross(A1-A0, B2-B1);
    if (detail::SAT_line(A1, A2, A3, B0, B1, B3, ae0be3))
      return false;

    // Ae0 v Be4
    V const ae0be4 = tiny::cross(A1-A0, B3-B1);
    if (detail::SAT_line(A1, A2, A3, B0, B1, B2, ae0be4))
      return false;

    // Ae0 v Be5
    V const ae0be5 = tiny::cross(A1-A0, B3-B2);
    if (detail::SAT_line(A1, A2, A3, B0, B1, B2, ae0be5))
      return false;

    // Ae1 v Be0
    V const ae1be0 = tiny::cross(A2-A0, B1-B0);
    if (detail::SAT_line(A1, A2, A3, B1, B2, B3, ae1be0))
      return false;

    // Ae1 v Be1
    V const ae1be1 = tiny::cross(A2-A0, B2-B0);
    if (detail::SAT_line(A1, A2, A3, B1, B2, B3, ae1be1))
      return false;

    // Ae1 v Be2
    V const ae1be2 = tiny::cross(A2-A0, B3-B0);
    if (detail::SAT_line(A1, A2, A3, B0, B1, B2, ae1be2))
      return false;

    // Ae1 v Be3
    V const ae1be3 = tiny::cross(A2-A0, B2-B1);
    if (detail::SAT_line(A1, A2, A3, B0, B1, B3, ae1be3))
      return false;

    // Ae1 v Be4
    V const ae1be4 = tiny::cross(A2-A0, B3-B1);
    if (detail::SAT_line(A1, A2, A3, B0, B1, B2, ae1be4))
      return false;

    // Ae1 v Be5
    V const ae1be5 = tiny::cross(A2-A0, B3-B2);
    if (detail::SAT_line(A1, A2, A3, B0, B1, B2, ae1be5))
      return false;

    // Ae2 v Be0
    V const ae2be0 = tiny::cross(A3-A0, B1-B0);
    if (detail::SAT_line(A1, A2, A3, B1, B2, B3, ae2be0))
      return false;

    // Ae2 v Be1
    V const ae2be1 = tiny::cross(A3-A0, B2-B0);
    if (detail::SAT_line(A1, A2, A3, B1, B2, B3, ae2be1))
      return false;

    // Ae2 v Be2
    V const ae2be2 = tiny::cross(A3-A0, B3-B0);
    if (detail::SAT_line(A1, A2, A3, B1, B2, B3, ae2be2))
      return false;

    // Ae2 v Be3
    V const ae2be3 = tiny::cross(A3-A0, B2-B1);
    if (detail::SAT_line(A1, A2, A3, B0, B1, B3, ae2be3))
      return false;

    // Ae2 v Be4
    V const ae2be4 = tiny::cross(A3-A0, B3-B1);
    if (detail::SAT_line(A1, A2, A3, B0, B1, B2, ae2be4))
      return false;

    // Ae2 v Be5
    V const ae2be5 = tiny::cross(A3-A0, B3-B2);
    if (detail::SAT_line(A1, A2, A3, B0, B1, B2, ae2be5))
      return false;

    // Ae3 v Be0
    V const ae3be0 = tiny::cross(A2-A1, B1-B0);
    if (detail::SAT_line(A0, A1, A3, B1, B2, B3, ae3be0))
      return false;

    // Ae3 v Be1
    V const ae3be1 = tiny::cross(A2-A1, B2-B0);
    if (detail::SAT_line(A0, A1, A3, B1, B2, B3, ae3be1))
      return false;

    // Ae3 v Be2
    V const ae3be2 = tiny::cross(A2-A1, B3-B0);
    if (detail::SAT_line(A0, A1, A3, B1, B2, B3, ae3be2))
      return false;

    // Ae3 v Be3
    V const ae3be3 = tiny::cross(A2-A1, B2-B1);
    if (detail::SAT_line(A0, A1, A3, B0, B1, B3, ae3be3))
      return false;

    // Ae3 v Be4
    V const ae3be4 = tiny::cross(A2-A1, B3-B1);
    if (detail::SAT_line(A0, A1, A3, B0, B1, B2, ae3be4))
      return false;

    // Ae3 v Be5
    V const ae3be5 = tiny::cross(A2-A1, B3-B2);
    if (detail::SAT_line(A0, A1, A3, B0, B1, B2, ae3be5))
      return false;

    // Ae4 v Be0
    V const ae4be0 = tiny::cross(A3-A1, B1-B0);
    if (detail::SAT_line(A0, A1, A2, B1, B2, B3, ae4be0))
      return false;

    // Ae4 v Be1
    V const ae4be1 = tiny::cross(A3-A1, B2-B0);
    if (detail::SAT_line(A0, A1, A2, B1, B2, B3, ae4be1))
      return false;

    // Ae4 v Be2
    V const ae4be2 = tiny::cross(A3-A1, B3-B0);
    if (detail::SAT_line(A0, A1, A2, B1, B2, B3, ae4be2))
      return false;

    // Ae4 v Be3
    V const ae4be3 = tiny::cross(A3-A1, B2-B1);
    if (detail::SAT_line(A0, A1, A2, B0, B1, B3, ae4be3))
      return false;

    // Ae4 v Be4
    V const ae4be4 = tiny::cross(A3-A1, B3-B1);
    if (detail::SAT_line(A0, A1, A2, B0, B1, B2, ae4be4))
      return false;

    // Ae4 v Be5
    V const ae4be5 = tiny::cross(A3-A1, B3-B2);
    if (detail::SAT_line(A0, A1, A2, B0, B1, B2, ae4be5))
      return false;

    // Ae5 v Be0
    V const ae5be0 = tiny::cross(A3-A2, B1-B0);
    if (detail::SAT_line(A0, A1, A2, B1, B2, B3, ae5be0))
      return false;

    // Ae5 v Be1
    V const ae5be1 = tiny::cross(A3-A2, B2-B0);
    if (detail::SAT_line(A0, A1, A2, B1, B2, B3, ae5be1))
      return false;

    // Ae5 v Be2
    V const ae5be2 = tiny::cross(A3-A2, B3-B0);
    if (detail::SAT_line(A0, A1, A2, B1, B2, B3, ae5be2))
      return false;

    // Ae5 v Be3
    V const ae5be3 = tiny::cross(A3-A2, B2-B1);
    if (detail::SAT_line(A0, A1, A2, B0, B1, B3, ae5be3))
      return false;

    // Ae5 v Be4
    V const ae5be4 = tiny::cross(A3-A2, B3-B1);
    if (detail::SAT_line(A0, A1, A2, B0, B1, B2, ae5be4))
      return false;

    // Ae5 v Be5
    V const ae5be5 = tiny::cross(A3-A2, B3-B2);
    if (detail::SAT_line(A0, A1, A2, B0, B1, B2, ae5be5))
      return false;
    
    return true;
  }
  
}// namespace geometry

// GEOMETRY_OVERLAP_TETRAHEDRON_TETRAHEDRON_H
#endif