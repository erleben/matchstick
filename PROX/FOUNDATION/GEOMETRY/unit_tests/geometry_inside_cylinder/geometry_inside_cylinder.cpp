#include <geometry.h>
#include <tiny_math_types.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(inside_cylinder_test)
{
  typedef tiny::MathTypes<double>   MT;
  typedef MT::vector3_type          V;
  typedef MT::real_type             T;

  V const center = V::make(1.0, 2.0, 4.0);
  V const axis   = V::make(0.0, 0.0, 1.0);
  T const radius = 2.0;
  T const height = 4.0;

  geometry::Cylinder<V> const cylinder = geometry::make_cylinder(radius, height, axis, center);

  BOOST_CHECK(geometry::is_valid(cylinder));

  // on bottom surface
  {
    V    const p    = V::make( 1.0, 2.0, 2.0);
    bool const test = geometry::inside_cylinder( p,  cylinder);
    BOOST_CHECK( test );
  }

  // on top surface
  {
    V    const p    = V::make( 1.0, 2.0, 6.0);
    bool const test = geometry::inside_cylinder( p,  cylinder);
    BOOST_CHECK( test );
  }

  // on cylinder surface
  {
    V    const p    = V::make( 3.0, 2.0, 4.0);
    bool const test = geometry::inside_cylinder( p,  cylinder);
    BOOST_CHECK( test );
  }


  // inside
  {
    V    const p    = V::make( 1.0, 2.0, 2.1);
    bool const test = geometry::inside_cylinder( p,  cylinder);
    BOOST_CHECK( test );
  }

  // inside
  {
    V    const p    = V::make( 1.0, 2.0, 5.9);
    bool const test = geometry::inside_cylinder( p,  cylinder);
    BOOST_CHECK( test );
  }

  // inside
  {
    V    const p    = V::make( 2.9, 2.0, 4.0);
    bool const test = geometry::inside_cylinder( p,  cylinder);
    BOOST_CHECK( test );
  }

  // outside
  {
    V    const p    = V::make( 1.0, 2.0, 1.9);
    bool const test = geometry::inside_cylinder( p,  cylinder);
    BOOST_CHECK( !test );
  }

  // outside
  {
    V    const p    = V::make( 1.0, 2.0, 6.1);
    bool const test = geometry::inside_cylinder( p,  cylinder);
    BOOST_CHECK( !test );
  }

  // outside
  {
    V    const p    = V::make( 3.1, 2.0, 4.0);
    bool const test = geometry::inside_cylinder( p,  cylinder);
    BOOST_CHECK( !test );
  }




}

BOOST_AUTO_TEST_SUITE_END();
