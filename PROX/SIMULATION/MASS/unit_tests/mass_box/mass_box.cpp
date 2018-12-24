#include <mass.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(mass);

BOOST_AUTO_TEST_CASE(box)
{
  double const rho = 1.0;
  double const W   = 1.0;
  double const H   = 1.0;
  double const D   = 1.0;
   
  mass::Properties<double> P = mass::compute_box(rho,W,H,D);
  
  BOOST_CHECK_CLOSE( P.m_m, 8.0, 0.01 );
  BOOST_CHECK( P.m_Ixx > 0.0 ); 
  BOOST_CHECK( P.m_Iyy > 0.0 ); 
  BOOST_CHECK( P.m_Izz > 0.0 ); 
  
  BOOST_CHECK_CLOSE( P.m_Ixx, P.m_Iyy, 0.01 );
  BOOST_CHECK_CLOSE( P.m_Iyy, P.m_Izz, 0.01 );
  
  BOOST_CHECK( !P.is_model_space() ); 
  BOOST_CHECK(  P.is_body_space()  ); 
}

BOOST_AUTO_TEST_SUITE_END();
