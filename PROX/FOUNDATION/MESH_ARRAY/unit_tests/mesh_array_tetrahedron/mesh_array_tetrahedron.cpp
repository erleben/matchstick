#include <mesh_array.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(MESH_ARRAY);

BOOST_AUTO_TEST_CASE(tetrahedron_test)
{	  
  mesh_array::Tetrahedron T1; 
  
  BOOST_CHECK_EQUAL(T1.idx(),  mesh_array::UNASSIGNED()); 
  BOOST_CHECK_EQUAL(T1.i(),    mesh_array::UNASSIGNED());
  BOOST_CHECK_EQUAL(T1.j(),    mesh_array::UNASSIGNED());
  BOOST_CHECK_EQUAL(T1.k(),    mesh_array::UNASSIGNED());
  BOOST_CHECK_EQUAL(T1.m(),    mesh_array::UNASSIGNED());
  
  
  mesh_array::Tetrahedron T2(4,5,6,7,1);
  
  BOOST_CHECK(T2.has_vertex(4));
  BOOST_CHECK(T2.has_vertex(5));
  BOOST_CHECK(T2.has_vertex(6));
  BOOST_CHECK(T2.has_vertex(7));
  BOOST_CHECK(!T2.has_vertex(10));
  
  BOOST_CHECK_EQUAL(T2.idx(),  1);
  BOOST_CHECK_EQUAL(T2.i(),    4);
  BOOST_CHECK_EQUAL(T2.j(),    5);
  BOOST_CHECK_EQUAL(T2.k(),    6);
  BOOST_CHECK_EQUAL(T2.m(),    7);
  
  BOOST_CHECK(!(T1 == T2)); 
  
  T1 = T2; 

  BOOST_CHECK(T1 == T2); 
  
  BOOST_CHECK_EQUAL(T1.idx(),  1);
  BOOST_CHECK_EQUAL(T1.i(),    4);
  BOOST_CHECK_EQUAL(T1.j(),    5);
  BOOST_CHECK_EQUAL(T1.k(),    6);
  BOOST_CHECK_EQUAL(T1.m(),    7);
  
  mesh_array::Tetrahedron T3(T1);
  
  BOOST_CHECK(T3 == T2);
  
  BOOST_CHECK_EQUAL(T3.idx(),  1);
  BOOST_CHECK_EQUAL(T3.i(),    4);
  BOOST_CHECK_EQUAL(T3.j(),    5);
  BOOST_CHECK_EQUAL(T3.k(),    6);
  BOOST_CHECK_EQUAL(T3.m(),    7);
  }
  
BOOST_AUTO_TEST_SUITE_END();
