#include <geometry.h>
#include <tiny.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

#include <vector>


BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(plane)
{
  typedef tiny::MathTypes<float> MT;
  typedef MT::vector3_type       V;
  typedef MT::real_type          T;
  
  {
    V const p0 = V::make(1,0,2);
    V const p1 = V::make(1,1,2);
    V const p2 = V::make(0,1,2);
    
    geometry::Plane<V> P = geometry::make_plane( p0, p1, p2);
    
    BOOST_CHECK_CLOSE( P.n()(0), 0.0f, 0.01f );
    BOOST_CHECK_CLOSE( P.n()(1), 0.0f, 0.01f );
    BOOST_CHECK_CLOSE( P.n()(2), 1.0f, 0.01f );
    BOOST_CHECK_CLOSE( P.w(),    2.0f, 0.01f );

    V const q0  = V::make(0,0,4);
    V const q1  = V::make(0,0,0);

    T const d0 = geometry::get_signed_distance(q0, P);
    BOOST_CHECK_CLOSE( d0,  2.0f, 0.01f );

    T const d1 = geometry::get_signed_distance(q1, P);
    BOOST_CHECK_CLOSE( d1,  -2.0f, 0.01f );

    T const a0 = geometry::get_distance(q0, P);
    BOOST_CHECK_CLOSE( a0,  2.0f, 0.01f );

    T const a1 = geometry::get_distance(q1, P);
    BOOST_CHECK_CLOSE( a1,  2.0f, 0.01f );
  }
  {
    V const normal = V::make(0,0,1);
    T const offset = 2.0;

    geometry::Plane<V> P = geometry::make_plane( normal, offset);

    BOOST_CHECK_CLOSE( P.n()(0), 0.0f, 0.01f );
    BOOST_CHECK_CLOSE( P.n()(1), 0.0f, 0.01f );
    BOOST_CHECK_CLOSE( P.n()(2), 1.0f, 0.01f );
    BOOST_CHECK_CLOSE( P.w(),    2.0f, 0.01f );
  }
  {
    V const p0 = V::make(1,0,2);
    V const p1 = V::make(1,1,2);
    V const p2 = V::make(0,1,2);

    geometry::Triangle<V> triangle = geometry::make_triangle(p0, p1, p2);

    geometry::Plane<V> P = geometry::make_plane( triangle );

    BOOST_CHECK_CLOSE( P.n()(0), 0.0f, 0.01f );
    BOOST_CHECK_CLOSE( P.n()(1), 0.0f, 0.01f );
    BOOST_CHECK_CLOSE( P.n()(2), 1.0f, 0.01f );
    BOOST_CHECK_CLOSE( P.w(),    2.0f, 0.01f );
  }


}

BOOST_AUTO_TEST_SUITE_END();
