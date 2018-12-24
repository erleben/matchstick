#include <sparse.h>

#include <sparse_fill.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(SPARSE);

BOOST_AUTO_TEST_CASE(compressed_vector_test)
{
  typedef sparse::Block<4,1,float>   block_type;
  typedef sparse::CompressedVector<block_type> vector_type;

  vector_type v; // default
  BOOST_CHECK(v.size() == 0u);
  BOOST_CHECK(v.nrows() == 0u);
  BOOST_CHECK(v.ncols() == 1u);
  BOOST_CHECK(v.begin() == v.end());
  
  v.resize(1u, 2u);
  BOOST_CHECK(v.size() == 0u);
  BOOST_CHECK(v.nrows() == 2u);
  BOOST_CHECK(v.ncols() == 1u);
  BOOST_CHECK(v.begin() == v.end());
  BOOST_CHECK(v(0) == sparse::zero_block<block_type>());
  
  block_type b;
  sparse::fill(b, 0.0f);
  
  v(0) = b;
  BOOST_CHECK(v(0) == b);
  
  vector_type v_copy(v);
  BOOST_CHECK(v_copy.size() == 1u);
  BOOST_CHECK(v_copy.nrows() == 2u);
  BOOST_CHECK(v.ncols() == 1u);
  BOOST_CHECK(v_copy.begin() != v_copy.end());
  BOOST_CHECK(v_copy.begin()+1 == v_copy.end());
  BOOST_CHECK(v_copy(0) == b);
  
  vector_type::reference b_ref = v_copy(0);
  b_ref(0) = -1.0f;
  BOOST_CHECK(v_copy(0) != b);
  
  v(0) = v_copy(0);
  BOOST_CHECK(v_copy(0) == v(0));
  
  v.resize(2u,2u);
  BOOST_CHECK(v.size() == 1u && v.nrows() == 2u);
  // 2010-04-03 Kenny: Warning comparision between signed and unsigned integer expressions
  BOOST_CHECK(std::distance(v.begin(), v.end()) == v.size());
  BOOST_CHECK(v(1) == sparse::zero_block<block_type>());
  
  v(1)(2) = 2.0;
  BOOST_CHECK(v(1) != sparse::zero_block<block_type>());
  
  vector_type::iterator iter = v.begin();
  BOOST_CHECK( (*iter) == v(0));
  ++iter;
  BOOST_CHECK( (*iter) == v(1));
  
  v.clear();
  BOOST_CHECK( v.size() == 0 );
  BOOST_CHECK( v.nrows() == 0);
  
  //vector_type test(5,10);
  //sparse::fill( test(5), 5u );
  //sparse::fill( test(4), 4u );
  //sparse::fill( test(9), 9u );
  //sparse::fill( test(8), 8u );
  //sparse::fill( test(0), 0u );
}

BOOST_AUTO_TEST_CASE(fast_compressed_vector_test)
{
  typedef sparse::Block<4,1,float>   block_type;
  typedef sparse::CompressedVector<block_type,sparse::detail::FastVectorCompressor> vector_type;

  vector_type v; // default
  BOOST_CHECK(v.size() == 0u);
  BOOST_CHECK(v.nrows() == 0u);
  BOOST_CHECK(v.ncols() == 1u);
  BOOST_CHECK(v.begin() == v.end());
  
  v.resize(1u, 2u);
  BOOST_CHECK(v.size() == 0u);
  BOOST_CHECK(v.nrows() == 2u);
  BOOST_CHECK(v.ncols() == 1u);
  BOOST_CHECK(v.begin() == v.end());
  BOOST_CHECK(v(0) == sparse::zero_block<block_type>());
  
  block_type b;
  sparse::fill(b, 0.0f);
  
  v(0) = b;
  BOOST_CHECK(v(0) == b);
  
  vector_type v_copy(v);
  BOOST_CHECK(v_copy.size() == 1u);
  BOOST_CHECK(v_copy.nrows() == 2u);
  BOOST_CHECK(v.ncols() == 1u);
  BOOST_CHECK(v_copy.begin() != v_copy.end());
  BOOST_CHECK(v_copy.begin()+1 == v_copy.end());
  BOOST_CHECK(v_copy(0) == b);
  
  vector_type::reference b_ref = v_copy(0);
  b_ref(0) = -1.0f;
  BOOST_CHECK(v_copy(0) != b);
  
  v(0) = v_copy(0);
  BOOST_CHECK(v_copy(0) == v(0));
  
  v.resize(2u,2u);
  BOOST_CHECK(v.size() == 1u && v.nrows() == 2u);
  // 2010-04-03 Kenny: Warning comparision between signed and unsigned integer expressions
  BOOST_CHECK(std::distance(v.begin(), v.end()) == v.size());
  BOOST_CHECK(v(1) == sparse::zero_block<block_type>());
  
  v(1)(2) = 2.0;
  BOOST_CHECK(v(1) != sparse::zero_block<block_type>());
  
  vector_type::iterator iter = v.begin();
  BOOST_CHECK( (*iter) == v(0));
  ++iter;
  BOOST_CHECK( (*iter) == v(1));
  
  v.clear();
  BOOST_CHECK( v.size() == 0 );
  BOOST_CHECK( v.nrows() == 0);
  
}

BOOST_AUTO_TEST_SUITE_END();
