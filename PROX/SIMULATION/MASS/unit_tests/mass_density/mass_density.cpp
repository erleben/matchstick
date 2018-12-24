#include <mass.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(mass);

BOOST_AUTO_TEST_CASE(density)
{
  double const rho = mass::get_density<double>( mass::WATER );
  std::string const units = mass::get_SI_units();
  
  BOOST_CHECK(rho > 0.0f);
}

BOOST_AUTO_TEST_SUITE_END();
