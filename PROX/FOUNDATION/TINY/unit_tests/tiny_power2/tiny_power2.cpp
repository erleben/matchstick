#include <tiny_power2.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(tiny_power2);

BOOST_AUTO_TEST_CASE(functional_test)
{
  BOOST_CHECK(  tiny::is_power2( 9 ) == false );
  BOOST_CHECK(  tiny::is_power2( 8 ) == true );
  
  BOOST_CHECK(  tiny::upper_power2( 8 ) == 8 );
  BOOST_CHECK(  tiny::lower_power2( 8 ) == 8 );
  
  BOOST_CHECK(  tiny::upper_power2( 9 ) == 16 );
  BOOST_CHECK(  tiny::lower_power2( 9 ) == 8 );
}

BOOST_AUTO_TEST_SUITE_END();
