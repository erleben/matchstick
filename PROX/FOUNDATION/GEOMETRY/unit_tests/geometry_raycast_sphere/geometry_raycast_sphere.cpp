#include <geometry.h>
#include <tiny_math_types.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>


BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(raycast_sphere)
{
  using std::sqrt;

  typedef tiny::MathTypes<double>   MT;
  typedef MT::vector3_type          V;
  typedef MT::real_type             T;
  typedef MT::value_traits          VT;


  V const center = V::make(0.0,0.0,0.0);
  T const radius = VT::one();

  geometry::Sphere<V> sphere = geometry::make_sphere(center, radius);

  // ray hitting straight on
  {
    V const r      = V::make( 0.0, 0.0, 1.0);
    V const p      = V::make( 0.0, 0.0,-3.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_sphere(ray, sphere, q, length);

    BOOST_CHECK( hit );

    BOOST_CHECK_CLOSE( length, 2.0, 0.01);
    BOOST_CHECK_CLOSE( q(0),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(1),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(2), -1.0, 0.01);
  }

  // ray shooting staright away
  {
    V const r      = V::make( 0.0, 0.0,-1.0);
    V const p      = V::make( 0.0, 0.0,-3.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_sphere(ray, sphere, q, length);

    BOOST_CHECK( !hit );

  }

  // ray starts inside
  {
    V const r      = V::make( 0.0, 0.0,-1.0);
    V const p      = V::make( 0.0, 0.0, 0.5);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_sphere(ray, sphere, q, length);

    BOOST_CHECK( !hit );
    
  }

  // ray shooting pass sphere
  {
    V const r      = V::make( 4.0, 0.0, 1.0);
    V const p      = V::make( 0.0, 0.0,-3.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool hit = geometry::compute_raycast_sphere(ray, sphere, q, length);

    BOOST_CHECK( !hit );
  }

}

BOOST_AUTO_TEST_SUITE_END();
