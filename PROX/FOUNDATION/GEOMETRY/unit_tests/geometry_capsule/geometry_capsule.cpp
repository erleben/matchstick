#include <geometry.h>
#include <tiny.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

typedef tiny::MathTypes<float> MT;
typedef MT::vector3_type       V;
typedef MT::real_type          T;
typedef MT::value_traits       VT;


BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(capsule_test)
{
  {
    geometry::Capsule<V> const A;
    
    BOOST_CHECK( geometry::is_valid(A) == true );

    BOOST_CHECK_CLOSE(A.point0()(0), 0.0, 0.01);
    BOOST_CHECK_CLOSE(A.point0()(1), 0.0, 0.01);
    BOOST_CHECK_CLOSE(A.point0()(2), 0.0, 0.01);

    BOOST_CHECK_CLOSE(A.point1()(0), 0.0, 0.01);
    BOOST_CHECK_CLOSE(A.point1()(1), 0.0, 0.01);
    BOOST_CHECK_CLOSE(A.point1()(2), 0.0, 0.01);

    BOOST_CHECK_CLOSE(A.radius(), 1.0, 0.01);
  }
  {
    V const point0 = V::make(1.0, 2.0, 3.0);
    V const point1 = V::make(4.0, 5.0, 6.0);
    T const radius = 7.0;

    geometry::Capsule<V> const A = geometry::make_capsule(radius, point0, point1);

    BOOST_CHECK( geometry::is_valid(A) == true );

    BOOST_CHECK_CLOSE(A.point0()(0), 1.0, 0.01);
    BOOST_CHECK_CLOSE(A.point0()(1), 2.0, 0.01);
    BOOST_CHECK_CLOSE(A.point0()(2), 3.0, 0.01);

    BOOST_CHECK_CLOSE(A.point1()(0), 4.0, 0.01);
    BOOST_CHECK_CLOSE(A.point1()(1), 5.0, 0.01);
    BOOST_CHECK_CLOSE(A.point1()(2), 6.0, 0.01);

    BOOST_CHECK_CLOSE(A.radius(), 7.0, 0.01);
  }
  {
    V const point0 = V::make(1.0, 2.0, 3.0);
    V const point1 = V::make(4.0, 5.0, 6.0);
    T const radius = 7.0;

    geometry::Capsule<V> A;

    A.point0() = point0;
    A.point1() = point1;
    A.set_radius(radius);

    BOOST_CHECK( geometry::is_valid(A) == true );

    BOOST_CHECK_CLOSE(A.point0()(0), 1.0, 0.01);
    BOOST_CHECK_CLOSE(A.point0()(1), 2.0, 0.01);
    BOOST_CHECK_CLOSE(A.point0()(2), 3.0, 0.01);

    BOOST_CHECK_CLOSE(A.point1()(0), 4.0, 0.01);
    BOOST_CHECK_CLOSE(A.point1()(1), 5.0, 0.01);
    BOOST_CHECK_CLOSE(A.point1()(2), 6.0, 0.01);

    BOOST_CHECK_CLOSE(A.radius(), 7.0, 0.01);
  }
  {
    V const point0 = V::make(1.0, 2.0, 3.0);
    V const point1 = V::make(4.0, 5.0, 6.0);
    T const radius = 7.0;

    geometry::Capsule<V> const A = geometry::make_capsule(radius, point0, point1);
    geometry::Capsule<V> const B(A);
    geometry::Capsule<V> const C = B;


    BOOST_CHECK( geometry::is_valid(B) == true );

    BOOST_CHECK_CLOSE(B.point0()(0), 1.0, 0.01);
    BOOST_CHECK_CLOSE(B.point0()(1), 2.0, 0.01);
    BOOST_CHECK_CLOSE(B.point0()(2), 3.0, 0.01);

    BOOST_CHECK_CLOSE(B.point1()(0), 4.0, 0.01);
    BOOST_CHECK_CLOSE(B.point1()(1), 5.0, 0.01);
    BOOST_CHECK_CLOSE(B.point1()(2), 6.0, 0.01);

    BOOST_CHECK_CLOSE(B.radius(), 7.0, 0.01);

    BOOST_CHECK_CLOSE(C.point0()(0), 1.0, 0.01);
    BOOST_CHECK_CLOSE(C.point0()(1), 2.0, 0.01);
    BOOST_CHECK_CLOSE(C.point0()(2), 3.0, 0.01);

    BOOST_CHECK_CLOSE(C.point1()(0), 4.0, 0.01);
    BOOST_CHECK_CLOSE(C.point1()(1), 5.0, 0.01);
    BOOST_CHECK_CLOSE(C.point1()(2), 6.0, 0.01);

    BOOST_CHECK_CLOSE(C.radius(), 7.0, 0.01);
  }

  {
    V const & center = V::zero();
    V const & axis   = V::k();
    T const & height = VT::two();
    T const & radius = VT::three();

    geometry::Cylinder<V> const & C = geometry::make_cylinder(radius, height, axis, center);
    geometry::Capsule<V>  const & A = geometry::convert( C );

    BOOST_CHECK_CLOSE(A.point0()(0),  0.0, 0.01);
    BOOST_CHECK_CLOSE(A.point0()(1),  0.0, 0.01);
    BOOST_CHECK_CLOSE(A.point0()(2), -1.0, 0.01);
    BOOST_CHECK_CLOSE(A.point1()(0),  0.0, 0.01);
    BOOST_CHECK_CLOSE(A.point1()(1),  0.0, 0.01);
    BOOST_CHECK_CLOSE(A.point1()(2),  1.0, 0.01);
    BOOST_CHECK_CLOSE(A.radius(),     3.0, 0.01);
  }

}

BOOST_AUTO_TEST_SUITE_END();
