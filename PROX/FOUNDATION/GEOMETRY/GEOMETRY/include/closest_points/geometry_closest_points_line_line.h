#ifndef GEOMETRY_CLOSEST_POINTS_LINE_LINE_H
#define GEOMETRY_CLOSEST_POINTS_LINE_LINE_H

#include <types/geometry_line.h>

#include <tiny_vector_functions.h>
#include <tiny_value_traits.h>
#include <tiny_is_finite.h>
#include <tiny_is_number.h>
#include <tiny_precision.h>

#include <cassert>

namespace geometry
{

  /**
   * Compute Closest Points between Lines
   *
   *
   * @param pA   Point on first line.
   * @param uA   Direction of first line (unit vector).
   * @param pB   Point on second line.
   * @param uB   Direction of second line (unit vector).
   * @param s    The line parameter of the closest point on the first line pA + uA*s
   * @param t    The line parameter of the closest point on the second line pB + uB*t
   *
   */
  template<typename V>
  inline void closest_points_line_line(
                                       V const & pointA
                                       , V const & directionA
                                       , V const & pointB
                                       , V const & directionB
                                       , typename V::real_type & s
                                       , typename V::real_type & t
                                       )
  {
    using std::fabs;

    typedef typename V::real_type         T;
    typedef          tiny::ValueTraits<T> VT;

    T const epsilon = tiny::working_precision<T>();

    V const r  =   pointB - pointA;
    T const k  =   tiny::inner_prod(directionA,directionB);
    T const q1 =   tiny::inner_prod(directionA,r);
    T const q2 = - tiny::inner_prod(directionB,r);
    T const w  =   VT::one() - k*k;

    assert( is_number( k )  || !"closest_points_line_line(): nan");
    assert( is_finite( k )  || !"closest_points_line_line(): inf");
    assert( is_number( q1 ) || !"closest_points_line_line(): nan");
    assert( is_finite( q1 ) || !"closest_points_line_line(): inf");
    assert( is_number( q2 ) || !"closest_points_line_line(): nan");
    assert( is_finite( q2 ) || !"closest_points_line_line(): inf");
    assert( is_number( w )  || !"closest_points_line_line(): nan");
    assert( is_finite( w )  || !"closest_points_line_line(): inf");

    s = VT::zero();
    t = VT::zero();

    if(fabs(w) > epsilon)
    {
      s = (q1 + k*q2)/w;
      t = (q2 + k*q1)/w;
    }

    assert( is_number( s ) || !"closest_points_line_line(): nan");
    assert( is_finite( s ) || !"closest_points_line_line(): inf");
    assert( is_number( t ) || !"closest_points_line_line(): nan");
    assert( is_finite( t ) || !"closest_points_line_line(): inf");
  }

  template<typename V>
  inline void closest_points_line_line(
                                               Line<V> const & A
                                               , Line<V> const & B
                                               , V & point_on_A
                                               , V & point_on_B
                                               )
  {
    typedef typename V::real_type T;

    T s;
    T t;

    closest_points_line_line(A.point(), A.direction(), B.point(), B.direction(), s, t);

    point_on_A = A.point() + s * A.direction();
    point_on_B = B.point() + t * B.direction();
  }

}//namespace geometry

//GEOMETRY_CLOSEST_POINTS_LINE_LINE_H
#endif