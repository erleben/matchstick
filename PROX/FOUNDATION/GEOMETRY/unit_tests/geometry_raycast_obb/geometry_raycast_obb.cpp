#include <geometry.h>
#include <tiny_math_types.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>


BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(raycast_obb)
{
  using std::sqrt;

  typedef tiny::MathTypes<double>   MT;
  typedef MT::vector3_type          V;
  typedef MT::real_type             T;
  typedef MT::value_traits          VT;
  typedef MT::quaternion_type       Q;

  V const center   = V::make(0.0, 0.0, 0.0);
  V const half_ext = V::make(1.0, 1.0, 1.0);
  Q const q        = Q::identity();

  geometry::OBB<MT> obb = geometry::make_obb<MT>(center, q, half_ext);

  // Hit straigth on
  {
    V const r      = V::make( 0.0, 0.0, 1.0);
    V const p      = V::make( 0.0, 0.0,-3.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_obb(ray, obb, q, length);

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

    bool hit = geometry::compute_raycast_obb(ray, obb, q, length);

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

    bool hit = geometry::compute_raycast_obb(ray, obb, q, length);

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

    bool hit = geometry::compute_raycast_obb(ray, obb, q, length);

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

    bool hit = geometry::compute_raycast_obb(ray, obb, q, length);

    BOOST_CHECK( !hit );

    BOOST_CHECK_CLOSE( length, VT::infinity(), 0.01);
    BOOST_CHECK_CLOSE( q(0),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(1),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(2),  0.0, 0.01);
  }


  // Obligue ray central hit
  {
    V const r      = V::make( 1.0,  1.0,  1.0);
    V const p      = V::make(-1.0, -1.0, -2.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_obb(ray, obb, q, length);

    BOOST_CHECK( hit );

    BOOST_CHECK_CLOSE( length, 1.732050807568877, 0.01);
    BOOST_CHECK_CLOSE( q(0),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(1),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(2), -1.0, 0.01);
  }


  // Obligue ray corner hit
  {
    V const r      = V::make(  1.0,  1.0,  1.0);
    V const p      = V::make( -1.0,  -1.0, -2.9999);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_obb(ray, obb, q, length);

    BOOST_CHECK( hit );

    BOOST_CHECK_CLOSE( length, 2*1.732050807568877, 0.1);
    BOOST_CHECK_CLOSE( q(0),  1.0, 0.1);
    BOOST_CHECK_CLOSE( q(1),  1.0, 0.1);
    BOOST_CHECK_CLOSE( q(2), -1.0, 0.1);
  }


  // Obligue ray edge hit
  {
    V const r      = V::make( 1.0,  1.0,  1.0);
    V const p      = V::make( 0.0, -1.0, -2.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_obb(ray, obb, q, length);

    BOOST_CHECK( hit );

    BOOST_CHECK_CLOSE( length, 1.732050807568877, 0.01);
    BOOST_CHECK_CLOSE( q(0),  1.0, 0.01);
    BOOST_CHECK_CLOSE( q(1),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(2), -1.0, 0.01);
  }

}


BOOST_AUTO_TEST_CASE(raycast_strange_size_obb)
{
  using std::sqrt;

  typedef tiny::MathTypes<double>   MT;
  typedef MT::vector3_type          V;
  typedef MT::real_type             T;
  typedef MT::value_traits          VT;
  typedef MT::quaternion_type       Q;

  V const center   = V::make(0.0, 0.0, 0.0);
  V const half_ext = V::make(1.0, 2.0, 3.0);
  //  Q const q        = Q::Rx(VT::pi_half() );
  Q const q        = Q::identity();

  geometry::OBB<MT> obb = geometry::make_obb<MT>(center, q, half_ext);

  {
    V const r      = V::make(  2.0,  3.0, -1.0);
    V const p      = V::make( -2.0, -3.0,  4.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_obb(ray, obb, q, length);

    BOOST_CHECK( hit );

    BOOST_CHECK_CLOSE( length, tiny::norm(r), 0.01);
    BOOST_CHECK_CLOSE( q(0),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(1),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(2),  3.0, 0.01);
  }
}


BOOST_AUTO_TEST_CASE(raycast_rotated_obb)
{
  using std::sqrt;

  typedef tiny::MathTypes<double>   MT;
  typedef MT::vector3_type          V;
  typedef MT::real_type             T;
  typedef MT::value_traits          VT;
  typedef MT::quaternion_type       Q;

  V const center   = V::make(0.0, 0.0, 0.0);
  V const half_ext = V::make(1.0, 2.0, 3.0);
  Q const q        = Q::Rx(VT::pi_half() );

  geometry::OBB<MT> obb = geometry::make_obb<MT>(center, q, half_ext);

  {
    V const r      = V::make(  2.0,   1.0,  3.0);
    V const p      = V::make( -2.0,  -4.0, -3.0 );

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_obb(ray, obb, q, length);

    BOOST_CHECK( hit );

    BOOST_CHECK_CLOSE( length, tiny::norm(r), 0.01);
    BOOST_CHECK_SMALL( q(0),   0.01);
    BOOST_CHECK_CLOSE( q(1),  -3.0, 0.01);
    BOOST_CHECK_SMALL( q(2),   0.01);
  }
}

BOOST_AUTO_TEST_SUITE_END();
