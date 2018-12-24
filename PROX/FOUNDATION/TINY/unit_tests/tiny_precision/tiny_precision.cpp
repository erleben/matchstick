#include <tiny_precision.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(tiny_precision);

BOOST_AUTO_TEST_CASE(value_tests)
{
  double value1  = tiny::machine_precision<double>();
  BOOST_CHECK_CLOSE( value1, 2.2204460492503131e-16, 0.01 );
  
  float  value2  = tiny::machine_precision<float>();
  BOOST_CHECK_CLOSE( value2, 1.1920929e-07f, 0.01f );
}

BOOST_AUTO_TEST_SUITE_END();
