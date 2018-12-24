#include <sparse.h>
#include <sparse_fill.h>
#include <prox_math_policy.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(prod_mass_vec);
BOOST_AUTO_TEST_CASE(mass_vec_prod_test_case)
{
  typedef prox::MathPolicy<float>           math_policy;
  typedef math_policy::value_traits         value_traits;
  typedef math_policy::diagonal_mass_type   matrix_type;
  typedef math_policy::vector6_type         vector_type;

  matrix_type M(1);
  sparse::fill(M(0,0), value_traits::zero() );// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
  M(0,0)[0] = value_traits::numeric_cast(2.5);

  BOOST_CHECK_EQUAL( M(0,0)[0] , value_traits::numeric_cast(2.5) );
  BOOST_CHECK_EQUAL( M(0,0)[1] , value_traits::numeric_cast(1) );
  BOOST_CHECK_EQUAL( M(0,0)[2] , value_traits::numeric_cast(2) );
  BOOST_CHECK_EQUAL( M(0,0)[3] , value_traits::numeric_cast(3) );
  BOOST_CHECK_EQUAL( M(0,0)[4] , value_traits::numeric_cast(4) );
  BOOST_CHECK_EQUAL( M(0,0)[5] , value_traits::numeric_cast(5) );
  BOOST_CHECK_EQUAL( M(0,0)[6] , value_traits::numeric_cast(6) );

  vector_type u(1);
  sparse::fill(u(0), 1.0f);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'

  BOOST_CHECK_EQUAL( u(0)(0) , value_traits::numeric_cast(1) );
  BOOST_CHECK_EQUAL( u(0)(1) , value_traits::numeric_cast(2) );
  BOOST_CHECK_EQUAL( u(0)(2) , value_traits::numeric_cast(3) );
  BOOST_CHECK_EQUAL( u(0)(3) , value_traits::numeric_cast(4) );
  BOOST_CHECK_EQUAL( u(0)(4) , value_traits::numeric_cast(5) );
  BOOST_CHECK_EQUAL( u(0)(5) , value_traits::numeric_cast(6) );

  vector_type r(1);
  sparse::prod(M, u, r);

  BOOST_CHECK_EQUAL( r(0)(0) , 2.5 );
  BOOST_CHECK_EQUAL( r(0)(1) , 5   );
  BOOST_CHECK_EQUAL( r(0)(2) , 7.5 );
  BOOST_CHECK_EQUAL( r(0)(3) , 32  );
  BOOST_CHECK_EQUAL( r(0)(4) , 58  );
  BOOST_CHECK_EQUAL( r(0)(5) , 73  );

  M.resize(2,2);
  sparse::fill(M(1,1), 0.0f);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
  M(1,1)[0] = 2.5f;
  u.resize(2);

  sparse::fill(u(1), 7.0f);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
  r.clear();
  r.resize(2);
  sparse::prod(M, u, r);

  BOOST_CHECK_EQUAL( r(0)(0) , 2.5 );
  BOOST_CHECK_EQUAL( r(0)(1) , 5   );
  BOOST_CHECK_EQUAL( r(0)(2) , 7.5 );
  BOOST_CHECK_EQUAL( r(0)(3) , 32  );
  BOOST_CHECK_EQUAL( r(0)(4) , 58  );
  BOOST_CHECK_EQUAL( r(0)(5) , 73  );
  BOOST_CHECK_EQUAL( r(1)(0) , 17.5);
  BOOST_CHECK_EQUAL( r(1)(1) , 20  );
  BOOST_CHECK_EQUAL( r(1)(2) , 22.5);
  BOOST_CHECK_EQUAL( r(1)(3) , 68  );
  BOOST_CHECK_EQUAL( r(1)(4) , 124 );
  BOOST_CHECK_EQUAL( r(1)(5) , 157 );

}
BOOST_AUTO_TEST_SUITE_END();

