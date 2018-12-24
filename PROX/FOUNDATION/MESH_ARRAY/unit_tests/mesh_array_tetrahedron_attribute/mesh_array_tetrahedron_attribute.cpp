#include <mesh_array.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(MESH_ARRAY);

BOOST_AUTO_TEST_CASE(tetrahedron_attribute)
{  
  mesh_array::T4Mesh M;
  
  mesh_array::t4_tetrahedron_int_attribute A1(M);
  
  BOOST_CHECK(A1.mesh() == &M);
  BOOST_CHECK_EQUAL(A1.size(),  0);
  
  mesh_array::t4_tetrahedron_int_attribute A2;
  
  M.set_capacity(4,4);
  
  M.push_vertex();
  M.push_vertex();
  M.push_vertex();
  M.push_vertex();
  
  M.push_tetrahedron( 0, 1, 2, 3);
  M.push_tetrahedron( 0, 1, 2, 3);
  M.push_tetrahedron( 0, 1, 2, 3);
  M.push_tetrahedron( 0, 1, 2, 3);
  
  A2.bind(M);
  
  BOOST_CHECK(A2.mesh() == &M);
  
  BOOST_CHECK_EQUAL(A2.size(),  4);
  
  A2(M.tetrahedron(0)) = 1;
  A2(M.tetrahedron(1)) = 2;
  A2(M.tetrahedron(2)) = 3;
  A2(M.tetrahedron(3)) = 4;
  
  BOOST_CHECK_EQUAL(A2(M.tetrahedron(0)),  1);
  BOOST_CHECK_EQUAL(A2(M.tetrahedron(1)),  2);
  BOOST_CHECK_EQUAL(A2(M.tetrahedron(2)),  3);
  BOOST_CHECK_EQUAL(A2(M.tetrahedron(3)),  4);
  
  BOOST_CHECK_EQUAL(A2[M.tetrahedron(0)],  1);
  BOOST_CHECK_EQUAL(A2[M.tetrahedron(1)],  2);
  BOOST_CHECK_EQUAL(A2[M.tetrahedron(2)],  3);
  BOOST_CHECK_EQUAL(A2[M.tetrahedron(3)],  4);
  
  A1 = A2;
  
  BOOST_CHECK_EQUAL(A1.size(),  4);
  BOOST_CHECK_EQUAL(A1[M.tetrahedron(0)],  1);
  BOOST_CHECK_EQUAL(A1[M.tetrahedron(1)],  2);
  BOOST_CHECK_EQUAL(A1[M.tetrahedron(2)],  3);
  BOOST_CHECK_EQUAL(A1[M.tetrahedron(3)],  4);
  
  A2.release();
  
  BOOST_CHECK_EQUAL(A2.size(),  0);
  
  mesh_array::t4_tetrahedron_int_attribute A3( A1 );
  
  BOOST_CHECK_EQUAL(A3.size(),  4);
  BOOST_CHECK_EQUAL(A3[M.tetrahedron(0)],  1);
  BOOST_CHECK_EQUAL(A3[M.tetrahedron(1)],  2);
  BOOST_CHECK_EQUAL(A3[M.tetrahedron(2)],  3);
  BOOST_CHECK_EQUAL(A3[M.tetrahedron(3)],  4);
}

BOOST_AUTO_TEST_SUITE_END();
