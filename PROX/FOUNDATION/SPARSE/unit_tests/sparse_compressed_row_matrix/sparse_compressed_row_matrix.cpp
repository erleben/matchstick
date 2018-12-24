#include <sparse.h>

#include <sparse_fill.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(SPARSE);

BOOST_AUTO_TEST_CASE(compressed_row_matrix_test)
{
  
  typedef sparse::Block<3,3,float> block_type;
  typedef sparse::CompressedRowMatrix<block_type> matrix_type;
  
  matrix_type A; // default
  BOOST_CHECK(A.size() == 0);
  BOOST_CHECK(A.nrows() == 0);
  BOOST_CHECK(A.ncols() == 0);
  BOOST_CHECK(A.begin() == A.end());
  
  A.resize(1,2,2);
  BOOST_CHECK(A.size() == 0); // no new elements inserted yet
  BOOST_CHECK(A.nrows() == 1);
  BOOST_CHECK(A.ncols() == 2);
  BOOST_CHECK(A.begin() == A.end());
  
  block_type b;
  sparse::fill(b, 0.0f);
  A(0,0) = b;  
  BOOST_CHECK(A.size() == 1);
  
  A(0,1) = b;
  BOOST_CHECK(A.size() == 2);
  BOOST_CHECK(A(0,1) == A(0,0));
  BOOST_CHECK(A.size() == 2);
  BOOST_CHECK(A.begin() != A.end());
  BOOST_CHECK(A.begin()+2 == A.end());
  
  A.resize(1,4,5);
  BOOST_CHECK(A.size() == 2);
  BOOST_CHECK(A.nrows() == 1);
  BOOST_CHECK(A.ncols() == 4);
  BOOST_CHECK(A(0,1) == b && A(0,1) == A(0,0));
  
  A.resize(2,4,5);
  BOOST_CHECK(A.size() == 2);
  BOOST_CHECK(A.nrows() == 2);
  BOOST_CHECK(A.ncols() == 4);
  
  A(1,1) = b;
  A(1,3) = b;
  BOOST_CHECK(A.size() == 4);
  BOOST_CHECK(A(1,1) == b);
  BOOST_CHECK(A(1,1) == A(1,3));
  BOOST_CHECK(A(0,0) == A(1,1));
  
  matrix_type const & A_read = A;
  BOOST_CHECK(A_read(1,2) == sparse::zero_block<block_type>());
  BOOST_CHECK(A.size() == 4); // const indexing does not add elements
  
  matrix_type A_copy(A);
  BOOST_CHECK( A.size() == A_copy.size()  );
  BOOST_CHECK( A.nrows() == A_copy.nrows() );
  BOOST_CHECK( A.ncols() == A_copy.ncols() );
  BOOST_CHECK( std::equal(A.begin(), A.end(), A_copy.begin()) );
  
  matrix_type::reference A_ref = A_copy[0];
  A_ref[0] = -1.0f;
  BOOST_CHECK( A_copy[0][0] == -1.0f );
  BOOST_CHECK( A_copy[0] != A[0] );
  
  matrix_type::iterator iter = A.begin();
  BOOST_CHECK( *iter == A[0]);
  BOOST_CHECK( row(iter) == 0 );
  BOOST_CHECK( col(iter) == 0 );
  
  ++iter;
  BOOST_CHECK(*iter == A[1] && row(iter) == 0 && col(iter) == 1);
  ++iter;
  BOOST_CHECK(*iter == A[2] && row(iter) == 1 && col(iter) == 1);
  ++iter;
  BOOST_CHECK(*iter == A[3] && row(iter) == 1 && col(iter) == 3);
  ++iter;
  BOOST_CHECK(iter == A.end());
  
  matrix_type::row_iterator r_iter = A.row_begin(0);
  BOOST_CHECK( *r_iter == A[0]  );
  BOOST_CHECK( row(r_iter) == 0 );
  BOOST_CHECK( col(r_iter) == 0 );
  
  ++r_iter;
  BOOST_CHECK( *r_iter == A[1] );
  BOOST_CHECK( row(r_iter) == 0);
  BOOST_CHECK( col(r_iter) == 1);
  
  ++r_iter;
  BOOST_CHECK(r_iter == A.row_end(0));
  
  r_iter = A.row_begin(1);
  BOOST_CHECK( *r_iter == A[2]);
  BOOST_CHECK( row(r_iter) == 1);
  BOOST_CHECK( col(r_iter) == 1);
  
  ++r_iter;
  BOOST_CHECK( *r_iter == A[3] );
  BOOST_CHECK( row(r_iter) == 1);
  BOOST_CHECK( col(r_iter) == 3);
  
  ++r_iter;
  BOOST_CHECK(r_iter == A.row_end(1));
  
  A.resize(3,4,4);
  r_iter = A.row_begin(2);
  BOOST_CHECK(r_iter == A.row_end(2));
  
  A.clear();
  BOOST_CHECK( A.size() == 0);
  BOOST_CHECK( A.nrows() == 0);
  BOOST_CHECK( A.ncols() == 0);
  
}

BOOST_AUTO_TEST_SUITE_END();
