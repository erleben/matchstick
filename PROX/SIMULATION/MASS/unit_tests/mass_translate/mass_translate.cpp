#include <mass.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(mass);

BOOST_AUTO_TEST_CASE(translate)
{
  
  mass::Properties<double> B = mass::compute_sphere(1.0,1.0);
  
  double const dx   = 1.0;
  double const dy   = 2.0;
  double const dz   = 3.0;
  
  BOOST_CHECK( B.is_body_space() );  

  mass::Properties<double> M = mass::translate_to_model_frame( dx, dy, dz, B);  
  
  BOOST_CHECK( M.is_model_space() );  

  BOOST_CHECK(M.m_Ixx > B.m_Ixx);
  BOOST_CHECK(M.m_Iyy > B.m_Iyy);
  BOOST_CHECK(M.m_Izz > B.m_Izz);
  BOOST_CHECK(M.m_Ixy < B.m_Ixy);
  BOOST_CHECK(M.m_Ixz < B.m_Ixz);
  BOOST_CHECK(M.m_Iyz < B.m_Iyz);
  
  BOOST_CHECK_CLOSE(M.m_m, B.m_m, 0.01 );
  
  BOOST_CHECK_CLOSE(M.m_x, dx, 0.01 );
  BOOST_CHECK_CLOSE(M.m_y, dy, 0.01 );
  BOOST_CHECK_CLOSE(M.m_z, dz, 0.01 );
  
  BOOST_CHECK_CLOSE(M.m_Qs, 1.0, 0.01 );
  BOOST_CHECK_CLOSE(M.m_Qx, 0.0, 0.01 );
  BOOST_CHECK_CLOSE(M.m_Qy, 0.0, 0.01 );
  BOOST_CHECK_CLOSE(M.m_Qz, 0.0, 0.01 );

  mass::Properties<double> C = mass::translate_to_body_frame( M);  

  BOOST_CHECK_CLOSE(C.m_Ixx , B.m_Ixx, 0.01);
  BOOST_CHECK_CLOSE(C.m_Iyy , B.m_Iyy, 0.01);
  BOOST_CHECK_CLOSE(C.m_Izz , B.m_Izz, 0.01);
  BOOST_CHECK_CLOSE(C.m_Ixy , B.m_Ixy, 0.01);
  BOOST_CHECK_CLOSE(C.m_Ixz , B.m_Ixz, 0.01);
  BOOST_CHECK_CLOSE(C.m_Iyz , B.m_Iyz, 0.01);
  BOOST_CHECK_CLOSE(C.m_m   , B.m_m  , 0.01 );
  BOOST_CHECK_CLOSE(C.m_x   , 0.0    , 0.01 );
  BOOST_CHECK_CLOSE(C.m_y   , 0.0    , 0.01 );
  BOOST_CHECK_CLOSE(C.m_z   , 0.0    , 0.01 );
  BOOST_CHECK_CLOSE(C.m_Qs  , 1.0    , 0.01 );
  BOOST_CHECK_CLOSE(C.m_Qx  , 0.0    , 0.01 );
  BOOST_CHECK_CLOSE(C.m_Qy  , 0.0    , 0.01 );
  BOOST_CHECK_CLOSE(C.m_Qz  , 0.0    , 0.01 );
  
  BOOST_CHECK( C.is_body_space() );  
}

BOOST_AUTO_TEST_SUITE_END();
