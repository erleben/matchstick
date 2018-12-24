#include <sparse.h>
#include <sparse_fill.h>

#include <narrow.h>

#include <prox_rigid_body.h>
#include <prox_get_velocity_vector.h>
#include <prox_set_velocity_vector.h>

#include <prox_math_policy.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/test_tools.hpp>

typedef prox::MathPolicy<float>      math_policy;
typedef math_policy::tiny_types      tiny_types;
typedef math_policy::real_type       real_type;
typedef math_policy::vector3_type    vector3_type;
typedef math_policy::vector6_type    vector6_type;
typedef math_policy::matrix3x3_type  matrix3x3_type;


BOOST_AUTO_TEST_SUITE(velocity_vector);
BOOST_AUTO_TEST_CASE(get_velocity_vector_test)
{

  // Initialize geometry
  narrow::System<tiny_types> narrow;

  size_t gid = narrow.create_geometry();

  // Initialize objects
  std::vector< prox::RigidBody< math_policy > > bodies;
  bodies.resize( 4u );

  bodies[0].set_geometry_idx( gid );
  bodies[0].set_velocity( vector3_type::make( 5.0,7.0,0.0) );

  bodies[1].set_geometry_idx( gid );
  bodies[1].set_velocity( vector3_type::make( 0.0,1.0,0.0) );

  bodies[2].set_geometry_idx( gid );
  bodies[2].set_velocity( vector3_type::make( 0.0,3.0,0.0) );

  bodies[3].set_geometry_idx( gid );
  bodies[3].set_velocity( vector3_type::make( 2.0,5.0,0.0) );

  vector6_type u;

  prox::get_velocity_vector(bodies.begin(),bodies.end(),u, math_policy());

  BOOST_CHECK_EQUAL(u(0)(0), 5);
  BOOST_CHECK_EQUAL(u(0)(1), 7);
  BOOST_CHECK_EQUAL(u(0)(2), 0);
  
  BOOST_CHECK_EQUAL(u(1)(0), 0);
  BOOST_CHECK_EQUAL(u(1)(1), 1);
  BOOST_CHECK_EQUAL(u(1)(2), 0);

  BOOST_CHECK_EQUAL(u(2)(0), 0);
  BOOST_CHECK_EQUAL(u(2)(1), 3);
  BOOST_CHECK_EQUAL(u(2)(2), 0);

  BOOST_CHECK_EQUAL(u(3)(0), 2);
  BOOST_CHECK_EQUAL(u(3)(1), 5);
  BOOST_CHECK_EQUAL(u(3)(2), 0);

  
  bodies[2].set_velocity( vector3_type::make( 8.0,3.0,0.5) );

  prox::get_velocity_vector(bodies.begin(),bodies.end(),u, math_policy());

  BOOST_CHECK_EQUAL(u(0)(0), 5);
  BOOST_CHECK_EQUAL(u(0)(1), 7);
  BOOST_CHECK_EQUAL(u(0)(2), 0);
  
  BOOST_CHECK_EQUAL(u(1)(0), 0);
  BOOST_CHECK_EQUAL(u(1)(1), 1);
  BOOST_CHECK_EQUAL(u(1)(2), 0);

  BOOST_CHECK_EQUAL(u(2)(0), 8);
  BOOST_CHECK_EQUAL(u(2)(1), 3);
  BOOST_CHECK_EQUAL(u(2)(2), 0.5);

  BOOST_CHECK_EQUAL(u(3)(0), 2);
  BOOST_CHECK_EQUAL(u(3)(1), 5);
  BOOST_CHECK_EQUAL(u(3)(2), 0);
}

BOOST_AUTO_TEST_CASE(set_velocity_vector_test)
{

  // Initialize geometry
  narrow::System<tiny_types> narrow;

  size_t gid = narrow.create_geometry();

  // Initialize objects
  std::vector< prox::RigidBody< math_policy > > bodies;
  bodies.resize( 4u );

  bodies[0].set_geometry_idx( gid );
  bodies[1].set_geometry_idx( gid );
  bodies[2].set_geometry_idx( gid );
  bodies[3].set_geometry_idx( gid );

  vector6_type u(4);

  sparse::fill(u(0));// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
  sparse::fill(u(1));// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
  sparse::fill(u(2));// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
  sparse::fill(u(3));// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'

  prox::set_velocity_vector(bodies.begin(),bodies.end(),u, math_policy());

  BOOST_CHECK_EQUAL(bodies[0].get_velocity()(0), 0);
  BOOST_CHECK_EQUAL(bodies[0].get_velocity()(1), 1);
  BOOST_CHECK_EQUAL(bodies[0].get_velocity()(2), 2);
  
  BOOST_CHECK_EQUAL(bodies[1].get_velocity()(0), 0);
  BOOST_CHECK_EQUAL(bodies[1].get_velocity()(1), 1);
  BOOST_CHECK_EQUAL(bodies[1].get_velocity()(2), 2);

  BOOST_CHECK_EQUAL(bodies[2].get_velocity()(0), 0);
  BOOST_CHECK_EQUAL(bodies[2].get_velocity()(1), 1);
  BOOST_CHECK_EQUAL(bodies[2].get_velocity()(2), 2);

  BOOST_CHECK_EQUAL(bodies[3].get_velocity()(0), 0);
  BOOST_CHECK_EQUAL(bodies[3].get_velocity()(1), 1);
  BOOST_CHECK_EQUAL(bodies[3].get_velocity()(2), 2);

  sparse::fill(u(2),4);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
  sparse::fill(u(3),7);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'

  prox::set_velocity_vector(bodies.begin(),bodies.end(),u, math_policy());

  BOOST_CHECK_EQUAL(bodies[0].get_velocity()(0), 0);
  BOOST_CHECK_EQUAL(bodies[0].get_velocity()(1), 1);
  BOOST_CHECK_EQUAL(bodies[0].get_velocity()(2), 2);
  
  BOOST_CHECK_EQUAL(bodies[1].get_velocity()(0), 0);
  BOOST_CHECK_EQUAL(bodies[1].get_velocity()(1), 1);
  BOOST_CHECK_EQUAL(bodies[1].get_velocity()(2), 2);

  BOOST_CHECK_EQUAL(bodies[2].get_velocity()(0), 4);
  BOOST_CHECK_EQUAL(bodies[2].get_velocity()(1), 5);
  BOOST_CHECK_EQUAL(bodies[2].get_velocity()(2), 6);

  BOOST_CHECK_EQUAL(bodies[3].get_velocity()(0), 7);
  BOOST_CHECK_EQUAL(bodies[3].get_velocity()(1), 8);
  BOOST_CHECK_EQUAL(bodies[3].get_velocity()(2), 9);
}
BOOST_AUTO_TEST_SUITE_END();
