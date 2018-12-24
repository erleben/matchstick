#include <geometry.h>
#include <tiny_math_types.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

typedef tiny::MathTypes<double>   MT;
typedef MT::vector3_type          V;
typedef MT::real_type             T;
typedef MT::value_traits          VT;


BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(inside_dop_test)
{
  std::vector<V> corners(2u);

  corners[0] = V::make(-1.0,-1.0,-1.0);
  corners[1] = V::make( 1.0, 1.0, 1.0);

  geometry::DOP<T, 6u> const dop = geometry::make_dop(corners.begin(), corners.end(), geometry::make3<V>() );

  BOOST_CHECK(geometry::is_valid(dop));

  {
    V    const p    = V::make( 0.0, 0.0, 0.0);
    bool const test = geometry::inside_dop(p, dop);
    BOOST_CHECK( test );
  }
  {
    V    const p    = V::make( 1.0, 1.0, 1.0);
    bool const test = geometry::inside_dop(p, dop);
    BOOST_CHECK( test );
  }
  {
    V    const p    = V::make( 2.0, 0.0, 0.0);
    bool const test = geometry::inside_dop(p, dop);
    BOOST_CHECK( !test );
  }

}

BOOST_AUTO_TEST_CASE(outside_dop_test)
{
  std::vector<V> corners(2u);

  corners[0] = V::make(-1.0,-1.0,-1.0);
  corners[1] = V::make( 1.0, 1.0, 1.0);

  geometry::DOP<T, 6u> const dop = geometry::make_dop(corners.begin(), corners.end(), geometry::make3<V>() );

  BOOST_CHECK(geometry::is_valid(dop));

  {
    V    const p    = V::make( 0.0, 0.0, 0.0);
    bool const test = geometry::outside_dop(p, dop);
    BOOST_CHECK( ! test );
  }
  {
    V    const p    = V::make( 1.0, 1.0, 1.0);
    bool const test = geometry::outside_dop(p, dop);
    BOOST_CHECK( ! test );
  }
  {
    V    const p    = V::make( 2.0, 0.0, 0.0);
    bool const test = geometry::outside_dop(p, dop);
    BOOST_CHECK( test );
  }
  
}

BOOST_AUTO_TEST_SUITE_END();
