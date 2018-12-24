#include <tiny_type_traits.h>
#include <tiny_vector.h>
#include <tiny_vector_functions.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(tiny_vector_algebra);

BOOST_AUTO_TEST_CASE(clear_test)
{	
	typedef tiny::ScalarTraits<float>     T;
  typedef tiny::Vector<3,T>             V;
  
  V A;
  A.clear();
  BOOST_CHECK_CLOSE( A(0) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(1) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(2) , 0.0f, 0.01f);
}

BOOST_AUTO_TEST_CASE(component_indexing_test)
{	
	typedef tiny::ScalarTraits<float>     T;
  typedef tiny::Vector<3,T>             V;
  
  V A;
  A.clear();
  
  A(0) = 1.0f;
  A(1) = 2.0f;
  A(2) = 3.0f;
  
  BOOST_CHECK_CLOSE( A(0) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(1) , 2.0f, 0.01f);
  BOOST_CHECK_CLOSE( A(2) , 3.0f, 0.01f);
  
  V B = A;
  BOOST_CHECK( (A==B) == true );
  B(0) += 0.1f;
  BOOST_CHECK( (A==B) == false );
}  


BOOST_AUTO_TEST_CASE(single_value_construction_test)
{	
	typedef tiny::ScalarTraits<float>   T;
  typedef tiny::Vector<3,T>           V;
  
  V B( 0.5f );
  
  BOOST_CHECK_CLOSE( B(0) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( B(1) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( B(2) , 0.5f, 0.01f);
}  

BOOST_AUTO_TEST_CASE(add_sub_test)
{	
	typedef tiny::ScalarTraits<float>   T;
  typedef tiny::Vector<3,T>           V;
  
  V A;
  A(0) = 1.0f;
  A(1) = 2.0f;
  A(2) = 3.0f;
  
  V B( 0.5f );
  V C;
  C.clear();
  
  C = A;
  BOOST_CHECK_CLOSE( C(0) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , 2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , 3.0f, 0.01f);
    
  C = A + B;
  BOOST_CHECK_CLOSE( C(0) , 1.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , 2.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , 3.5f, 0.01f);
  
  C += B;
  BOOST_CHECK_CLOSE( C(0) , 2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , 3.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , 4.0f, 0.01f);

  C -= A;
  BOOST_CHECK_CLOSE( C(0) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , 1.0f, 0.01f);
  
  C = A - B;
  BOOST_CHECK_CLOSE( C(0) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , 1.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , 2.5f, 0.01f);

  C = C;
  BOOST_CHECK_CLOSE( C(0) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , 1.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , 2.5f, 0.01f);

  C = C - B;
  BOOST_CHECK_CLOSE( C(0) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , 2.0f, 0.01f);
  
  C = C + B;
  BOOST_CHECK_CLOSE( C(0) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , 1.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , 2.5f, 0.01f);
  
  C = B - C;
  BOOST_CHECK_CLOSE( C(0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , -1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , -2.0f, 0.01f);

  C = B + C;
  BOOST_CHECK_CLOSE( C(0) ,  0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , -0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , -1.5f, 0.01f);

  C = B;
  C += C;
  BOOST_CHECK_CLOSE( C(0) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , 1.0f, 0.01f);
  
  C -= C;
  BOOST_CHECK_CLOSE( C(0) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , 0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , 0.0f, 0.01f);
  
  C = -B;
  BOOST_CHECK_CLOSE( C(0) , -0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , -0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , -0.5f, 0.01f);  
}

BOOST_AUTO_TEST_CASE(scalar_mul_div_test)
{	
	typedef tiny::ScalarTraits<float>   T;
  typedef tiny::Vector<3,T>           V;
  
  V A;
  A(0) = 1.0f;
  A(1) = 2.0f;
  A(2) = 3.0f;
  
  V C;
  C.clear();
  
  C = A*0.5f;
  BOOST_CHECK_CLOSE( C(0) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , 1.5f, 0.01f);
    
  C *= 2.0f;
  BOOST_CHECK_CLOSE( C(0) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , 2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , 3.0f, 0.01f);
  
  C /= 2.0f;
  BOOST_CHECK_CLOSE( C(0) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , 1.5f, 0.01f);
  
  C.clear();
  C = 0.5f*A;
  BOOST_CHECK_CLOSE( C(0) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , 1.5f, 0.01f);
  
  C.clear();
  C = A/2.0f;
  BOOST_CHECK_CLOSE( C(0) , 0.5f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , 1.5f, 0.01f);
  
//  C.clear();
//  C = 2.0f/A;  // 2009-07-13 Kenny: Hmmm a bit strange!
//  BOOST_CHECK_CLOSE( C(0) , 0.5f, 0.01f);
//  BOOST_CHECK_CLOSE( C(1) , 1.0f, 0.01f);
//  BOOST_CHECK_CLOSE( C(2) , 1.5f, 0.01f);  
}  

BOOST_AUTO_TEST_CASE(diverse_ops_test)
{	
	typedef tiny::ScalarTraits<float>   T;
  typedef tiny::Vector<3,T>           V;
  
  V A;
  A(0) =  1.0f;
  A(1) = -2.0f;
  A(2) =  3.0f;
  
  V B = tiny::abs(A);
  BOOST_CHECK_CLOSE( B(0) , 1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1) , 2.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2) , 3.0f, 0.01f);
  
  BOOST_CHECK_CLOSE( tiny::min(A) ,   -2.0f, 0.01f);
  BOOST_CHECK_CLOSE( tiny::max(A) ,    3.0f, 0.01f);
  BOOST_CHECK_CLOSE( tiny::norm_1(A) , 3.0f, 0.01f);
  
  A(0) =   1.0f;
  A(1) =   2.0f;
  A(2) =  -3.0f;
  BOOST_CHECK_CLOSE( tiny::min(A) ,   -3.0f, 0.01f);
  BOOST_CHECK_CLOSE( tiny::max(A) ,    2.0f, 0.01f);
  BOOST_CHECK_CLOSE( tiny::norm_1(A) , 3.0f, 0.01f);
  
  B = tiny::sign(A);
  BOOST_CHECK_CLOSE( B(0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2) ,  -1.0f, 0.01f);
  
  A(0) = 1.1f;
  A(1) = -2.1f;
  A(2) = 3.1f;
  B = tiny::round(A);
  BOOST_CHECK_CLOSE( B(0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(1) , -2.0f, 0.01f);
  BOOST_CHECK_CLOSE( B(2) ,  3.0f, 0.01f);
  
  A(0) =  1.0f;
  A(1) =  2.0f;
  A(2) =  3.0f;
  
  B(0) = -1.0f;
  B(1) = -2.0f;
  B(2) = -3.0f;
  
  V C;
  C = tiny::min(A,B);
  BOOST_CHECK_CLOSE( C(0) , -1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) , -2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) , -3.0f, 0.01f);
  
  C = tiny::max(A,B);
  BOOST_CHECK_CLOSE( C(0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) ,  2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) ,  3.0f, 0.01f);
  
  BOOST_CHECK_CLOSE( tiny::sum(A), 6.0f, 0.01f);
  
  A(0) =  1.1f;
  A(1) =  2.2f;
  A(2) = 3.3f;
  C = tiny::floor(A);
  BOOST_CHECK_CLOSE( C(0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) ,  2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) ,  3.0f, 0.01f);
  
  C = tiny::ceil(A);
  BOOST_CHECK_CLOSE( C(0) ,  2.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) ,  3.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) ,  4.0f, 0.01f);
  
  A(0) =  1.0f;
  A(1) =  0.0f;
  A(2) =  0.0f;
  C = tiny::unit( A );
  BOOST_CHECK_CLOSE( C(0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) ,  0.0f, 0.01f);

  A(0) =  0.0f;
  A(1) =  0.0f;
  A(2) =  0.0f;
  C = tiny::unit( A );
  BOOST_CHECK_CLOSE( C(0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) ,  0.0f, 0.01f);
  
}  

BOOST_AUTO_TEST_CASE(comparison_tests)
{	
	typedef tiny::ScalarTraits<float>   T;
  typedef tiny::Vector<3,T>           V;
  
  V A;
  A(0) = 1.0f;
  A(1) = 2.0f;
  A(2) = 3.0f;
  
  V B;
  B(0) = 1.0f;
  B(1) = 2.0f;
  B(2) = 3.0f;
  
  V C;
  C = tiny::equal(A , B);
  BOOST_CHECK_CLOSE( C(0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) ,  1.0f, 0.01f);
  
  C = tiny::equal(A , A);
  BOOST_CHECK_CLOSE( C(0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) ,  1.0f, 0.01f);
  
  B(0) = 10.0f;
  B(1) = 2.0f;
  B(2) = 3.0f;
  
  C = tiny::equal(A , B);
  BOOST_CHECK_CLOSE( C(0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) ,  1.0f, 0.01f);
  
  C = tiny::not_equal(A , B);
  BOOST_CHECK_CLOSE( C(0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) ,  0.0f, 0.01f);
  
  C = (A < B);
  BOOST_CHECK_CLOSE( C(0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) ,  0.0f, 0.01f);
  
  C = (A <= B);
  BOOST_CHECK_CLOSE( C(0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) ,  1.0f, 0.01f);
  
  C = (A > B);
  BOOST_CHECK_CLOSE( C(0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) ,  0.0f, 0.01f);
  
  C = (A >= B);
  BOOST_CHECK_CLOSE( C(0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( C(2) ,  1.0f, 0.01f);
}

BOOST_AUTO_TEST_CASE(factory_tests)
{	
	typedef tiny::ScalarTraits<float>   T;
  typedef tiny::Vector<3,T>           V;

  V v;
  v = V::make(1.0,2.0,3.0);
  BOOST_CHECK_CLOSE( v(0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( v(1) ,  2.0f, 0.01f);
  BOOST_CHECK_CLOSE( v(2) ,  3.0f, 0.01f);
  
  v = V::i();
  BOOST_CHECK_CLOSE( v(0) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( v(1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( v(2) ,  0.0f, 0.01f);
  
  v = V::j();
  BOOST_CHECK_CLOSE( v(0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( v(1) ,  1.0f, 0.01f);
  BOOST_CHECK_CLOSE( v(2) ,  0.0f, 0.01f);
  
  v = V::k();
  BOOST_CHECK_CLOSE( v(0) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( v(1) ,  0.0f, 0.01f);
  BOOST_CHECK_CLOSE( v(2) ,  1.0f, 0.01f);
  
  v = V::random(0.5f,0.8f);
  BOOST_CHECK( v(0) > 0.5f );
  BOOST_CHECK( v(0) < 0.8f );
  BOOST_CHECK( v(1) > 0.5f );
  BOOST_CHECK( v(1) < 0.8f );
  BOOST_CHECK( v(2) > 0.5f );
  BOOST_CHECK( v(2) < 0.8f );
}  

BOOST_AUTO_TEST_SUITE_END();
