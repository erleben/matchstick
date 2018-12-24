#include <mesh_array.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(MESH_ARRAY);

BOOST_AUTO_TEST_CASE(positive_orientation_test)
{
  mesh_array::T4Mesh  M;

  mesh_array::VertexAttribute<float, mesh_array::T4Mesh >  X;
  mesh_array::VertexAttribute<float, mesh_array::T4Mesh >  Y;
  mesh_array::VertexAttribute<float, mesh_array::T4Mesh >  Z;
  
  
  M.set_capacity(4u,1u);
  
  mesh_array::Vertex const vi = M.push_vertex();
  mesh_array::Vertex const vj = M.push_vertex();
  mesh_array::Vertex const vk = M.push_vertex();
  mesh_array::Vertex const vm = M.push_vertex();
  
  mesh_array::Tetrahedron const t = M.push_tetrahedron(vi,vj,vk,vm);
  
  X.bind(M);
  Y.bind(M);
  Z.bind(M);
  
  X(vi) = 0.0f;  Y(vi) = 0.0f;  Z(vi) = 0.0f;
  X(vj) = 1.0f;  Y(vj) = 0.0f;  Z(vj) = 0.0f;
  X(vk) = 0.0f;  Y(vk) = 1.0f;  Z(vk) = 0.0f;
  X(vm) = 0.0f;  Y(vm) = 0.0f;  Z(vm) = 1.0f;
  
  bool const is_pos = mesh_array::is_positive_orientation( M, X, Y, Z);
  
  BOOST_CHECK( is_pos );
  
  
}

BOOST_AUTO_TEST_CASE(negative_orientation_test)
{
  mesh_array::T4Mesh  M;
  
  mesh_array::VertexAttribute<float, mesh_array::T4Mesh >  X;
  mesh_array::VertexAttribute<float, mesh_array::T4Mesh >  Y;
  mesh_array::VertexAttribute<float, mesh_array::T4Mesh >  Z;
  
  
  M.set_capacity(4u,1u);
  
  mesh_array::Vertex const vi = M.push_vertex();
  mesh_array::Vertex const vj = M.push_vertex();
  mesh_array::Vertex const vk = M.push_vertex();
  mesh_array::Vertex const vm = M.push_vertex();
  
  mesh_array::Tetrahedron const t = M.push_tetrahedron(vi,vj,vk,vm);
  
  X.bind(M);
  Y.bind(M);
  Z.bind(M);
  
  X(vi) = 0.0f;  Y(vi) = 0.0f;  Z(vi) = 0.0f;
  X(vj) = 1.0f;  Y(vj) = 0.0f;  Z(vj) = 0.0f;
  X(vk) = 0.0f;  Y(vk) = 1.0f;  Z(vk) = 0.0f;
  X(vm) = 0.0f;  Y(vm) = 0.0f;  Z(vm) = -1.0f;
  
  bool const is_pos = mesh_array::is_positive_orientation( M, X, Y, Z);
  
  BOOST_CHECK( !is_pos );
  
  
}

BOOST_AUTO_TEST_SUITE_END();
