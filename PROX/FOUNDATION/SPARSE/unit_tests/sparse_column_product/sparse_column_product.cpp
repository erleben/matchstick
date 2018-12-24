#include <sparse.h>
#include <sparse_fill.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(SPARSE);

BOOST_AUTO_TEST_CASE(col_prod_with_transpose)
{
  // setup test
  typedef sparse::Block<6,4,float> block6x4_type;
  typedef sparse::Block<4,6,float> block4x6_type;
  typedef sparse::Block<4,1,float> block4x1_type;
  typedef sparse::Block<6,1,float> block6x1_type;
  typedef sparse::CompressedRowMatrix<block6x4_type> matrix6x4_type;
  typedef sparse::CompressedRowMatrix<block4x6_type> matrix4x6_type;
  typedef sparse::Vector<block6x1_type> vector_type;
  typedef sparse::CompressedVector<block6x1_type> compressed_vector_type;
  typedef sparse::CompressedVector<block6x1_type, sparse::detail::FastVectorCompressor> fast_compressed_vector_type;

  matrix6x4_type JT(4,3,6);
  sparse::fill(JT(0,0), 0.0f);
  sparse::fill(JT(1,1), 0.0f);
  sparse::fill(JT(1,2), 0.0f);
  sparse::fill(JT(2,0), 0.0f);
  sparse::fill(JT(2,2), 0.0f);
  sparse::fill(JT(3,1), 0.0f);

  matrix4x6_type J;
  sparse::transpose(JT, J); // exploiting that J = J^T^T

  vector_type w(4);
  sparse::fill(w(0));
  sparse::fill(w(1));
  sparse::fill(w(2));
  sparse::fill(w(3));

  block6x1_type to_add;
  to_add[0] = 14;
  to_add[1] = 38;
  to_add[2] = 62;
  to_add[3] = 86;
  to_add[4] = 110;
  to_add[5] = 134;

  block6x1_type original;
  sparse::fill(original,0.0f);

  block6x1_type result;

  block4x1_type delta_x(0);
  sparse::fill(delta_x);

  // test column prod
  column_prod( JT, J, delta_x, w, 0);
  add(original, to_add, result);
  BOOST_CHECK( w(0) == result );
  BOOST_CHECK( w(1) == original );
  BOOST_CHECK( w(2) == result );
  BOOST_CHECK( w(3) == original);

  column_prod( JT, J, delta_x, w, 1);
  BOOST_CHECK( w(0) == result );
  BOOST_CHECK( w(1) == result );
  BOOST_CHECK( w(2) == result );
  BOOST_CHECK( w(3) == result );

  column_prod( JT, J, delta_x, w, 2);
  BOOST_CHECK( w(0) == result );
  BOOST_CHECK( w(3) == result );
  add(to_add, result);
  BOOST_CHECK( w(1) == result );
  BOOST_CHECK( w(2) == result );

  // initialize a dense compressed vector and test again
  compressed_vector_type c_w(4,4);
  sparse::fill(c_w(0));
  sparse::fill(c_w(1));
  sparse::fill(c_w(2));
  sparse::fill(c_w(3));

  column_prod( JT, J, delta_x, c_w, 0);
  add(original, to_add, result);
  BOOST_CHECK( c_w(0) == result );
  BOOST_CHECK( c_w(1) == original );
  BOOST_CHECK( c_w(2) == result );
  BOOST_CHECK( c_w(3) == original);

  column_prod( JT, J, delta_x, c_w, 1);
  BOOST_CHECK( c_w(0) == result );
  BOOST_CHECK( c_w(1) == result );
  BOOST_CHECK( c_w(2) == result );
  BOOST_CHECK( c_w(3) == result );

  column_prod( JT, J, delta_x, c_w, 2);
  BOOST_CHECK( c_w(0) == result );
  BOOST_CHECK( c_w(3) == result );
  add(to_add, result);
  BOOST_CHECK( c_w(1) == result );
  BOOST_CHECK( c_w(2) == result );

  // initialize a dense fast compressed vector and test again
  fast_compressed_vector_type f_w(4,4);
  sparse::fill(f_w(0));
  sparse::fill(f_w(1));
  sparse::fill(f_w(2));
  sparse::fill(f_w(3));

  column_prod( JT, J, delta_x, f_w, 0);
  add(original, to_add, result);
  BOOST_CHECK( f_w(0) == result );
  BOOST_CHECK( f_w(1) == original );
  BOOST_CHECK( f_w(2) == result );
  BOOST_CHECK( f_w(3) == original);

  column_prod( JT, J, delta_x, f_w, 1);
  BOOST_CHECK( f_w(0) == result );
  BOOST_CHECK( f_w(1) == result );
  BOOST_CHECK( f_w(2) == result );
  BOOST_CHECK( f_w(3) == result );

  column_prod( JT, J, delta_x, f_w, 2);
  BOOST_CHECK( f_w(0) == result );
  BOOST_CHECK( f_w(3) == result );
  add(to_add, result);
  BOOST_CHECK( f_w(1) == result );
  BOOST_CHECK( f_w(2) == result );
}
BOOST_AUTO_TEST_SUITE_END();
