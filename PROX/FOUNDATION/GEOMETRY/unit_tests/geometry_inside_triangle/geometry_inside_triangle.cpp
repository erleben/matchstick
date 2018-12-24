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

BOOST_AUTO_TEST_CASE(inside_triangle_test)
{
  V const p0 = V::make(0.0, 0.0, 0.0);
  V const p1 = V::make(1.0, 0.0, 0.0);
  V const p2 = V::make(0.0, 1.0, 0.0);

  geometry::Triangle<V> const A = geometry::make_triangle(p0,p1,p2);

  BOOST_CHECK( geometry::is_valid(A) == true );

  // Close to corners are allways inside
  {
    V const k0 = V::make(0.0001, 0.0001, 0.0);
    V const k1 = V::make(0.9999, 0.0, 0.0);
    V const k2 = V::make(0.0, 0.9999, 0.0);

    bool test0 = geometry::inside_triangle( k0, A, true );
    bool test1 = geometry::inside_triangle( k1, A, true );
    bool test2 = geometry::inside_triangle( k2, A, true );

    BOOST_CHECK( test0 );
    BOOST_CHECK( test1 );
    BOOST_CHECK( test2 );

    test0 = geometry::inside_triangle( k0, A, false );
    test1 = geometry::inside_triangle( k1, A, false );
    test2 = geometry::inside_triangle( k2, A, false );

    BOOST_CHECK( test0 );
    BOOST_CHECK( test1 );
    BOOST_CHECK( test2 );

  }
  // Midpoint is allways inside
  {
    bool const test0 = geometry::inside_triangle( (p0+p1+p2)/3.0 , A, true );
    bool const test1 = geometry::inside_triangle( (p0+p1+p2)/3.0 , A, false );

    BOOST_CHECK( test0 );
    BOOST_CHECK( test1 );
  }
  // Some arbitary points
  {
    V const k0 = V::make( 0.2,  0.2,  0.0);  // inside and on plane
    V const k1 = V::make( 0.2,  0.2,  0.2);  // inside and above plane
    V const k2 = V::make( 0.2,  0.2, -0.2); // inside and below plane
    V const k3 = V::make(-1.0, -1.0,  0.0);   // outside and on plane
    V const k4 = V::make(-1.0, -1.0,  0.2);  // outside and above plane
    V const k5 = V::make(-1.0, -1.0, -0.2); // outside and below plane

    bool test0 = geometry::inside_triangle( k0, A, true );
    bool test1 = geometry::inside_triangle( k1, A, true );
    bool test2 = geometry::inside_triangle( k2, A, true );
    bool test3 = geometry::inside_triangle( k3, A, true );
    bool test4 = geometry::inside_triangle( k4, A, true );
    bool test5 = geometry::inside_triangle( k5, A, true );

    BOOST_CHECK( test0 );
    BOOST_CHECK( !test1 );
    BOOST_CHECK( !test2 );
    BOOST_CHECK( !test3 );
    BOOST_CHECK( !test4 );
    BOOST_CHECK( !test5 );

    test0 = geometry::inside_triangle( k0, A, false );
    test1 = geometry::inside_triangle( k1, A, false );
    test2 = geometry::inside_triangle( k2, A, false );
    test3 = geometry::inside_triangle( k3, A, false );
    test4 = geometry::inside_triangle( k4, A, false );
    test5 = geometry::inside_triangle( k5, A, false );

    BOOST_CHECK( test0 );
    BOOST_CHECK( test1 );
    BOOST_CHECK( test2 );
    BOOST_CHECK( !test3 );
    BOOST_CHECK( !test4 );
    BOOST_CHECK( !test5 );
  }
  // Different outside tests -- trying to generate all different cases....
  {
    V const k0 = V::make( 0.5, -0.1, 0.0);
    V const k1 = V::make( 1.0,  1.0, 0.0);
    V const k2 = V::make(-0.1,  0.5, 0.0);
    V const k3 = V::make(-1.0, -1.0, 0.0);
    V const k4 = V::make( 0.1,  1.0, 0.0);
    V const k5 = V::make( 1.0,  0.1, 0.0);

    bool const test0 = geometry::inside_triangle( k0, A, false );
    bool const test1 = geometry::inside_triangle( k1, A, false );
    bool const test2 = geometry::inside_triangle( k2, A, false );
    bool const test3 = geometry::inside_triangle( k3, A, false );
    bool const test4 = geometry::inside_triangle( k4, A, false );
    bool const test5 = geometry::inside_triangle( k5, A, false );

    BOOST_CHECK( !test0 );
    BOOST_CHECK( !test1 );
    BOOST_CHECK( !test2 );
    BOOST_CHECK( !test3 );
    BOOST_CHECK( !test4 );
    BOOST_CHECK( !test5 );
  }
}

BOOST_AUTO_TEST_SUITE_END();
