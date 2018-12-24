#include <tiny_math_types.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(tiny_quaternion_rotate);

BOOST_AUTO_TEST_CASE(simple_test)
{
  typedef tiny::MathTypes<double> math_types;

  typedef math_types::value_traits     value_traits;
  typedef math_types::real_type        T;
  typedef math_types::vector3_type     V;
  typedef math_types::quaternion_type  Q;

  BOOST_CHECK(  Q::accessor::stride()     == 1);
  BOOST_CHECK(  Q::accessor::padding()    == 0);
  BOOST_CHECK(  Q::accessor::J_padded()   == 4);
  BOOST_CHECK(  Q::accessor::allocsize()  == 4);
  
  {
    // Set up a test rotation
    Q q;
    T const phi = value_traits::pi_half();
    V const m = V::make( 1.0, 0.0, 0.0 );
    q = Q::Ru( phi, m);
    BOOST_CHECK_CLOSE( tiny::norm( q ), value_traits::one(), 0.01 );
    V const n = V::make( 0.0, 1.0, 0.0 );
    // Try to rotate n-vector
    V const k = tiny::rotate( q, n);
    // Test if result is what we expect
    BOOST_CHECK( fabs( k(0) ) < 10e-10 );
    BOOST_CHECK( fabs( k(1) ) < 10e-10 );
    BOOST_CHECK( fabs( 1.0 - k(2) ) < 10e-10 );
  }
  {
    // Set up a test rotation
    Q q;
    T const phi = value_traits::pi_half();
    V const m = V::make( 1.0, 0.0, 0.0 );
    q = Q::Ru( phi, m);
    BOOST_CHECK_CLOSE( tiny::norm( q ), value_traits::one(), 0.01 );
    V const n = V::make( 0.0, 0.0, 1.0 );
    // Try to rotate n-vector
    V const k = tiny::rotate( q, n);
    // Test if result is what we expect
    BOOST_CHECK( fabs( k(0) ) < 10e-10 );
    BOOST_CHECK( fabs( 1.0 + k(1) ) < 10e-10 );
    BOOST_CHECK( fabs( k(2) ) < 10e-10 );
  }
  {
    // Set up a test rotation
    Q q;
    T const phi = value_traits::pi_half();
    V const m = V::make( 1.0, 0.0, 0.0 );
    q = Q::Ru( phi, m);
    BOOST_CHECK_CLOSE( tiny::norm( q ), value_traits::one(), 0.01 );
    V const n = V::make( 0.0, -1.0, 0.0 );
    // Try to rotate n-vector
    V const k = tiny::rotate( q, n);
    // Test if result is what we expect
    BOOST_CHECK( fabs( k(0) ) < 10e-10 );
    BOOST_CHECK( fabs( k(1) ) < 10e-10 );
    BOOST_CHECK( fabs( 1.0 + k(2) ) < 10e-10 );
  }
  {
    // Set up a test rotation
    Q q;
    T const phi = value_traits::pi_half();
    V const m = V::make( 1.0, 0.0, 0.0 );
    q = Q::Ru( phi, m);
    BOOST_CHECK_CLOSE( tiny::norm( q ), value_traits::one(), 0.01 );
    V const n = V::make( 0.0, 0.0, -1.0 );
    // Try to rotate n-vector
    V const k = tiny::rotate( q, n);
    // Test if result is what we expect
    BOOST_CHECK( fabs( k(0) ) < 10e-10 );
    BOOST_CHECK( fabs( 1.0 - k(1) ) < 10e-10 );
    BOOST_CHECK( fabs( k(2) ) < 10e-10 );
  }
  {
    // Set up a test rotation
    Q q;
    T const phi = value_traits::pi_half();
    V const m = V::make( 0.0, 1.0, 0.0 );
    q = Q::Ru( phi, m);
    BOOST_CHECK_CLOSE( tiny::norm( q ), value_traits::one(), 0.01 );
    V const n = V::make( 1.0, 0.0, 0.0 );
    // Try to rotate n-vector
    V const k = tiny::rotate( q, n);
    // Test if result is what we expect
    BOOST_CHECK( fabs( k(0) ) < 10e-10 );
    BOOST_CHECK( fabs( k(1) ) < 10e-10 );
    BOOST_CHECK( fabs( 1.0 + k(2) ) < 10e-10 );
  }
  {
    // Set up a test rotation
    Q q;
    T const phi = value_traits::pi_half();
    V const m = V::make( 0.0, 0.0, 1.0 );
    q = Q::Ru( phi, m);
    BOOST_CHECK_CLOSE( tiny::norm( q ), value_traits::one(), 0.01 );
    V const n = V::make( 1.0, 0.0, 0.0 );
    // Try to rotate n-vector
    V const k = tiny::rotate( q, n);
    // Test if result is what we expect
    BOOST_CHECK( fabs( k(0) ) < 10e-10 );
    BOOST_CHECK( fabs( 1.0 - k(1) ) < 10e-10 );
    BOOST_CHECK( fabs( k(2) ) < 10e-10 );
  }
}

BOOST_AUTO_TEST_SUITE_END();
