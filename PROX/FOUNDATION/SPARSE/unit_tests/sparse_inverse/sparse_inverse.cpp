#include <sparse.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(SPARSE);

BOOST_AUTO_TEST_CASE(inverse_test)
{    
  typedef sparse::Block<1,1,float>   block_type;
  typedef sparse::DiagonalMatrix<block_type> matrix_type;
  
  matrix_type D;
  
  D.resize(4);
  
  D(0) =  2.0;
  D(1) =  4.0;
  D(2) =  8.0;
  D(3) = 16.0;

  sparse::inverse( D );
  
  BOOST_CHECK( D(0) == 1.0f/2.0f );
  BOOST_CHECK( D(1) == 1.0f/4.0f );
  BOOST_CHECK( D(2) == 1.0f/8.0f );
  BOOST_CHECK( D(3) == 1.0f/16.0f );
         
}

BOOST_AUTO_TEST_SUITE_END();