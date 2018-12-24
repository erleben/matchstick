#include <sparse.h>

#include <sparse_fill.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(SPARSE);

BOOST_AUTO_TEST_CASE(two_column_matrix_test)
{
  typedef sparse::Block<3,3,float> block_type;
  typedef sparse::TwoColumnMatrix<block_type> matrix_type;
  
  matrix_type A; // default
  BOOST_CHECK(A.size() == 0 );
  BOOST_CHECK(A.nrows() == 0 );
  BOOST_CHECK(A.ncols() == 0 );
  BOOST_CHECK(A.begin() == A.end() );
  
  A.resize(1,2,2);
  BOOST_CHECK( A.size() == 2 );
  BOOST_CHECK( A.nrows() == 1 );
  BOOST_CHECK( A.ncols() == 2 );
  BOOST_CHECK( A.begin() != A.end() );
  BOOST_CHECK( A.begin()+2 == A.end() );
  BOOST_CHECK( A[0] == sparse::zero_block<block_type>() );
  BOOST_CHECK( A[0] == A[1] );
  
  block_type b;
  sparse::fill(b, 0.0f);
  A(0,0) = b;
  A(0,1) = b;
  BOOST_CHECK( A(0,1) == A(0,0) );
  
  A.resize(1,4,2);
  BOOST_CHECK( A.size() == 2 );
  BOOST_CHECK( A.nrows() == 1 );
  BOOST_CHECK( A.ncols() == 4 );
  BOOST_CHECK( A(0,1) == b );
  BOOST_CHECK( A(0,1) == A(0,0) );
  
  A.resize(2,4,4);
  BOOST_CHECK( A.size() == 4) ;
  BOOST_CHECK( A.nrows() == 2 );
  BOOST_CHECK( A.ncols() == 4 );
  
  A(1,1) = b;
  A(1,3) = b;
  BOOST_CHECK( A(1,1) == b );
  BOOST_CHECK( A(1,1) == A(1,3) );
  BOOST_CHECK( A(0,0) == A(1,1) );
  
  matrix_type const & A_read = A;
  BOOST_CHECK( A_read(0,3) == sparse::zero_block<block_type>() );
  
  matrix_type A_copy(A);
  BOOST_CHECK( A.size() == A_copy.size() );
  BOOST_CHECK( A.nrows() == A_copy.nrows() );
  BOOST_CHECK( A.ncols() == A_copy.ncols() );
  BOOST_CHECK( std::equal(A.begin(), A.end(), A_copy.begin()) );
  
  matrix_type::reference A_ref = A_copy[0];
  A_ref[0] = -1.0f;
  BOOST_CHECK( A_copy[0][0] == -1.0f );
  BOOST_CHECK( A_copy[0] != A[0] );
  
  matrix_type::iterator iter = A.begin();
  BOOST_CHECK(*iter == A[0] );
  BOOST_CHECK( row(iter) == 0 );
  BOOST_CHECK( col(iter) == 0 );
  
  ++iter;
  BOOST_CHECK( *iter == A[1] );
  BOOST_CHECK( row(iter) == 0 );
  BOOST_CHECK( col(iter) == 1 );
  
  ++iter;
  BOOST_CHECK( *iter == A[2] );
  BOOST_CHECK( row(iter) == 1 );
  BOOST_CHECK( col(iter) == 1 );
  
  ++iter;
  BOOST_CHECK( *iter == A[3] );
  BOOST_CHECK( row(iter) == 1 );
  BOOST_CHECK( col(iter) == 3 );
  
  A.clear();
  BOOST_CHECK( A.size() == 0 );
  BOOST_CHECK( A.nrows() == 0 );
  BOOST_CHECK( A.ncols() == 0 );
  
}

BOOST_AUTO_TEST_SUITE_END();
