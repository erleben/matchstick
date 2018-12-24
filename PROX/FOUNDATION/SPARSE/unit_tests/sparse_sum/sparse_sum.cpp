#include <sparse.h>
#include <sparse_fill.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(SPARSE);

BOOST_AUTO_TEST_CASE(sum_test)
{    
  
  typedef sparse::Block<4,1,float>             block_type;
  typedef sparse::Vector<block_type>           vector_type;
  
  vector_type a; 
  vector_type b; 
  vector_type c; 
  
  a.resize(2);
  b.resize(2);
  c.resize(2);
  
  a(0)(0) = 1.0f;
  a(0)(1) = 1.0f;
  a(0)(2) = 1.0f;
  a(0)(3) = 1.0f;
  a(1)(0) = 1.0f;
  a(1)(1) = 1.0f;
  a(1)(2) = 1.0f;
  a(1)(3) = 1.0f;
  
  b(0)(0) = 0.0f;
  b(0)(1) = 1.0f;
  b(0)(2) = 2.0f;
  b(0)(3) = 3.0f;
  b(1)(0) = 4.0f;
  b(1)(1) = 5.0f;
  b(1)(2) = 6.0f;
  b(1)(3) = 7.0f;
  
  c(0)(0) = 0.0f;
  c(0)(1) = 0.0f;
  c(0)(2) = 0.0f;
  c(0)(3) = 0.0f;
  c(1)(0) = 0.0f;
  c(1)(1) = 0.0f;
  c(1)(2) = 0.0f;
  c(1)(3) = 0.0f;
  
  sparse::add(a,b,c);
  
  BOOST_CHECK( c(0)(0) == 1.0f );
  BOOST_CHECK( c(0)(1) == 2.0f );
  BOOST_CHECK( c(0)(2) == 3.0f );
  BOOST_CHECK( c(0)(3) == 4.0f );
  BOOST_CHECK( c(1)(0) == 5.0f );
  BOOST_CHECK( c(1)(1) == 6.0f );
  BOOST_CHECK( c(1)(2) == 7.0f );
  BOOST_CHECK( c(1)(3) == 8.0f );
}

BOOST_AUTO_TEST_CASE(compressed_vector_sum_test)
{    
  
  typedef sparse::Block<4,1,float>             block_type;
  typedef sparse::CompressedVector<block_type> compressed_vector_type;
  typedef sparse::Vector<block_type>           vector_type;
  
  vector_type c; 

  c.resize(2);
  c(0)(0) = 1.0f;
  c(0)(1) = 2.0f;
  c(0)(2) = 3.0f;
  c(0)(3) = 4.0f;
  c(1)(0) = 5.0f;
  c(1)(1) = 6.0f;
  c(1)(2) = 7.0f;
  c(1)(3) = 8.0f;

  compressed_vector_type v(1u, 2u);
  v(1)(0) = 6.0f;
  v(1)(1) = 7.0f;
  v(1)(2) = 8.0f;
  v(1)(3) = 9.0f;

  sparse::add(v,c);
  BOOST_CHECK( c(0)(0) == 1.0f );
  BOOST_CHECK( c(0)(1) == 2.0f );
  BOOST_CHECK( c(0)(2) == 3.0f );
  BOOST_CHECK( c(0)(3) == 4.0f );
  BOOST_CHECK( c(1)(0) == 11.0f );
  BOOST_CHECK( c(1)(1) == 13.0f );
  BOOST_CHECK( c(1)(2) == 15.0f );
  BOOST_CHECK( c(1)(3) == 17.0f );

  compressed_vector_type w(2u, 2u);
  w(0)(0) = 2.0f;
  w(0)(1) = 3.0f;
  w(0)(2) = 4.0f;
  w(0)(3) = 5.0f;
  w(1)(0) = 6.0f;
  w(1)(1) = 7.0f;
  w(1)(2) = 8.0f;
  w(1)(3) = 9.0f;

  sparse::add(v,w);
  BOOST_CHECK( w(0)(0) == 2.0f );
  BOOST_CHECK( w(0)(1) == 3.0f );
  BOOST_CHECK( w(0)(2) == 4.0f );
  BOOST_CHECK( w(0)(3) == 5.0f );
  BOOST_CHECK( w(1)(0) == 12.0f );
  BOOST_CHECK( w(1)(1) == 14.0f );
  BOOST_CHECK( w(1)(2) == 16.0f );
  BOOST_CHECK( w(1)(3) == 18.0f );

  compressed_vector_type u(1u, 2u);
  u(1)(0) = 9.0f;
  u(1)(1) = 8.0f;
  u(1)(2) = 7.0f;
  u(1)(3) = 6.0f;
  
  sparse::add_same_dimension(v,u);
  BOOST_CHECK( u.size() == 1u );
  BOOST_CHECK( u.nrows() == 2u );
  BOOST_CHECK( u(1)(0) == 15.0f );
  BOOST_CHECK( u(1)(1) == 15.0f );
  BOOST_CHECK( u(1)(2) == 15.0f );
  BOOST_CHECK( u(1)(3) == 15.0f );
  BOOST_CHECK( v.size() == 1u );
  BOOST_CHECK( v.nrows() == 2u );
  BOOST_CHECK( v(1)(0) == 6.0f );
  BOOST_CHECK( v(1)(1) == 7.0f );
  BOOST_CHECK( v(1)(2) == 8.0f );
  BOOST_CHECK( v(1)(3) == 9.0f );
}

BOOST_AUTO_TEST_CASE(fast_compressed_vector_sum_test)
{    
  
  typedef sparse::Block<4,1,float>             block_type;
  typedef sparse::CompressedVector<block_type,sparse::detail::FastVectorCompressor> fast_compressed_vector_type;
  typedef sparse::Vector<block_type>           vector_type;
  
  vector_type c; 

  c.resize(2);
  c(0)(0) = 1.0f;
  c(0)(1) = 2.0f;
  c(0)(2) = 3.0f;
  c(0)(3) = 4.0f;
  c(1)(0) = 5.0f;
  c(1)(1) = 6.0f;
  c(1)(2) = 7.0f;
  c(1)(3) = 8.0f;

  fast_compressed_vector_type v(1u, 2u);
  v(1)(0) = 6.0f;
  v(1)(1) = 7.0f;
  v(1)(2) = 8.0f;
  v(1)(3) = 9.0f;

  sparse::add(v,c);
  BOOST_CHECK( c(0)(0) == 1.0f );
  BOOST_CHECK( c(0)(1) == 2.0f );
  BOOST_CHECK( c(0)(2) == 3.0f );
  BOOST_CHECK( c(0)(3) == 4.0f );
  BOOST_CHECK( c(1)(0) == 11.0f );
  BOOST_CHECK( c(1)(1) == 13.0f );
  BOOST_CHECK( c(1)(2) == 15.0f );
  BOOST_CHECK( c(1)(3) == 17.0f );

  fast_compressed_vector_type w(2u, 2u);
  w(0)(0) = 2.0f;
  w(0)(1) = 3.0f;
  w(0)(2) = 4.0f;
  w(0)(3) = 5.0f;
  w(1)(0) = 6.0f;
  w(1)(1) = 7.0f;
  w(1)(2) = 8.0f;
  w(1)(3) = 9.0f;

  sparse::add(v,w);
  BOOST_CHECK( w(0)(0) == 2.0f );
  BOOST_CHECK( w(0)(1) == 3.0f );
  BOOST_CHECK( w(0)(2) == 4.0f );
  BOOST_CHECK( w(0)(3) == 5.0f );
  BOOST_CHECK( w(1)(0) == 12.0f );
  BOOST_CHECK( w(1)(1) == 14.0f );
  BOOST_CHECK( w(1)(2) == 16.0f );
  BOOST_CHECK( w(1)(3) == 18.0f );

  fast_compressed_vector_type u(1u, 2u);
  u(1)(0) = 9.0f;
  u(1)(1) = 8.0f;
  u(1)(2) = 7.0f;
  u(1)(3) = 6.0f;
  
  sparse::add_same_dimension(v,u);
  BOOST_CHECK( u.size() == 1u );
  BOOST_CHECK( u.nrows() == 2u );
  BOOST_CHECK( u(1)(0) == 15.0f );
  BOOST_CHECK( u(1)(1) == 15.0f );
  BOOST_CHECK( u(1)(2) == 15.0f );
  BOOST_CHECK( u(1)(3) == 15.0f );
  BOOST_CHECK( v.size() == 1u );
  BOOST_CHECK( v.nrows() == 2u );
  BOOST_CHECK( v(1)(0) == 6.0f );
  BOOST_CHECK( v(1)(1) == 7.0f );
  BOOST_CHECK( v(1)(2) == 8.0f );
  BOOST_CHECK( v(1)(3) == 9.0f );
}

BOOST_AUTO_TEST_SUITE_END();
