#include <solvers/sub/prox_analytical_ellipsoid.h>
#include <solvers/sub/prox_analytical_sphere.h>
#include <solvers/sub/prox_numerical_ellipsoid.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(prox_mappings);

BOOST_AUTO_TEST_CASE(analytical_ellipsoid_test_case)
{  
  float lambda_n   = 1.0f;
  float lambda_s   = 0.0f;
  float lambda_t   = 0.0f;
  float lambda_tau = 0.0f;

  prox::detail::analytical_ellipsoid(
                                     0.0f, 0.0f, 0.0f,
                                     1.0f, 1.0f, 1.0f,
                                     lambda_n,
                                     lambda_s, lambda_t, lambda_tau
                                     );
  BOOST_CHECK_CLOSE(lambda_s, 0.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_t, 0.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_tau, 0.0f, 0.1f);
  
  prox::detail::analytical_ellipsoid(
                                     0.0f, 0.0f, 2.0f,
                                     1.0f, 1.0f, 1.0f,
                                     lambda_n,
                                     lambda_s, lambda_t, lambda_tau
                                     );
  BOOST_CHECK_CLOSE(lambda_s, 0.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_t, 0.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_tau, 1.0f, 0.1f);
  
  prox::detail::analytical_ellipsoid(
                                     1.0f, 1.0f, 0.0f,
                                     1.0f, 1.0f, 1.0f,
                                     lambda_n,
                                     lambda_s, lambda_t, lambda_tau
                                     );
  BOOST_CHECK_CLOSE(lambda_s, 0.707106769f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_t, 0.707106769f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_tau, 0.0f, 0.1f);
  
  prox::detail::analytical_ellipsoid(
                                     1.0f, 0.0f, 0.0f,
                                     1.0f, 1.0f, 1.0f,
                                     lambda_n,
                                     lambda_s, lambda_t, lambda_tau
                                     );
  BOOST_CHECK_CLOSE(lambda_s, 1.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_t, 0.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_tau, 0.0f, 0.1f);
  
  lambda_n   = 0.0f;
  prox::detail::analytical_ellipsoid(
                                     1.0f, 0.0f, 0.0f,
                                     1.0f, 1.0f, 1.0f,
                                     lambda_n,
                                     lambda_s, lambda_t, lambda_tau
                                     );
  BOOST_CHECK_CLOSE(lambda_s, 0.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_t, 0.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_tau, 0.0f, 0.1f);
}

BOOST_AUTO_TEST_CASE(analytical_sphere_test_case)
{
  float lambda_n   = 1.0f;
  float lambda_s   = 0.0f;
  float lambda_t   = 0.0f;
  float lambda_tau = 0.0f;

  prox::detail::analytical_sphere( 0.0f, 0.0f, 0.0f, 1.0f, lambda_n, lambda_s, lambda_t, lambda_tau );
  BOOST_CHECK_CLOSE(lambda_s, 0.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_t, 0.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_tau, 0.0f, 0.1f);
  
  prox::detail::analytical_sphere( 0.0f, 0.0f, 2.0f, 1.0f, lambda_n, lambda_s, lambda_t, lambda_tau );
  BOOST_CHECK_CLOSE(lambda_s, 0.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_t, 0.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_tau, 1.0f, 0.1f);
  
  prox::detail::analytical_sphere( 1.0f, 1.0f, 0.0f, 1.0f, lambda_n, lambda_s, lambda_t, lambda_tau );
  BOOST_CHECK_CLOSE(lambda_s, 0.707106769f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_t, 0.707106769f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_tau, 0.0f, 0.1f);
  
  prox::detail::analytical_sphere( 1.0f, 0.0f, 0.0f, 1.0f, lambda_n, lambda_s, lambda_t, lambda_tau );
  BOOST_CHECK_CLOSE(lambda_s, 1.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_t, 0.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_tau, 0.0f, 0.1f);

  lambda_n   = 0.0f;
  prox::detail::analytical_sphere( 1.0f, 0.0f, 0.0f, 1.0f, lambda_n, lambda_s, lambda_t, lambda_tau );
  BOOST_CHECK_CLOSE(lambda_s, 0.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_t, 0.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_tau, 0.0f, 0.1f);
}

BOOST_AUTO_TEST_CASE(numerical_test_case)
{
  float lambda_n   = 1.0f;
  float lambda_s   = 0.0f;
  float lambda_t   = 0.0f;
  float lambda_tau = 0.0f;

  prox::detail::numerical_ellipsoid( 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, lambda_n, lambda_s, lambda_t, lambda_tau );
  BOOST_CHECK_CLOSE(lambda_s, 0.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_t, 0.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_tau, 0.0f, 0.1f);
  
  prox::detail::numerical_ellipsoid( 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f, lambda_n, lambda_s, lambda_t, lambda_tau );
  BOOST_CHECK_CLOSE(lambda_s, 0.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_t, 1.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_tau, 0.0f, 0.1f);
  
  prox::detail::numerical_ellipsoid( 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, lambda_n, lambda_s, lambda_t, lambda_tau );
  BOOST_CHECK_CLOSE(lambda_s, 0.707106769f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_t, 0.707106769f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_tau, 0.0f, 0.1f);
  
  prox::detail::numerical_ellipsoid( 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, lambda_n, lambda_s, lambda_t, lambda_tau );
  BOOST_CHECK_CLOSE(lambda_s, 1.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_t, 0.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_tau, 0.0f, 0.1f);
  
  lambda_n   = 0.0f;
  prox::detail::numerical_ellipsoid( 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, lambda_n, lambda_s, lambda_t, lambda_tau );
  BOOST_CHECK_CLOSE(lambda_s, 0.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_t, 0.0f, 0.1f);
  BOOST_CHECK_CLOSE(lambda_tau, 0.0f, 0.1f);  
}  

BOOST_AUTO_TEST_SUITE_END();
