#include <sparse.h>

#include <sparse_fill.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(SPARSE);

BOOST_AUTO_TEST_CASE(vector_test)
{
  typedef sparse::Block<4,1,float>   block_type;
  typedef sparse::Vector<block_type> vector_type;
  
  vector_type v; // default
  BOOST_CHECK(v.size() == 0);
  BOOST_CHECK(v.nrows() == 0);
  BOOST_CHECK(v.begin() == v.end());
  
  v.resize(1);
  BOOST_CHECK(v.size() == 1);
  BOOST_CHECK(v.nrows() == 1);
  BOOST_CHECK(v.begin() != v.end() && v.begin()+1 == v.end());
  BOOST_CHECK(v(0) == sparse::zero_block<block_type>());
  
  block_type b;
  sparse::fill(b, 0.0f);
  
  v(0) = b;
  BOOST_CHECK(v(0) == b);
  
  vector_type v_copy(v);
  BOOST_CHECK(v_copy.size() == 1);
  BOOST_CHECK(v_copy.nrows() == 1);
  BOOST_CHECK(v_copy.begin() != v_copy.end());
  BOOST_CHECK(v_copy.begin()+1 == v_copy.end());
  BOOST_CHECK(v_copy(0) == b);
  
  vector_type::reference b_ref = v_copy(0);
  b_ref(0) = -1.0f;
  BOOST_CHECK(v_copy(0) != b);
  
  v(0) = v_copy(0);
  BOOST_CHECK(v_copy(0) == v(0));
  
  v.resize(2);
  BOOST_CHECK(v.size() == 2 && v.nrows() == 2);
  // 2009-07-01 Kenny: Comparision between signed and unsigned integer expressions?
  BOOST_CHECK(std::distance(v.begin(), v.end()) == v.size());
  BOOST_CHECK(v(1) == sparse::zero_block<block_type>());
  
  v(1)(2) = 2.0;
  BOOST_CHECK(v(1) != sparse::zero_block<block_type>());
  
  vector_type::iterator iter = v.begin();
  BOOST_CHECK( *iter == v(0));
  BOOST_CHECK( row(iter) == 0);
  BOOST_CHECK( col(iter) == 1);
  ++iter;
  BOOST_CHECK(*iter == v(1));
  BOOST_CHECK( row(iter) == 1);
  BOOST_CHECK( col(iter) == 1);
  
  v.clear();
  BOOST_CHECK( v.size() == 0 );
  BOOST_CHECK( v.nrows() == 0);
  
}

BOOST_AUTO_TEST_SUITE_END();
