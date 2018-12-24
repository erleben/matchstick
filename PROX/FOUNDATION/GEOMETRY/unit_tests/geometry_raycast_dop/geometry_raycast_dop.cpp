#include <geometry.h>
#include <tiny_math_types.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>


BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(raycast_dop)
{
  using std::sqrt;

  typedef tiny::MathTypes<double>   MT;
  typedef MT::vector3_type          V;
  typedef MT::real_type             T;
  typedef MT::value_traits          VT;

  std::vector<V> corners(2u);

  corners[0] = V::make(-1.0,-1.0,-1.0);
  corners[1] = V::make( 1.0, 1.0, 1.0);

  geometry::DOP<T, 6u> const dop = geometry::make_dop(corners.begin(), corners.end(), geometry::make3<V>() );

  BOOST_CHECK(geometry::is_valid(dop));

  // Hit straigth on
  {
    V const r      = V::make( 0.0, 0.0, 1.0);
    V const p      = V::make( 0.0, 0.0,-3.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_dop(ray, dop, q, length);

    BOOST_CHECK( hit );

    BOOST_CHECK_CLOSE( length, 2.0, 0.01);
    BOOST_CHECK_CLOSE( q(0),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(1),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(2), -1.0, 0.01);
  }

  // Hit straigth on corner
  {
    V const r      = V::make( 0.0, 0.0, 1.0);
    V const p      = V::make( -1.0, -1.0,-3.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_dop(ray, dop, q, length);

    BOOST_CHECK( hit );

    BOOST_CHECK_CLOSE( length, 2.0, 0.01);
    BOOST_CHECK_CLOSE( q(0), -1.0, 0.01);
    BOOST_CHECK_CLOSE( q(1), -1.0, 0.01);
    BOOST_CHECK_CLOSE( q(2), -1.0, 0.01);
  }


  // Hit straigth on edge
  {
    V const r      = V::make( 0.0, 0.0, 1.0);
    V const p      = V::make( 0.0, -1.0,-3.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_dop(ray, dop, q, length);

    BOOST_CHECK( hit );

    BOOST_CHECK_CLOSE( length, 2.0, 0.01);
    BOOST_CHECK_CLOSE( q(0),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(1), -1.0, 0.01);
    BOOST_CHECK_CLOSE( q(2), -1.0, 0.01);
  }


  // Aligned ray no hitting
  {
    V const r      = V::make( 0.0, 0.0, 1.0);
    V const p      = V::make( -2.0, 0.0,-3.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_dop(ray, dop, q, length);

    BOOST_CHECK( !hit );

    BOOST_CHECK_CLOSE( length, VT::infinity(), 0.01);
    BOOST_CHECK_CLOSE( q(0),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(1),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(2),  0.0, 0.01);
  }


  // Obligue ray no hitting
  {
    V const r      = V::make( 1.0, 1.0, 1.0);
    V const p      = V::make( 0.0, 0.0,-10.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_dop(ray, dop, q, length);

    BOOST_CHECK( !hit );

    BOOST_CHECK_CLOSE( length, VT::infinity(), 0.01);
    BOOST_CHECK_CLOSE( q(0),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(1),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(2),  0.0, 0.01);
  }


  // Obligue ray central hit
  {
    V const r      = V::make( 1.0,  1.0,  1.0);
    V const p      = V::make(-2.0, -2.0, -3.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_dop(ray, dop, q, length);

    BOOST_CHECK( hit );

    BOOST_CHECK_CLOSE( length, 2*1.732050807568877, 0.01);
    BOOST_CHECK_CLOSE( q(0),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(1),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(2), -1.0, 0.01);
  }


  // Obligue ray corner hit
  {
    V const r      = V::make( 1.0,  1.0,  1.0);
    V const p      = V::make( 0.0,  0.0, -2.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_dop(ray, dop, q, length);

    BOOST_CHECK( hit );

    BOOST_CHECK_CLOSE( length, 1.732050807568877, 0.01);
    BOOST_CHECK_CLOSE( q(0),  1.0, 0.01);
    BOOST_CHECK_CLOSE( q(1),  1.0, 0.01);
    BOOST_CHECK_CLOSE( q(2), -1.0, 0.01);
  }


  // Obligue ray edge hit
  {
    V const r      = V::make( 1.0,  1.0,  1.0);
    V const p      = V::make( 0.0, -1.0, -2.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_dop(ray, dop, q, length);

    BOOST_CHECK( hit );

    BOOST_CHECK_CLOSE( length, 1.732050807568877, 0.01);
    BOOST_CHECK_CLOSE( q(0),  1.0, 0.01);
    BOOST_CHECK_CLOSE( q(1),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(2), -1.0, 0.01);
  }

}

BOOST_AUTO_TEST_SUITE_END();
