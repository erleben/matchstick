#include <mass.h>
#include <tiny_math_types.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(mass);

BOOST_AUTO_TEST_CASE(rotate_test)
{
  using namespace tiny;
  
  typedef MathTypes<double>       types;
  typedef types::quaternion_type  Q;
  typedef types::value_traits     VT;
  
  Q q = Q::Rx( VT::convert_to_radians(10.0) );
  
  double const Qs  = q.real();
  double const Qx  = q.imag()(0);
  double const Qy  = q.imag()(1);
  double const Qz  = q.imag()(2);

  mass::Properties<double> P;
  
  P.m_Ixx = 9.0;
  P.m_Iyy = 10.0;
  P.m_Izz = 11.0;
  P.m_Ixy = 0.0;
  P.m_Ixz = 0.0;
  P.m_Iyz = 0.0;
  
  P = mass::rotate( Qs, Qx, Qy, Qz, P);
  
  BOOST_CHECK_CLOSE( P.m_Ixx, 9.0, 0.1); 
  BOOST_CHECK_CLOSE( P.m_Iyy, 10.030153689607044, 0.1); 
  BOOST_CHECK_CLOSE( P.m_Izz, 10.969846310392953, 0.1); 
  BOOST_CHECK_CLOSE( P.m_Ixy, 0.0, 0.1); 
  BOOST_CHECK_CLOSE( P.m_Ixz, 0.0, 0.1); 
  BOOST_CHECK_CLOSE( P.m_Iyz, -0.17101007166283444, 0.1);   
}


BOOST_AUTO_TEST_CASE(rotate_to_body_frame_test)
{
  mass::Properties<double> P;
  
  P.m_Ixx =  9.0;
  P.m_Iyy = 10.030153689607044;
  P.m_Izz = 10.969846310392953;
  P.m_Ixy =  0.0;
  P.m_Ixz =  0.0;
  P.m_Iyz =  -0.17101007166283444;
  
  P = mass::rotate_to_body_frame( P );
    
  BOOST_CHECK_CLOSE( P.m_Ixx , 11.0, 0.1 );
  BOOST_CHECK_CLOSE( P.m_Iyy , 10.0, 0.1 );
  BOOST_CHECK_CLOSE( P.m_Izz ,  9.0, 0.1 );
  BOOST_CHECK_CLOSE( (P.m_Qs*P.m_Qs + P.m_Qx*P.m_Qx + P.m_Qy*P.m_Qy + P.m_Qz*P.m_Qz) , 1.0, 0.1); 

}

BOOST_AUTO_TEST_SUITE_END();
