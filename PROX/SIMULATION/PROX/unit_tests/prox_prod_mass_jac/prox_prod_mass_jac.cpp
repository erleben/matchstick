#include <sparse.h>
#include <sparse_fill.h>
#include <prox_math_policy.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(prod_mass_jac);
BOOST_AUTO_TEST_CASE(mass_compress_prod_test_case)
{  
  typedef prox::MathPolicy<float>   math_policy;
  typedef math_policy::diagonal_mass_type    mass_matrix_type;
  typedef math_policy::compressed6x4_type    transposed_jacobian_type;
  
  mass_matrix_type M(1);
  sparse::fill(M(0,0), 1.5f); // 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
  
  transposed_jacobian_type JT(1,1,1);
  sparse::fill(JT(0,0), 2.0f); // 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
  
  transposed_jacobian_type C(1,1,1);
  sparse::prod(M, JT, C);
  
  BOOST_CHECK_EQUAL( C(0,0)(0,0) , 3     );
  BOOST_CHECK_EQUAL( C(0,0)(0,1) , 4.5   );
  BOOST_CHECK_EQUAL( C(0,0)(0,2) , 6     );
  BOOST_CHECK_EQUAL( C(0,0)(0,3) , 7.5   );
  
  BOOST_CHECK_EQUAL( C(0,0)(1,0) , 9     );
  BOOST_CHECK_EQUAL( C(0,0)(1,1) , 10.5  );
  BOOST_CHECK_EQUAL( C(0,0)(1,2) , 12    );
  BOOST_CHECK_EQUAL( C(0,0)(1,3) , 13.5  );
  
  BOOST_CHECK_EQUAL( C(0,0)(2,0) , 15    );
  BOOST_CHECK_EQUAL( C(0,0)(2,1) , 16.5  );
  BOOST_CHECK_EQUAL( C(0,0)(2,2) , 18    );
  BOOST_CHECK_EQUAL( C(0,0)(2,3) , 19.5  );
  
  BOOST_CHECK_EQUAL( C(0,0)(3,0) , 197   );
  BOOST_CHECK_EQUAL( C(0,0)(3,1) , 207.5 );
  BOOST_CHECK_EQUAL( C(0,0)(3,2) , 218   );
  BOOST_CHECK_EQUAL( C(0,0)(3,3) , 228.5 );
  
  BOOST_CHECK_EQUAL( C(0,0)(4,0) , 291   );
  BOOST_CHECK_EQUAL( C(0,0)(4,1) , 306.5 );
  BOOST_CHECK_EQUAL( C(0,0)(4,2) , 322   );
  BOOST_CHECK_EQUAL( C(0,0)(4,3) , 337.5 );
  
  BOOST_CHECK_EQUAL( C(0,0)(5,0) , 345   );
  BOOST_CHECK_EQUAL( C(0,0)(5,1) , 363.5 );
  BOOST_CHECK_EQUAL( C(0,0)(5,2) , 382   );
  BOOST_CHECK_EQUAL( C(0,0)(5,3) , 400.5 );
}

BOOST_AUTO_TEST_CASE(mass_compress_resized_prod_test_case)
{  
  typedef prox::MathPolicy<float>   math_policy;
  typedef math_policy::diagonal_mass_type    mass_matrix_type;
  typedef math_policy::compressed6x4_type    transposed_jacobian_type;
  
  mass_matrix_type M(1);
  sparse::fill(M(0,0), 1.5f);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
  
  transposed_jacobian_type JT(1,1,1);
  sparse::fill(JT(0,0), 2.0f);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
  
  transposed_jacobian_type C(1,1,1);
  sparse::prod(M, JT, C);
  M.resize(2,2);
  sparse::fill(M(1,1), 2.5f);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
  
  JT.resize(2,4,2);
  sparse::fill(JT(1,2), 3.0f);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
  
  C.clear();
  C.resize(2,4,2);
  sparse::prod(M, JT, C);
  
  BOOST_CHECK_EQUAL( C(0,0)(0,0) , 3     );
  BOOST_CHECK_EQUAL( C(0,0)(0,1) , 4.5   ); 
  BOOST_CHECK_EQUAL( C(0,0)(0,2) , 6     ); 
  BOOST_CHECK_EQUAL( C(0,0)(0,3) , 7.5   );
  BOOST_CHECK_EQUAL( C(0,0)(1,0) , 9     );
  BOOST_CHECK_EQUAL( C(0,0)(1,1) , 10.5  ); 
  BOOST_CHECK_EQUAL( C(0,0)(1,2) , 12    );
  BOOST_CHECK_EQUAL( C(0,0)(1,3) , 13.5  );
  BOOST_CHECK_EQUAL( C(0,0)(2,0) , 15    );
  BOOST_CHECK_EQUAL( C(0,0)(2,1) , 16.5  ); 
  BOOST_CHECK_EQUAL( C(0,0)(2,2) , 18    );
  BOOST_CHECK_EQUAL( C(0,0)(2,3) , 19.5  );
  BOOST_CHECK_EQUAL( C(0,0)(3,0) , 197   );
  BOOST_CHECK_EQUAL( C(0,0)(3,1) , 207.5 ); 
  BOOST_CHECK_EQUAL( C(0,0)(3,2) , 218   );
  BOOST_CHECK_EQUAL( C(0,0)(3,3) , 228.5 );
  BOOST_CHECK_EQUAL( C(0,0)(4,0) , 291   );
  BOOST_CHECK_EQUAL( C(0,0)(4,1) , 306.5 ); 
  BOOST_CHECK_EQUAL( C(0,0)(4,2) , 322   );
  BOOST_CHECK_EQUAL( C(0,0)(4,3) , 337.5 );
  BOOST_CHECK_EQUAL( C(0,0)(5,0) , 345   );
  BOOST_CHECK_EQUAL( C(0,0)(5,1) , 363.5 ); 
  BOOST_CHECK_EQUAL( C(0,0)(5,2) , 382   );
  BOOST_CHECK_EQUAL( C(0,0)(5,3) , 400.5 );
  
  BOOST_CHECK_EQUAL( C(1,2)(0,0) , 7.5   );
  BOOST_CHECK_EQUAL( C(1,2)(0,1) , 10    ); 
  BOOST_CHECK_EQUAL( C(1,2)(0,2) , 12.5  );
  BOOST_CHECK_EQUAL( C(1,2)(0,3) , 15    );
  BOOST_CHECK_EQUAL( C(1,2)(1,0) , 17.5  );
  BOOST_CHECK_EQUAL( C(1,2)(1,1) , 20    ); 
  BOOST_CHECK_EQUAL( C(1,2)(1,2) , 22.5  );
  BOOST_CHECK_EQUAL( C(1,2)(1,3) , 25    );
  BOOST_CHECK_EQUAL( C(1,2)(2,0) , 27.5  );
  BOOST_CHECK_EQUAL( C(1,2)(2,1) , 30    ); 
  BOOST_CHECK_EQUAL( C(1,2)(2,2) , 32.5  );
  BOOST_CHECK_EQUAL( C(1,2)(2,3) , 35    );
  BOOST_CHECK_EQUAL( C(1,2)(3,0) , 264.5 );
  BOOST_CHECK_EQUAL( C(1,2)(3,1) , 278   );
  BOOST_CHECK_EQUAL( C(1,2)(3,2) , 291.5 );
  BOOST_CHECK_EQUAL( C(1,2)(3,3) , 305   );
  BOOST_CHECK_EQUAL( C(1,2)(4,0) , 363.5 );
  BOOST_CHECK_EQUAL( C(1,2)(4,1) , 382   );
  BOOST_CHECK_EQUAL( C(1,2)(4,2) , 400.5 );
  BOOST_CHECK_EQUAL( C(1,2)(4,3) , 419   );
  BOOST_CHECK_EQUAL( C(1,2)(5,0) , 420.5 );
  BOOST_CHECK_EQUAL( C(1,2)(5,1) , 442   );
  BOOST_CHECK_EQUAL( C(1,2)(5,2) , 463.5 );
  BOOST_CHECK_EQUAL( C(1,2)(5,3) , 485   );
}
BOOST_AUTO_TEST_CASE(compress_zero_init_clear_test_case)
{  
  typedef prox::MathPolicy<float>          math_policy;
  typedef math_policy::compressed6x4_type  transposed_jacobian_type;
  typedef math_policy::value_traits        value_traits;
 
  transposed_jacobian_type C(1,1,1);
  
  BOOST_CHECK_EQUAL( C.nrows() , 1u );
  BOOST_CHECK_EQUAL( C.ncols() , 1u );
  
  BOOST_CHECK_EQUAL( C(0,0)[0] , value_traits::zero() );
  BOOST_CHECK_EQUAL( C(0,0)[1] , value_traits::zero() );
  BOOST_CHECK_EQUAL( C(0,0)[2] , value_traits::zero() );
  BOOST_CHECK_EQUAL( C(0,0)[3] , value_traits::zero() );
  BOOST_CHECK_EQUAL( C(0,0)[4] , value_traits::zero() );
  BOOST_CHECK_EQUAL( C(0,0)[5] , value_traits::zero() );  
  BOOST_CHECK_EQUAL( C(0,0)[6] , value_traits::zero() );
  BOOST_CHECK_EQUAL( C(0,0)[7] , value_traits::zero() );
  BOOST_CHECK_EQUAL( C(0,0)[8] , value_traits::zero() );
  BOOST_CHECK_EQUAL( C(0,0)[9] , value_traits::zero() );
  BOOST_CHECK_EQUAL( C(0,0)[10] , value_traits::zero() );
  BOOST_CHECK_EQUAL( C(0,0)[11] , value_traits::zero() );  
  BOOST_CHECK_EQUAL( C(0,0)[12] , value_traits::zero() );
  BOOST_CHECK_EQUAL( C(0,0)[13] , value_traits::zero() );
  BOOST_CHECK_EQUAL( C(0,0)[14] , value_traits::zero() );
  BOOST_CHECK_EQUAL( C(0,0)[15] , value_traits::zero() );
  BOOST_CHECK_EQUAL( C(0,0)[16] , value_traits::zero() );
  BOOST_CHECK_EQUAL( C(0,0)[17] , value_traits::zero() );
  BOOST_CHECK_EQUAL( C(0,0)[18] , value_traits::zero() );
  BOOST_CHECK_EQUAL( C(0,0)[19] , value_traits::zero() );
  BOOST_CHECK_EQUAL( C(0,0)[20] , value_traits::zero() );
  BOOST_CHECK_EQUAL( C(0,0)[21] , value_traits::zero() );
  BOOST_CHECK_EQUAL( C(0,0)[22] , value_traits::zero() );
  BOOST_CHECK_EQUAL( C(0,0)[23] , value_traits::zero() );

  C.clear();

  BOOST_CHECK_EQUAL( C.size() , 0u );
  BOOST_CHECK_EQUAL( C.nrows() , 0u );
  BOOST_CHECK_EQUAL( C.ncols() , 0u );

}
BOOST_AUTO_TEST_SUITE_END();
