#include <tiny_trilinear.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(tiny_trilinear);

BOOST_AUTO_TEST_CASE(simple_test)
{
  
  double d000 = 1.0;
  double d001 = 2.0;
  double d010 = 3.0;
  double d011 = 4.0;
  double d100 = 5.0;
  double d101 = 6.0;
  double d110 = 7.0;
  double d111 = 8.0;

  double value = tiny::trilinear( d000, d001, d010, d011, d100, d101, d110, d111, 0.0, 0.0, 0.0 );
  BOOST_CHECK_CLOSE( value, d000, 0.01 );
  
  value = tiny::trilinear( d000, d001, d010, d011, d100, d101, d110, d111, 1.0, 0.0, 0.0 );
  BOOST_CHECK_CLOSE( value, d001, 0.01 );
  
  value = tiny::trilinear( d000, d001, d010, d011, d100, d101, d110, d111, 0.0, 1.0, 0.0 );
  BOOST_CHECK_CLOSE( value, d010, 0.01 );
  
  value = tiny::trilinear( d000, d001, d010, d011, d100, d101, d110, d111, 1.0, 1.0, 0.0 );
  BOOST_CHECK_CLOSE( value, d011, 0.01 );
  
  value = tiny::trilinear( d000, d001, d010, d011, d100, d101, d110, d111, 0.0, 0.0, 1.0 );
  BOOST_CHECK_CLOSE( value, d100, 0.01 );
  
  value = tiny::trilinear( d000, d001, d010, d011, d100, d101, d110, d111, 1.0, 0.0, 1.0 );
  BOOST_CHECK_CLOSE( value, d101, 0.01 );
  
  value = tiny::trilinear( d000, d001, d010, d011, d100, d101, d110, d111, 0.0, 1.0, 1.0 );
  BOOST_CHECK_CLOSE( value, d110, 0.01 );
  
  value = tiny::trilinear( d000, d001, d010, d011, d100, d101, d110, d111, 1.0, 1.0, 1.0 );
  BOOST_CHECK_CLOSE( value, d111, 0.01 );
  
}

BOOST_AUTO_TEST_SUITE_END();
