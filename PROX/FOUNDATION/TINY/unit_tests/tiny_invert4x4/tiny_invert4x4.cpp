#include <tiny_invert4x4.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(tiny_invert4x4);

BOOST_AUTO_TEST_CASE(correctness_test)
{
  double M[4][4];
      
  M[0][0] = 0.8147;  M[0][1] = 0.6324;  M[0][2] = 0.9575;  M[0][3] = 0.9572;
  M[1][0] = 0.9058;  M[1][1] = 0.0975;  M[1][2] = 0.9649;  M[1][3] = 0.4854;
  M[2][0] = 0.1270;  M[2][1] = 0.2785;  M[2][2] = 0.1576;  M[2][3] = 0.8003;
  M[3][0] = 0.9134;  M[3][1] = 0.5469;  M[3][2] = 0.9706;  M[3][3] = 0.1419;

  tiny::invert4x4(M);
  
  BOOST_CHECK_CLOSE( M[0][0], -15.2997, 1.0 );
  BOOST_CHECK_CLOSE( M[0][1],   3.0761, 1.0 );
  BOOST_CHECK_CLOSE( M[0][2],  14.7235, 1.0 );
  BOOST_CHECK_CLOSE( M[0][3],   9.6445, 1.0 );

  BOOST_CHECK_CLOSE( M[1][0], -0.2088, 1.0 );
  BOOST_CHECK_CLOSE( M[1][1], -1.8442, 1.0 );
  BOOST_CHECK_CLOSE( M[1][2],  1.0366, 1.0 );
  BOOST_CHECK_CLOSE( M[1][3],  1.8711, 1.0 );

  BOOST_CHECK_CLOSE( M[2][0],  14.5694, 1.0 );
  BOOST_CHECK_CLOSE( M[2][1],  -1.9337, 1.0 );
  BOOST_CHECK_CLOSE( M[2][2], -14.6497, 1.0 );
  BOOST_CHECK_CLOSE( M[2][3],  -9.0413, 1.0 );

  BOOST_CHECK_CLOSE( M[3][0], -0.3690, 1.0 );
  BOOST_CHECK_CLOSE( M[3][1],  0.5345, 1.0 );
  BOOST_CHECK_CLOSE( M[3][2],  1.4378, 1.0 );
  BOOST_CHECK_CLOSE( M[3][3], -0.4008, 1.0 );
}

BOOST_AUTO_TEST_SUITE_END();
