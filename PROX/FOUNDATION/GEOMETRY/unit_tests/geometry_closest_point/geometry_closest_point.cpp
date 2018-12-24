#include <geometry.h>
#include <tiny.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

#include <vector>


BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(closest_point_test)
{
  typedef tiny::MathTypes<float> MT;
  typedef MT::vector3_type       V;

  {
    geometry::Line<V> const L = geometry::Line<V>( V::make(0,0,0), V::make(1,0,0));  // x-axis line
    
    V const q = geometry::closest_point_on_line( V::make(10,4,5), L );
    
    BOOST_CHECK_CLOSE( q(0), 10.0f, 0.01f );
    BOOST_CHECK_CLOSE( q(1), 0.0f, 0.01f );
    BOOST_CHECK_CLOSE( q(2), 0.0f, 0.01f );
  }
  {
    geometry::Plane<V> const P = geometry::make_plane( V::make(1,0,0), 0);  // y-z plane

    V const q = geometry::closest_point_on_plane( V::make(10,4,5), P );
    
    BOOST_CHECK_CLOSE( q(0), 0.0f, 0.01f );
    BOOST_CHECK_CLOSE( q(1), 4.0f, 0.01f );
    BOOST_CHECK_CLOSE( q(2), 5.0f, 0.01f );
  }
  {
    geometry::Line<V> const L1 = geometry::Line<V>( V::make(0,0,1), V::make(0,1,0));  // y-axis line
    geometry::Line<V> const L2 = geometry::Line<V>( V::make(0,0,0), V::make(1,0,0));  // x-axis line

    V p1, p2;

    geometry::closest_points_line_line( L1, L2, p1, p2 );

    BOOST_CHECK_CLOSE( p1(0), 0.0f, 0.01f );
    BOOST_CHECK_CLOSE( p1(1), 0.0f, 0.01f );
    BOOST_CHECK_CLOSE( p1(2), 1.0f, 0.01f );

    BOOST_CHECK_CLOSE( p2(0), 0.0f, 0.01f );
    BOOST_CHECK_CLOSE( p2(1), 0.0f, 0.01f );
    BOOST_CHECK_CLOSE( p2(2), 0.0f, 0.01f );

  }
}

BOOST_AUTO_TEST_SUITE_END();
