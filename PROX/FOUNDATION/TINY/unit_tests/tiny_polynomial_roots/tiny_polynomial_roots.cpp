#include <tiny_polynomial_roots.h>

#include <algorithm>  // Needed for std::sort

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(tiny_polynomial_roots);

BOOST_AUTO_TEST_CASE(simple_test)
{
  unsigned int count = 0u;
  
  double roots[4] = {0.0, 0.0, 0.0, 0.0};
  
  // Case of one root
  {
    //  0 = c1 x + c0    <=> 0 = (x - r1)
    double r1 = 4.0;
    double C0 = -r1;
    double C1 = 1.0;
    bool success = tiny::compute_polynomial_roots( C0, C1, count, roots );    
    BOOST_CHECK( success );
    BOOST_CHECK( count == 1 );
    BOOST_CHECK_CLOSE( roots[0] , r1, 0.01 );
  }
  
  // Case of two roots
  {
    //  0 = c2 x^2 + c1 x + c0    <=> 0 = (x - r1)*(x - r2)
    double r1 = 1.0;
    double r2 = 2.0;
    double C0 = r1*r2;
    double C1 = -r1 -r2;
    double C2 = 1.0;
    bool success = tiny::compute_polynomial_roots( C0, C1, C2, count, roots );
    BOOST_CHECK( success );
    BOOST_CHECK( count == 2 );
    std::sort( roots, roots+count );
    BOOST_CHECK_CLOSE( roots[0] , r1, 0.01 );
    BOOST_CHECK_CLOSE( roots[1] , r2, 0.01 );
  }

  // Case of three roots
  {
    //  0 = c3 x^3 + c2 x^2 + c1 x + c0    <=> 0 = (x - r1)*(x - r2)*(x - r3)
    double r1 = 1.0;
    double r2 = 2.0;
    double r3 = 4.0;
    double C0 = -r1*r2*r3;
    double C1 = r1*r2 + r1*r3 + r2*r3;
    double C2 = - r1 - r2 - r3;
    double C3 = 1.0;
    bool success = tiny::compute_polynomial_roots( C0, C1, C2, C3, count, roots );
    BOOST_CHECK( success );
    BOOST_CHECK( count == 3 );
    std::sort( roots, roots+count );
    BOOST_CHECK_CLOSE( roots[0] , r1, 0.01 );
    BOOST_CHECK_CLOSE( roots[1] , r2, 0.01 );
    BOOST_CHECK_CLOSE( roots[2] , r3, 0.01 );
  }

  // Case of four roots
  {
    //  0 = c4^x^4 + c3 x^3 + c2 x^2 + c1 x + c0    <=> 0 = (x - r1)*(x - r2)*(x - r3)*(x - r4)
    double r1 = 1.0;
    double r2 = 2.0;
    double r3 = 4.0;
    double r4 = 8.0;
    
    double C0 = r1*r2*r3*r4;
    double C1 = -r1*r2*r3 -r1*r2*r4 -r1*r3*r4 -r2*r3*r4;
    double C2 = r1*r2 + r1*r3 + r1*r4 + r2*r3 + r2*r4 + r3*r4;
    double C3 = -r4 -r3 -r2 -r1;
    double C4 = 1.0;
    bool success = tiny::compute_polynomial_roots( C0, C1, C2, C3, C4, count, roots );
    BOOST_CHECK( success );
    BOOST_CHECK( count == 4 );
    std::sort( roots, roots+count );
    BOOST_CHECK_CLOSE( roots[0] , r1, 0.01 );
    BOOST_CHECK_CLOSE( roots[1] , r2, 0.01 );
    BOOST_CHECK_CLOSE( roots[2] , r3, 0.01 );
    BOOST_CHECK_CLOSE( roots[3] , r4, 0.01 );
  }
  
}

BOOST_AUTO_TEST_SUITE_END();
