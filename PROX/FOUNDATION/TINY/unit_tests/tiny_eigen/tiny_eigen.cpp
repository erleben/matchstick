
#include <tiny_math_types.h>
#include <tiny_matrix_functions.h>
#include <tiny_eigen3x3.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

#include <cmath>                       // for fabs

template<typename vector3_type,typename matrix3x3_type>
inline void eigen_value_decomposition_test(vector3_type d,matrix3x3_type R)
{
  using std::fabs;

  typedef typename vector3_type::real_type     real_type;
  typedef typename vector3_type::value_traits  value_traits;

  real_type const tol = value_traits::numeric_cast( 0.0001 );
  real_type const d0  = d(0);
  real_type const d1  = d(1);
  real_type const d2  = d(2);
  
  matrix3x3_type D = matrix3x3_type::make_diag(d);
  matrix3x3_type A = R*D*tiny::trans(R);
  BOOST_CHECK( tiny::is_symmetric(A,tol) );

  matrix3x3_type V;
  tiny::eigen(A, V, d);

  real_type determinant = tiny::det(V);
  BOOST_CHECK_CLOSE( fabs( determinant ), value_traits::one(), tol );

  real_type epsilon = 10e-7;

  matrix3x3_type Itest = tiny::trans(V)*V;

  BOOST_CHECK_CLOSE( Itest(0,0), value_traits::one() , tol );
  BOOST_CHECK_CLOSE( Itest(1,1), value_traits::one() , tol );
  BOOST_CHECK_CLOSE( Itest(2,2), value_traits::one() , tol );
  //--- The check close version behaves strange when rhs is exactly zero?, so we use the check-version instead...
  BOOST_CHECK( fabs(Itest(0,1))<epsilon );
  BOOST_CHECK( fabs(Itest(0,2))<epsilon );
  BOOST_CHECK( fabs(Itest(1,0))<epsilon );
  BOOST_CHECK( fabs(Itest(1,2))<epsilon );
  BOOST_CHECK( fabs(Itest(2,0))<epsilon );
  BOOST_CHECK( fabs(Itest(2,1))<epsilon );

  matrix3x3_type Atest = A - V*matrix3x3_type::make_diag(d)*tiny::trans(V);
  
  BOOST_CHECK( fabs(Atest(0,0))<epsilon );
  BOOST_CHECK( fabs(Atest(0,1))<epsilon );
  BOOST_CHECK( fabs(Atest(0,2))<epsilon );
  BOOST_CHECK( fabs(Atest(1,0))<epsilon );
  BOOST_CHECK( fabs(Atest(1,1))<epsilon );
  BOOST_CHECK( fabs(Atest(1,2))<epsilon );
  BOOST_CHECK( fabs(Atest(2,0))<epsilon );
  BOOST_CHECK( fabs(Atest(2,1))<epsilon );
  BOOST_CHECK( fabs(Atest(2,2))<epsilon );

  bool match1 = fabs( d0 - d(0) ) < epsilon &&  
                fabs( d1 - d(1) ) < epsilon &&
                fabs( d2 - d(2) ) < epsilon ;

  bool match2 = fabs( d0 - d(0) )< epsilon &&
                fabs( d1 - d(2) )< epsilon &&
                fabs( d2 - d(1) )< epsilon ;

  bool match3 = fabs( d0 - d(1) )< epsilon &&
                fabs( d1 - d(0) )< epsilon &&
                fabs( d2 - d(2) )< epsilon ;

  bool match4 = fabs( d0 - d(1) )< epsilon &&
                fabs( d1 - d(2) )< epsilon &&
                fabs( d2 - d(0) )< epsilon ;

  bool match5 = fabs( d0 - d(2) )< epsilon &&
                fabs( d1 - d(0) )< epsilon &&
                fabs( d2 - d(1) )< epsilon ;

  bool match6 = fabs( d0 - d(2) )< epsilon &&
                fabs( d1 - d(1) )< epsilon &&
                fabs( d2 - d(0) )< epsilon ;

  BOOST_CHECK( match1 || match2 || match3 || match4 || match5 || match6 );
}

BOOST_AUTO_TEST_SUITE(tiny_eigen);

  BOOST_AUTO_TEST_CASE(random_testing)
  {
    typedef tiny::MathTypes<double>                    math_types;
    typedef math_types::vector3_type                  vector3_type;
    typedef math_types::matrix3x3_type                matrix3x3_type;

    matrix3x3_type R;
    vector3_type d;

    for(int i= 0;i<100;++i)
    {
      //--- non-negative eigen-values
      d = vector3_type::random(0,1);
      R = matrix3x3_type::random();
      R = tiny::ortonormalize(R);
      eigen_value_decomposition_test(d,R);

      //--- non-positive eigen-values
      d = vector3_type::random(-1,0);
      R = matrix3x3_type::random();
      R = tiny::ortonormalize(R);
      eigen_value_decomposition_test(d,R);

      //--- one zero eigen-values
      d = vector3_type::random(0,1);
      d(0) = 0;
      R = matrix3x3_type::random();
      R = tiny::ortonormalize(R);
      eigen_value_decomposition_test(d,R);

      //--- one zero eigen-values
      d = vector3_type::random(0,1);
      d(1) = 0;
      R = matrix3x3_type::random();
      R = tiny::ortonormalize(R);
      eigen_value_decomposition_test(d,R);

      //--- one zero eigen-values
      d = vector3_type::random(0,1);
      d(2) = 0;
      R = matrix3x3_type::random();
      R = tiny::ortonormalize(R);
      eigen_value_decomposition_test(d,R);

      //--- two zero eigen-values
      d = vector3_type::random(0,1);
      d(0) = 0;
      d(1) = 0;
      R = matrix3x3_type::random();
      R = tiny::ortonormalize(R);
      eigen_value_decomposition_test(d,R);

      //--- two zero eigen-values
      d = vector3_type::random(0,1);
      d(0) = 0;
      d(2) = 0;
      R = matrix3x3_type::random();
      R = tiny::ortonormalize(R);
      eigen_value_decomposition_test(d,R);

      //--- two zero eigen-values
      d = vector3_type::random(0,1);
      d(1) = 0;
      d(2) = 0;
      R = matrix3x3_type::random();
      R = tiny::ortonormalize(R);
      eigen_value_decomposition_test(d,R);

      //--- three zero eigen-values
      d.clear();
      R = matrix3x3_type::random();
      R = tiny::ortonormalize(R);
      eigen_value_decomposition_test(d,R);

      //--- multiplicity of 3
      d = vector3_type::random(0,1);
      d(1) = d(0);
      d(2) = d(0);
      R = matrix3x3_type::random();
      R = tiny::ortonormalize(R);
      eigen_value_decomposition_test(d,R);

      //--- multiplicity of 2
      d = vector3_type::random(0,1);
      d(1) = d(0);
      R = matrix3x3_type::random();
      R = tiny::ortonormalize(R);
      eigen_value_decomposition_test(d,R);

      //--- multiplicity of 2
      d = vector3_type::random(0,1);
      d(2) = d(0);
      R = matrix3x3_type::random();
      R = tiny::ortonormalize(R);
      eigen_value_decomposition_test(d,R);

      //--- multiplicity of 2
      d = vector3_type::random(0,1);
      d(2) = d(1);
      R = matrix3x3_type::random();
      R = tiny::ortonormalize(R);
      eigen_value_decomposition_test(d,R);
    }
  }


BOOST_AUTO_TEST_CASE(detailed_testing)
{  
  typedef tiny::MathTypes<double>           MT;
  typedef MT::vector3_type                  V;
  typedef MT::matrix3x3_type                M;
  typedef MT::value_traits                  VT;

  M A;
  
  // This test case is known to result in a R-matrix with det(R)=-1
  A(0,0) =  10.0;  A(0,1) = -1.0;  A(0,2) = -1.0;
  A(1,0) =  -1.0;  A(1,1) = 10.0;  A(1,2) = -1.0;
  A(2,0) =  -1.0;  A(2,1) = -1.0;  A(2,2) = 10.0;
    
  M R;
  V d;
  tiny::eigen(A, R, d);
  
  double determinant = tiny::det(R);
  BOOST_CHECK_CLOSE( determinant, -VT::one(), 0.01 );
  
}

BOOST_AUTO_TEST_SUITE_END();
