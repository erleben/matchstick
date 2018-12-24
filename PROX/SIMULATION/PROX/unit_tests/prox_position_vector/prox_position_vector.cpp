#include <sparse.h>
#include <sparse_fill.h>

#include <narrow.h>

#include <prox_rigid_body.h>
#include <prox_get_position_vector.h>
#include <prox_set_position_vector.h>

#include <prox_math_policy.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/test_tools.hpp>

typedef prox::MathPolicy<float>      math_policy;
typedef math_policy::tiny_types      tiny_types;
typedef math_policy::real_type       real_type;
typedef math_policy::vector3_type    vector3_type;
typedef math_policy::vector7_type    vector7_type;
typedef math_policy::matrix3x3_type  matrix3x3_type;


BOOST_AUTO_TEST_SUITE(position_vector);
BOOST_AUTO_TEST_CASE(get_position_vector_test)
{
  // Initialize geometry
  narrow::System<tiny_types> narrow;

  size_t gid = narrow.create_geometry();

  // Initialize objects
  std::vector< prox::RigidBody< math_policy > > bodies;
  bodies.resize( 4u );

  bodies[0].set_geometry_idx( gid );
  bodies[0].set_position( vector3_type::make( 5.0,7.0,0.0) );

  bodies[1].set_geometry_idx( gid );
  bodies[1].set_position( vector3_type::make( 0.0,1.0,0.0) );

  bodies[2].set_geometry_idx( gid );
  bodies[2].set_position( vector3_type::make( 0.0,3.0,0.0) );

  bodies[3].set_geometry_idx( gid );
  bodies[3].set_position( vector3_type::make( 2.0,5.0,0.0) );

  vector7_type q;

  prox::get_position_vector(bodies.begin(),bodies.end(),q, math_policy());

  BOOST_CHECK_EQUAL(q(0)(0), 5);
  BOOST_CHECK_EQUAL(q(0)(1), 7);
  BOOST_CHECK_EQUAL(q(0)(2), 0);
  
  BOOST_CHECK_EQUAL(q(1)(0), 0);
  BOOST_CHECK_EQUAL(q(1)(1), 1);
  BOOST_CHECK_EQUAL(q(1)(2), 0);

  BOOST_CHECK_EQUAL(q(2)(0), 0);
  BOOST_CHECK_EQUAL(q(2)(1), 3);
  BOOST_CHECK_EQUAL(q(2)(2), 0);

  BOOST_CHECK_EQUAL(q(3)(0), 2);
  BOOST_CHECK_EQUAL(q(3)(1), 5);
  BOOST_CHECK_EQUAL(q(3)(2), 0);

  
  bodies[2].set_position( vector3_type::make( 8.0,3.0,0.5) );

  prox::get_position_vector(bodies.begin(),bodies.end(),q, math_policy());

  BOOST_CHECK_EQUAL(q(0)(0), 5);
  BOOST_CHECK_EQUAL(q(0)(1), 7);
  BOOST_CHECK_EQUAL(q(0)(2), 0);
  
  BOOST_CHECK_EQUAL(q(1)(0), 0);
  BOOST_CHECK_EQUAL(q(1)(1), 1);
  BOOST_CHECK_EQUAL(q(1)(2), 0);

  BOOST_CHECK_EQUAL(q(2)(0), 8);
  BOOST_CHECK_EQUAL(q(2)(1), 3);
  BOOST_CHECK_EQUAL(q(2)(2), 0.5);

  BOOST_CHECK_EQUAL(q(3)(0), 2);
  BOOST_CHECK_EQUAL(q(3)(1), 5);
  BOOST_CHECK_EQUAL(q(3)(2), 0);
}

BOOST_AUTO_TEST_CASE(set_position_vector_test)
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

  vector7_type q(4);

  sparse::fill(q(0));// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
  sparse::fill(q(1));// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
  sparse::fill(q(2));// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
  sparse::fill(q(3));// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'

  prox::set_position_vector(bodies.begin(),bodies.end(),q, math_policy());

  BOOST_CHECK_EQUAL(bodies[0].get_position()(0), 0);
  BOOST_CHECK_EQUAL(bodies[0].get_position()(1), 1);
  BOOST_CHECK_EQUAL(bodies[0].get_position()(2), 2);
  
  BOOST_CHECK_EQUAL(bodies[1].get_position()(0), 0);
  BOOST_CHECK_EQUAL(bodies[1].get_position()(1), 1);
  BOOST_CHECK_EQUAL(bodies[1].get_position()(2), 2);

  BOOST_CHECK_EQUAL(bodies[2].get_position()(0), 0);
  BOOST_CHECK_EQUAL(bodies[2].get_position()(1), 1);
  BOOST_CHECK_EQUAL(bodies[2].get_position()(2), 2);

  BOOST_CHECK_EQUAL(bodies[3].get_position()(0), 0);
  BOOST_CHECK_EQUAL(bodies[3].get_position()(1), 1);
  BOOST_CHECK_EQUAL(bodies[3].get_position()(2), 2);

  sparse::fill(q(2),4);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
  sparse::fill(q(3),7);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'

  prox::set_position_vector(bodies.begin(),bodies.end(),q, math_policy());

  BOOST_CHECK_EQUAL(bodies[0].get_position()(0), 0);
  BOOST_CHECK_EQUAL(bodies[0].get_position()(1), 1);
  BOOST_CHECK_EQUAL(bodies[0].get_position()(2), 2);
  
  BOOST_CHECK_EQUAL(bodies[1].get_position()(0), 0);
  BOOST_CHECK_EQUAL(bodies[1].get_position()(1), 1);
  BOOST_CHECK_EQUAL(bodies[1].get_position()(2), 2);

  BOOST_CHECK_EQUAL(bodies[2].get_position()(0), 4);
  BOOST_CHECK_EQUAL(bodies[2].get_position()(1), 5);
  BOOST_CHECK_EQUAL(bodies[2].get_position()(2), 6);

  BOOST_CHECK_EQUAL(bodies[3].get_position()(0), 7);
  BOOST_CHECK_EQUAL(bodies[3].get_position()(1), 8);
  BOOST_CHECK_EQUAL(bodies[3].get_position()(2), 9);
}
BOOST_AUTO_TEST_SUITE_END();
