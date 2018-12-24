#include <geometry.h>
#include <tiny.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

#include <vector>

BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(line)
{
  typedef tiny::MathTypes<float> MT;
  typedef MT::vector3_type       V;
  
  {
    V const p0 = V::make( 0.0f, 0.0f, 0.0f );
    V const p1 = V::make( 2.0f, 0.0f, 0.0f );
    
    geometry::Line<V> L = geometry::make_line(p0, p1);
    
    BOOST_CHECK_EQUAL( L.point()(0), 0.0f );
    BOOST_CHECK_EQUAL( L.point()(1), 0.0f );
    BOOST_CHECK_EQUAL( L.point()(2), 0.0f );
    
    BOOST_CHECK_EQUAL( L.direction()(0), 1.0f );
    BOOST_CHECK_EQUAL( L.direction()(1), 0.0f );
    BOOST_CHECK_EQUAL( L.direction()(2), 0.0f );
    
  }
  {
    V const p0 = V::make( 0.0f, 0.0f, 0.0f );
    V const p1 = V::make( 2.0f, 0.0f, 0.0f );

    geometry::Line<V> L = geometry::make_line(p0, p1, geometry::FROM_POINTS() );

    BOOST_CHECK_EQUAL( L.point()(0), 0.0f );
    BOOST_CHECK_EQUAL( L.point()(1), 0.0f );
    BOOST_CHECK_EQUAL( L.point()(2), 0.0f );

    BOOST_CHECK_EQUAL( L.direction()(0), 1.0f );
    BOOST_CHECK_EQUAL( L.direction()(1), 0.0f );
    BOOST_CHECK_EQUAL( L.direction()(2), 0.0f );

  }
  {
    V const p = V::make( 0.0f, 0.0f, 0.0f );
    V const d = V::make( 0.5f, 0.0f, 0.0f );

    geometry::Line<V> L = geometry::make_line(p, d, geometry::FROM_DIRECTION() );

    BOOST_CHECK_EQUAL( L.point()(0), 0.0f );
    BOOST_CHECK_EQUAL( L.point()(1), 0.0f );
    BOOST_CHECK_EQUAL( L.point()(2), 0.0f );

    BOOST_CHECK_EQUAL( L.direction()(0), 1.0f );
    BOOST_CHECK_EQUAL( L.direction()(1), 0.0f );
    BOOST_CHECK_EQUAL( L.direction()(2), 0.0f );
  }

}

BOOST_AUTO_TEST_SUITE_END();
