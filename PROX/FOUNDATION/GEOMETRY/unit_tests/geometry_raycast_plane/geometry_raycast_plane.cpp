#include <geometry.h>
#include <tiny_math_types.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(raycast_plane)
{

  using std::sqrt;

  typedef tiny::MathTypes<double>   MT;
  typedef MT::vector3_type          V;
  typedef MT::real_type             T;
  typedef MT::value_traits          VT;


  V                  const normal = V::make(1.0,1.0,1.0);
  T                  const offset = norm( normal );
  geometry::Plane<V> const plane  = geometry::make_plane( unit(normal), offset);

  BOOST_CHECK(geometry::is_valid(plane));

  // Orthogonal hit from back-side
  {
    V                const p   = V::make( 0.0, 0.0, 0.0);
    V                const r   = V::make( 1.0, 1.0, 1.0);
    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_plane(ray, plane, q, length);

    BOOST_CHECK( hit );

    BOOST_CHECK_CLOSE( length, offset, 0.01);
    BOOST_CHECK_CLOSE( q(0),  1.0, 0.01);
    BOOST_CHECK_CLOSE( q(1),  1.0, 0.01);
    BOOST_CHECK_CLOSE( q(2),  1.0, 0.01);
  }

  // Orthogonal hit from back-side with front-face only
  {
    V                const p   = V::make( 0.0, 0.0, 0.0);
    V                const r   = V::make( 1.0, 1.0, 1.0);
    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_plane(ray, plane, q, length, true);

    BOOST_CHECK( !hit );
  }

  // Orthogonal hit from front-side but with ray origin on back-side
  {
    V                const p   = V::make( 0.0, 0.0, 0.0);
    V                const r   = V::make( -1.0, -1.0, -1.0);
    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_plane(ray, plane, q, length, true);

    BOOST_CHECK( !hit );
  }


  // Parallel ray and plane
  {
    V                const p   = V::make( 0.0, 0.0, 0.0);
    V                const r   = V::make( -1.0, 1.0, 0.0);
    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_plane(ray, plane, q, length);

    BOOST_CHECK( !hit );
  }

  // Oblique hit
  {
    V                const p   = V::make( 2.0, 1.0, 1.0);
    V                const r   = V::make( -1.0, 0.0, 0.0);
    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_plane(ray, plane, q, length);

    BOOST_CHECK( hit );

    BOOST_CHECK_CLOSE( length, 1.0, 0.01);
    BOOST_CHECK_CLOSE( q(0),  1.0, 0.01);
    BOOST_CHECK_CLOSE( q(1),  1.0, 0.01);
    BOOST_CHECK_CLOSE( q(2),  1.0, 0.01);
  }

}

BOOST_AUTO_TEST_SUITE_END();
