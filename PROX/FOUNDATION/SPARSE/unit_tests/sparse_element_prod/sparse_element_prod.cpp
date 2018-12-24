#include <sparse.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(SPARSE);

BOOST_AUTO_TEST_CASE(element_prod_test)
{    
  typedef sparse::Block<2,1,float>   block_type;
  typedef sparse::Vector<block_type> vector_type;
  
  vector_type a;
  vector_type b;
  vector_type c;
  
  a.resize(2);
  b.resize(2);
  c.resize(2);
  
  a(0)(0) = 1.0f;
  a(0)(1) = 2.0f;
  a(1)(0) = 3.0f;
  a(1)(1) = 4.0f;
  
  b(0)(0) = 4.0f;
  b(0)(1) = 3.0f;
  b(1)(0) = 2.0f;
  b(1)(1) = 1.0f;
  
  c(0)(0) = 1.0f;
  c(0)(1) = 1.0f;
  c(1)(0) = 1.0f;
  c(1)(1) = 1.0f;
  
  sparse::element_prod( a, b, c);
  
  BOOST_CHECK( c(0)(0) == 4.0f );
  BOOST_CHECK( c(0)(1) == 6.0f );
  BOOST_CHECK( c(1)(0) == 6.0f );
  BOOST_CHECK( c(1)(1) == 4.0f );
  
}

BOOST_AUTO_TEST_SUITE_END();
