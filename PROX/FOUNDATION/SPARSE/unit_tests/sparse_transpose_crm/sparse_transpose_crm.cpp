#include <sparse.h>

#include <sparse_fill.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(SPARSE);

BOOST_AUTO_TEST_CASE(transpose_crm_test)
{
  
  // scalar case
  typedef sparse::CompressedRowMatrix< sparse::Block<1,1,float> > scalar_matrix_type;
  scalar_matrix_type sA(1,1,1);
  sparse::fill(sA(0,0));
  scalar_matrix_type sB;
  sparse::transpose(sA,sB);
  BOOST_CHECK(sA.size() == sB.size());
  BOOST_CHECK(sA.ncols() == sB.nrows());
  BOOST_CHECK(sA.nrows() == sB.ncols());
  BOOST_CHECK(sA[0] == sB[0]);
  sA.clear();
  sB.clear();
  
  sA.resize(5,5,8);
  sparse::fill(sA(0,0));
  sparse::fill(sA(0,4));
  sparse::fill(sA(1,1));
  sparse::fill(sA(1,2));
  sparse::fill(sA(2,3));
  sparse::fill(sA(2,4));
  sparse::fill(sA(4,2));
  sparse::fill(sA(4,4));
  sparse::transpose(sA,sB);
  
  BOOST_CHECK( sA.size()  == sB.size() );
  BOOST_CHECK( sA.ncols() == sB.nrows());
  BOOST_CHECK( sA.nrows() == sB.ncols());
  BOOST_CHECK( sA(0,0) == sB(0,0));
  BOOST_CHECK( sA(0,4) == sB(4,0));
  BOOST_CHECK( sA(1,1) == sB(1,1));
  BOOST_CHECK( sA(1,2) == sB(2,1));
  BOOST_CHECK( sA(2,3) == sB(3,2));
  BOOST_CHECK( sA(2,4) == sB(4,2));
  BOOST_CHECK( sA(4,2) == sB(2,4));
  BOOST_CHECK( sA(4,4) == sB(4,4));
  
  // block case
  typedef sparse::Block<3,4,float> block_type;
  typedef sparse::Block<4,3,float> transposed_block_type;
  
  typedef sparse::CompressedRowMatrix<block_type> matrix_type;
  matrix_type A(1,1,1);
  sparse::fill(A(0,0));
  
  typedef sparse::CompressedRowMatrix<transposed_block_type> transposed_matrix_type;
  transposed_matrix_type B;
  
  sparse::transpose(A,B);
  BOOST_CHECK(A.size() == B.size());
  BOOST_CHECK(A.ncols() == B.nrows());
  BOOST_CHECK(A.nrows() == B.ncols());
  
  transposed_block_type b;
  sparse::transpose(A[0],b);
  
  BOOST_CHECK(b == B[0]);
  A.clear();
  B.clear();
  
  A.resize(5,5,8);
  sparse::fill(A(0,0));
  sparse::fill(A(0,4));
  sparse::fill(A(1,1));
  sparse::fill(A(1,2));
  sparse::fill(A(2,3));
  sparse::fill(A(2,4));
  sparse::fill(A(4,2));
  sparse::fill(A(4,4));
  sparse::transpose(A,B);
  BOOST_CHECK(A.size() == B.size());
  BOOST_CHECK(A.ncols() == B.nrows());
  BOOST_CHECK(A.nrows() == B.ncols());
  
  sparse::transpose(A(0,0), b);
  BOOST_CHECK(b == B(0,0));
  
  sparse::transpose(A(0,4), b);
  BOOST_CHECK(b == B(4,0));
  
  sparse::transpose(A(1,1), b);
  BOOST_CHECK(b == B(1,1));
  
  sparse::transpose(A(1,2), b);
  BOOST_CHECK(b == B(2,1));
  
  sparse::transpose(A(2,3), b);
  BOOST_CHECK(b == B(3,2));
  
  sparse::transpose(A(2,4), b);
  BOOST_CHECK(b == B(4,2));
  
  sparse::transpose(A(4,2), b);
  BOOST_CHECK(b == B(2,4));
  
  sparse::transpose(A(4,4), b);
  BOOST_CHECK(b == B(4,4));
  
}

BOOST_AUTO_TEST_SUITE_END();

