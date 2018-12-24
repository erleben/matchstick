#include <mesh_array.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(MESH_ARRAY);

BOOST_AUTO_TEST_CASE(triangle_ciculator)
{
  typedef mesh_array::T3Mesh                        mesh_type;
  typedef mesh_array::VertexRing< mesh_type >       vertex_ring_type;
  typedef mesh_array::TriangleCirculator<mesh_type> circulator_type;
  
  // Empty mesh case
  {
    mesh_type M;
    
    vertex_ring_type ring_info( M );
    
    circulator_type C(ring_info, 0);
    
    BOOST_CHECK_EQUAL( C() , false);
    BOOST_CHECK_EQUAL( C->idx(), mesh_array::UNASSIGNED());
    ++C;
    BOOST_CHECK_EQUAL( C() , false);
  }
  
  // Non empty mesh case
  {
    mesh_type M;
    
    M.set_capacity(9,7);
    
    /**
     
     0   1   2 
     *---*---*
     |\ / \ / 
     | *3--*4  *5
     |/ \ / \ 
     *---*---*
     6   7   8
     */
    
    M.push_vertex();
    M.push_vertex();
    M.push_vertex();
    M.push_vertex();
    M.push_vertex();
    M.push_vertex();
    M.push_vertex();
    M.push_vertex();
    M.push_vertex();
    
    M.push_triangle( 0, 6, 3);
    M.push_triangle( 0, 3, 1);
    M.push_triangle( 1, 3, 4);
    M.push_triangle( 1, 4, 2);
    M.push_triangle( 3, 6, 7);
    M.push_triangle( 3, 7, 4);
    M.push_triangle( 4, 7, 8);
    
    vertex_ring_type ring_info( M );
    
    // Vertex with 2 triangles
    circulator_type C(ring_info, 0);
    BOOST_CHECK_EQUAL( C(), true );
    BOOST_CHECK_EQUAL( C->idx(), 0);
    ++C;
    BOOST_CHECK_EQUAL( C(), true );
    BOOST_CHECK_EQUAL( C->idx(), 1);
    ++C;
    BOOST_CHECK_EQUAL( C() , false);
    BOOST_CHECK_EQUAL( C->idx(), mesh_array::UNASSIGNED());
    ++C;
    BOOST_CHECK_EQUAL( C() , false);
    
    // Vertex with 0 triangles
    C =  circulator_type(ring_info, 5);
    BOOST_CHECK_EQUAL( C() , false);
    BOOST_CHECK_EQUAL( C->idx(), mesh_array::UNASSIGNED());
    ++C;
    BOOST_CHECK_EQUAL( C() , false);
    BOOST_CHECK_EQUAL( C->idx(), mesh_array::UNASSIGNED());
    
    // Vertex with 1 triangle
    C =  circulator_type(ring_info, 2);
    BOOST_CHECK_EQUAL( C() , true);
    BOOST_CHECK_EQUAL( C->idx(), 3);
    ++C;
    BOOST_CHECK_EQUAL( C() , false);
    
    // Vertex with 4 triangles  
    C =  circulator_type(ring_info, 4);
    BOOST_CHECK_EQUAL( C() , true);
    BOOST_CHECK_EQUAL( C->idx(), 2);
    ++C;
    BOOST_CHECK_EQUAL( C() , true);
    BOOST_CHECK_EQUAL( C->idx(), 3);
    ++C;
    BOOST_CHECK_EQUAL( C() , true);
    BOOST_CHECK_EQUAL( C->idx(), 5);
    ++C;
    BOOST_CHECK_EQUAL( C() , true);
    BOOST_CHECK_EQUAL( C->idx(), 6);
    ++C;
    BOOST_CHECK_EQUAL( C() , false);
  }
}

BOOST_AUTO_TEST_CASE(tetrahedron_ciculator)
{
  typedef mesh_array::T4Mesh                           mesh_type;
  typedef mesh_array::VertexRing< mesh_type >          vertex_ring_type;
  typedef mesh_array::TetrahedronCirculator<mesh_type> circulator_type;
  
  // Empty mesh case
  {
    mesh_type M;
    
    vertex_ring_type ring_info( M );
    
    circulator_type C(ring_info, 0);
    
    BOOST_CHECK_EQUAL( C() , false);
    BOOST_CHECK_EQUAL( C->idx(), mesh_array::UNASSIGNED());
    ++C;
    BOOST_CHECK_EQUAL( C() , false);
  }
  
  // Non empty mesh case
  {
    mesh_type M;
    
    M.set_capacity(10,7);
    
    /**
     
     0   1   2 
     *---*---*
     |\ / \ / 
     | *3--*4  *5
     |/ \ / \ 
     *---*---*
     6   7   8
     */
    
    M.push_vertex();
    M.push_vertex();
    M.push_vertex();
    M.push_vertex();
    M.push_vertex();
    M.push_vertex();
    M.push_vertex();
    M.push_vertex();
    M.push_vertex();
    M.push_vertex();
    
    M.push_tetrahedron( 0, 6, 3, 9);
    M.push_tetrahedron( 0, 3, 1, 9);
    M.push_tetrahedron( 1, 3, 4, 9);
    M.push_tetrahedron( 1, 4, 2, 9);
    M.push_tetrahedron( 3, 6, 7, 9);
    M.push_tetrahedron( 3, 7, 4, 9);
    M.push_tetrahedron( 4, 7, 8, 9);
    
    vertex_ring_type ring_info( M );
    
    // Vertex with 2 triangles
    circulator_type C(ring_info, 0);
    BOOST_CHECK_EQUAL( C(), true );
    BOOST_CHECK_EQUAL( C->idx(), 0);
    ++C;
    BOOST_CHECK_EQUAL( C(), true );
    BOOST_CHECK_EQUAL( C->idx(), 1);
    ++C;
    BOOST_CHECK_EQUAL( C() , false);
    BOOST_CHECK_EQUAL( C->idx(), mesh_array::UNASSIGNED());
    ++C;
    BOOST_CHECK_EQUAL( C() , false);
    
    // Vertex with 0 triangles
    C =  circulator_type(ring_info, 5);
    BOOST_CHECK_EQUAL( C() , false);
    BOOST_CHECK_EQUAL( C->idx(), mesh_array::UNASSIGNED());
    ++C;
    BOOST_CHECK_EQUAL( C() , false);
    BOOST_CHECK_EQUAL( C->idx(), mesh_array::UNASSIGNED());
    
    // Vertex with 1 triangle
    C =  circulator_type(ring_info, 2);
    BOOST_CHECK_EQUAL( C() , true);
    BOOST_CHECK_EQUAL( C->idx(), 3);
    ++C;
    BOOST_CHECK_EQUAL( C() , false);
    
    // Vertex with 4 triangles  
    C =  circulator_type(ring_info, 4);
    BOOST_CHECK_EQUAL( C() , true);
    BOOST_CHECK_EQUAL( C->idx(), 2);
    ++C;
    BOOST_CHECK_EQUAL( C() , true);
    BOOST_CHECK_EQUAL( C->idx(), 3);
    ++C;
    BOOST_CHECK_EQUAL( C() , true);
    BOOST_CHECK_EQUAL( C->idx(), 5);
    ++C;
    BOOST_CHECK_EQUAL( C() , true);
    BOOST_CHECK_EQUAL( C->idx(), 6);
    ++C;
    BOOST_CHECK_EQUAL( C() , false);
    
  }
}

BOOST_AUTO_TEST_SUITE_END();
