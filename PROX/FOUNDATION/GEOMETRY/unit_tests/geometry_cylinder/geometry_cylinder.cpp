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


BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(cylinder_test)
{

  {
    geometry::Cylinder<V> const A;
    
    BOOST_CHECK( geometry::is_valid(A) == true );

    BOOST_CHECK_CLOSE(A.center()(0), 0.0, 0.01);
    BOOST_CHECK_CLOSE(A.center()(1), 0.0, 0.01);
    BOOST_CHECK_CLOSE(A.center()(2), 0.0, 0.01);

    BOOST_CHECK_CLOSE(A.axis()(0), 0.0, 0.01);
    BOOST_CHECK_CLOSE(A.axis()(1), 0.0, 0.01);
    BOOST_CHECK_CLOSE(A.axis()(2), 1.0, 0.01);

    BOOST_CHECK_CLOSE(A.radius(), 1.0, 0.01);
    BOOST_CHECK_CLOSE(A.height(), 1.0, 0.01);

  }

  {

    V const center = V::make(1.0, 2.0, 3.0);
    V const axis   = V::make(0.0, 5.0, 0.0);
    T const radius = 2.0;
    T const height = 4.0;

    geometry::Cylinder<V> const A = geometry::make_cylinder(radius, height, axis, center);

    BOOST_CHECK( geometry::is_valid(A) == true );

    BOOST_CHECK_CLOSE(A.center()(0), 1.0, 0.01);
    BOOST_CHECK_CLOSE(A.center()(1), 2.0, 0.01);
    BOOST_CHECK_CLOSE(A.center()(2), 3.0, 0.01);

    BOOST_CHECK_CLOSE(A.axis()(0), 0.0, 0.01);
    BOOST_CHECK_CLOSE(A.axis()(1), 1.0, 0.01);
    BOOST_CHECK_CLOSE(A.axis()(2), 0.0, 0.01);

    BOOST_CHECK_CLOSE(A.radius(), 2.0, 0.01);
    BOOST_CHECK_CLOSE(A.height(), 4.0, 0.01);
  }


  {
    V const center = V::make(1.0, 2.0, 3.0);
    V const axis   = V::make(0.0, 5.0, 0.0);
    T const radius = 2.0;
    T const height = 4.0;

    geometry::Cylinder<V> A;

    A.center() = center;
    A.set_axis(axis);
    A.set_radius(radius);
    A.set_height(height);

    BOOST_CHECK( geometry::is_valid(A) == true );

    BOOST_CHECK_CLOSE(A.center()(0), 1.0, 0.01);
    BOOST_CHECK_CLOSE(A.center()(1), 2.0, 0.01);
    BOOST_CHECK_CLOSE(A.center()(2), 3.0, 0.01);

    BOOST_CHECK_CLOSE(A.axis()(0), 0.0, 0.01);
    BOOST_CHECK_CLOSE(A.axis()(1), 1.0, 0.01);
    BOOST_CHECK_CLOSE(A.axis()(2), 0.0, 0.01);

    BOOST_CHECK_CLOSE(A.radius(), 2.0, 0.01);
    BOOST_CHECK_CLOSE(A.height(), 4.0, 0.01);
  }

  {

    V const center = V::make(1.0, 2.0, 3.0);
    V const axis   = V::make(0.0, 5.0, 0.0);
    T const radius = 2.0;
    T const height = 4.0;

    geometry::Cylinder<V> const A = geometry::make_cylinder(radius, height, axis, center);
    geometry::Cylinder<V> const B(A);
    geometry::Cylinder<V> const C = B;


    BOOST_CHECK( geometry::is_valid(B) == true );

    BOOST_CHECK_CLOSE(B.center()(0), 1.0, 0.01);
    BOOST_CHECK_CLOSE(B.center()(1), 2.0, 0.01);
    BOOST_CHECK_CLOSE(B.center()(2), 3.0, 0.01);
    BOOST_CHECK_CLOSE(B.axis()(0), 0.0, 0.01);
    BOOST_CHECK_CLOSE(B.axis()(1), 1.0, 0.01);
    BOOST_CHECK_CLOSE(B.axis()(2), 0.0, 0.01);
    BOOST_CHECK_CLOSE(B.radius(), 2.0, 0.01);
    BOOST_CHECK_CLOSE(B.height(), 4.0, 0.01);

    BOOST_CHECK_CLOSE(C.center()(0), 1.0, 0.01);
    BOOST_CHECK_CLOSE(C.center()(1), 2.0, 0.01);
    BOOST_CHECK_CLOSE(C.center()(2), 3.0, 0.01);
    BOOST_CHECK_CLOSE(C.axis()(0), 0.0, 0.01);
    BOOST_CHECK_CLOSE(C.axis()(1), 1.0, 0.01);
    BOOST_CHECK_CLOSE(C.axis()(2), 0.0, 0.01);
    BOOST_CHECK_CLOSE(C.radius(), 2.0, 0.01);
    BOOST_CHECK_CLOSE(C.height(), 4.0, 0.01);

  }





}

BOOST_AUTO_TEST_SUITE_END();
