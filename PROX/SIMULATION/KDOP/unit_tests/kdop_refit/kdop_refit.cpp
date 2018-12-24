#include <kdop_refit_tree.h>
#include <kdop_mesh_reorder.h>
#include <kdop_make_tree.h>
#include <mesh_array.h>
#include <tiny.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

#include <vector>


BOOST_AUTO_TEST_SUITE(kdop);

BOOST_AUTO_TEST_CASE(kdop_refit)
{
  typedef tiny::MathTypes<float> MT;
  typedef MT::vector3_type       V;
  typedef MT::real_type          T;

  mesh_array::T3Mesh surface;
  mesh_array::VertexAttribute<T,mesh_array::T3Mesh> sX;
  mesh_array::VertexAttribute<T,mesh_array::T3Mesh> sY;
  mesh_array::VertexAttribute<T,mesh_array::T3Mesh> sZ;
  
  T      const width   = 1.0f;
  T      const height   = 1.0f;
  T      const depth   = 2.0f;
  mesh_array::make_box<MT>( width, height, depth, surface, sX, sY, sZ); 
  
  mesh_array::T4Mesh mesh_in;  
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> X_in;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Y_in;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Z_in;
  mesh_array::tetgen(surface, sX, sY, sZ, mesh_in, X_in, Y_in, Z_in);
  
  mesh_array::T4Mesh mesh_out;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> X_out;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Y_out;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Z_out;
  kdop::mesh_reorder( mesh_in, X_in, Y_in, Z_in, mesh_out, X_out, Y_out, Z_out );
  
  kdop::Tree<T,8> tree = kdop::make_tree<V,8,T>( 32000, mesh_out, X_out, Y_out, Z_out );
    
  kdop::refit_tree<V,8,T>(tree, mesh_out, X_out, Y_out, Z_out );
}

BOOST_AUTO_TEST_SUITE_END();
