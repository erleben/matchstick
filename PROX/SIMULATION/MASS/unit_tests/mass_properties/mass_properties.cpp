#include <mass.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(mass);

BOOST_AUTO_TEST_CASE(properties)
{
  typedef mass::Properties<float> prop_type;
  
  prop_type A;
  
  BOOST_CHECK_CLOSE( A.m_m,   0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_Ixx, 0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_Iyy, 0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_Izz, 0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_Ixy, 0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_Ixz, 0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_Iyz, 0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_x,   0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_y,   0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_z,   0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_Qs,  1.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_Qx,  0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_Qy,  0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_Qz,  0.0f, 0.01f );
  
  BOOST_CHECK( A.is_body_space() == true );
  BOOST_CHECK( A.is_model_space() == false );
  
  A.m_x = 5.0f;
  BOOST_CHECK( A.is_body_space() == false );
  BOOST_CHECK( A.is_model_space() == true );
  
  A.clear();    
  BOOST_CHECK_CLOSE( A.m_m,   0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_Ixx, 0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_Iyy, 0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_Izz, 0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_Ixy, 0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_Ixz, 0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_Iyz, 0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_x,   0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_y,   0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_z,   0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_Qs,  1.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_Qx,  0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_Qy,  0.0f, 0.01f );
  BOOST_CHECK_CLOSE( A.m_Qz,  0.0f, 0.01f );
 
  A.m_x = 5.0f;
  
  prop_type B;
  BOOST_CHECK( (A==B) == false ); 
  B = A;
  BOOST_CHECK( (A==B) == true ); 
  
  prop_type C = prop_type( A );
  BOOST_CHECK( (A==C) == true ); 
}

BOOST_AUTO_TEST_SUITE_END();
