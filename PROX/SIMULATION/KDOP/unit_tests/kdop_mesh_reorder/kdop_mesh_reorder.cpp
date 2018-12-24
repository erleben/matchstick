#include <kdop_mesh_reorder.h>
#include <mesh_array.h>
#include <tiny.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

#include <vector>

BOOST_AUTO_TEST_SUITE(kdop);

BOOST_AUTO_TEST_CASE(kdop_mesh_reorder_sphere)
{
  typedef tiny::MathTypes<float> MT;
  typedef MT::real_type          T;
  
  mesh_array::T3Mesh surf;
  mesh_array::VertexAttribute<T,mesh_array::T3Mesh> sX;
  mesh_array::VertexAttribute<T,mesh_array::T3Mesh> sY;
  mesh_array::VertexAttribute<T,mesh_array::T3Mesh> sZ;
  
  size_t const slices = 12u;
  size_t const segments = 12u;
  T const radius = 1.0f;
  mesh_array::make_sphere<MT>(radius, slices, segments, surf, sX, sY, sZ);
  
  mesh_array::T4Mesh mesh_in;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Xin;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Yin;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Zin;
  mesh_array::tetgen(surf, sX, sY, sZ, mesh_in, Xin, Yin, Zin);
  
  mesh_array::T4Mesh mesh_out;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Xout;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Yout;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Zout;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> dists;
  
  kdop::mesh_reorder( mesh_in, Xin, Yin, Zin, mesh_out, Xout, Yout, Zout );
  
}
BOOST_AUTO_TEST_CASE(kdop_mesh_reorder_box)
{
  typedef tiny::MathTypes<float> MT;
  typedef MT::real_type          T;
  
  mesh_array::T3Mesh surf;
  mesh_array::VertexAttribute<T,mesh_array::T3Mesh> sX;
  mesh_array::VertexAttribute<T,mesh_array::T3Mesh> sY;
  mesh_array::VertexAttribute<T,mesh_array::T3Mesh> sZ;
  
  T      const width   = 1.0f;
  T      const height  = 1.0f;
  T      const depth   = 1.0f;
  mesh_array::make_box<MT>( width, height, depth, surf, sX, sY, sZ);
  
  mesh_array::T4Mesh mesh_in;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Xin;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Yin;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Zin;
  mesh_array::tetgen(surf, sX, sY, sZ, mesh_in, Xin, Yin, Zin);
  
  mesh_array::T4Mesh mesh_out;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Xout;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Yout;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Zout;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> dists;
  
  kdop::mesh_reorder( mesh_in, Xin, Yin, Zin, mesh_out, Xout, Yout, Zout );
  
}
BOOST_AUTO_TEST_CASE(kdop_mesh_reorder_cone)
{
  typedef tiny::MathTypes<float> MT;
  typedef MT::real_type          T;
  
  mesh_array::T3Mesh surf;
  mesh_array::VertexAttribute<T,mesh_array::T3Mesh> sX;
  mesh_array::VertexAttribute<T,mesh_array::T3Mesh> sY;
  mesh_array::VertexAttribute<T,mesh_array::T3Mesh> sZ;
  
  T      const height   = 1.0f;
  T      const radius   = 1.0f;
  size_t const slices   = 12u;
  
  mesh_array::make_cone<MT>(radius, height, slices, surf, sX, sY, sZ);
  
  mesh_array::T4Mesh mesh_in;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Xin;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Yin;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Zin;
  mesh_array::tetgen(surf, sX, sY, sZ, mesh_in, Xin, Yin, Zin);
  
  mesh_array::T4Mesh mesh_out;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Xout;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Yout;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Zout;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> dists;
  
  kdop::mesh_reorder( mesh_in, Xin, Yin, Zin, mesh_out, Xout, Yout, Zout );
  
}
BOOST_AUTO_TEST_CASE(kdop_mesh_reorder_capsule)
{
  typedef tiny::MathTypes<float> MT;
  typedef MT::real_type          T;
  
  mesh_array::T3Mesh surf;
  mesh_array::VertexAttribute<T,mesh_array::T3Mesh> sX;
  mesh_array::VertexAttribute<T,mesh_array::T3Mesh> sY;
  mesh_array::VertexAttribute<T,mesh_array::T3Mesh> sZ;
  
  T      const height   = 1.0f;
  T      const radius   = 1.0f;
  size_t const slices   = 12u;
  size_t const segments = 12u;

  mesh_array::make_capsule<MT>(radius, height, slices, segments, surf, sX, sY, sZ);
  
  mesh_array::T4Mesh mesh_in;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Xin;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Yin;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Zin;
  mesh_array::tetgen(surf, sX, sY, sZ, mesh_in, Xin, Yin, Zin);
  
  mesh_array::T4Mesh mesh_out;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Xout;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Yout;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Zout;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> dists;
  
  kdop::mesh_reorder( mesh_in, Xin, Yin, Zin, mesh_out, Xout, Yout, Zout );
  
}


BOOST_AUTO_TEST_SUITE_END();
