#include <tiny_math_types.h>
#include <tiny_matrix_functions.h>
#include <tiny_covariance3x3.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(tiny_covariance3x3);

BOOST_AUTO_TEST_CASE(simple_test)
{
  
  typedef tiny::MathTypes<double> math_types;
  
  typedef  math_types::matrix3x3_type   matrix3x3_type;
  typedef  math_types::vector3_type     vector3_type;
  
  
  matrix3x3_type C;
  vector3_type mean;
  
  vector3_type samples[100];
    
  for( size_t i=0;i<100;++i)
    samples[i] = vector3_type::random(  );
  
  tiny::covariance( samples, samples+100, mean, C );
  
  BOOST_CHECK_CLOSE( C(1,0), C(0,1), 0.01 );
  BOOST_CHECK_CLOSE( C(2,0), C(0,2), 0.01 );
  BOOST_CHECK_CLOSE( C(2,1), C(1,2), 0.01 );
  
  BOOST_CHECK( mean(0) > 0.0 );
  BOOST_CHECK( mean(0) < 1.0 );
  BOOST_CHECK( mean(1) > 0.0 );
  BOOST_CHECK( mean(1) < 1.0 );
  BOOST_CHECK( mean(2) > 0.0 );
  BOOST_CHECK( mean(2) < 1.0 );
  
  matrix3x3_type C1 = C;
  vector3_type mean1 = mean;
  
  matrix3x3_type C2 = C;
  vector3_type mean2 = mean;
  
  matrix3x3_type C3;
  vector3_type mean3;
  tiny::covariance_union( mean1, C1, mean2, C2, mean3, C3 );
  
  
  BOOST_CHECK_CLOSE( C3(1,0), C3(0,1), 0.01 );
  BOOST_CHECK_CLOSE( C3(2,0), C3(0,2), 0.01 );
  BOOST_CHECK_CLOSE( C3(2,1), C3(1,2), 0.01 );
  
  BOOST_CHECK( mean3(0) > 0.0 );
  BOOST_CHECK( mean3(0) < 1.0 );
  BOOST_CHECK( mean3(1) > 0.0 );
  BOOST_CHECK( mean3(1) < 1.0 );
  BOOST_CHECK( mean3(2) > 0.0 );
  BOOST_CHECK( mean3(2) < 1.0 );
  
}

BOOST_AUTO_TEST_SUITE_END();
