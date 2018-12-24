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
typedef MT::quaternion_type    Q;
typedef MT::real_type          T;


BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(closest_points_tetrahedron_tetrahedron_test)
{
  // Get V-V contact
  {
    V const a0 = V::make(0.0, 0.0, 0.0);
    V const a1 = V::make(1.0, 0.0, 0.0);
    V const a2 = V::make(0.0, 1.0, 0.0);
    V const a3 = V::make(0.0, 0.0, 1.0);

    V const offset = V::make( 2.0, 0.0, 0.0);

    V const b0 = V::make(0.0, 0.0, 0.0) + offset;
    V const b1 = V::make(1.0, 0.0, 0.0) + offset;
    V const b2 = V::make(0.0, 1.0, 0.0) + offset;
    V const b3 = V::make(0.0, 0.0, 1.0) + offset;

    geometry::Tetrahedron<V> const A = geometry::make_tetrahedron(a0, a1, a2 ,a3);
    geometry::Tetrahedron<V> const B = geometry::make_tetrahedron(b0, b1, b2 ,b3);

    V a;
    V b;
    T distance;

    geometry::closest_points_tetrahedron_tetrahedron( A, B, a, b, distance );

    BOOST_CHECK_CLOSE( distance, 1.0, 0.01);
    BOOST_CHECK_CLOSE( a(0), 1.0, 0.01);
    BOOST_CHECK_CLOSE( a(1), 0.0, 0.01);
    BOOST_CHECK_CLOSE( a(2), 0.0, 0.01);
    BOOST_CHECK_CLOSE( b(0), 2.0, 0.01);
    BOOST_CHECK_CLOSE( b(1), 0.0, 0.01);
    BOOST_CHECK_CLOSE( b(2), 0.0, 0.01);
  }

  // Get V-F contact
  {
    V const a0 = V::make(0.0, 0.0, 0.0);
    V const a1 = V::make(1.0, 0.0, 0.0);
    V const a2 = V::make(0.0, 1.0, 0.0);
    V const a3 = V::make(0.0, 0.0, 1.0);

    V const offset = V::make( 2.0, 2.0, 2.0);

    V const b0 = V::make(0.0, 0.0, 0.0) + offset;
    V const b1 = V::make(1.0, 0.0, 0.0) + offset;
    V const b2 = V::make(0.0, 1.0, 0.0) + offset;
    V const b3 = V::make(0.0, 0.0, 1.0) + offset;

    geometry::Tetrahedron<V> const A = geometry::make_tetrahedron(a0, a1, a2 ,a3);
    geometry::Tetrahedron<V> const B = geometry::make_tetrahedron(b0, b1, b2 ,b3);

    V a;
    V b;
    T distance;

    geometry::closest_points_tetrahedron_tetrahedron( A, B, a, b, distance );

    BOOST_CHECK_CLOSE( distance, 2.88675141, 0.01);
    BOOST_CHECK_CLOSE( a(0), 0.33333, 0.01);
    BOOST_CHECK_CLOSE( a(1), 0.33333, 0.01);
    BOOST_CHECK_CLOSE( a(2), 0.33333, 0.01);
    BOOST_CHECK_CLOSE( b(0), 2.0, 0.01);
    BOOST_CHECK_CLOSE( b(1), 2.0, 0.01);
    BOOST_CHECK_CLOSE( b(2), 2.0, 0.01);
  }

  // Get V-E contact
  {
    V const a0 = V::make(0.0, 0.0, 0.0);
    V const a1 = V::make(1.0, 0.0, 0.0);
    V const a2 = V::make(0.0, 1.0, 0.0);
    V const a3 = V::make(0.0, 0.0, 1.0);

    V const offset = V::make( 0.5, 0.0, -2.0);

    V const b0 = V::make(0.0, 0.0, 0.0) + offset;
    V const b1 = V::make(1.0, 0.0, 0.0) + offset;
    V const b2 = V::make(0.0, 1.0, 0.0) + offset;
    V const b3 = V::make(0.0, 0.0, 1.0) + offset;

    geometry::Tetrahedron<V> const A = geometry::make_tetrahedron(a0, a1, a2 ,a3);
    geometry::Tetrahedron<V> const B = geometry::make_tetrahedron(b0, b1, b2 ,b3);

    V a;
    V b;
    T distance;

    geometry::closest_points_tetrahedron_tetrahedron( A, B, a, b, distance );

    BOOST_CHECK_CLOSE( distance, 1.0, 0.01);

    BOOST_CHECK_CLOSE( a(0), 0.5, 0.01);
    BOOST_CHECK_CLOSE( a(1), 0.0, 0.01);
    BOOST_CHECK_CLOSE( a(2), 0.0, 0.01);

    BOOST_CHECK_CLOSE( b(0),  0.5, 0.01);
    BOOST_CHECK_CLOSE( b(1),  0.0, 0.01);
    BOOST_CHECK_CLOSE( b(2), -1.0, 0.01);
  }

  // Get E-E contact
  {
    V const a0 = V::make(0.0, 0.0, 0.0);
    V const a1 = V::make(1.0, 0.0, 0.0);
    V const a2 = V::make(0.0, 1.0, 0.0);
    V const a3 = V::make(0.0, 0.0, 1.0);

    V const offset = V::make( 2.0, 2.0, -0.5);

    V const b0 = V::make(0.0, 0.0, 0.0) + offset;
    V const b1 = V::make(1.0, 0.0, 0.0) + offset;
    V const b2 = V::make(0.0, 1.0, 0.0) + offset;
    V const b3 = V::make(0.0, 0.0, 1.0) + offset;

    geometry::Tetrahedron<V> const A = geometry::make_tetrahedron(a0, a1, a2 ,a3);
    geometry::Tetrahedron<V> const B = geometry::make_tetrahedron(b0, b1, b2 ,b3);

    V a;
    V b;
    T distance;

    geometry::closest_points_tetrahedron_tetrahedron( A, B, a, b, distance );

    BOOST_CHECK_CLOSE( distance, 2.121320343559642, 0.01);

    BOOST_CHECK_CLOSE( a(0), 0.5, 0.01);
    BOOST_CHECK_CLOSE( a(1), 0.5, 0.01);
    BOOST_CHECK_CLOSE( a(2), 0.0, 0.01);

    BOOST_CHECK_CLOSE( b(0),  2.0, 0.01);
    BOOST_CHECK_CLOSE( b(1),  2.0, 0.01);
    BOOST_CHECK_CLOSE( b(2),  0.0, 0.01);
  }

}

BOOST_AUTO_TEST_SUITE_END();
