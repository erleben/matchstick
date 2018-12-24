#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

// These rely on boost includes
#include "container_test.h"
#include "vector_test.h"
#include "matrix_test.h"
#include "quaternion_test.h"

BOOST_AUTO_TEST_SUITE(tiny_types);

BOOST_AUTO_TEST_CASE(default_types_testing)
{
  container_test::run<1,1>();
  container_test::run<2,2>();
  container_test::run<4,4>();
  container_test::run<8,8>();
  container_test::run<8,1>();
  container_test::run<1,8>();
  
  vector_test::run<1>();
  vector_test::run<2>();
  vector_test::run<3>();
  vector_test::run<4>();
  vector_test::run<6>();
  
  matrix_test::run<1,1>();
  matrix_test::run<2,2>();
  matrix_test::run<3,3>();
  matrix_test::run<4,4>();
  matrix_test::run<6,6>();
  matrix_test::run<3,4>();
  matrix_test::run<4,3>();
  
  quaternion_test::run();
}

BOOST_AUTO_TEST_SUITE_END();
