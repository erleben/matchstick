#include <mesh_array.h>
#include "mesh_array_data_path.h"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(MESH_ARRAY);

BOOST_AUTO_TEST_CASE(tetgen_test)
{
  mesh_array::T3Mesh surface;
  mesh_array::t3_vertex_float_attribute inX;
  mesh_array::t3_vertex_float_attribute inY;
  mesh_array::t3_vertex_float_attribute inZ;
  
  mesh_array::read_obj(mesh_array::data_path + "objs/box.obj",surface, inX, inY, inZ);

  
  mesh_array::T4Mesh volume;
  mesh_array::t4_vertex_float_attribute outX;
  mesh_array::t4_vertex_float_attribute outY;
  mesh_array::t4_vertex_float_attribute outZ;
  
  mesh_array::tetgen( surface, inX, inY, inZ, volume, outX, outY, outZ);
  
  bool const is_pos = is_positive_orientation( volume, outX, outY, outZ );
  
  BOOST_CHECK(is_pos);
  
  mesh_array::write_matlab("test.m", volume, outX, outY, outZ);
}

BOOST_AUTO_TEST_SUITE_END();
