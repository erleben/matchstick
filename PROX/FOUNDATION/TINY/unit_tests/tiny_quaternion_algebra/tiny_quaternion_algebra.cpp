#include <tiny_type_traits.h>
#include <tiny_quaternion.h>
#include <tiny_quaternion_functions.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(tiny_quaternion_algebra);

BOOST_AUTO_TEST_CASE(clear_test)
{	
	typedef tiny::ScalarTraits<float>     T;
  typedef tiny::Quaternion<T>           Q;
  
  Q A;
  A.clear();
  BOOST_CHECK_CLOSE( A.real() , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( A.imag()(0) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( A.imag()(1) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( A.imag()(2) , 0.0f, 0.01f);  
  BOOST_CHECK_CLOSE( A(0) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(1) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(2) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(3) , 0.0f, 0.01f);
}

BOOST_AUTO_TEST_CASE(component_indexing_test)
{	
	typedef tiny::ScalarTraits<float>     T;
  typedef tiny::Quaternion<T>           Q;
  
  Q A;
  A.clear();
  
  A(0) = 1.0f;
  A(1) = 2.0f;
  A(2) = 3.0f;
  A(3) = 4.0f;
  
  BOOST_CHECK_CLOSE( A(0) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(1) , 2.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(2) , 3.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(3) , 4.0f, 0.01f);
 
}  

BOOST_AUTO_TEST_CASE(scalar_mul_div_test)
{	
	typedef tiny::ScalarTraits<float>     T;
  typedef tiny::Quaternion<T>           Q;
  
  Q A( 4.0f, 1.0f, 2.0f, 3.0f);
  
  Q C;
  C.clear();
  
  C = A*0.5f;
  BOOST_CHECK_CLOSE( C(0) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , 1.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(3) , 2.0f, 0.01f);
    
  C.clear();
  C = 0.5f*A;
  BOOST_CHECK_CLOSE( C(0) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , 1.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(3) , 2.0f, 0.01f);
  
  C.clear();
  C = A/2.0f;
  BOOST_CHECK_CLOSE( C(0) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , 1.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(3) , 2.0f, 0.01f);
  
}  

BOOST_AUTO_TEST_CASE(diverse_ops_test)
{	
	typedef tiny::ScalarTraits<float>     T;
  typedef tiny::Quaternion<T>           Q;
  typedef tiny::Matrix<3,3,T>           M;
  typedef tiny::Vector<3,T>             V;
  
  Q A( 2.0f, 3.0f, 4.0f, 1.0f);
  
  Q C;
  C = tiny::hat(A);
  BOOST_CHECK_CLOSE( tiny::inner_prod(A,C), 0.0f, 0.01f);
  
  A   = Q::Ru( M::value_traits::pi_half(), V::i() );
  Q B = Q::Rx( M::value_traits::pi_half() );
  BOOST_CHECK_CLOSE( A(0) , B(0), 0.01f);
  BOOST_CHECK_CLOSE( A(1) , B(1), 0.01f);
  BOOST_CHECK_CLOSE( A(2) , B(2), 0.01f);
  BOOST_CHECK_CLOSE( A(3) , B(3), 0.01f);
  A = Q::Ru( M::value_traits::pi_half(), V::j() );
  B = Q::Ry( M::value_traits::pi_half() );
  BOOST_CHECK_CLOSE( A(0) , B(0), 0.01f);
  BOOST_CHECK_CLOSE( A(1) , B(1), 0.01f);
  BOOST_CHECK_CLOSE( A(2) , B(2), 0.01f);
  BOOST_CHECK_CLOSE( A(3) , B(3), 0.01f);
  A = Q::Ru( M::value_traits::pi_half(), V::k() );
  B = Q::Rz( M::value_traits::pi_half() );
  BOOST_CHECK_CLOSE( A(0) , B(0), 0.01f);
  BOOST_CHECK_CLOSE( A(1) , B(1), 0.01f);
  BOOST_CHECK_CLOSE( A(2) , B(2), 0.01f);
  BOOST_CHECK_CLOSE( A(3) , B(3), 0.01f);
  
  A = tiny::make( M::Rx( M::value_traits::pi_half() ) );
  B = Q::Rx( Q::value_traits::pi_half() );
  BOOST_CHECK( std::fabs( A(0) - B(0) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(1) - B(1) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(2) - B(2) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(3) - B(3) ) < 10e-7f);

  A = tiny::make( M::Ry( M::value_traits::pi_half() ) );
  B = Q::Ry( Q::value_traits::pi_half() );
  BOOST_CHECK( std::fabs( A(0) - B(0) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(1) - B(1) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(2) - B(2) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(3) - B(3) ) < 10e-7f);

  A = tiny::make( M::Rz( M::value_traits::pi_half() ) );
  B = Q::Rz( Q::value_traits::pi_half() );
  BOOST_CHECK( std::fabs( A(0) - B(0) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(1) - B(1) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(2) - B(2) ) < 10e-7f);
  BOOST_CHECK( std::fabs( A(3) - B(3) ) < 10e-7f);
}  

BOOST_AUTO_TEST_SUITE_END();
