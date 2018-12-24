#include <tiny_math_types.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(tiny_norm_1);

BOOST_AUTO_TEST_CASE(simple_test)
{
  typedef tiny::MathTypes<double> math_types;

  typedef math_types::real_type        T;
  typedef math_types::vector3_type     V;

  V v0 = V::make(0.1, -0.01, 0.0 );
  T n0 = tiny::norm_1( v0 );
  BOOST_CHECK_CLOSE( n0, 0.1, 0.01);

  V v1 = V::make(-0.1, -0.01, 0.0 );
  T n1 = tiny::norm_1( v1 );
  BOOST_CHECK_CLOSE( n1, 0.1, 0.01);

  V v2 = V::make( -0.01, 0.1, 0.0 );
  T n2 = tiny::norm_1( v2 );
  BOOST_CHECK_CLOSE( n2, 0.1, 0.01);

  V v3 = V::make( -0.01, -0.1, 0.0 );
  T n3 = tiny::norm_1( v3 );
  BOOST_CHECK_CLOSE( n3, 0.1, 0.01);

  V v4 = V::make( -0.01, 0.0, 0.1 );
  T n4 = tiny::norm_1( v4 );
  BOOST_CHECK_CLOSE( n4, 0.1, 0.01);

  V v5 = V::make( -0.01, 0.0, -0.1 );
  T n5 = tiny::norm_1( v5 );
  BOOST_CHECK_CLOSE( n5, 0.1, 0.01);
}

BOOST_AUTO_TEST_SUITE_END();
