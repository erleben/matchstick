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

BOOST_AUTO_TEST_CASE(ray_test)
{

  {
    geometry::Ray<V> A;
    BOOST_CHECK( geometry::is_valid(A) == true );

    BOOST_CHECK_CLOSE(A.origin()(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.origin()(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.origin()(2), 0.0, 0.01 );

    BOOST_CHECK_CLOSE(A.direction()(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.direction()(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.direction()(2), 1.0, 0.01 );
  }
  {
    V const o = V::make(1.0, 2.0, 3.0);
    V const d = V::make(4.0, 0.0, 0.0);

    geometry::Ray<V> A = geometry::make_ray(o, d);

    BOOST_CHECK( geometry::is_valid(A) == true );

    BOOST_CHECK_CLOSE(A.origin()(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(A.origin()(1), 2.0, 0.01 );
    BOOST_CHECK_CLOSE(A.origin()(2), 3.0, 0.01 );

    BOOST_CHECK_CLOSE(A.direction()(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(A.direction()(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.direction()(2), 0.0, 0.01 );
  }
  {
    V const o = V::make(1.0, 2.0, 3.0);
    V const d = V::make(4.0, 0.0, 0.0);

    geometry::Ray<V> A;

    A.origin() = o;
    A.set_direction(d);

    BOOST_CHECK( geometry::is_valid(A) == true );

    BOOST_CHECK_CLOSE(A.origin()(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(A.origin()(1), 2.0, 0.01 );
    BOOST_CHECK_CLOSE(A.origin()(2), 3.0, 0.01 );

    BOOST_CHECK_CLOSE(A.direction()(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(A.direction()(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.direction()(2), 0.0, 0.01 );
  }
  {
    V const o = V::make(1.0, 2.0, 3.0);
    V const d = V::make(4.0, 0.0, 0.0);

    geometry::Ray<V> A = geometry::make_ray(o, d);

    geometry::Ray<V> B = A;
    geometry::Ray<V> C(A);

    BOOST_CHECK( geometry::is_valid(B) == true );
    BOOST_CHECK( geometry::is_valid(C) == true );

    BOOST_CHECK_CLOSE(B.origin()(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(B.origin()(1), 2.0, 0.01 );
    BOOST_CHECK_CLOSE(B.origin()(2), 3.0, 0.01 );

    BOOST_CHECK_CLOSE(B.direction()(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(B.direction()(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(B.direction()(2), 0.0, 0.01 );

    BOOST_CHECK_CLOSE(C.origin()(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(C.origin()(1), 2.0, 0.01 );
    BOOST_CHECK_CLOSE(C.origin()(2), 3.0, 0.01 );

    BOOST_CHECK_CLOSE(C.direction()(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(C.direction()(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(C.direction()(2), 0.0, 0.01 );


  }

}

BOOST_AUTO_TEST_SUITE_END();
