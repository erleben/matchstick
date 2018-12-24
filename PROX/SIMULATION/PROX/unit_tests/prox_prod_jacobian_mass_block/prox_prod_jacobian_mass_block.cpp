#include <sparse.h>
#include <sparse_fill.h>
#include <prox_math_policy.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(prod_mass_block);

BOOST_AUTO_TEST_CASE(mass_4x6_block_prod_test_case)
{
  typedef prox::MathPolicy<float>      math_policy;
  typedef math_policy::mass_block_type mass_block_type;
  typedef math_policy::block4x6_type   jacobian_block_type;

  jacobian_block_type jb;
  jacobian_block_type jb_result(sparse::zero_block<jacobian_block_type>());
  sparse::fill(jb, 1.0f);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
  mass_block_type mb;
  sparse::fill(mb, 0.0f);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
  mb[0] = -2.0f;

  prox::prod(jb,mb,jb_result);

  BOOST_CHECK_EQUAL( jb_result[0]  , -2  );
  BOOST_CHECK_EQUAL( jb_result[6]  , -14 ); 
  BOOST_CHECK_EQUAL( jb_result[12] , -26 ); 
  BOOST_CHECK_EQUAL( jb_result[18] , -38 );
  BOOST_CHECK_EQUAL( jb_result[1]  , -4  );
  BOOST_CHECK_EQUAL( jb_result[7]  , -16 ); 
  BOOST_CHECK_EQUAL( jb_result[13] , -28 ); 
  BOOST_CHECK_EQUAL( jb_result[19] , -40 );
  BOOST_CHECK_EQUAL( jb_result[2]  , -6  );
  BOOST_CHECK_EQUAL( jb_result[8]  , -18 ); 
  BOOST_CHECK_EQUAL( jb_result[14] , -30 ); 
  BOOST_CHECK_EQUAL( jb_result[20] , -42 );
  BOOST_CHECK_EQUAL( jb_result[3]  , 32  );
  BOOST_CHECK_EQUAL( jb_result[9]  , 68  ); 
  BOOST_CHECK_EQUAL( jb_result[15] , 104 ); 
  BOOST_CHECK_EQUAL( jb_result[21] , 140 );
  BOOST_CHECK_EQUAL( jb_result[4]  , 58  );
  BOOST_CHECK_EQUAL( jb_result[10] , 124 ); 
  BOOST_CHECK_EQUAL( jb_result[16] , 190 ); 
  BOOST_CHECK_EQUAL( jb_result[22] , 256 );
  BOOST_CHECK_EQUAL( jb_result[5]  , 73  );
  BOOST_CHECK_EQUAL( jb_result[11] , 157 ); 
  BOOST_CHECK_EQUAL( jb_result[17] , 241 ); 
  BOOST_CHECK_EQUAL( jb_result[23] , 325 );

}

BOOST_AUTO_TEST_SUITE_END();
