#include <mesh_array.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(MESH_ARRAY);

BOOST_AUTO_TEST_CASE(triangle_adjacency)
{
  typedef mesh_array::T3Mesh                           mesh_type;
  typedef mesh_array::VertexRing< mesh_type >          vertex_ring_type;
  typedef mesh_array::AdjacencyInfo<mesh_type>         adjacency_type;
  
  // Empty mesh case
  {
    mesh_type M;
    
    vertex_ring_type R( M );
    
    adjacency_type A(R);
    
    // BOOST_CHECK_EQUAL( A.i(0) , mesh_array::UNASSIGNED());   // Hmmm really not defined!
  }
  
  // Small mesh case
  {
    mesh_type M;
    
    M.set_capacity(4,2);
    
    /**
     
     0 
     *
     / \    
     1 *---* 2
     \ /  
     *
     3
     */
    
    M.push_vertex();
    M.push_vertex();
    M.push_vertex();
    M.push_vertex();
    
    M.push_triangle( 0, 1, 2);
    M.push_triangle( 1, 3, 2);
    
    vertex_ring_type R( M );
    
    adjacency_type  A(R);
    
    BOOST_CHECK_EQUAL( A.i(0), 1 );
    BOOST_CHECK_EQUAL( A.j(0), mesh_array::UNASSIGNED() );
    BOOST_CHECK_EQUAL( A.k(0), mesh_array::UNASSIGNED() );
    
    BOOST_CHECK_EQUAL( A.i(1), mesh_array::UNASSIGNED() );
    BOOST_CHECK_EQUAL( A.j(1), 0 );
    BOOST_CHECK_EQUAL( A.k(1), mesh_array::UNASSIGNED() );
  }
  
  // Non empty mesh case
  {
    mesh_type M;    
    M.set_capacity(9,7);
    
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
    
    vertex_ring_type R( M );    
    adjacency_type  A(R);
    
    // 0 6 3
    BOOST_CHECK_EQUAL( A.i(0), 4 );
    BOOST_CHECK_EQUAL( A.j(0), 1 );
    BOOST_CHECK_EQUAL( A.k(0), mesh_array::UNASSIGNED() );
    
    // 0 3 1
    BOOST_CHECK_EQUAL( A.i(1), 2 );
    BOOST_CHECK_EQUAL( A.j(1), mesh_array::UNASSIGNED() );
    BOOST_CHECK_EQUAL( A.k(1), 0 );
    
    // 1 3 4
    BOOST_CHECK_EQUAL( A.i(2), 5 );
    BOOST_CHECK_EQUAL( A.j(2), 3 );
    BOOST_CHECK_EQUAL( A.k(2), 1 );
    
    // 1 4 2
    BOOST_CHECK_EQUAL( A.i(3), mesh_array::UNASSIGNED() );
    BOOST_CHECK_EQUAL( A.j(3), mesh_array::UNASSIGNED() );
    BOOST_CHECK_EQUAL( A.k(3), 2 );
    
    // 3 6 7
    BOOST_CHECK_EQUAL( A.i(4), mesh_array::UNASSIGNED() );
    BOOST_CHECK_EQUAL( A.j(4), 5 );
    BOOST_CHECK_EQUAL( A.k(4), 0 );
    
    // 3 7 4
    BOOST_CHECK_EQUAL( A.i(5), 6 );
    BOOST_CHECK_EQUAL( A.j(5), 2 );
    BOOST_CHECK_EQUAL( A.k(5), 4 );
    
    // 4 7 8
    BOOST_CHECK_EQUAL( A.i(6), mesh_array::UNASSIGNED() );
    BOOST_CHECK_EQUAL( A.j(6), mesh_array::UNASSIGNED() );
    BOOST_CHECK_EQUAL( A.k(6), 5 );
    
    /**
     
     0   1   2 
     *---*---*
     |\1/2\3/ 
     |0*3--*4  *5
     |/4\5/6\ 
     *---*---*
     6   7   8
     */
    
  }
}


BOOST_AUTO_TEST_CASE(tetrahedron_adjacency)
{
  typedef mesh_array::T4Mesh                           mesh_type;
  typedef mesh_array::VertexRing< mesh_type >          vertex_ring_type;
  typedef mesh_array::AdjacencyInfo<mesh_type>          adjacency_type;
  
  // Empty mesh case
  {
    mesh_type M;
    
    vertex_ring_type R( M );
    
    adjacency_type A(R);
    
    // BOOST_CHECK_EQUAL( A.i(0) , mesh_array::UNASSIGNED());   // Hmmm really not defined!
  }
  
  // Small mesh case
  {
    mesh_type M;
    
    M.set_capacity(5,2);
    
    /**
     
         0 
         *
        / \    
     1 *---* 2
        \ /  
         *
        3
     */
    
    M.push_vertex();
    M.push_vertex();
    M.push_vertex();
    M.push_vertex();
    M.push_vertex();
    
    M.push_tetrahedron( 0, 1, 2, 4);
    M.push_tetrahedron( 1, 3, 2, 4);
    
    vertex_ring_type R( M );
    
    adjacency_type  A(R);
    
    BOOST_CHECK_EQUAL( A.i(0), 1 );
    BOOST_CHECK_EQUAL( A.j(0), mesh_array::UNASSIGNED() );
    BOOST_CHECK_EQUAL( A.k(0), mesh_array::UNASSIGNED() );
    BOOST_CHECK_EQUAL( A.m(0), mesh_array::UNASSIGNED() );

    BOOST_CHECK_EQUAL( A.i(1), mesh_array::UNASSIGNED() );
    BOOST_CHECK_EQUAL( A.j(1), 0 );
    BOOST_CHECK_EQUAL( A.k(1), mesh_array::UNASSIGNED() );
    BOOST_CHECK_EQUAL( A.m(1), mesh_array::UNASSIGNED() );    
  }
  
  // Non empty mesh case
  {
    mesh_type M;    
    M.set_capacity(10,7);
        
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
    
    vertex_ring_type R( M );    
    adjacency_type  A(R);
        
    // 0 6 3
    BOOST_CHECK_EQUAL( A.i(0), 4 );
    BOOST_CHECK_EQUAL( A.j(0), 1 );
    BOOST_CHECK_EQUAL( A.k(0), mesh_array::UNASSIGNED() );
    BOOST_CHECK_EQUAL( A.m(0), mesh_array::UNASSIGNED() );

    // 0 3 1
    BOOST_CHECK_EQUAL( A.i(1), 2 );
    BOOST_CHECK_EQUAL( A.j(1), mesh_array::UNASSIGNED() );
    BOOST_CHECK_EQUAL( A.k(1), 0 );
    BOOST_CHECK_EQUAL( A.m(1), mesh_array::UNASSIGNED() );
    
    // 1 3 4
    BOOST_CHECK_EQUAL( A.i(2), 5 );
    BOOST_CHECK_EQUAL( A.j(2), 3 );
    BOOST_CHECK_EQUAL( A.k(2), 1 );
    BOOST_CHECK_EQUAL( A.m(2), mesh_array::UNASSIGNED() );
    
    // 1 4 2
    BOOST_CHECK_EQUAL( A.i(3), mesh_array::UNASSIGNED() );
    BOOST_CHECK_EQUAL( A.j(3), mesh_array::UNASSIGNED() );
    BOOST_CHECK_EQUAL( A.k(3), 2 );
    BOOST_CHECK_EQUAL( A.m(3), mesh_array::UNASSIGNED() );
    
    // 3 6 7
    BOOST_CHECK_EQUAL( A.i(4), mesh_array::UNASSIGNED() );
    BOOST_CHECK_EQUAL( A.j(4), 5 );
    BOOST_CHECK_EQUAL( A.k(4), 0 );
    BOOST_CHECK_EQUAL( A.m(4), mesh_array::UNASSIGNED() );
    
    // 3 7 4
    BOOST_CHECK_EQUAL( A.i(5), 6 );
    BOOST_CHECK_EQUAL( A.j(5), 2 );
    BOOST_CHECK_EQUAL( A.k(5), 4 );
    BOOST_CHECK_EQUAL( A.m(5), mesh_array::UNASSIGNED() );
    
    // 4 7 8
    BOOST_CHECK_EQUAL( A.i(6), mesh_array::UNASSIGNED() );
    BOOST_CHECK_EQUAL( A.j(6), mesh_array::UNASSIGNED() );
    BOOST_CHECK_EQUAL( A.k(6), 5 );
    BOOST_CHECK_EQUAL( A.m(6), mesh_array::UNASSIGNED() );
    
    /**
     
     0   1   2 
     *---*---*
     |\1/2\3/ 
     |0*3--*4  *5
     |/4\5/6\ 
     *---*---*
     6   7   8
     */
    
  }
}

BOOST_AUTO_TEST_SUITE_END();
