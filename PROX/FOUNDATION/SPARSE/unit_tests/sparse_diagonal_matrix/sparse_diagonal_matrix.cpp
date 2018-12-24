#include <sparse.h>

#include <sparse_fill.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(SPARSE);

BOOST_AUTO_TEST_CASE(diagonal_matrix_test)
{
  
  typedef sparse::Block<1,1,float>           block_type;
  typedef sparse::DiagonalMatrix<block_type> matrix_type;
  
  matrix_type A; // default
  BOOST_CHECK(A.size() == 0);
  BOOST_CHECK(A.nrows() == 0);
  BOOST_CHECK(A.ncols() == 0);
  BOOST_CHECK(A.begin() == A.end());
  
  A.resize(1,1);
  BOOST_CHECK(A.size() == 1);
  BOOST_CHECK(A.nrows() == 1);
  BOOST_CHECK(A.ncols() == 1);
  BOOST_CHECK(A.begin() != A.end());
  BOOST_CHECK( A.begin()+1 == A.end());
  
  BOOST_CHECK(A[0] == sparse::zero_block<block_type>());
  
  block_type b;
  sparse::fill(b, 0.0f);
  A[0] = b;
  BOOST_CHECK(A[0] == b);
  BOOST_CHECK(A(0,0) == A[0]);
  
  matrix_type A_copy(A);
  BOOST_CHECK(A_copy.size() == 1);
  BOOST_CHECK(A_copy.nrows() == 1);
  BOOST_CHECK(A_copy.begin() != A_copy.end());
  BOOST_CHECK(A_copy.begin()+1 == A_copy.end());
  BOOST_CHECK(A_copy[0] == A[0]);
  
  matrix_type::reference A_ref = A_copy[0];
  A_ref[0] = -1.0f;
  BOOST_CHECK(A_copy[0] != A[0]);
  
  A.resize(2);
  BOOST_CHECK(A.size() == 2);
  BOOST_CHECK(A.nrows() == 2);
  BOOST_CHECK(A.ncols() == 2);
  // 2009-07-01 Kenny: Comparision between signed and unsigned integer expressions?
  BOOST_CHECK(std::distance(A.begin(), A.end()) == A.size());
  BOOST_CHECK(A(1,1) == sparse::zero_block<block_type>());
  BOOST_CHECK(A(0,1) == sparse::zero_block<block_type>());
  BOOST_CHECK(A(1,0) == sparse::zero_block<block_type>());
  A[1] = -2.0f;
  BOOST_CHECK(A[1] == -2.0f);
  BOOST_CHECK(A[1] == A(1,1));
  
  matrix_type::iterator iter = A.begin();
  BOOST_CHECK(*iter == A[0]);
  BOOST_CHECK(row(iter) == 0);
  BOOST_CHECK(col(iter) == 0);
  ++iter;
  BOOST_CHECK(*iter == A[1]);
  BOOST_CHECK(row(iter) == 1);
  BOOST_CHECK(col(iter) == 1);
  
  A.clear();
  BOOST_CHECK(A.size() == 0);
  BOOST_CHECK(A.nrows() == 0);
  BOOST_CHECK(A.ncols() == 0);
  
}

BOOST_AUTO_TEST_SUITE_END();
