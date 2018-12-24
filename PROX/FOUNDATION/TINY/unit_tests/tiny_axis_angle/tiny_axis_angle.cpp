#include <tiny_math_types.h>
#include <tiny_vector_functions.h>


#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(tiny_axis_angle);

BOOST_AUTO_TEST_CASE(simple_test)
{

  typedef tiny::MathTypes<double>       math_types;

  typedef math_types::real_type        T;
  typedef math_types::vector3_type     V;
  typedef math_types::quaternion_type  Q;

  // Small positive angle
  {
    Q q;
    T const phi = 0.01;
    V const m = tiny::unit( V::make(1.0, 1.0, 1.0) );
    q = Q::Ru( phi, m);

    T theta;
    V n;
    tiny::get_axis_angle( q, n, theta );

    BOOST_CHECK_CLOSE( phi, theta, 0.01 );
    BOOST_CHECK_CLOSE( m(0), n(0), 0.01 );
    BOOST_CHECK_CLOSE( m(1), n(1), 0.01 );
    BOOST_CHECK_CLOSE( m(2), n(2), 0.01 );
  }
  // See what happens if axis is flipped
  {
    Q q;
    T const phi = 0.01;
    V const m = -tiny::unit( V::make(1.0, 1.0, 1.0) );
    q = Q::Ru( phi, m);

    T theta;
    V n;
    tiny::get_axis_angle( q, n, theta );

    BOOST_CHECK_CLOSE( phi, theta, 0.01 );
    BOOST_CHECK_CLOSE( m(0), n(0), 0.01 );
    BOOST_CHECK_CLOSE( m(1), n(1), 0.01 );
    BOOST_CHECK_CLOSE( m(2), n(2), 0.01 );
  }
  // Pick larger angle
  {
    Q q;
    T const phi = 3.0;
    V const m = tiny::unit( V::make(1.0, 1.0, 1.0) );
    q = Q::Ru( phi, m);

    T theta;
    V n;
    tiny::get_axis_angle( q, n, theta );

    BOOST_CHECK_CLOSE( phi, theta, 0.01 );
    BOOST_CHECK_CLOSE( m(0), n(0), 0.01 );
    BOOST_CHECK_CLOSE( m(1), n(1), 0.01 );
    BOOST_CHECK_CLOSE( m(2), n(2), 0.01 );
  }
  // Pick larger negative angle, the positive angle version representation should be returned!
  {
    Q q;
    T const phi = -3.0;
    V const m = tiny::unit( V::make(1.0, 1.0, 1.0) );
    q = Q::Ru( phi, m);

    T theta;
    V n;
    tiny::get_axis_angle( q, n, theta );

    BOOST_CHECK_CLOSE( -phi, theta, 0.01 );
    BOOST_CHECK_CLOSE( -m(0), n(0), 0.01 );
    BOOST_CHECK_CLOSE( -m(1), n(1), 0.01 );
    BOOST_CHECK_CLOSE( -m(2), n(2), 0.01 );
  }
  // Flip the axis
  {
    Q q;
    T const phi = -3.0;
    V const m = -tiny::unit( V::make(1.0, 1.0, 1.0) );
    q = Q::Ru( phi, m);

    T theta;
    V n;
    tiny::get_axis_angle( q, n, theta );

    BOOST_CHECK_CLOSE( -phi, theta, 0.01 );
    BOOST_CHECK_CLOSE( -m(0), n(0), 0.01 );
    BOOST_CHECK_CLOSE( -m(1), n(1), 0.01 );
    BOOST_CHECK_CLOSE( -m(2), n(2), 0.01 );
  }
  // Large positive angle
  {
    Q q;
    T const phi = 6.0;
    V const m = tiny::unit( V::make(1.0, 1.0, 1.0) );
    q = Q::Ru( phi, m);

    T theta;
    V n;
    tiny::get_axis_angle( q, n, theta );

    BOOST_CHECK_CLOSE( phi, theta, 0.01 );
    BOOST_CHECK_CLOSE( m(0), n(0), 0.01 );
    BOOST_CHECK_CLOSE( m(1), n(1), 0.01 );
    BOOST_CHECK_CLOSE( m(2), n(2), 0.01 );
  }

}

BOOST_AUTO_TEST_SUITE_END();
