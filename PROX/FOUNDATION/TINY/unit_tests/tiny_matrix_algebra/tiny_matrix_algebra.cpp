#include <tiny_type_traits.h>
#include <tiny_matrix.h>
#include <tiny_matrix_functions.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(tiny_matrix_algebra);

BOOST_AUTO_TEST_CASE(clear_test)
{	
	typedef tiny::ScalarTraits<float>     T;
  typedef tiny::Matrix<3,3,T>           M;
  
  M A;
  A.clear();
  BOOST_CHECK_CLOSE( A(0,0) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(0,1) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(0,2) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(1,0) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(1,1) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(1,2) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(2,0) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(2,1) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(2,2) , 0.0f, 0.01f);
}

BOOST_AUTO_TEST_CASE(component_indexing_test)
{	
	typedef tiny::ScalarTraits<float>     T;
  typedef tiny::Matrix<3,3,T>           M;
  
  M A;
  A.clear();
  
  A(0,0) = 1.0f;
  A(0,1) = 2.0f;
  A(0,2) = 3.0f;
  A(1,0) = 4.0f;
  A(1,1) = 5.0f;
  A(1,2) = 6.0f;
  A(2,0) = 7.0f;
  A(2,1) = 8.0f;
  A(2,2) = 9.0f;
  
  BOOST_CHECK_CLOSE( A(0,0) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(0,1) , 2.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(0,2) , 3.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(1,0) , 4.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(1,1) , 5.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(1,2) , 6.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(2,0) , 7.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(2,1) , 8.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(2,2) , 9.0f, 0.01f);
}  


BOOST_AUTO_TEST_CASE(single_value_construction_test)
{	
	typedef tiny::ScalarTraits<float>     T;
  typedef tiny::Matrix<3,3,T>           M;
  
  M B( 0.5f );
  
  BOOST_CHECK_CLOSE( B(0,0) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( B(0,1) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( B(0,2) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,0) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,1) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,2) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,0) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,1) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,2) , 0.5f, 0.01f);
}  

BOOST_AUTO_TEST_CASE(add_sub_test)
{	
	typedef tiny::ScalarTraits<float>     T;
  typedef tiny::Matrix<3,3,T>           M;
  
  M A;
  A(0,0) = 1.0f;
  A(0,1) = 2.0f;
  A(0,2) = 3.0f;
  A(1,0) = 4.0f;
  A(1,1) = 5.0f;
  A(1,2) = 6.0f;
  A(2,0) = 7.0f;
  A(2,1) = 8.0f;
  A(2,2) = 9.0f;

  M B( 0.5f );
  M C;
  C.clear();
  
  C = A;
  BOOST_CHECK_CLOSE( C(0,0) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) , 2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) , 3.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) , 4.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) , 5.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) , 6.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) , 7.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) , 8.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) , 9.0f, 0.01f);
  
  C = A + B;
  BOOST_CHECK_CLOSE( C(0,0) , 1.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) , 2.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) , 3.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) , 4.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) , 5.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) , 6.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) , 7.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) , 8.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) , 9.5f, 0.01f);
  
  C += B;
  BOOST_CHECK_CLOSE( C(0,0) , 2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) , 3.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) , 4.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) , 5.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) , 6.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) , 7.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) , 8.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) , 9.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) , 10.0f, 0.01f);

  C -= A;
  BOOST_CHECK_CLOSE( C(0,0) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) , 1.0f, 0.01f);
  
  C = A - B;
  BOOST_CHECK_CLOSE( C(0,0) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) , 1.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) , 2.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) , 3.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) , 4.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) , 5.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) , 6.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) , 7.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) , 8.5f, 0.01f);

  C = C;
  BOOST_CHECK_CLOSE( C(0,0) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) , 1.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) , 2.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) , 3.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) , 4.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) , 5.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) , 6.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) , 7.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) , 8.5f, 0.01f);

  C = C - B;
  BOOST_CHECK_CLOSE( C(0,0) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) , 2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) , 3.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) , 4.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) , 5.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) , 6.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) , 7.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) , 8.0f, 0.01f);
  
  C = C + B;
  BOOST_CHECK_CLOSE( C(0,0) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) , 1.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) , 2.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) , 3.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) , 4.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) , 5.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) , 6.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) , 7.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) , 8.5f, 0.01f);
  
  C = B - C;
  BOOST_CHECK_CLOSE( C(0,0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) , -1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) , -2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) , -3.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) , -4.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) , -5.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) , -6.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) , -7.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) , -8.0f, 0.01f);

  C = B + C;
  BOOST_CHECK_CLOSE( C(0,0) ,  0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) , -0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) , -1.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) , -2.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) , -3.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) , -4.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) , -5.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) , -6.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) , -7.5f, 0.01f);

  C = B;
  C += C;
  BOOST_CHECK_CLOSE( C(0,0) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) , 1.0f, 0.01f);
  
  C -= C;
  BOOST_CHECK_CLOSE( C(0,0) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) , 0.0f, 0.01f);  
  
  C = -B;
  BOOST_CHECK_CLOSE( C(0,0) , -0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) , -0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) , -0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) , -0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) , -0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) , -0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) , -0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) , -0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) , -0.5f, 0.01f);  
  
}

BOOST_AUTO_TEST_CASE(scalar_mul_div_test)
{	
	typedef tiny::ScalarTraits<float>     T;
  typedef tiny::Matrix<3,3,T>           M;
  
  M A;
  A(0,0) = 1.0f;
  A(0,1) = 2.0f;
  A(0,2) = 3.0f;
  A(1,0) = 4.0f;
  A(1,1) = 5.0f;
  A(1,2) = 6.0f;
  A(2,0) = 7.0f;
  A(2,1) = 8.0f;
  A(2,2) = 9.0f;
  
  M C;
  C.clear();
  
  C = A*0.5f;
  BOOST_CHECK_CLOSE( C(0,0) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) , 1.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) , 2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) , 2.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) , 3.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) , 3.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) , 4.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) , 4.5f, 0.01f);
  
  C *= 2.0f;
  BOOST_CHECK_CLOSE( C(0,0) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) , 2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) , 3.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) , 4.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) , 5.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) , 6.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) , 7.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) , 8.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) , 9.0f, 0.01f);

  C /= 2.0f;
  BOOST_CHECK_CLOSE( C(0,0) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) , 1.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) , 2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) , 2.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) , 3.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) , 3.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) , 4.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) , 4.5f, 0.01f);

  C.clear();
  C = 0.5f*A;
  BOOST_CHECK_CLOSE( C(0,0) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) , 1.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) , 2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) , 2.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) , 3.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) , 3.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) , 4.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) , 4.5f, 0.01f);

  
  C.clear();
  C = A/2.0f;
  BOOST_CHECK_CLOSE( C(0,0) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) , 1.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) , 2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) , 2.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) , 3.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) , 3.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) , 4.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) , 4.5f, 0.01f);


//  C.clear();
//  C = 2.0f/A;  // 2009-07-13 Kenny: Hmmm a bit strange!
//  BOOST_CHECK_CLOSE( C(0,0) , 0.5f, 0.01f);
//  BOOST_CHECK_CLOSE( C(0,1) , 1.0f, 0.01f);
//  BOOST_CHECK_CLOSE( C(0,2) , 1.5f, 0.01f);
//  BOOST_CHECK_CLOSE( C(1,0) , 2.0f, 0.01f);
//  BOOST_CHECK_CLOSE( C(1,1) , 2.5f, 0.01f);
//  BOOST_CHECK_CLOSE( C(1,2) , 3.0f, 0.01f);
//  BOOST_CHECK_CLOSE( C(2,0) , 3.5f, 0.01f);
//  BOOST_CHECK_CLOSE( C(2,1) , 4.0f, 0.01f);
//  BOOST_CHECK_CLOSE( C(2,2) , 4.5f, 0.01f);
  
}  


BOOST_AUTO_TEST_CASE(matrix_prod_test)
{	
	typedef tiny::ScalarTraits<float>     T;
  typedef tiny::Matrix<3,3,T>           M;
  
  M A;
  A(0,0) = 1.0f; A(0,1) = 2.0f; A(0,2) = 3.0f;
  A(1,0) = 2.0f; A(1,1) = 1.0f; A(1,2) = 2.0f;
  A(2,0) = 3.0f; A(2,1) = 2.0f; A(2,2) = 1.0f;
  
  M B;
  B(0,0) = -0.3750f;  B(0,1) =  0.5000f; B(0,2) =  0.1250f; 
  B(1,0) =  0.5000f;  B(1,1) = -1.0000f; B(1,2) =  0.5000f;
  B(2,0) =  0.1250f;  B(2,1) =  0.5000f; B(2,2) = -0.3750f;
  
  M C;
  C.clear();
  
  C = A*B;
  BOOST_CHECK_CLOSE( C(0,0) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) , 1.0f, 0.01f);
}  

BOOST_AUTO_TEST_CASE(inverse_test)
{	
	typedef tiny::ScalarTraits<float>     T;
  typedef tiny::Matrix<3,3,T>           M;
  
  M A;
  A(0,0) = 1.0f; A(0,1) = 2.0f; A(0,2) = 3.0f;
  A(1,0) = 2.0f; A(1,1) = 1.0f; A(1,2) = 2.0f;
  A(2,0) = 3.0f; A(2,1) = 2.0f; A(2,2) = 1.0f;
  
  M B;
  B(0,0) = -0.3750f;  B(0,1) =  0.5000f; B(0,2) =  0.1250f; 
  B(1,0) =  0.5000f;  B(1,1) = -1.0000f; B(1,2) =  0.5000f;
  B(2,0) =  0.1250f;  B(2,1) =  0.5000f; B(2,2) = -0.3750f;
  
  M C;
  C.clear();
  C = tiny::inverse( A );
  BOOST_CHECK_CLOSE( C(0,0) , B(0,0), 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) , B(0,1), 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) , B(0,2), 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) , B(1,0), 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) , B(1,1), 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) , B(1,2), 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) , B(2,0), 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) , B(2,1), 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) , B(2,2), 0.01f);
}  

BOOST_AUTO_TEST_CASE(diverse_ops_test)
{	
	typedef tiny::ScalarTraits<float>     T;
  typedef tiny::Matrix<3,3,T>           M;
  
  M A;
  A(0,0) = 1.0f; A(0,1) = 2.0f; A(0,2) = 3.0f;
  A(1,0) = 2.0f; A(1,1) = 1.0f; A(1,2) = 2.0f;
  A(2,0) = 3.0f; A(2,1) = 2.0f; A(2,2) = 1.0f;
  
  BOOST_CHECK_CLOSE( tiny::trace(A) , 3.0f, 0.01f);
  BOOST_CHECK_CLOSE( tiny::trace(A) , 3.0f, 0.01f);
  BOOST_CHECK_CLOSE( tiny::det(A) , 8.0f, 0.01f);
  BOOST_CHECK( tiny::is_symmetric(A,0.0001f) == true);
  
  A(0,0) = 1.0f; A(0,1) = 2.0f; A(0,2) = 3.0f;
  A(1,0) = -2.0f; A(1,1) = 1.0f; A(1,2) = 2.0f;
  A(2,0) = -3.0f; A(2,1) = -2.0f; A(2,2) = 1.0f;
  BOOST_CHECK( tiny::is_symmetric(A,0.0001f) == false);
  
  M B;
  
  B = tiny::trans(A);
  BOOST_CHECK_CLOSE( A(0,0) , B(0,0), 0.01f);
  BOOST_CHECK_CLOSE( A(0,1) , B(1,0), 0.01f);
  BOOST_CHECK_CLOSE( A(0,2) , B(2,0), 0.01f);
  BOOST_CHECK_CLOSE( A(1,0) , B(0,1), 0.01f);
  BOOST_CHECK_CLOSE( A(1,1) , B(1,1), 0.01f);
  BOOST_CHECK_CLOSE( A(1,2) , B(2,1), 0.01f);
  BOOST_CHECK_CLOSE( A(2,0) , B(0,2), 0.01f);
  BOOST_CHECK_CLOSE( A(2,1) , B(1,2), 0.01f);
  BOOST_CHECK_CLOSE( A(2,2) , B(2,2), 0.01f);

  A(0,0) = 1.0f; A(0,1) = 2.0f; A(0,2) = 3.0f;
  A(1,0) = -2.0f; A(1,1) = 1.0f; A(1,2) = 2.0f;
  A(2,0) = -3.0f; A(2,1) = -2.0f; A(2,2) = 1.0f;
  B = tiny::abs(A);
  BOOST_CHECK_CLOSE( B(0,0) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(0,1) , 2.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(0,2) , 3.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,0) , 2.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,2) , 2.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,0) , 3.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,1) , 2.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,2) , 1.0f, 0.01f);
  
  A(0,0) = 1.0f; A(0,1) = 2.0f; A(0,2) = 4.0f;
  A(1,0) = -2.0f; A(1,1) = 1.0f; A(1,2) = 2.0f;
  A(2,0) = -3.0f; A(2,1) = -2.0f; A(2,2) = 1.0f;
  BOOST_CHECK_CLOSE( tiny::min(A) ,   -3.0f, 0.01f);
  BOOST_CHECK_CLOSE( tiny::max(A) ,    4.0f, 0.01f);
  BOOST_CHECK_CLOSE( tiny::norm_1(A) , 4.0f, 0.01f);
  
  A(0,0) = 1.0f; A(0,1) = 2.0f; A(0,2) = 3.0f;
  A(1,0) = -2.0f; A(1,1) = 1.0f; A(1,2) = 2.0f;
  A(2,0) = -4.0f; A(2,1) = -2.0f; A(2,2) = 1.0f;
  BOOST_CHECK_CLOSE( tiny::min(A) ,   -4.0f, 0.01f);
  BOOST_CHECK_CLOSE( tiny::max(A) ,    3.0f, 0.01f);
  BOOST_CHECK_CLOSE( tiny::norm_1(A) , 4.0f, 0.01f);
  
  A(0,0) = 1.0f; A(0,1) = 2.0f; A(0,2) = 4.0f;
  A(1,0) = -2.0f; A(1,1) = 4.0f; A(1,2) = 2.0f;
  A(2,0) = -3.0f; A(2,1) = -2.0f; A(2,2) = 1.0f;
  BOOST_CHECK_CLOSE( tiny::min(A) ,   -3.0f, 0.01f);
  BOOST_CHECK_CLOSE( tiny::max(A) ,    4.0f, 0.01f);
  BOOST_CHECK_CLOSE( tiny::norm_1(A) , 4.0f, 0.01f);

  A(0,0) = 1.0f; A(0,1) = 2.0f; A(0,2) = 3.0f;
  A(1,0) = -2.0f; A(1,1) = 1.0f; A(1,2) = 2.0f;
  A(2,0) = -3.0f; A(2,1) = -2.0f; A(2,2) = 1.0f;
  B = tiny::sign(A);
  BOOST_CHECK_CLOSE( B(0,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(0,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(0,2) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,0) , -1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,2) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,0) , -1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,1) , -1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,2) ,  1.0f, 0.01f);
  
  A(0,0) =  1.1f; A(0,1) =  2.1f; A(0,2) = 3.1f;
  A(1,0) = -2.2f; A(1,1) =  1.2f; A(1,2) = 2.2f;
  A(2,0) = -3.3f; A(2,1) = -2.3f; A(2,2) = 1.3f;
  B = tiny::round(A);
  BOOST_CHECK_CLOSE( B(0,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(0,1) ,  2.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(0,2) ,  3.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,0) , -2.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,2) ,  2.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,0) , -3.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,1) , -2.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,2) ,  1.0f, 0.01f);
  
  A(0,0) =  1.0f; A(0,1) =  2.0f; A(0,2) = 3.0f;
  A(1,0) = -2.0f; A(1,1) =  1.0f; A(1,2) = 2.0f;
  A(2,0) = -3.0f; A(2,1) = -2.0f; A(2,2) = 1.0f;
  
  B(0,0) = -1.0f; B(0,1) =  -2.0f; B(0,2) = -3.0f;
  B(1,0) =  2.0f; B(1,1) =  -1.0f; B(1,2) = -2.0f;
  B(2,0) =  3.0f; B(2,1) =   2.0f; B(2,2) = -1.0f;
  
  M C;
  C = tiny::min(A,B);
  BOOST_CHECK_CLOSE( C(0,0) , -1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) , -2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) , -3.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) , -2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) , -1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) , -2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) , -3.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) , -2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) , -1.0f, 0.01f);
  
  C = tiny::max(A,B);
  BOOST_CHECK_CLOSE( C(0,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) ,  2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) ,  3.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) ,  2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) ,  2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) ,  3.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) ,  2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) ,  1.0f, 0.01f);

  BOOST_CHECK_CLOSE( tiny::sum(A), 3.0f, 0.01f);
  

  A(0,0) =  1.1f; A(0,1) =  2.2f; A(0,2) = 3.3f;
  A(1,0) = -2.4f; A(1,1) =  1.5f; A(1,2) = 2.6f;
  A(2,0) = -3.7f; A(2,1) = -2.8f; A(2,2) = 1.9f;
  
  C = tiny::floor(A);
  BOOST_CHECK_CLOSE( C(0,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) ,  2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) ,  3.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) ,  -3.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) ,  2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) ,  -4.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) ,  -3.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) ,  1.0f, 0.01f);
  
  C = tiny::ceil(A);
  BOOST_CHECK_CLOSE( C(0,0) ,  2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) ,  3.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) ,  4.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) ,  -2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) ,  2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) ,  3.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) ,  -3.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) ,  -2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) ,  2.0f, 0.01f);
}  

BOOST_AUTO_TEST_CASE(comparison_tests)
{	
	typedef tiny::ScalarTraits<float>     T;
  typedef tiny::Matrix<3,3,T>           M;
  
  M A;
  A(0,0) = 1.0f; A(0,1) = 2.0f; A(0,2) = 3.0f;
  A(1,0) = 2.0f; A(1,1) = 1.0f; A(1,2) = 2.0f;
  A(2,0) = 3.0f; A(2,1) = 2.0f; A(2,2) = 1.0f;

  M B;
  B(0,0) = 1.0f; B(0,1) = 2.0f; B(0,2) = 3.0f;
  B(1,0) = 2.0f; B(1,1) = 1.0f; B(1,2) = 2.0f;
  B(2,0) = 3.0f; B(2,1) = 2.0f; B(2,2) = 1.0f;
  
  M C;
  C = tiny::equal(A , B);
  BOOST_CHECK_CLOSE( C(0,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) ,  1.0f, 0.01f);

  C = tiny::equal(A , A);
  BOOST_CHECK_CLOSE( C(0,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) ,  1.0f, 0.01f);

  B(0,0) = 10.0f; B(0,1) = 2.0f; B(0,2) = 3.0f;
  B(1,0) = 2.0f; B(1,1) = 10.0f; B(1,2) = 2.0f;
  B(2,0) = 3.0f; B(2,1) = 2.0f; B(2,2) = 10.0f;
  
  C = tiny::equal(A , B);
  BOOST_CHECK_CLOSE( C(0,0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) ,  0.0f, 0.01f);
  
  
  C = tiny::not_equal(A , B);
  BOOST_CHECK_CLOSE( C(0,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) ,  1.0f, 0.01f);
  
  C = (A < B);
  BOOST_CHECK_CLOSE( C(0,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) ,  1.0f, 0.01f);

  C = (A <= B);
  BOOST_CHECK_CLOSE( C(0,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) ,  1.0f, 0.01f);

  
  C = (A > B);
  BOOST_CHECK_CLOSE( C(0,0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) ,  0.0f, 0.01f);
  
  C = (A >= B);
  BOOST_CHECK_CLOSE( C(0,0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(0,2) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1,2) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2,2) ,  0.0f, 0.01f);
  
}

BOOST_AUTO_TEST_CASE(matrix_vector_tests)
{	
	typedef tiny::ScalarTraits<float>     T;
  typedef tiny::Matrix<3,3,T>           M;
  typedef M::column_type               C;
  typedef M::row_type                  R;
  
  M A;
  A(0,0) = 1.0f; A(0,1) = 2.0f; A(0,2) = 3.0f;
  A(1,0) = 2.0f; A(1,1) = 1.0f; A(1,2) = 2.0f;
  A(2,0) = 3.0f; A(2,1) = 2.0f; A(2,2) = 1.0f;
  
  
  R v;
  v(0) = 1.0;
  v(1) = 1.0;
  v(2) = 1.0;
  
  C w;
  w.clear();
  w = A*v;
  
  BOOST_CHECK_CLOSE( w(0) ,  6.0f, 0.01f);
  BOOST_CHECK_CLOSE( w(1) ,  5.0f, 0.01f);
  BOOST_CHECK_CLOSE( w(2) ,  6.0f, 0.01f);
  
  w(0) = 1.0;
  w(1) = 1.0;
  w(2) = 1.0;
  v.clear();
  v = w*A;

  BOOST_CHECK_CLOSE( v(0) ,  6.0f, 0.01f);
  BOOST_CHECK_CLOSE( v(1) ,  5.0f, 0.01f);
  BOOST_CHECK_CLOSE( v(2) ,  6.0f, 0.01f);
}  

BOOST_AUTO_TEST_CASE(geometry_tests)
{	
	typedef tiny::ScalarTraits<float>     T;
  typedef tiny::Matrix<3,3,T>           M;
  typedef tiny::Vector<3,T>             V;
  V v;
  v(0) = 1.0f;
  v(1) = 1.0f;
  v(2) = 1.0f;
  
  V w;
  w(0) = 1.0f;
  w(1) = 2.0f;
  w(2) = 3.0f;
  
  M A;
  
  A = tiny::outer_prod(v,w);
  
  BOOST_CHECK_CLOSE( A(0,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(0,1) ,  2.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(0,2) ,  3.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(1,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(1,1) ,  2.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(1,2) ,  3.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(2,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(2,1) ,  2.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(2,2) ,  3.0f, 0.01f);
  
  V a =  tiny::star(w) * v;
  V b =  tiny::cross(w,v);
  BOOST_CHECK_CLOSE( a(0) ,  b(0), 0.01f);
  BOOST_CHECK_CLOSE( a(1) ,  b(1), 0.01f);
  BOOST_CHECK_CLOSE( a(2) ,  b(2), 0.01f);
  
  
  A(0,0) = 2.0f; A(0,1) = 0.0f; A(0,2) = 0.0f;
  A(1,0) = 0.0f; A(1,1) = 4.0f; A(1,2) = 0.0f;
  A(2,0) = 0.0f; A(2,1) = 0.0f; A(2,2) = 8.0f;
  
  M B = tiny::ortonormalize( A );
  
  BOOST_CHECK_CLOSE( B(0,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(0,1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(0,2) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,2) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,2) ,  1.0f, 0.01f);
  
  A(0,0) = 2.0f; A(0,1) = 0.0f; A(0,2) = 0.0f;
  A(1,0) = 10.0f; A(1,1) = 4.0f; A(1,2) = 0.0f;
  A(2,0) = 10.0f; A(2,1) = 10.0f; A(2,2) = 8.0f;
  
  B = tiny::ortonormalize( A );
  
  BOOST_CHECK_CLOSE( B(0,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(0,1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(0,2) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,2) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,2) ,  1.0f, 0.01f);
  
}

BOOST_AUTO_TEST_CASE(factory_tests)
{	
	typedef tiny::ScalarTraits<float>     T;
  typedef tiny::Matrix<3,3,T>           M;
  typedef tiny::Vector<3,T>             V;
  
  V v;
  v(0) = 1.0;
  v(1) = 2.0;
  v(2) = 3.0;
  M B = M::make_diag(v);
  BOOST_CHECK_CLOSE( B(0,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(0,1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(0,2) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,1) ,  2.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,2) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,2) ,  3.0f, 0.01f);

  B = M::make_diag(5.0f);
  BOOST_CHECK_CLOSE( B(0,0) ,  5.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(0,1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(0,2) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,1) ,  5.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,2) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,2) ,  5.0f, 0.01f);
  
  B = M::make_diag(1.0f,2.0f,3.0f);
  BOOST_CHECK_CLOSE( B(0,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(0,1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(0,2) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,1) ,  2.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,2) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,2) ,  3.0f, 0.01f);
  
  B = M::make(1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f,9.0f);
  BOOST_CHECK_CLOSE( B(0,0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(0,1) ,  2.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(0,2) ,  3.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,0) ,  4.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,1) ,  5.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1,2) ,  6.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,0) ,  7.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,1) ,  8.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2,2) ,  9.0f, 0.01f);

  B = M::random(0.25f,0.75f);
  BOOST_CHECK( B(0,0) > 0.25f );
  BOOST_CHECK( B(0,0) < 0.75f );
  BOOST_CHECK( B(0,1) > 0.25f );
  BOOST_CHECK( B(0,1) < 0.75f );
  BOOST_CHECK( B(0,2) > 0.25f );
  BOOST_CHECK( B(0,2) < 0.75f );
  BOOST_CHECK( B(1,0) > 0.25f );
  BOOST_CHECK( B(1,0) < 0.75f );
  BOOST_CHECK( B(1,1) > 0.25f );
  BOOST_CHECK( B(1,1) < 0.75f );
  BOOST_CHECK( B(1,2) > 0.25f );
  BOOST_CHECK( B(1,2) < 0.75f );
  BOOST_CHECK( B(2,0) > 0.25f );
  BOOST_CHECK( B(2,0) < 0.75f );
  BOOST_CHECK( B(2,1) > 0.25f );
  BOOST_CHECK( B(2,1) < 0.75f );
  BOOST_CHECK( B(2,2) > 0.25f );
  BOOST_CHECK( B(2,2) < 0.75f );
  
  
  M A = M::Ru( M::value_traits::pi_half(), V::i() );
  B = M::Rx( M::value_traits::pi_half() );
  BOOST_CHECK_CLOSE( A(0,0) , B(0,0), 0.01f);
  BOOST_CHECK_CLOSE( A(0,1) , B(0,1), 0.01f);
  BOOST_CHECK_CLOSE( A(0,2) , B(0,2), 0.01f);
  BOOST_CHECK_CLOSE( A(1,0) , B(1,0), 0.01f);
  BOOST_CHECK_CLOSE( A(1,1) , B(1,1), 0.01f);
  BOOST_CHECK_CLOSE( A(1,2) , B(1,2), 0.01f);
  BOOST_CHECK_CLOSE( A(2,0) , B(2,0), 0.01f);
  BOOST_CHECK_CLOSE( A(2,1) , B(2,1), 0.01f);
  BOOST_CHECK_CLOSE( A(2,2) , B(2,2), 0.01f);
  

  A = M::Ru( M::value_traits::pi_half(), V::j() );
  B = M::Ry( M::value_traits::pi_half() );
  BOOST_CHECK_CLOSE( A(0,0) , B(0,0), 0.01f);
  BOOST_CHECK_CLOSE( A(0,1) , B(0,1), 0.01f);
  BOOST_CHECK_CLOSE( A(0,2) , B(0,2), 0.01f);
  BOOST_CHECK_CLOSE( A(1,0) , B(1,0), 0.01f);
  BOOST_CHECK_CLOSE( A(1,1) , B(1,1), 0.01f);
  BOOST_CHECK_CLOSE( A(1,2) , B(1,2), 0.01f);
  BOOST_CHECK_CLOSE( A(2,0) , B(2,0), 0.01f);
  BOOST_CHECK_CLOSE( A(2,1) , B(2,1), 0.01f);
  BOOST_CHECK_CLOSE( A(2,2) , B(2,2), 0.01f);
  
  A = M::Ru( M::value_traits::pi_half(), V::k() );
  B = M::Rz( M::value_traits::pi_half() );
  BOOST_CHECK_CLOSE( A(0,0) , B(0,0), 0.01f);
  BOOST_CHECK_CLOSE( A(0,1) , B(0,1), 0.01f);
  BOOST_CHECK_CLOSE( A(0,2) , B(0,2), 0.01f);
  BOOST_CHECK_CLOSE( A(1,0) , B(1,0), 0.01f);
  BOOST_CHECK_CLOSE( A(1,1) , B(1,1), 0.01f);
  BOOST_CHECK_CLOSE( A(1,2) , B(1,2), 0.01f);
  BOOST_CHECK_CLOSE( A(2,0) , B(2,0), 0.01f);
  BOOST_CHECK_CLOSE( A(2,1) , B(2,1), 0.01f);
  BOOST_CHECK_CLOSE( A(2,2) , B(2,2), 0.01f);
}  


BOOST_AUTO_TEST_CASE(quaternion_tests)
{	
	typedef tiny::ScalarTraits<float>     T;
  typedef tiny::Matrix<3,3,T>           M;
  typedef tiny::Quaternion<T>           Q;
  
  M A = tiny::make( Q::Rx( M::value_traits::pi_half() ) );
  M B = M::Rx( M::value_traits::pi_half() );
  BOOST_CHECK( std::fabs( A(0,0) - B(0,0) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(0,1) - B(0,1) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(0,2) - B(0,2) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(1,0) - B(1,0) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(1,1) - B(1,1) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(1,2) - B(1,2) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(2,0) - B(2,0) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(2,1) - B(2,1) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(2,2) - B(2,2) ) < 10e-7f);

  A = tiny::make( Q::Ry( M::value_traits::pi_half() ) );
  B = M::Ry( M::value_traits::pi_half() );
  BOOST_CHECK( std::fabs( A(0,0) - B(0,0) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(0,1) - B(0,1) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(0,2) - B(0,2) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(1,0) - B(1,0) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(1,1) - B(1,1) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(1,2) - B(1,2) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(2,0) - B(2,0) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(2,1) - B(2,1) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(2,2) - B(2,2) ) < 10e-7f);
  
  A = tiny::make( Q::Rz( M::value_traits::pi_half() ) );
  B = M::Rz( M::value_traits::pi_half() );
  BOOST_CHECK( std::fabs( A(0,0) - B(0,0) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(0,1) - B(0,1) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(0,2) - B(0,2) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(1,0) - B(1,0) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(1,1) - B(1,1) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(1,2) - B(1,2) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(2,0) - B(2,0) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(2,1) - B(2,1) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(2,2) - B(2,2) ) < 10e-7f);
  
}  

BOOST_AUTO_TEST_SUITE_END();
