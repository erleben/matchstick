#include <mesh_array.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(MESH_ARRAY);

BOOST_AUTO_TEST_CASE(t3mesh_test)
{
  mesh_array::T3Mesh  M1;
  
  BOOST_CHECK_EQUAL(M1.triangle_size(),     0);
  BOOST_CHECK_EQUAL(M1.vertex_size(),       0);
  
  M1.set_capacity(4,2);
  
  BOOST_CHECK_EQUAL(M1.vertex_capacity(),   4);
  BOOST_CHECK_EQUAL(M1.triangle_capacity(), 2);
  BOOST_CHECK_EQUAL(M1.vertex_size(),       0);
  BOOST_CHECK_EQUAL(M1.triangle_size(),     0);
  
  M1.push_vertex();
  M1.push_vertex();
  M1.push_vertex();
  M1.push_vertex();
  BOOST_CHECK_EQUAL(M1.vertex_size(),       4);
  BOOST_CHECK_EQUAL(M1.triangle_size(),     0);
  
  M1.push_triangle(0,1,2);
  M1.push_triangle(3,2,1);
  BOOST_CHECK_EQUAL(M1.vertex_size(),       4);
  BOOST_CHECK_EQUAL(M1.triangle_size(),     2);
    
  BOOST_CHECK_EQUAL(M1.triangle(0).idx(),   0);
  BOOST_CHECK_EQUAL(M1.triangle(1).idx(),   1); 
  
  BOOST_CHECK_EQUAL(M1.vertex(0).idx(), 0);
  BOOST_CHECK_EQUAL(M1.vertex(1).idx(), 1);
  BOOST_CHECK_EQUAL(M1.vertex(2).idx(), 2);
  BOOST_CHECK_EQUAL(M1.vertex(3).idx(), 3);
    
  mesh_array::T3Mesh M2 = M1;
  
  BOOST_CHECK_EQUAL(M2.vertex_size(), 4);
  BOOST_CHECK_EQUAL(M2.triangle_size(), 2);
  BOOST_CHECK_EQUAL(M2.vertex_capacity(), 4);
  BOOST_CHECK_EQUAL(M2.triangle_capacity(), 2);
  
  BOOST_CHECK_EQUAL(M2.triangle(0).idx(), 0);
  BOOST_CHECK_EQUAL(M2.triangle(1).idx(), 1); 
  
  BOOST_CHECK_EQUAL(M2.vertex(0).idx(), 0);
  BOOST_CHECK_EQUAL(M2.vertex(1).idx(), 1);
  BOOST_CHECK_EQUAL(M2.vertex(2).idx(), 2);
  BOOST_CHECK_EQUAL(M2.vertex(3).idx(), 3);
  
  mesh_array::T3Mesh M3(M2);
  
  BOOST_CHECK_EQUAL(M3.vertex_size(), 4);
  BOOST_CHECK_EQUAL(M3.triangle_size(), 2);
  
  BOOST_CHECK_EQUAL(M3.triangle(0).idx(), 0);
  BOOST_CHECK_EQUAL(M3.triangle(1).idx(), 1); 
  
  BOOST_CHECK_EQUAL(M3.vertex(0).idx(), 0);
  BOOST_CHECK_EQUAL(M3.vertex(1).idx(), 1);
  BOOST_CHECK_EQUAL(M3.vertex(2).idx(), 2);
  BOOST_CHECK_EQUAL(M3.vertex(3).idx(), 3);
  
}

BOOST_AUTO_TEST_CASE(t4mesh_test)
{
  mesh_array::T4Mesh  M1;
  
  BOOST_CHECK_EQUAL(M1.tetrahedron_size(),0);
  BOOST_CHECK_EQUAL(M1.vertex_size(),0);
  
  M1.set_capacity(5,2);
  
  BOOST_CHECK_EQUAL(M1.vertex_size(),0);
  BOOST_CHECK_EQUAL(M1.tetrahedron_size(),0);
  BOOST_CHECK_EQUAL(M1.vertex_capacity(),5);
  BOOST_CHECK_EQUAL(M1.tetrahedron_capacity(),2);
  
  M1.push_vertex();
  M1.push_vertex();
  M1.push_vertex();
  M1.push_vertex();
  M1.push_vertex();
  
  M1.push_tetrahedron(0,1,2,3);  
  M1.push_tetrahedron(4,3,2,1);

  BOOST_CHECK_EQUAL(M1.vertex_size(),5);
  BOOST_CHECK_EQUAL(M1.tetrahedron_size(),2);
  
  BOOST_CHECK_EQUAL(M1.tetrahedron(0).idx(), 0);
  BOOST_CHECK_EQUAL(M1.tetrahedron(1).idx(), 1); 
  
  BOOST_CHECK_EQUAL(M1.vertex(0).idx(), 0);
  BOOST_CHECK_EQUAL(M1.vertex(1).idx(), 1);
  BOOST_CHECK_EQUAL(M1.vertex(2).idx(), 2);
  BOOST_CHECK_EQUAL(M1.vertex(3).idx(), 3);
  BOOST_CHECK_EQUAL(M1.vertex(4).idx(), 4);
    
  mesh_array::T4Mesh  M2 = M1;

  BOOST_CHECK_EQUAL(M2.vertex_size(),5);
  BOOST_CHECK_EQUAL(M2.tetrahedron_size(),2);
  
  BOOST_CHECK_EQUAL(M2.tetrahedron(0).idx(), 0);
  BOOST_CHECK_EQUAL(M2.tetrahedron(1).idx(), 1); 
  
  BOOST_CHECK_EQUAL(M2.vertex(0).idx(), 0);
  BOOST_CHECK_EQUAL(M2.vertex(1).idx(), 1);
  BOOST_CHECK_EQUAL(M2.vertex(2).idx(), 2);
  BOOST_CHECK_EQUAL(M2.vertex(3).idx(), 3);
  BOOST_CHECK_EQUAL(M2.vertex(4).idx(), 4);

  mesh_array::T4Mesh  M3(M2);
  
  BOOST_CHECK_EQUAL(M3.vertex_size(),5);
  BOOST_CHECK_EQUAL(M3.tetrahedron_size(),2);
  
  BOOST_CHECK_EQUAL(M3.tetrahedron(0).idx(), 0);
  BOOST_CHECK_EQUAL(M3.tetrahedron(1).idx(), 1); 
  
  BOOST_CHECK_EQUAL(M3.vertex(0).idx(), 0);
  BOOST_CHECK_EQUAL(M3.vertex(1).idx(), 1);
  BOOST_CHECK_EQUAL(M3.vertex(2).idx(), 2);
  BOOST_CHECK_EQUAL(M3.vertex(3).idx(), 3);
  BOOST_CHECK_EQUAL(M3.vertex(4).idx(), 4);
}

BOOST_AUTO_TEST_SUITE_END();
