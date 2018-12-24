#include <geometry.h>
#include <tiny.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(interval)
{
  typedef tiny::MathTypes<float> MT;
  typedef MT::value_traits       VT;
  typedef MT::real_type          T;
  
  {
    geometry::Interval<T> I;

    BOOST_CHECK_EQUAL( I.lower(), VT::highest() );
    BOOST_CHECK_EQUAL( I.upper(), VT::lowest() );
    BOOST_CHECK( geometry::is_valid(I) == false );
  }

  // No overlap
  {
    geometry::Interval<T> A;
    geometry::Interval<T> B;
    
    A.lower() = -2.0f;
    A.upper() = -1.0f;
    B.lower() =  1.0f;
    B.upper() =  2.0f;
    
    BOOST_CHECK_EQUAL( A.lower(), -2.0f );
    BOOST_CHECK_EQUAL( A.upper(), -1.0f );
    BOOST_CHECK_EQUAL( B.lower(),  1.0f );
    BOOST_CHECK_EQUAL( B.upper(),  2.0f );
    
    BOOST_CHECK( geometry::is_valid(A) == true );
    BOOST_CHECK( geometry::is_valid(B) == true );
    
    BOOST_CHECK( geometry::overlap_interval_interval(A,B) == false);
    BOOST_CHECK( geometry::overlap_interval_interval(B,A) == false);
  }

  // Deep overlap
  {
    geometry::Interval<T> A;
    geometry::Interval<T> B;
    
    A.lower() = -2.0f;
    A.upper() =  1.0f;
    B.lower() = -1.0f;
    B.upper() =  2.0f;
    
    BOOST_CHECK_EQUAL( A.lower(), -2.0f );
    BOOST_CHECK_EQUAL( A.upper(),  1.0f );
    BOOST_CHECK_EQUAL( B.lower(), -1.0f );
    BOOST_CHECK_EQUAL( B.upper(),  2.0f );
    
    BOOST_CHECK( geometry::overlap_interval_interval(A,B) == true);
    BOOST_CHECK( geometry::overlap_interval_interval(B,A) == true);
  }

  // Touching case
  {
    geometry::Interval<T> A;
    geometry::Interval<T> B;
    
    A.lower() = -2.0f;
    A.upper() =  0.0f;
    B.lower() =  0.0f;
    B.upper() =  2.0f;
    
    BOOST_CHECK_EQUAL( A.lower(), -2.0f );
    BOOST_CHECK_EQUAL( A.upper(),  0.0f );
    BOOST_CHECK_EQUAL( B.lower(),  0.0f );
    BOOST_CHECK_EQUAL( B.upper(),  2.0f );
    
    BOOST_CHECK( geometry::overlap_interval_interval(A,B) == true);
    BOOST_CHECK( geometry::overlap_interval_interval(B,A) == true);
  }

  // Indentical case
  {
    geometry::Interval<T> A;
    geometry::Interval<T> B;
    
    A.lower() = -2.0f;
    A.upper() =  0.0f;
    B.lower() = -2.0f;
    B.upper() =  0.0f;
    
    BOOST_CHECK_EQUAL( A.lower(), -2.0f );
    BOOST_CHECK_EQUAL( A.upper(),  0.0f );
    BOOST_CHECK_EQUAL( B.lower(), -2.0f );
    BOOST_CHECK_EQUAL( B.upper(),  0.0f );
    
    BOOST_CHECK( geometry::overlap_interval_interval(A,B) == true);
    BOOST_CHECK( geometry::overlap_interval_interval(B,A) == true);
  }

  // Zero sized interval case
  {
    geometry::Interval<T> A;
    geometry::Interval<T> B;
    
    A.lower() = -2.0f;
    A.upper() = -2.0f;
    B.lower() = -2.0f;
    B.upper() =  0.0f;
    
    BOOST_CHECK_EQUAL( A.lower(), -2.0f );
    BOOST_CHECK_EQUAL( A.upper(), -2.0f );
    BOOST_CHECK_EQUAL( B.lower(), -2.0f );
    BOOST_CHECK_EQUAL( B.upper(),  0.0f );
    
    BOOST_CHECK( geometry::overlap_interval_interval(A,B) == true);
    BOOST_CHECK( geometry::overlap_interval_interval(B,A) == true);
    
    BOOST_CHECK( geometry::is_valid(A) == true );
    
  }
  
}

BOOST_AUTO_TEST_SUITE_END();
