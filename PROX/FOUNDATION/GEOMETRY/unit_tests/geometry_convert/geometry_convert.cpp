#include <geometry.h>
#include <tiny.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

#include <vector>


BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(convert_test)
{
  typedef tiny::MathTypes<float> MT;
  typedef MT::vector3_type       V;
  typedef MT::value_traits       VT;
  typedef MT::real_type          T;
  
  {
    V                   const center = V::zero();
    T                   const radius = VT::one();
    geometry::Sphere<V> const sphere = geometry::make_sphere(center, radius);
    geometry::DOP<T,6> const dop     = geometry::convert<6,V>(sphere);

    BOOST_CHECK_EQUAL(  dop.size(), 6u);
    BOOST_CHECK_CLOSE(  dop(0).lower(), -VT::one(), 0.01 );
    BOOST_CHECK_CLOSE(  dop(1).lower(), -VT::one(), 0.01 );
    BOOST_CHECK_CLOSE(  dop(2).lower(), -VT::one(), 0.01 );
    BOOST_CHECK_CLOSE(  dop(0).upper(),  VT::one(), 0.01 );
    BOOST_CHECK_CLOSE(  dop(1).upper(),  VT::one(), 0.01 );
    BOOST_CHECK_CLOSE(  dop(2).upper(),  VT::one(), 0.01 );
    
  }

  {
    V                  const min_coord = V::make( -VT::one(), -VT::one(), -VT::one() );
    V                  const max_coord = V::make(  VT::one(),  VT::one(),  VT::one() );
    geometry::AABB<V>  const aabb      = geometry::make_aabb(min_coord, max_coord);
    geometry::DOP<T,6> const dop       = geometry::convert<6,V>(aabb);

    BOOST_CHECK_EQUAL(  dop.size(), 6u);
    BOOST_CHECK_CLOSE(  dop(0).lower(), -VT::one(), 0.01 );
    BOOST_CHECK_CLOSE(  dop(1).lower(), -VT::one(), 0.01 );
    BOOST_CHECK_CLOSE(  dop(2).lower(), -VT::one(), 0.01 );
    BOOST_CHECK_CLOSE(  dop(0).upper(),  VT::one(), 0.01 );
    BOOST_CHECK_CLOSE(  dop(1).upper(),  VT::one(), 0.01 );
    BOOST_CHECK_CLOSE(  dop(2).upper(),  VT::one(), 0.01 );

  }



}

BOOST_AUTO_TEST_SUITE_END();
