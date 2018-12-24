#include <mass.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(mass);

BOOST_AUTO_TEST_CASE(tetrahedron)
{
  double const rho  =  1.0;
  
  double const X1   = -1.0;
  double const Y1   =  0.0;
  double const Z1   = -1.0;
  double const X2   =  1.0;
  double const Y2   =  0.0;
  double const Z2   = -1.0;
  double const X3   =  0.0;
  double const Y3   = -1.0;
  double const Z3   =  1.0;
  double const X4   =  0.0;
  double const Y4   =  1.0;
  double const Z4   =  1.0;
  
    
  mass::Properties<double> P = mass::compute_tetrahedron(
                            rho
                            ,X1,Y1,Z1
                            ,X2,Y2,Z2
                            ,X3,Y3,Z3
                            ,X4,Y4,Z4
                            );
  
  BOOST_CHECK_CLOSE(P.m_x, 0.0, 0.01 );
  BOOST_CHECK_CLOSE(P.m_y, 0.0, 0.01 );
  BOOST_CHECK_CLOSE(P.m_z, 0.0, 0.01 );
  BOOST_CHECK(P.m_m  > 0.0);
  BOOST_CHECK(P.m_Ixx > 0.0);
  BOOST_CHECK(P.m_Iyy > 0.0);
  BOOST_CHECK(P.m_Izz > 0.0);
  BOOST_CHECK_CLOSE(P.m_Ixy, 0.0, 0.01 );
  BOOST_CHECK_CLOSE(P.m_Ixz, 0.0, 0.01 );
  BOOST_CHECK_CLOSE(P.m_Iyz, 0.0, 0.01 );
  BOOST_CHECK_CLOSE(P.m_Ixx, P.m_Iyy, 0.01 );
  BOOST_CHECK(P.m_Izz < P.m_Ixx);
  
  BOOST_CHECK(   P.is_body_space() );  
  BOOST_CHECK(  !P.is_model_space() );  
}

BOOST_AUTO_TEST_SUITE_END();
