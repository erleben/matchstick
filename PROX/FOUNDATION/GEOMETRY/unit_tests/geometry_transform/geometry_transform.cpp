#include <geometry.h>
#include <tiny.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

typedef tiny::MathTypes<float> MT;
typedef MT::quaternion_type    Q;
typedef MT::vector3_type       V;
typedef MT::real_type          T;
typedef MT::value_traits       VT;


BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(obb_transform_test)
{
    V const center   = V::make(1.0, 1.0, 1.0);
    V const half_ext = V::make(1.0, 2.0, 3.0);
    Q const q        = Q::Rx(VT::pi_half());

    geometry::OBB<MT> const obb = geometry::make_obb<MT>(center, q, half_ext);

    V const p0 = geometry::transform_to_obb(center, obb);

    BOOST_CHECK_CLOSE(p0(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(p0(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(p0(2), 0.0, 0.01 );

    V const p1 = geometry::transform_from_obb(p0, obb);

    BOOST_CHECK_CLOSE(p1(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(p1(1), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(p1(2), 1.0, 0.01 );

    V const p2 = geometry::transform_from_obb(V::make(0.0, 2.0, 0.0), obb);

    BOOST_CHECK_CLOSE(p2(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(p2(1), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(p2(2), 3.0, 0.01 );

    V const p3 = geometry::transform_to_obb(p2, obb);

    BOOST_CHECK_CLOSE(p3(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(p3(1), 2.0, 0.01 );
    BOOST_CHECK_CLOSE(p3(2), 0.0, 0.01 );
}

BOOST_AUTO_TEST_CASE(cylinder_transform_test)
{
  V const center   = V::make(1.0, 1.0, 1.0);
  V const axis     = V::make(1.0, 0.0, 0.0);
  T const radius   = 1.0;
  T const height   = 1.0;

  geometry::Cylinder<V> const cyl = geometry::make_cylinder(radius, height, axis, center);

  V const p0 = geometry::transform_to_cylinder(center, cyl);

  BOOST_CHECK_CLOSE(p0(0), 0.0, 0.01 );
  BOOST_CHECK_CLOSE(p0(1), 0.0, 0.01 );
  BOOST_CHECK_CLOSE(p0(2), 0.0, 0.01 );

  V const p1 = geometry::transform_from_cylinder(p0, cyl);

  BOOST_CHECK_CLOSE(p1(0), 1.0, 0.01 );
  BOOST_CHECK_CLOSE(p1(1), 1.0, 0.01 );
  BOOST_CHECK_CLOSE(p1(2), 1.0, 0.01 );

  V const p2 = geometry::transform_from_cylinder(V::make(0.0, 2.0, 0.0), cyl);

  BOOST_CHECK_CLOSE(p2(0), 1.0, 0.01 );
  BOOST_CHECK_CLOSE(p2(1), 3.0, 0.01 );
  BOOST_CHECK_CLOSE(p2(2), 1.0, 0.01 );

  V const p3 = geometry::transform_to_cylinder(p2, cyl);

  BOOST_CHECK_CLOSE(p3(0), 0.0, 0.01 );
  BOOST_CHECK_CLOSE(p3(1), 2.0, 0.01 );
  BOOST_CHECK_CLOSE(p3(2), 0.0, 0.01 );
}

BOOST_AUTO_TEST_SUITE_END();
