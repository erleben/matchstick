#include <geometry.h>
#include <tiny_math_types.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(raycast_triangle)
{

  using std::sqrt;

  typedef tiny::MathTypes<double>   MT;
  typedef MT::vector3_type          V;
  typedef MT::real_type             T;
  typedef MT::value_traits          VT;


  V const p0 = V::make(0.0, 0.0, 0.0);
  V const p1 = V::make(1.0, 0.0, 0.0);
  V const p2 = V::make(0.0, 1.0, 0.0);

  geometry::Triangle<V> const triangle = geometry::make_triangle(p0,p1,p2);

  // Ray hitting from front
  {
    V                const p   = V::make( 0.0, 0.0, 1.0);
    V                const r   = V::make( 0.2, 0.2, -1.0);
    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_triangle(ray, triangle, q, length, true );

    BOOST_CHECK( hit );
    BOOST_CHECK_CLOSE( length, tiny::norm(r), 0.01);
    BOOST_CHECK_CLOSE( q(0),  0.2, 0.01);
    BOOST_CHECK_CLOSE( q(1),  0.2, 0.01);
    BOOST_CHECK_CLOSE( q(2),  0.0, 0.01);
  }
  // Ray hitting from front
  {
    V                const p   = V::make( 0.0, 0.0, 1.0);
    V                const r   = V::make( 0.2, 0.2, -1.0);
    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_triangle(ray, triangle, q, length, false );

    BOOST_CHECK(  hit );
    BOOST_CHECK_CLOSE( length, tiny::norm(r), 0.01);
    BOOST_CHECK_CLOSE( q(0),  0.2, 0.01);
    BOOST_CHECK_CLOSE( q(1),  0.2, 0.01);
    BOOST_CHECK_CLOSE( q(2),  0.0, 0.01);
  }
  // Ray hitting from back
  {
    V                const p   = V::make( 0.0, 0.0, -1.0);
    V                const r   = V::make( 0.2, 0.2,  1.0);
    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_triangle(ray, triangle, q, length, true );

    BOOST_CHECK( !hit );
  }
  // Ray hitting from back
  {
    V                const p   = V::make( 0.0, 0.0, -1.0);
    V                const r   = V::make( 0.2, 0.2,  1.0);
    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_triangle(ray, triangle, q, length, false );

    BOOST_CHECK(  hit );
    BOOST_CHECK_CLOSE( length, tiny::norm(r), 0.01);
    BOOST_CHECK_CLOSE( q(0),  0.2, 0.01);
    BOOST_CHECK_CLOSE( q(1),  0.2, 0.01);
    BOOST_CHECK_CLOSE( q(2),  0.0, 0.01);
  }

  // Rays missing
  {
    V                const p   = V::make( 0.0, 0.0,  1.0);
    V                const r   = V::make( 0.6, 0.6, -1.0);
    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_triangle(ray, triangle, q, length, false );

    BOOST_CHECK(  !hit );
  }
  {
    V                const p   = V::make( 0.0, 0.0,  1.0);
    V                const r   = V::make( 0.5,-0.1, -1.0);
    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_triangle(ray, triangle, q, length, false );

    BOOST_CHECK(  !hit );
  }

  // Ray gracing
  {
    V                const p   = V::make( 0.0, 0.0, -1.0);
    V                const r   = V::make( 0.4999, 0.4999,  1.0);
    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_triangle(ray, triangle, q, length, false );

    BOOST_CHECK(  hit );
    BOOST_CHECK_CLOSE( length, tiny::norm(r), 0.01);
    BOOST_CHECK_CLOSE( q(0),  r(0), 0.01);
    BOOST_CHECK_CLOSE( q(1),  r(1), 0.01);
    BOOST_CHECK_CLOSE( q(2),  0.0, 0.01);
  }


}

BOOST_AUTO_TEST_SUITE_END();
