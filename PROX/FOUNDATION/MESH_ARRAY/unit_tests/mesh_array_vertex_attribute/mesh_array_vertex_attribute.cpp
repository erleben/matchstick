#include <mesh_array.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(MESH_ARRAY);

BOOST_AUTO_TEST_CASE(vertex_attribute_t3_test)
{  
  mesh_array::T3Mesh M;
  
  mesh_array::t3_vertex_int_attribute A1(M);
  
  BOOST_CHECK( A1.mesh() == &M );
  BOOST_CHECK_EQUAL( A1.size(),  0);
  
  mesh_array::t3_vertex_int_attribute A2;
  
  M.set_capacity(4,0);
  
  M.push_vertex();
  M.push_vertex();
  M.push_vertex();
  M.push_vertex();
  
  A2.bind(M);
  
  BOOST_CHECK( A2.mesh() == &M);
  
  BOOST_CHECK_EQUAL( A2.size(),  4);
  
  A2(0) = 1;
  A2(1) = 2;
  A2(2) = 3;
  A2(3) = 4;
  
  BOOST_CHECK_EQUAL( A2(M.vertex(0) ),  1);
  BOOST_CHECK_EQUAL( A2(M.vertex(1)),  2);
  BOOST_CHECK_EQUAL( A2(M.vertex(2)),  3);
  BOOST_CHECK_EQUAL( A2(M.vertex(3)),  4);
  
  BOOST_CHECK_EQUAL( A2[M.vertex(0)],  1);
  BOOST_CHECK_EQUAL( A2[M.vertex(1)],  2);
  BOOST_CHECK_EQUAL( A2[M.vertex(2)],  3);
  BOOST_CHECK_EQUAL( A2[M.vertex(3)],  4);
  
  BOOST_CHECK_EQUAL( A2(0),  1);
  BOOST_CHECK_EQUAL( A2(1),  2);
  BOOST_CHECK_EQUAL( A2(2),  3);
  BOOST_CHECK_EQUAL( A2(3),  4);
  
  BOOST_CHECK_EQUAL( A2[0],  1);
  BOOST_CHECK_EQUAL( A2[1],  2);
  BOOST_CHECK_EQUAL( A2[2],  3);
  BOOST_CHECK_EQUAL( A2[3],  4);
  
  A1 = A2;
  
  BOOST_CHECK_EQUAL( A1.size(),  4);
  BOOST_CHECK_EQUAL( A1(0),  1);
  BOOST_CHECK_EQUAL( A1(1),  2);
  BOOST_CHECK_EQUAL( A1(2),  3);
  BOOST_CHECK_EQUAL( A1(3),  4);
  
  A2.release();
  
  BOOST_CHECK_EQUAL( A2.size(),  0);
  
  mesh_array::t3_vertex_int_attribute A3( A1 );
  
  BOOST_CHECK_EQUAL( A3.size(),  4);
  BOOST_CHECK_EQUAL( A3(0),  1);
  BOOST_CHECK_EQUAL( A3(1),  2);
  BOOST_CHECK_EQUAL( A3(2),  3);
  BOOST_CHECK_EQUAL( A3(3),  4); 
}

BOOST_AUTO_TEST_CASE(vertex_attribute_t4_test)
{
  mesh_array::T4Mesh  M;
  
  mesh_array::t4_vertex_int_attribute A1(M);
  
  BOOST_CHECK( A1.mesh() == &M);
  BOOST_CHECK_EQUAL( A1.size(),  0);
  
  mesh_array::t4_vertex_int_attribute A2;
  
  M.set_capacity(4,0);
  
  M.push_vertex();
  M.push_vertex();
  M.push_vertex();
  M.push_vertex();
  
  A2.bind(M);
  
  BOOST_CHECK( A2.mesh() == &M);  
  BOOST_CHECK_EQUAL( A2.size(),  4);
  
  A2(0) = 1;
  A2(1) = 2;
  A2(2) = 3;
  A2(3) = 4;

  BOOST_CHECK_EQUAL( A2(M.vertex(0)),  1);
  BOOST_CHECK_EQUAL( A2(M.vertex(1)),  2);
  BOOST_CHECK_EQUAL( A2(M.vertex(2)),  3);
  BOOST_CHECK_EQUAL( A2(M.vertex(3)),  4);
  
  BOOST_CHECK_EQUAL( A2[M.vertex(0)],  1);
  BOOST_CHECK_EQUAL( A2[M.vertex(1)],  2);
  BOOST_CHECK_EQUAL( A2[M.vertex(2)],  3);
  BOOST_CHECK_EQUAL( A2[M.vertex(3)],  4);
  
  BOOST_CHECK_EQUAL( A2(0),  1);
  BOOST_CHECK_EQUAL( A2(1),  2);
  BOOST_CHECK_EQUAL( A2(2),  3);
  BOOST_CHECK_EQUAL( A2(3),  4);
  
  BOOST_CHECK_EQUAL( A2[0],  1);
  BOOST_CHECK_EQUAL( A2[1],  2);
  BOOST_CHECK_EQUAL( A2[2],  3);
  BOOST_CHECK_EQUAL( A2[3],  4);
  
  A1 = A2;
  
  BOOST_CHECK_EQUAL( A1.size(),  4);
  BOOST_CHECK_EQUAL( A1(0),  1);
  BOOST_CHECK_EQUAL( A1(1),  2);
  BOOST_CHECK_EQUAL( A1(2),  3);
  BOOST_CHECK_EQUAL( A1(3),  4);
  
  A2.release();
  
  BOOST_CHECK_EQUAL( A2.size(),  0);
  
  mesh_array::t4_vertex_int_attribute A3( A1 );
  
  BOOST_CHECK_EQUAL( A3.size(),  4);
  BOOST_CHECK_EQUAL( A3(0),  1);
  BOOST_CHECK_EQUAL( A3(1),  2);
  BOOST_CHECK_EQUAL( A3(2),  3);
  BOOST_CHECK_EQUAL( A3(3),  4); 
}

BOOST_AUTO_TEST_SUITE_END();
