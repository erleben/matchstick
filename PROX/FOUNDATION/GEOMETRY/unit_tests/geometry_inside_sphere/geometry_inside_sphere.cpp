#include <geometry.h>
#include <tiny_math_types.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(inside_sphere_test)
{
  typedef tiny::MathTypes<double>   MT;
  typedef MT::vector3_type          V;
  typedef MT::real_type             T;
  typedef MT::value_traits          VT;

  V const center  = V::zero();
  T const radius  = VT::one();

  geometry::Sphere<V> const sphere = geometry::make_sphere(center , radius );

  BOOST_CHECK(geometry::is_valid(sphere));

  {
    V    const p    = V::make( 0.0, 0.0, 0.0);
    bool const test = geometry::inside_sphere( p,  sphere);
    BOOST_CHECK( test );
  }
  {
    V    const p    = V::make( 1.0, 0.0, 0.0);
    bool const test = geometry::inside_sphere( p,  sphere);
    BOOST_CHECK( test );
  }
  {
    V    const p    = V::make( 2.0, 0.0, 0.0);
    bool const test = geometry::inside_sphere( p,  sphere);
    BOOST_CHECK( !test );
  }

}

BOOST_AUTO_TEST_SUITE_END();
