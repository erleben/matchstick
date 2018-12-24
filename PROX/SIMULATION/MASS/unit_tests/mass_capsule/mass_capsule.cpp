#include <mass.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(mass);

BOOST_AUTO_TEST_CASE(capsule)
{
  float const rho = 1.0f;
  float const R   = 1.0f;
  float const H   = 2.0f;
    
  mass::Properties<float> P = mass::compute_capsule(rho,R,H);
  
  BOOST_CHECK( P.m_m   > 0.0f ); 
  BOOST_CHECK( P.m_Ixx > 0.0f ); 
  BOOST_CHECK( P.m_Iyy > 0.0f ); 
  BOOST_CHECK( P.m_Izz > 0.0f ); 
  BOOST_CHECK_CLOSE( P.m_Ixx, P.m_Izz, 0.01f );
  BOOST_CHECK( P.m_Izz > P.m_Iyy );  
  
  BOOST_CHECK(  P.is_body_space() );  
  BOOST_CHECK( !P.is_model_space() );  
}

BOOST_AUTO_TEST_SUITE_END();
