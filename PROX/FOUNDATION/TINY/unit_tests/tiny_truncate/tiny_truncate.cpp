#include <tiny.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>


typedef tiny::Vector<3u, tiny::float_traits>  Vf;
typedef tiny::Vector<3u, tiny::double_traits> Vd;


BOOST_AUTO_TEST_SUITE(tiny_truncate);

  BOOST_AUTO_TEST_CASE(double_testing)
  {
    double const tol = 0.1;

    Vd const A  = Vd::make( 0.2, 0.01, 0.09999 );

    Vd const tB = tiny::truncate(A, tol);


    BOOST_CHECK_CLOSE(tB(0), 0.2, 0.01);
    BOOST_CHECK_CLOSE(tB(1), 0.0, 0.01);
    BOOST_CHECK_CLOSE(tB(2), 0.0, 0.01);
  }


BOOST_AUTO_TEST_SUITE_END();
