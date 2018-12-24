#include <sparse.h>

#include <sparse_fill.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(SPARSE);

BOOST_AUTO_TEST_CASE(product_block_test)
{  
  // scalar case
  typedef sparse::Block<1,1,float> block_type1;
  
  block_type1 s1(5), s2(2), s3(0);
  
  sparse::prod(s1,s2,s3);
  BOOST_CHECK(s3[0] == 5*2);
  
  // block case
  sparse::Block<4,3,float> a;
  sparse::Block<3,3,float> b;
  sparse::Block<4,3,float> c(sparse::zero_block< sparse::Block<4,3,float> >() );
  sparse::fill(a, 1);
  sparse::fill(b, 1);
  sparse::prod(a,b,c);
  
  BOOST_CHECK( c[0] ==  30 );
  BOOST_CHECK( c[1] ==  36 );
  BOOST_CHECK( c[2] ==  42 );
  BOOST_CHECK( c[3] ==  66 );
  BOOST_CHECK( c[4] ==  81 );
  BOOST_CHECK( c[5] ==  96 );
  BOOST_CHECK( c[6] == 102 );
  BOOST_CHECK( c[7] == 126 );
  BOOST_CHECK( c[8] == 150 );
  BOOST_CHECK( c[9] == 138 );
  BOOST_CHECK( c[10] == 171 );
  BOOST_CHECK( c[11] == 204 );
  
}

BOOST_AUTO_TEST_SUITE_END();

