#include <tiny_math_types.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/test_tools.hpp>

template<typename math_types>
inline void compile_test_math_types()
{
  typedef typename math_types::real_type           real_type;
  typedef typename math_types::vector3_type        vector3_type;
  typedef typename math_types::matrix3x3_type      matrix3x3_type;
  typedef typename math_types::quaternion_type     quaternion_type;
  typedef typename math_types::coordsys_type       coordsys_type;
  typedef typename math_types::value_traits        value_traits;

  vector3_type     v;
  matrix3x3_type   m;
  quaternion_type  q;
  coordsys_type    c;

  real_type s1 = value_traits::zero();
  real_type s2 = value_traits::one();
  real_type s3 = value_traits::two();
  real_type s4 = value_traits::pi();
  real_type s5 = value_traits::pi_2();
  real_type s6 = value_traits::infinity();

  BOOST_CHECK( s1 == value_traits::zero() );
  BOOST_CHECK( s2 == value_traits::one() );
  BOOST_CHECK( s3 == value_traits::two() );
  BOOST_CHECK( s4 == value_traits::pi() );
  BOOST_CHECK( s5 == value_traits::pi_2() );
  BOOST_CHECK( s6 == value_traits::infinity() );
}


BOOST_AUTO_TEST_SUITE(tiny_math_types);

		BOOST_AUTO_TEST_CASE(type_and_member_compile_test)
		{
      //--- Compile testing that we can instantiate most common types and access members
      typedef tiny::MathTypes<float > type1;
      typedef tiny::MathTypes<double > type2;
      void (*ptr1)() = &(compile_test_math_types<type1>);
      void (*ptr2)() = &(compile_test_math_types<type2>);
      ptr1 = 0;
      ptr2 = 0;
    }

BOOST_AUTO_TEST_SUITE_END();
