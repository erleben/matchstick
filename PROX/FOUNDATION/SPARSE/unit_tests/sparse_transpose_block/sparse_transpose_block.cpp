#include <sparse.h>

#include <sparse_fill.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(SPARSE);

BOOST_AUTO_TEST_CASE(transpose_block_test)
{
  
  // scalar case
  typedef sparse::Block<1,1,float> scalar_block_type;
  scalar_block_type s1(5);
  scalar_block_type s2(0);
  sparse::transpose(s1,s2);
  BOOST_CHECK(s2[0] == 5);
  
  // block case
  typedef sparse::Block<4,3,float> block_type;
  typedef sparse::Block<3,4,float> transposed_block_type;

  block_type a;
  sparse::fill(a);
  transposed_block_type b;
  sparse::transpose(a,b);
  for (size_t i = 0; i < block_type::nrows(); ++i)
  {
    for (size_t j = 0; j < block_type::ncols(); ++j)
    {
      BOOST_CHECK(a(i,j) == b(j,i));
    }
  }
  
}

BOOST_AUTO_TEST_SUITE_END();
