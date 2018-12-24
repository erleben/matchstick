
#include <tiny_math_types.h>
#include <tiny_eigen3x3.h>
#include <tiny_polar_decomposition3x3.h>
#include <tiny_matrix_functions.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

#include <cmath>

BOOST_AUTO_TEST_SUITE(tiny_polar_decomposition);

  BOOST_AUTO_TEST_CASE(eigen_method)
  {
    typedef tiny::MathTypes<double>                   math_types;
    typedef math_types::matrix3x3_type                matrix3x3_type;
    typedef math_types::real_type                     real_type;
    typedef math_types::value_traits                  value_traits;

    real_type epsilon = 10e-7;
    matrix3x3_type A,R,S,D;

    for(size_t i=0;i<10;++i)
    {
      S = matrix3x3_type::random();      
      S = tiny::trans(S)*S;
      A = matrix3x3_type::random();
      R = tiny::ortonormalize( A );
      A = R*S;
      R = matrix3x3_type::make_diag(1.0);
      S = matrix3x3_type::make_diag(1.0);
      bool success = tiny::polar_decomposition_eigen(A,R,S);
      if(success)
      {
        bool right_handed = tiny::det(R) > value_traits::zero();
        BOOST_CHECK( right_handed );
        
        D = A - R*S;
        real_type maximum_deviation =  tiny::max(  abs(D) );

        //BOOST_CHECK_CLOSE( maximum_deviation, value_traits::zero(), tol);
        BOOST_CHECK( maximum_deviation<epsilon );
      }
    }
  }

BOOST_AUTO_TEST_SUITE_END();
