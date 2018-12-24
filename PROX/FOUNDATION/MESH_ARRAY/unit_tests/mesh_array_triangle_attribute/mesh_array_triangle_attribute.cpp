#include <mesh_array.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(MESH_ARRAY);

BOOST_AUTO_TEST_CASE(triangle_attribute)
{  
  mesh_array::T3Mesh M;
  
  mesh_array::t3_triangle_int_attribute A1(M);
  
  BOOST_CHECK(A1.mesh() == &M);
  BOOST_CHECK_EQUAL(A1.size(),  0);
  
  mesh_array::t3_triangle_int_attribute A2;
  
  M.set_capacity(3,4);
  
  M.push_vertex();
  M.push_vertex();
  M.push_vertex();

  M.push_triangle(0, 1, 2);
  M.push_triangle(0, 1, 2);
  M.push_triangle(0, 1, 2);
  M.push_triangle(0, 1, 2);
  
  A2.bind(M);
  
  BOOST_CHECK(A2.mesh() == &M);
  
  BOOST_CHECK_EQUAL(A2.size(),  4);
  
  A2( M.triangle(0) ) = 1;
  A2( M.triangle(1) ) = 2;
  A2( M.triangle(2) ) = 3;
  A2( M.triangle(3) ) = 4;
  
  BOOST_CHECK_EQUAL(A2(M.triangle(0)),  1);
  BOOST_CHECK_EQUAL(A2(M.triangle(1)),  2);
  BOOST_CHECK_EQUAL(A2(M.triangle(2)),  3);
  BOOST_CHECK_EQUAL(A2(M.triangle(3)),  4);
  
  BOOST_CHECK_EQUAL(A2[M.triangle(0)],  1);
  BOOST_CHECK_EQUAL(A2[M.triangle(1)],  2);
  BOOST_CHECK_EQUAL(A2[M.triangle(2)],  3);
  BOOST_CHECK_EQUAL(A2[M.triangle(3)],  4);
  
  A1 = A2;
  
  BOOST_CHECK_EQUAL(A1.size(),  4);
  BOOST_CHECK_EQUAL(A1(M.triangle(0)),  1);
  BOOST_CHECK_EQUAL(A1(M.triangle(1)),  2);
  BOOST_CHECK_EQUAL(A1(M.triangle(2)),  3);
  BOOST_CHECK_EQUAL(A1(M.triangle(3)),  4);
  
  A2.release();
  
  BOOST_CHECK_EQUAL(A2.size(),  0);
  
  mesh_array::t3_triangle_int_attribute A3( A1 );
  
  BOOST_CHECK_EQUAL(A3.size(),  4);
  BOOST_CHECK_EQUAL(A3(M.triangle(0)),  1);
  BOOST_CHECK_EQUAL(A3(M.triangle(1)),  2);
  BOOST_CHECK_EQUAL(A3(M.triangle(2)),  3);
  BOOST_CHECK_EQUAL(A3(M.triangle(3)),  4);
}

BOOST_AUTO_TEST_SUITE_END();
