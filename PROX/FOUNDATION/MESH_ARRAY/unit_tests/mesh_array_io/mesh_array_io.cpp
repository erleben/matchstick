#include <mesh_array.h>
#include "mesh_array_data_path.h"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(MESH_ARRAY);

void test_correct_tetgen(  mesh_array::T3Mesh const & M
                      , mesh_array::t3_vertex_float_attribute const & X
                      , mesh_array::t3_vertex_float_attribute const & Y
                      , mesh_array::t3_vertex_float_attribute const & Z
                      )
{
  BOOST_CHECK_EQUAL(X.size(),  8);
  BOOST_CHECK_EQUAL(Y.size(),  8);
  BOOST_CHECK_EQUAL(Z.size(),  8);
  BOOST_CHECK_EQUAL(M.triangle_size(),  7);
  BOOST_CHECK_EQUAL(M.vertex_size(),  8);
  
  BOOST_CHECK_EQUAL(M.triangle(0).i(),  0); 
  BOOST_CHECK_EQUAL(M.triangle(0).j(),  2);
  BOOST_CHECK_EQUAL(M.triangle(0).k(),  1);
  
  BOOST_CHECK_EQUAL(M.triangle(1).i(),  3);
  BOOST_CHECK_EQUAL(M.triangle(1).j(),  4);
  BOOST_CHECK_EQUAL(M.triangle(1).k(),  2);
  
  BOOST_CHECK_EQUAL(M.triangle(2).i(),  2);
  BOOST_CHECK_EQUAL(M.triangle(2).j(),  4);
  BOOST_CHECK_EQUAL(M.triangle(2).k(),  1);
  
  BOOST_CHECK_EQUAL(M.triangle(3).i(),  4);
  BOOST_CHECK_EQUAL(M.triangle(3).j(),  5);
  BOOST_CHECK_EQUAL(M.triangle(3).k(),  1);
  
  BOOST_CHECK_EQUAL(M.triangle(4).i(),  1);
  BOOST_CHECK_EQUAL(M.triangle(4).j(),  5);
  BOOST_CHECK_EQUAL(M.triangle(4).k(),  6);
  
  BOOST_CHECK_EQUAL(M.triangle(5).i(),  1);
  BOOST_CHECK_EQUAL(M.triangle(5).j(),  6);
  BOOST_CHECK_EQUAL(M.triangle(5).k(),  7);
  
  BOOST_CHECK_EQUAL(M.triangle(6).i(),  1);
  BOOST_CHECK_EQUAL(M.triangle(6).j(),  7);
  BOOST_CHECK_EQUAL(M.triangle(6).k(),  0);
  
  BOOST_CHECK_CLOSE(X(0), 2, 0.001);
  BOOST_CHECK_CLOSE(X(1), 3, 0.001);
  BOOST_CHECK_CLOSE(X(2), 1, 0.001);
  BOOST_CHECK_CLOSE(X(3), 0, 0.001);
  BOOST_CHECK_CLOSE(X(4), 2, 0.001);
  BOOST_CHECK_CLOSE(X(5), 4, 0.001);
  BOOST_CHECK_CLOSE(X(6), 5, 0.001);
  BOOST_CHECK_CLOSE(X(7), 4, 0.001);
  
  BOOST_CHECK_CLOSE(Y(0), 4, 0.001);
  BOOST_CHECK_CLOSE(Y(1), 2, 0.001);
  BOOST_CHECK_CLOSE(Y(2), 2, 0.001);
  BOOST_CHECK_CLOSE(Y(3), 0, 0.001);
  BOOST_CHECK_CLOSE(Y(4), 0, 0.001);
  BOOST_CHECK_CLOSE(Y(5), 0, 0.001);
  BOOST_CHECK_CLOSE(Y(6), 2, 0.001);
  BOOST_CHECK_CLOSE(Y(7), 4, 0.001);
  
  BOOST_CHECK_CLOSE(Z(0), 0, 0.001);
  BOOST_CHECK_CLOSE(Z(1), 0, 0.001);
  BOOST_CHECK_CLOSE(Z(2), 0, 0.001);
  BOOST_CHECK_CLOSE(Z(3), 0, 0.001);
  BOOST_CHECK_CLOSE(Z(4), 0, 0.001);
  BOOST_CHECK_CLOSE(Z(5), 0, 0.001);
  BOOST_CHECK_CLOSE(Z(6), 0, 0.001);
  BOOST_CHECK_CLOSE(Z(7), 0, 0.001);
}


BOOST_AUTO_TEST_CASE(io_t3_tetgen_test)
{
  mesh_array::T3Mesh  M1;  
  mesh_array::t3_vertex_float_attribute X1(M1);
  mesh_array::t3_vertex_float_attribute Y1(M1);
  mesh_array::t3_vertex_float_attribute Z1(M1);
  
  mesh_array::read_tetgen(mesh_array::data_path + "tetgen/tri", M1, X1, Y1, Z1);

  test_correct_tetgen(M1, X1, Y1, Z1);

  mesh_array::write_tetgen("test", M1, X1, Y1, Z1);

  mesh_array::T3Mesh  M2;  
  mesh_array::t3_vertex_float_attribute X2(M2);
  mesh_array::t3_vertex_float_attribute Y2(M2);
  mesh_array::t3_vertex_float_attribute Z2(M2);
  
  mesh_array::read_tetgen("test", M2, X2, Y2, Z2);
  
  test_correct_tetgen(M2, X2, Y2, Z2);
}


void test_correct_tetgen(  mesh_array::T4Mesh const & M
                         , mesh_array::t4_vertex_float_attribute const & X
                         , mesh_array::t4_vertex_float_attribute const & Y
                         , mesh_array::t4_vertex_float_attribute const & Z
                         )
{
  BOOST_CHECK_EQUAL(X.size(),  8);
  BOOST_CHECK_EQUAL(Y.size(),  8);
  BOOST_CHECK_EQUAL(Z.size(),  8);
  BOOST_CHECK_EQUAL(M.tetrahedron_size(),  6);
  BOOST_CHECK_EQUAL(M.vertex_size(),  8);
  
  BOOST_CHECK_EQUAL(M.tetrahedron(0).i(),  0); 
  BOOST_CHECK_EQUAL(M.tetrahedron(0).j(),  1);
  BOOST_CHECK_EQUAL(M.tetrahedron(0).k(),  2);
  BOOST_CHECK_EQUAL(M.tetrahedron(0).m(),  3);
  
  BOOST_CHECK_EQUAL(M.tetrahedron(1).i(),  0);
  BOOST_CHECK_EQUAL(M.tetrahedron(1).j(),  3);
  BOOST_CHECK_EQUAL(M.tetrahedron(1).k(),  2);
  BOOST_CHECK_EQUAL(M.tetrahedron(1).m(),  4);
  
  BOOST_CHECK_EQUAL(M.tetrahedron(2).i(),  0);
  BOOST_CHECK_EQUAL(M.tetrahedron(2).j(),  3);
  BOOST_CHECK_EQUAL(M.tetrahedron(2).k(),  4);
  BOOST_CHECK_EQUAL(M.tetrahedron(2).m(),  5);
  
  BOOST_CHECK_EQUAL(M.tetrahedron(3).i(),  6);
  BOOST_CHECK_EQUAL(M.tetrahedron(3).j(),  0);
  BOOST_CHECK_EQUAL(M.tetrahedron(3).k(),  5);
  BOOST_CHECK_EQUAL(M.tetrahedron(3).m(),  3);
  
  BOOST_CHECK_EQUAL(M.tetrahedron(4).i(),  7);
  BOOST_CHECK_EQUAL(M.tetrahedron(4).j(),  0);
  BOOST_CHECK_EQUAL(M.tetrahedron(4).k(),  3);
  BOOST_CHECK_EQUAL(M.tetrahedron(4).m(),  1);
  
  BOOST_CHECK_EQUAL(M.tetrahedron(5).i(),  7);
  BOOST_CHECK_EQUAL(M.tetrahedron(5).j(),  6);
  BOOST_CHECK_EQUAL(M.tetrahedron(5).k(),  3);
  BOOST_CHECK_EQUAL(M.tetrahedron(5).m(),  0);
  
  BOOST_CHECK_CLOSE(X(0), -0.999978, 0.001); 
  BOOST_CHECK_CLOSE(X(1), -1.00004,  0.001);
  BOOST_CHECK_CLOSE(X(2), -0.999962, 0.001);
  BOOST_CHECK_CLOSE(X(3),  0.999975, 0.001);
  BOOST_CHECK_CLOSE(X(4), -1.00004,  0.001);
  BOOST_CHECK_CLOSE(X(5),  0.99991,  0.001);
  BOOST_CHECK_CLOSE(X(6),  1.00007,  0.001);
  BOOST_CHECK_CLOSE(X(7),  1.00007,  0.001);
  
  BOOST_CHECK_CLOSE(Y(0),  -1.00002,  0.001); 
  BOOST_CHECK_CLOSE(Y(1),  -0.999932, 0.001);
  BOOST_CHECK_CLOSE(Y(2),   1.00007,  0.001);
  BOOST_CHECK_CLOSE(Y(3),   0.999919, 0.001);
  BOOST_CHECK_CLOSE(Y(4),   0.999955, 0.001);
  BOOST_CHECK_CLOSE(Y(5),   0.999902, 0.001);
  BOOST_CHECK_CLOSE(Y(6),  -0.999901, 0.001);
  BOOST_CHECK_CLOSE(Y(7),  -1.00006,  0.001);
  
  BOOST_CHECK_CLOSE(Z(0),   0.999953, 0.001); 
  BOOST_CHECK_CLOSE(Z(1),  -1.0001,   0.001);
  BOOST_CHECK_CLOSE(Z(2),  -0.999955, 0.001);
  BOOST_CHECK_CLOSE(Z(3),  -0.999965, 0.001);
  BOOST_CHECK_CLOSE(Z(4),   1.00002,  0.001);
  BOOST_CHECK_CLOSE(Z(5),   1.00008,  0.001);
  BOOST_CHECK_CLOSE(Z(6),   1.0001,   0.001);
  BOOST_CHECK_CLOSE(Z(7),  -0.999944, 0.001);
}

BOOST_AUTO_TEST_CASE(io_t4_tetgen_test)
{
  mesh_array::T4Mesh  M1;  
  mesh_array::t4_vertex_float_attribute X1(M1);
  mesh_array::t4_vertex_float_attribute Y1(M1);
  mesh_array::t4_vertex_float_attribute Z1(M1);
  
  mesh_array::read_tetgen(mesh_array::data_path + "tetgen/tet", M1, X1, Y1, Z1);
  
  test_correct_tetgen(M1, X1, Y1, Z1);
  
  mesh_array::write_tetgen("test", M1, X1, Y1, Z1);
  
  mesh_array::T4Mesh  M2;  
  mesh_array::t4_vertex_float_attribute X2(M2);
  mesh_array::t4_vertex_float_attribute Y2(M2);
  mesh_array::t4_vertex_float_attribute Z2(M2);
  
  mesh_array::read_tetgen("test", M2, X2, Y2, Z2);
  
  test_correct_tetgen(M2, X2, Y2, Z2);
}

BOOST_AUTO_TEST_CASE(io_t4_xml_test)
{
  mesh_array::T4Mesh M1;  
  mesh_array::t4_vertex_float_attribute X1(M1);
  mesh_array::t4_vertex_float_attribute Y1(M1);
  mesh_array::t4_vertex_float_attribute Z1(M1);
  
  mesh_array::read_tetgen(mesh_array::data_path + "tetgen/tet", M1, X1, Y1, Z1);

  test_correct_tetgen(M1, X1, Y1, Z1);
  
  mesh_array::write_xml("test.xml", M1, X1, Y1, Z1);
  
  mesh_array::T4Mesh M2;  
  mesh_array::t4_vertex_float_attribute X2(M2);
  mesh_array::t4_vertex_float_attribute Y2(M2);
  mesh_array::t4_vertex_float_attribute Z2(M2);
  
  mesh_array::read_xml("test.xml", M2, X2, Y2, Z2);
  
  test_correct_tetgen(M2, X2, Y2, Z2);
}


void test_correct_obj(  mesh_array::T3Mesh const & M
                      , mesh_array::t3_vertex_float_attribute const & X
                      , mesh_array::t3_vertex_float_attribute const & Y
                      , mesh_array::t3_vertex_float_attribute const & Z
)
{
  BOOST_CHECK_EQUAL(X.size(),  8);
  BOOST_CHECK_EQUAL(Y.size(),  8);
  BOOST_CHECK_EQUAL(Z.size(),  8);
  BOOST_CHECK_EQUAL(M.triangle_size(),  12);
  BOOST_CHECK_EQUAL(M.vertex_size(),  8);
  
  //  f 3/3/3 1/1/1 2/2/2
  BOOST_CHECK_EQUAL(M.triangle(0).i(),  2); 
  BOOST_CHECK_EQUAL(M.triangle(0).j(),  0);
  BOOST_CHECK_EQUAL(M.triangle(0).k(),  1);
  
  //  f 4/4/4 2/2/2 1/1/1
  BOOST_CHECK_EQUAL(M.triangle(1).i(),  3);
  BOOST_CHECK_EQUAL(M.triangle(1).j(),  1);
  BOOST_CHECK_EQUAL(M.triangle(1).k(),  0);
  
  //  f 7/7/7 5/5/5 6/6/6
  BOOST_CHECK_EQUAL(M.triangle(2).i(),  6);
  BOOST_CHECK_EQUAL(M.triangle(2).j(),  4);
  BOOST_CHECK_EQUAL(M.triangle(2).k(),  5);
  
  //  f 8/8/8 6/6/6 5/5/5
  BOOST_CHECK_EQUAL(M.triangle(3).i(),  7);
  BOOST_CHECK_EQUAL(M.triangle(3).j(),  5);
  BOOST_CHECK_EQUAL(M.triangle(3).k(),  4);
  
  //  f 4/4/4 7/7/7 2/2/2
  BOOST_CHECK_EQUAL(M.triangle(4).i(),  3);
  BOOST_CHECK_EQUAL(M.triangle(4).j(),  6);
  BOOST_CHECK_EQUAL(M.triangle(4).k(),  1);
  
  //  f 6/6/6 2/2/2 7/7/7
  BOOST_CHECK_EQUAL(M.triangle(5).i(),  5);
  BOOST_CHECK_EQUAL(M.triangle(5).j(),  1);
  BOOST_CHECK_EQUAL(M.triangle(5).k(),  6);
  
  //  f 1/1/1 5/5/5 4/4/4
  BOOST_CHECK_EQUAL(M.triangle(6).i(),  0);
  BOOST_CHECK_EQUAL(M.triangle(6).j(),  4);
  BOOST_CHECK_EQUAL(M.triangle(6).k(),  3);
  
  //  f 7/7/7 4/4/4 5/5/5
  BOOST_CHECK_EQUAL(M.triangle(7).i(),  6);
  BOOST_CHECK_EQUAL(M.triangle(7).j(),  3);
  BOOST_CHECK_EQUAL(M.triangle(7).k(),  4);
  
  //  f 3/3/3 8/8/8 1/1/1
  BOOST_CHECK_EQUAL(M.triangle(8).i(),  2);
  BOOST_CHECK_EQUAL(M.triangle(8).j(),  7);
  BOOST_CHECK_EQUAL(M.triangle(8).k(),  0);
  
  //  f 5/5/5 1/1/1 8/8/8
  BOOST_CHECK_EQUAL(M.triangle(9).i(),  4);
  BOOST_CHECK_EQUAL(M.triangle(9).j(),  0);
  BOOST_CHECK_EQUAL(M.triangle(9).k(),  7);
  
  //  f 2/2/2 6/6/6 3/3/3
  BOOST_CHECK_EQUAL(M.triangle(10).i(),  1);
  BOOST_CHECK_EQUAL(M.triangle(10).j(),  5);
  BOOST_CHECK_EQUAL(M.triangle(10).k(),  2);
  
  //  f 8/8/8 3/3/3 6/6/6
  BOOST_CHECK_EQUAL(M.triangle(11).i(),  7);
  BOOST_CHECK_EQUAL(M.triangle(11).j(),  2);
  BOOST_CHECK_EQUAL(M.triangle(11).k(),  5);
  
  //  v  0.5  0.5 -0.5
  //  v -0.5 -0.5 -0.5
  //  v -0.5  0.5 -0.5
  //  v  0.5 -0.5 -0.5
  //  v  0.5  0.5  0.5
  //  v -0.5 -0.5  0.5
  //  v  0.5 -0.5  0.5
  //  v -0.5  0.5  0.5
  
  BOOST_CHECK_CLOSE(X(0),  0.5, 0.001);
  BOOST_CHECK_CLOSE(X(1), -0.5, 0.001);
  BOOST_CHECK_CLOSE(X(2), -0.5, 0.001);
  BOOST_CHECK_CLOSE(X(3),  0.5, 0.001);
  BOOST_CHECK_CLOSE(X(4),  0.5, 0.001);
  BOOST_CHECK_CLOSE(X(5), -0.5, 0.001);
  BOOST_CHECK_CLOSE(X(6),  0.5, 0.001);
  BOOST_CHECK_CLOSE(X(7), -0.5, 0.001);
  
  BOOST_CHECK_CLOSE(Y(0),  0.5, 0.001);
  BOOST_CHECK_CLOSE(Y(1), -0.5, 0.001);
  BOOST_CHECK_CLOSE(Y(2),  0.5, 0.001);
  BOOST_CHECK_CLOSE(Y(3), -0.5, 0.001);
  BOOST_CHECK_CLOSE(Y(4),  0.5, 0.001);
  BOOST_CHECK_CLOSE(Y(5), -0.5, 0.001);
  BOOST_CHECK_CLOSE(Y(6), -0.5, 0.001);
  BOOST_CHECK_CLOSE(Y(7),  0.5, 0.001);
  
  BOOST_CHECK_CLOSE(Z(0), -0.5, 0.001);
  BOOST_CHECK_CLOSE(Z(1), -0.5, 0.001);
  BOOST_CHECK_CLOSE(Z(2), -0.5, 0.001);
  BOOST_CHECK_CLOSE(Z(3), -0.5, 0.001);
  BOOST_CHECK_CLOSE(Z(4),  0.5, 0.001);
  BOOST_CHECK_CLOSE(Z(5),  0.5, 0.001);
  BOOST_CHECK_CLOSE(Z(6),  0.5, 0.001);
  BOOST_CHECK_CLOSE(Z(7),  0.5, 0.001);
}

BOOST_AUTO_TEST_CASE(io_t3_obj_test)
{
  mesh_array::T3Mesh  M1;
  mesh_array::t3_vertex_float_attribute X1(M1);
  mesh_array::t3_vertex_float_attribute Y1(M1);
  mesh_array::t3_vertex_float_attribute Z1(M1);
  
  mesh_array::read_obj(mesh_array::data_path + "objs/box.obj", M1, X1, Y1, Z1);

  test_correct_obj( M1, X1, Y1, Z1);
  
  mesh_array::write_obj("test.obj", M1, X1, Y1, Z1);
  
  mesh_array::T3Mesh  M2;  
  mesh_array::t3_vertex_float_attribute X2(M2);
  mesh_array::t3_vertex_float_attribute Y2(M2);
  mesh_array::t3_vertex_float_attribute Z2(M2);
  mesh_array::read_obj("test.obj", M2, X2, Y2, Z2);

  test_correct_obj( M2, X2, Y2, Z2);
}

BOOST_AUTO_TEST_CASE(io_t3_matlab_test)
{
  mesh_array::T3Mesh  M1;
  mesh_array::t3_vertex_float_attribute X1(M1);
  mesh_array::t3_vertex_float_attribute Y1(M1);
  mesh_array::t3_vertex_float_attribute Z1(M1);
  
  mesh_array::read_obj(mesh_array::data_path + "objs/box.obj", M1, X1, Y1, Z1);
  
  test_correct_obj( M1, X1, Y1, Z1);
  
  
  mesh_array::write_matlab("tri_test.m", M1, X1, Y1, Z1);
  
  mesh_array::T3Mesh  M2;  
  mesh_array::t3_vertex_float_attribute X2(M2);
  mesh_array::t3_vertex_float_attribute Y2(M2);
  mesh_array::t3_vertex_float_attribute Z2(M2);
  
  mesh_array::read_matlab("tri_test.m", M2, X2, Y2, Z2);
  
  test_correct_obj( M2, X2, Y2, Z2);
}

BOOST_AUTO_TEST_CASE(io_t4_matlab_test)
{
  mesh_array::T4Mesh  M1;  
  mesh_array::t4_vertex_float_attribute X1(M1);
  mesh_array::t4_vertex_float_attribute Y1(M1);
  mesh_array::t4_vertex_float_attribute Z1(M1);
  
  mesh_array::read_tetgen(mesh_array::data_path + "tetgen/tet", M1, X1, Y1, Z1);
  
  test_correct_tetgen(M1, X1, Y1, Z1);
  
  mesh_array::write_matlab("tet_test.m", M1, X1, Y1, Z1);
  
  mesh_array::T4Mesh  M2;  
  mesh_array::t4_vertex_float_attribute X2(M2);
  mesh_array::t4_vertex_float_attribute Y2(M2);
  mesh_array::t4_vertex_float_attribute Z2(M2);
  
  mesh_array::read_matlab("tet_test.m", M2, X2, Y2, Z2);
  
  test_correct_tetgen(M2, X2, Y2, Z2);
}

BOOST_AUTO_TEST_SUITE_END();
