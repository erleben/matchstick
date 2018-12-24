#include <geometry.h>
#include <tiny.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

#include <vector>

typedef tiny::MathTypes<float> MT;
typedef MT::vector3_type       V;
typedef MT::real_type          T;
typedef MT::value_traits       VT;

BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(make_union_test)
{


  V                  const min_coord1 = V::make( -VT::two(), -VT::two(), -VT::two() );
  V                  const max_coord1 = V::make( -VT::one(), -VT::one(), -VT::one() );
  geometry::AABB<V>  const aabb1      = geometry::make_aabb(min_coord1, max_coord1);
  geometry::DOP<T,6> const left       = geometry::convert<6,V>(aabb1);

  V                  const min_coord2 = V::make(  VT::one(),  VT::one(),  VT::one() );
  V                  const max_coord2 = V::make(  VT::two(),  VT::two(),  VT::two() );
  geometry::AABB<V>  const aabb2      = geometry::make_aabb(min_coord2, max_coord2);
  geometry::DOP<T,6> const right       = geometry::convert<6,V>(aabb2);


  geometry::DOP<T,6> const all = geometry::make_union(left, right);


  BOOST_CHECK( geometry::is_valid(all) );

  BOOST_CHECK_EQUAL(all.size(), 6u);

  BOOST_CHECK_EQUAL(  all(0).lower(), -2.0 );
  BOOST_CHECK_EQUAL(  all(1).lower(), -2.0 );
  BOOST_CHECK_EQUAL(  all(2).lower(), -2.0 );
  BOOST_CHECK_EQUAL(  all(0).upper(),  2.0 );
  BOOST_CHECK_EQUAL(  all(1).upper(),  2.0 );
  BOOST_CHECK_EQUAL(  all(2).upper(),  2.0 );

}

BOOST_AUTO_TEST_SUITE_END();
