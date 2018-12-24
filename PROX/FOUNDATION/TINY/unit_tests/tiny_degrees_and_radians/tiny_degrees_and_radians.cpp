#include <tiny_math_types.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>


typedef tiny::MathTypes<double>  MT;
typedef MT::value_traits         VT;


BOOST_AUTO_TEST_SUITE(tiny_degrees_and_radians);

BOOST_AUTO_TEST_CASE(conversion)
{
  double const degrees180 = 180.0;
  double const radians180 = VT::convert_to_radians(degrees180);

  BOOST_CHECK_CLOSE( radians180, VT::pi(), 0.1 );

  double const radiansPI = VT::pi();
  double const degreesPI = VT::convert_to_degrees(radiansPI);

  BOOST_CHECK_CLOSE( degreesPI, 180.0, 0.1 );

}

BOOST_AUTO_TEST_SUITE_END();
