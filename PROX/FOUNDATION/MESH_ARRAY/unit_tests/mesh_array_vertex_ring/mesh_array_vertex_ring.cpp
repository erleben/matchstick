#include <mesh_array.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(MESH_ARRAY);

BOOST_AUTO_TEST_CASE(vertex_ring_t3mesh)
{
  // Empty mesh
  {
    mesh_array::T3Mesh  M1;
    mesh_array::VertexRing< mesh_array::T3Mesh > R1;
    
    R1.bind(M1);
    
    BOOST_CHECK_EQUAL(R1.V2T().size(), 0);
    
    BOOST_CHECK_EQUAL(R1.offset().size(), 0);
  }
  
  // All vertices have triangles
  {
    mesh_array::T3Mesh  M2;
    mesh_array::VertexRing< mesh_array::T3Mesh > R2;
    
    M2.set_capacity(6, 5);
    
    M2.push_vertex();
    M2.push_vertex();
    M2.push_vertex();
    M2.push_vertex();
    M2.push_vertex();
    M2.push_vertex();
    
    M2.push_triangle( 0, 1, 2 );
    M2.push_triangle( 0, 2, 3 );
    M2.push_triangle( 5, 1, 0 );
    M2.push_triangle( 5, 4, 3 );
    M2.push_triangle( 4, 3, 2 );
    
    //
    // T2V
    //  (V) 0 1 2    0 2 3    5 1 0    5 4 3    4 3 2
    //  (T) 0 0 0    1 1 1    2 2 2    3 3 3    4 4 4
    //
    // V2T
    //
    //  (V) 0 0 0   1 1   2 2 2   3 3 3   4 4   5 5
    //  (T) 0 1 2   0 2   0 1 4   1 3 4   3 4   2 3
    //
    R2.bind(M2);
    BOOST_CHECK_EQUAL(R2.V2T().size(), 15);
    BOOST_CHECK_EQUAL( R2.V2T()[0].first, 0 );
    BOOST_CHECK_EQUAL( R2.V2T()[1].first, 0 );
    BOOST_CHECK_EQUAL( R2.V2T()[2].first, 0 );    
    BOOST_CHECK_EQUAL( R2.V2T()[3].first, 1 );
    BOOST_CHECK_EQUAL( R2.V2T()[4].first, 1 );    
    BOOST_CHECK_EQUAL( R2.V2T()[5].first, 2 );
    BOOST_CHECK_EQUAL( R2.V2T()[6].first, 2 );
    BOOST_CHECK_EQUAL( R2.V2T()[7].first, 2 );    
    BOOST_CHECK_EQUAL( R2.V2T()[8].first, 3 );
    BOOST_CHECK_EQUAL( R2.V2T()[9].first, 3 );
    BOOST_CHECK_EQUAL( R2.V2T()[10].first, 3 );    
    BOOST_CHECK_EQUAL( R2.V2T()[11].first, 4 );
    BOOST_CHECK_EQUAL( R2.V2T()[12].first, 4 );    
    BOOST_CHECK_EQUAL( R2.V2T()[13].first, 5 );
    BOOST_CHECK_EQUAL( R2.V2T()[14].first, 5 );
    
    BOOST_CHECK_EQUAL( R2.V2T()[0].second, 0 );
    BOOST_CHECK_EQUAL( R2.V2T()[1].second, 1 );
    BOOST_CHECK_EQUAL( R2.V2T()[2].second, 2 );  
    BOOST_CHECK_EQUAL( R2.V2T()[3].second, 0 );
    BOOST_CHECK_EQUAL( R2.V2T()[4].second, 2 );
    BOOST_CHECK_EQUAL( R2.V2T()[5].second, 0 );
    BOOST_CHECK_EQUAL( R2.V2T()[6].second, 1 );
    BOOST_CHECK_EQUAL( R2.V2T()[7].second, 4 );
    BOOST_CHECK_EQUAL( R2.V2T()[8].second, 1 );
    BOOST_CHECK_EQUAL( R2.V2T()[9].second, 3 );
    BOOST_CHECK_EQUAL( R2.V2T()[10].second, 4 );
    BOOST_CHECK_EQUAL( R2.V2T()[11].second, 3 );
    BOOST_CHECK_EQUAL( R2.V2T()[12].second, 4 );
    BOOST_CHECK_EQUAL( R2.V2T()[13].second, 2 );
    BOOST_CHECK_EQUAL( R2.V2T()[14].second, 3 );
        
    BOOST_CHECK_EQUAL( R2.offset().size(), 7  );
    BOOST_CHECK_EQUAL( R2.offset()[0],     0  );
    BOOST_CHECK_EQUAL( R2.offset()[1],     3  );
    BOOST_CHECK_EQUAL( R2.offset()[2],     5  );
    BOOST_CHECK_EQUAL( R2.offset()[3],     8  );
    BOOST_CHECK_EQUAL( R2.offset()[4],    11  );
    BOOST_CHECK_EQUAL( R2.offset()[5],    13  );
    BOOST_CHECK_EQUAL( R2.offset()[6],    15  );
  }  

  // Last vertex has no triangles
  {
    mesh_array::T3Mesh  M3;
    mesh_array::VertexRing< mesh_array::T3Mesh > R3;
    
    M3.set_capacity(4, 1);
    
    M3.push_vertex();
    M3.push_vertex();
    M3.push_vertex();
    M3.push_vertex();
    
    M3.push_triangle( 0, 1, 2);
    
    //
    // T2V
    //  (V) 0 1 2
    //  (T) 0 0 0
    //
    
    R3.bind(M3);
        
    BOOST_CHECK_EQUAL(R3.V2T().size(), 3);
    
    BOOST_CHECK_EQUAL( R3.V2T()[0].first, 0 );
    BOOST_CHECK_EQUAL( R3.V2T()[1].first, 1 );
    BOOST_CHECK_EQUAL( R3.V2T()[2].first, 2 );
    
    BOOST_CHECK_EQUAL( R3.V2T()[0].second, 0 );
    BOOST_CHECK_EQUAL( R3.V2T()[1].second, 0 );
    BOOST_CHECK_EQUAL( R3.V2T()[2].second, 0 );  
        
    BOOST_CHECK_EQUAL( R3.offset().size(), 5);
    BOOST_CHECK_EQUAL( R3.offset()[0],  0);
    BOOST_CHECK_EQUAL( R3.offset()[1],  1);
    BOOST_CHECK_EQUAL( R3.offset()[2],  2);
    BOOST_CHECK_EQUAL( R3.offset()[3],  3);
    BOOST_CHECK_EQUAL( R3.offset()[4],  3);
  }  

  // First vertex has no triangles
  {
    mesh_array::T3Mesh  M4;
    mesh_array::VertexRing< mesh_array::T3Mesh > R4;
    
    M4.set_capacity(4, 1);
    
    M4.push_vertex();
    M4.push_vertex();
    M4.push_vertex();
    M4.push_vertex();
    
    M4.push_triangle( 1, 2, 3);
        
    R4.bind(M4);
        
    BOOST_CHECK_EQUAL( R4.V2T().size(), 3);
    
    BOOST_CHECK_EQUAL( R4.V2T()[0].first, 1 );
    BOOST_CHECK_EQUAL( R4.V2T()[1].first, 2 );
    BOOST_CHECK_EQUAL( R4.V2T()[2].first, 3 );
    
    BOOST_CHECK_EQUAL( R4.V2T()[0].second, 0 );
    BOOST_CHECK_EQUAL( R4.V2T()[1].second, 0 );
    BOOST_CHECK_EQUAL( R4.V2T()[2].second, 0 );  
        
    BOOST_CHECK_EQUAL( R4.offset().size(), 5);
    
    BOOST_CHECK_EQUAL( R4.offset()[0],  0);
    BOOST_CHECK_EQUAL( R4.offset()[1],  0);
    BOOST_CHECK_EQUAL( R4.offset()[2],  1);
    BOOST_CHECK_EQUAL( R4.offset()[3],  2);
    BOOST_CHECK_EQUAL( R4.offset()[4],  3);
  }  

  // Middle vertex has no triangles
  {
    mesh_array::T3Mesh  M4;
    mesh_array::VertexRing< mesh_array::T3Mesh > R4;
    
    M4.set_capacity(4, 1);
    
    M4.push_vertex();
    M4.push_vertex();
    M4.push_vertex();
    M4.push_vertex();
    
    M4.push_triangle( 0, 1, 3);
        
    R4.bind(M4);
    
    BOOST_CHECK_EQUAL( R4.V2T().size(), 3);
    
    BOOST_CHECK_EQUAL( R4.V2T()[0].first, 0 );
    BOOST_CHECK_EQUAL( R4.V2T()[1].first, 1 );
    BOOST_CHECK_EQUAL( R4.V2T()[2].first, 3 );
    
    BOOST_CHECK_EQUAL( R4.V2T()[0].second, 0 );
    BOOST_CHECK_EQUAL( R4.V2T()[1].second, 0 );
    BOOST_CHECK_EQUAL( R4.V2T()[2].second, 0 );  
    
    BOOST_CHECK_EQUAL( R4.offset().size(), 5);
    
    BOOST_CHECK_EQUAL( R4.offset()[0],  0);
    BOOST_CHECK_EQUAL( R4.offset()[1],  1);
    BOOST_CHECK_EQUAL( R4.offset()[2],  2);
    BOOST_CHECK_EQUAL( R4.offset()[3],  2);
    BOOST_CHECK_EQUAL( R4.offset()[4],  3);
  }  
}

BOOST_AUTO_TEST_CASE(vertex_ring_t4mesh)
{
  // Empty mesh
  {
    mesh_array::T4Mesh  M1;
    mesh_array::VertexRing< mesh_array::T4Mesh > R1;
    R1.bind(M1);
    
    BOOST_CHECK_EQUAL(R1.V2T().size(), 0);
    
    BOOST_CHECK_EQUAL(R1.offset().size(), 0);
  }
  
  // All vertices have tetrahedra
  {
    mesh_array::T4Mesh  M2;
    mesh_array::VertexRing< mesh_array::T4Mesh > R2;
    
    M2.set_capacity(6, 5);
    
    M2.push_vertex();
    M2.push_vertex();
    M2.push_vertex();
    M2.push_vertex();
    M2.push_vertex();
    M2.push_vertex();
    
    M2.push_tetrahedron( 0, 1, 2, 3);
    M2.push_tetrahedron( 0, 2, 3, 4);
    M2.push_tetrahedron( 5, 1, 0, 3);
    M2.push_tetrahedron( 5, 4, 3, 1);
    M2.push_tetrahedron( 4, 3, 2, 1);
    
    //
    // T2V
    //  (V) 0 1 2 3  0 2 3 4  5 1 0 3  5 4 3 1    4 3 2 1
    //  (T) 0 0 0 0  1 1 1 1  2 2 2 2  3 3 3 3    4 4 4 4
    //
    // V2T
    //
    // 0 : 1 2 3
    // 1 : 0 2 3 4
    // 2 : 0 1 4
    // 3 : 0 1 2 3 4
    // 4 : 1 3 4
    // 5 : 2 3
    //
    
    R2.bind(M2);
    
    BOOST_CHECK_EQUAL(R2.V2T().size(), 20);
    
    BOOST_CHECK_EQUAL( R2.V2T()[0].first, 0 );
    BOOST_CHECK_EQUAL( R2.V2T()[1].first, 0 );
    BOOST_CHECK_EQUAL( R2.V2T()[2].first, 0 );
    BOOST_CHECK_EQUAL( R2.V2T()[3].first, 1 );
    BOOST_CHECK_EQUAL( R2.V2T()[4].first, 1 );
    BOOST_CHECK_EQUAL( R2.V2T()[5].first, 1 );
    BOOST_CHECK_EQUAL( R2.V2T()[6].first, 1 );
    BOOST_CHECK_EQUAL( R2.V2T()[7].first, 2 );
    BOOST_CHECK_EQUAL( R2.V2T()[8].first, 2 );
    BOOST_CHECK_EQUAL( R2.V2T()[9].first, 2 );
    BOOST_CHECK_EQUAL( R2.V2T()[10].first, 3 );
    BOOST_CHECK_EQUAL( R2.V2T()[11].first, 3 );
    BOOST_CHECK_EQUAL( R2.V2T()[12].first, 3 );
    BOOST_CHECK_EQUAL( R2.V2T()[13].first, 3 );
    BOOST_CHECK_EQUAL( R2.V2T()[14].first, 3 );
    BOOST_CHECK_EQUAL( R2.V2T()[15].first, 4 );
    BOOST_CHECK_EQUAL( R2.V2T()[16].first, 4 );
    BOOST_CHECK_EQUAL( R2.V2T()[17].first, 4 );
    BOOST_CHECK_EQUAL( R2.V2T()[18].first, 5 );
    BOOST_CHECK_EQUAL( R2.V2T()[19].first, 5 );
    
    BOOST_CHECK_EQUAL( R2.V2T()[0].second, 0 );
    BOOST_CHECK_EQUAL( R2.V2T()[1].second, 1 );
    BOOST_CHECK_EQUAL( R2.V2T()[2].second, 2 );  
    BOOST_CHECK_EQUAL( R2.V2T()[3].second, 0 );
    BOOST_CHECK_EQUAL( R2.V2T()[4].second, 2 );
    BOOST_CHECK_EQUAL( R2.V2T()[5].second, 3 );
    BOOST_CHECK_EQUAL( R2.V2T()[6].second, 4 );
    BOOST_CHECK_EQUAL( R2.V2T()[7].second, 0 );
    BOOST_CHECK_EQUAL( R2.V2T()[8].second, 1 );
    BOOST_CHECK_EQUAL( R2.V2T()[9].second, 4 );
    BOOST_CHECK_EQUAL( R2.V2T()[10].second, 0 );
    BOOST_CHECK_EQUAL( R2.V2T()[11].second, 1 );
    BOOST_CHECK_EQUAL( R2.V2T()[12].second, 2 );
    BOOST_CHECK_EQUAL( R2.V2T()[13].second, 3 );
    BOOST_CHECK_EQUAL( R2.V2T()[14].second, 4 );
    BOOST_CHECK_EQUAL( R2.V2T()[15].second, 1 );
    BOOST_CHECK_EQUAL( R2.V2T()[16].second, 3 );
    BOOST_CHECK_EQUAL( R2.V2T()[17].second, 4 );
    BOOST_CHECK_EQUAL( R2.V2T()[18].second, 2 );
    BOOST_CHECK_EQUAL( R2.V2T()[19].second, 3 );
    
    BOOST_CHECK_EQUAL( R2.offset().size(), 7  );
    BOOST_CHECK_EQUAL( R2.offset()[0],     0  );
    BOOST_CHECK_EQUAL( R2.offset()[1],     3  );
    BOOST_CHECK_EQUAL( R2.offset()[2],     7  );
    BOOST_CHECK_EQUAL( R2.offset()[3],    10  );
    BOOST_CHECK_EQUAL( R2.offset()[4],    15  );
    BOOST_CHECK_EQUAL( R2.offset()[5],    18  );
    BOOST_CHECK_EQUAL( R2.offset()[6],    20  );
  }  
  
  // Last vertex has no tetrahedra
  {
    mesh_array::T4Mesh  M3;
    mesh_array::VertexRing< mesh_array::T4Mesh > R3;
    
    M3.set_capacity(5, 1);
    
    M3.push_vertex();
    M3.push_vertex();
    M3.push_vertex();
    M3.push_vertex();
    M3.push_vertex();
    
    M3.push_tetrahedron( 0, 1, 2, 3);
    
    //
    // T2V
    //  (V) 0 1 2 3  
    //  (T) 0 0 0 0  
    //
    // V2T
    //
    // 0 : 0
    // 1 : 0
    // 2 : 0
    // 3 : 0
    // 4 : inf
    //
    
    R3.bind(M3);
    
    BOOST_CHECK_EQUAL(R3.V2T().size(), 4);
    
    BOOST_CHECK_EQUAL( R3.V2T()[0].first, 0 );
    BOOST_CHECK_EQUAL( R3.V2T()[1].first, 1 );
    BOOST_CHECK_EQUAL( R3.V2T()[2].first, 2 );
    BOOST_CHECK_EQUAL( R3.V2T()[3].first, 3 );
    
    BOOST_CHECK_EQUAL( R3.V2T()[0].second, 0 );
    BOOST_CHECK_EQUAL( R3.V2T()[1].second, 0 );
    BOOST_CHECK_EQUAL( R3.V2T()[2].second, 0 );  
    BOOST_CHECK_EQUAL( R3.V2T()[3].second, 0 );
    
    BOOST_CHECK_EQUAL( R3.offset().size(), 6);
    BOOST_CHECK_EQUAL( R3.offset()[0],  0);
    BOOST_CHECK_EQUAL( R3.offset()[1],  1);
    BOOST_CHECK_EQUAL( R3.offset()[2],  2);
    BOOST_CHECK_EQUAL( R3.offset()[3],  3);
    BOOST_CHECK_EQUAL( R3.offset()[4],  4);
    BOOST_CHECK_EQUAL( R3.offset()[5],  4);
  }  
  
  // First vertex has no tetrahedra
  {
    mesh_array::T4Mesh  M4;
    mesh_array::VertexRing< mesh_array::T4Mesh > R4;
    
    M4.set_capacity(5, 1);
    
    M4.push_vertex();
    M4.push_vertex();
    M4.push_vertex();
    M4.push_vertex();
    M4.push_vertex();
    
    M4.push_tetrahedron( 1, 2, 3, 4);
    
    R4.bind(M4);
    
    BOOST_CHECK_EQUAL( R4.V2T().size(), 4);
    
    BOOST_CHECK_EQUAL( R4.V2T()[0].first, 1 );
    BOOST_CHECK_EQUAL( R4.V2T()[1].first, 2 );
    BOOST_CHECK_EQUAL( R4.V2T()[2].first, 3 );
    BOOST_CHECK_EQUAL( R4.V2T()[3].first, 4 );
    
    BOOST_CHECK_EQUAL( R4.V2T()[0].second, 0 );
    BOOST_CHECK_EQUAL( R4.V2T()[1].second, 0 );
    BOOST_CHECK_EQUAL( R4.V2T()[2].second, 0 );  
    BOOST_CHECK_EQUAL( R4.V2T()[3].second, 0 );
    
    BOOST_CHECK_EQUAL( R4.offset().size(), 6);
    
    BOOST_CHECK_EQUAL( R4.offset()[0],  0);
    BOOST_CHECK_EQUAL( R4.offset()[1],  0);
    BOOST_CHECK_EQUAL( R4.offset()[2],  1);
    BOOST_CHECK_EQUAL( R4.offset()[3],  2);
    BOOST_CHECK_EQUAL( R4.offset()[4],  3);
    BOOST_CHECK_EQUAL( R4.offset()[5],  4); 
  }  
  
  // Middle vertex has no tetrahedra
  {
    mesh_array::T4Mesh  M4;
    mesh_array::VertexRing< mesh_array::T4Mesh > R4;
    
    M4.set_capacity(5, 1);
    
    M4.push_vertex();
    M4.push_vertex();
    M4.push_vertex();
    M4.push_vertex();
    M4.push_vertex();
    
    M4.push_tetrahedron( 0, 1, 3, 4);
    
    R4.bind(M4);
    
    BOOST_CHECK_EQUAL( R4.V2T().size(), 4);
    
    BOOST_CHECK_EQUAL( R4.V2T()[0].first, 0 );
    BOOST_CHECK_EQUAL( R4.V2T()[1].first, 1 );
    BOOST_CHECK_EQUAL( R4.V2T()[2].first, 3 );
    BOOST_CHECK_EQUAL( R4.V2T()[3].first, 4 );
    
    BOOST_CHECK_EQUAL( R4.V2T()[0].second, 0 );
    BOOST_CHECK_EQUAL( R4.V2T()[1].second, 0 );
    BOOST_CHECK_EQUAL( R4.V2T()[2].second, 0 );  
    BOOST_CHECK_EQUAL( R4.V2T()[3].second, 0 );
    
    BOOST_CHECK_EQUAL( R4.offset().size(), 6);
    
    BOOST_CHECK_EQUAL( R4.offset()[0],  0);
    BOOST_CHECK_EQUAL( R4.offset()[1],  1);
    BOOST_CHECK_EQUAL( R4.offset()[2],  2);
    BOOST_CHECK_EQUAL( R4.offset()[3],  2);
    BOOST_CHECK_EQUAL( R4.offset()[4],  3);
    BOOST_CHECK_EQUAL( R4.offset()[5],  4); 
  }  
}


BOOST_AUTO_TEST_SUITE_END();
