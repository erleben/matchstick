#include <tiny_math_types.h>
#include <tiny_coordsys_functions.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(tiny_coordsys_prod);

BOOST_AUTO_TEST_CASE(simple_test)
{
  typedef tiny::MathTypes<double> math_types;

  typedef math_types::value_traits     value_traits;
  typedef math_types::vector3_type     V;
  typedef math_types::quaternion_type  Q;
  typedef math_types::coordsys_type    X;

  {
    X I;
    I.identity();

    X L;
    L.T() = V::make(1.0,2.0,3.0);
    L.Q() = Q::Ru( value_traits::pi(), V::make(1.0, 2.0, 3.0) );

    X R = tiny::prod( L, I );

    BOOST_CHECK( fabs( R.T()(0) - L.T()(0) ) < 10e-10 );
    BOOST_CHECK( fabs( R.T()(1) - L.T()(1) ) < 10e-10 );
    BOOST_CHECK( fabs( R.T()(2) - L.T()(2) ) < 10e-10 );
    BOOST_CHECK( fabs( R.Q().real() - L.Q().real() ) < 10e-10 );
    BOOST_CHECK( fabs( R.Q().imag()(0) - L.Q().imag()(0) ) < 10e-10 );
    BOOST_CHECK( fabs( R.Q().imag()(1) - L.Q().imag()(1) ) < 10e-10 );
    BOOST_CHECK( fabs( R.Q().imag()(2) - L.Q().imag()(2) ) < 10e-10 );
  }
  {
    X I;
    I.identity();

    X L;
    L.T() = V::make(1.0,2.0,3.0);
    L.Q() = Q::Ru( value_traits::pi(), V::make(1.0, 2.0, 3.0) );

    X R = tiny::prod( I, L );

    BOOST_CHECK( fabs( R.T()(0) - L.T()(0) ) < 10e-10 );
    BOOST_CHECK( fabs( R.T()(1) - L.T()(1) ) < 10e-10 );
    BOOST_CHECK( fabs( R.T()(2) - L.T()(2) ) < 10e-10 );
    BOOST_CHECK( fabs( R.Q().real() - L.Q().real() ) < 10e-10 );
    BOOST_CHECK( fabs( R.Q().imag()(0) - L.Q().imag()(0) ) < 10e-10 );
    BOOST_CHECK( fabs( R.Q().imag()(1) - L.Q().imag()(1) ) < 10e-10 );
    BOOST_CHECK( fabs( R.Q().imag()(2) - L.Q().imag()(2) ) < 10e-10 );
  }
  {
    X I;
    I.identity();

    X R = tiny::prod( I, I );

    BOOST_CHECK( fabs( R.T()(0) - I.T()(0) ) < 10e-10 );
    BOOST_CHECK( fabs( R.T()(1) - I.T()(1) ) < 10e-10 );
    BOOST_CHECK( fabs( R.T()(2) - I.T()(2) ) < 10e-10 );
    BOOST_CHECK( fabs( R.Q().real() - I.Q().real() ) < 10e-10 );
    BOOST_CHECK( fabs( R.Q().imag()(0) - I.Q().imag()(0) ) < 10e-10 );
    BOOST_CHECK( fabs( R.Q().imag()(1) - I.Q().imag()(1) ) < 10e-10 );
    BOOST_CHECK( fabs( R.Q().imag()(2) - I.Q().imag()(2) ) < 10e-10 );
  }
  {
    X I;
    I.identity();

    X L;
    L.T() = V::make(1.0,2.0,3.0);
    L.Q() = Q::Ru( value_traits::pi(), V::make(1.0, 2.0, 3.0) );

    X invL = tiny::inverse( L );
    X R = tiny::prod( invL, L );

    BOOST_CHECK( fabs( R.T()(0) - I.T()(0) ) < 10e-10 );
    BOOST_CHECK( fabs( R.T()(1) - I.T()(1) ) < 10e-10 );
    BOOST_CHECK( fabs( R.T()(2) - I.T()(2) ) < 10e-10 );
    BOOST_CHECK( fabs( R.Q().real() - I.Q().real() ) < 10e-10 );
    BOOST_CHECK( fabs( R.Q().imag()(0) - I.Q().imag()(0) ) < 10e-10 );
    BOOST_CHECK( fabs( R.Q().imag()(1) - I.Q().imag()(1) ) < 10e-10 );
    BOOST_CHECK( fabs( R.Q().imag()(2) - I.Q().imag()(2) ) < 10e-10 );
  }
}

BOOST_AUTO_TEST_SUITE_END();
