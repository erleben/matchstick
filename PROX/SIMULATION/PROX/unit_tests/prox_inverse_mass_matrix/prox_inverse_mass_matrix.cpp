#include <sparse.h>
#include <sparse_fill.h>     // 2009-08-04 Kenny: Damn this header is part of the sparse unit tests but not sparse. The reason is that it uses some ugly Boost MPL stuff
#include <prox_math_policy.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(mass_block_jac);
BOOST_AUTO_TEST_CASE(mass_block_test_case)
{
  typedef prox::MathPolicy<float>           math_policy;
  typedef math_policy::diagonal_mass_type   mass_matrix_type;
  typedef math_policy::value_traits         value_traits;

  mass_matrix_type A(1);
  sparse::fill(A(0,0), 0.0f);   // 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'

  BOOST_CHECK_EQUAL( A(0,0)[0] , value_traits::zero()  );
  BOOST_CHECK_EQUAL( A(0,0)[1] , value_traits::one()   );
  BOOST_CHECK_EQUAL( A(0,0)[2] , value_traits::two()   );
  BOOST_CHECK_EQUAL( A(0,0)[3] , value_traits::three() );
  BOOST_CHECK_EQUAL( A(0,0)[4] , value_traits::four()  );
  BOOST_CHECK_EQUAL( A(0,0)[5] , value_traits::numeric_cast(5) );
  BOOST_CHECK_EQUAL( A(0,0)[6] , value_traits::numeric_cast(6) );

  A(0,0)[0] = value_traits::numeric_cast(2.5);
  mass_matrix_type B(A);
  
  BOOST_CHECK_EQUAL( B(0,0)[0] , value_traits::numeric_cast(2.5));
  BOOST_CHECK_EQUAL( B(0,0)[1] , value_traits::one()            );
  BOOST_CHECK_EQUAL( B(0,0)[2] , value_traits::two()            );
  BOOST_CHECK_EQUAL( B(0,0)[3] , value_traits::three()          );
  BOOST_CHECK_EQUAL( B(0,0)[4] , value_traits::four()           );
  BOOST_CHECK_EQUAL( B(0,0)[5] , value_traits::numeric_cast(5) );
  BOOST_CHECK_EQUAL( B(0,0)[6] , value_traits::numeric_cast(6) );
  
  sparse::inverse(A);

  BOOST_CHECK_EQUAL( A(0,0)[0] , 1/value_traits::numeric_cast(2.5) );
  BOOST_CHECK_EQUAL( A(0,0)[1] , value_traits::one()   );
  BOOST_CHECK_EQUAL( A(0,0)[2] , value_traits::numeric_cast(-3)   );
  BOOST_CHECK_EQUAL( A(0,0)[3] , value_traits::two() );
  BOOST_CHECK_EQUAL( A(0,0)[4] , value_traits::three()  );
  BOOST_CHECK_EQUAL( A(0,0)[5] , value_traits::numeric_cast(-1) );
  BOOST_CHECK_EQUAL( A(0,0)[6] , value_traits::zero() );
}

BOOST_AUTO_TEST_SUITE_END();
