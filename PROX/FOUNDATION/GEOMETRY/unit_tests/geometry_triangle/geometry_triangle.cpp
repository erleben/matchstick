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

BOOST_AUTO_TEST_CASE(triangle_test)
{

  {
    geometry::Triangle<V> A;
    BOOST_CHECK( geometry::is_valid(A) == true );

    BOOST_CHECK_CLOSE(A.point(0)(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(0)(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(0)(2), 0.0, 0.01 );

    BOOST_CHECK_CLOSE(A.point(1)(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(1)(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(1)(2), 0.0, 0.01 );

    BOOST_CHECK_CLOSE(A.point(2)(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(2)(1), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(2)(2), 0.0, 0.01 );

    T const v = geometry::get_area(A);

    BOOST_CHECK_CLOSE(v, 0.5, 0.01 );
  }

  {
    V const p0 = V::make(0.0, 0.0, 0.0);
    V const p1 = V::make(0.0, 1.0, 0.0);
    V const p2 = V::make(1.0, 0.0, 0.0);

    geometry::Triangle<V> A = geometry::make_triangle(p0,p1,p2);
    BOOST_CHECK( geometry::is_valid(A) == true );

    BOOST_CHECK_CLOSE(A.point(0)(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(0)(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(0)(2), 0.0, 0.01 );

    BOOST_CHECK_CLOSE(A.point(1)(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(1)(1), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(1)(2), 0.0, 0.01 );

    BOOST_CHECK_CLOSE(A.point(2)(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(2)(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(2)(2), 0.0, 0.01 );

    T const v = geometry::get_area(A);

    BOOST_CHECK_CLOSE(v, 0.5, 0.01 );
  }

}

BOOST_AUTO_TEST_SUITE_END();
