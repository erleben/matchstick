#include <mesh_array.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(MESH_ARRAY);

BOOST_AUTO_TEST_CASE(vertex_test)
{	  
  mesh_array::Vertex V1;
  
  BOOST_CHECK_EQUAL(V1.idx(), mesh_array::UNASSIGNED());
  
  mesh_array::Vertex V2(200);
  
  BOOST_CHECK_EQUAL(V2.idx(), 200);
  
  BOOST_CHECK(!(V1 == V2));
  
  V1 = V2;
  
  BOOST_CHECK(V1 == V2);
  
  BOOST_CHECK_EQUAL(V1.idx(), 200);
  
  mesh_array::Vertex V3(V1);
  
  BOOST_CHECK(V3 == V1);
  
  BOOST_CHECK_EQUAL(V3.idx(), 200);
  
}

BOOST_AUTO_TEST_SUITE_END();
