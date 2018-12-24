#include <tiny_type_traits.h>
#include <tiny_container.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(tiny_container);



BOOST_AUTO_TEST_CASE(iterator_test)
{	
	typedef tiny::ScalarTraits<float>                 float_traits;
  typedef tiny::detail::Container<4,4,float_traits> container4x4;
  
  typedef  container4x4::iterator  iterator;
  
  container4x4 C;  
  C(0,0) = 1.0f; C(0,1) = 2.0f; C(0,2) = 3.0f; C(0,3) = 4.0f;
  C(1,0) = 5.0f; C(1,1) = 6.0f; C(1,2) = 7.0f; C(1,3) = 8.0f;
  C(2,0) = 9.0f; C(2,1) = 0.0f; C(2,2) = 0.1f; C(2,3) = 1.1f;
  C(3,0) = 1.2f; C(3,1) = 1.3f; C(3,2) = 1.4f; C(3,3) = 1.5f;
  
  iterator c = C.begin();
  
  BOOST_CHECK_CLOSE( *c, 1.0f, 0.01f );
  BOOST_CHECK( c != C.end()  );
  ++c;
  BOOST_CHECK_CLOSE( *c, 2.0f, 0.01f );
  BOOST_CHECK( c != C.end()  );
  ++c;
  BOOST_CHECK_CLOSE( *c, 3.0f, 0.01f );
  BOOST_CHECK( c != C.end()  );
  ++c;
  BOOST_CHECK_CLOSE( *c, 4.0f, 0.01f );
  BOOST_CHECK( c != C.end()  );
  ++c;
  BOOST_CHECK_CLOSE( *c, 5.0f, 0.01f );
  BOOST_CHECK( c != C.end()  );
  ++c;
  BOOST_CHECK_CLOSE( *c, 6.0f, 0.01f );
  BOOST_CHECK( c != C.end()  );
  ++c;
  BOOST_CHECK_CLOSE( *c, 7.0f, 0.01f );
  BOOST_CHECK( c != C.end()  );
  ++c;
  BOOST_CHECK_CLOSE( *c, 8.0f, 0.01f );
  BOOST_CHECK( c != C.end()  );
  ++c;
  BOOST_CHECK_CLOSE( *c, 9.0f, 0.01f );
  BOOST_CHECK( c != C.end()  );
  ++c;
  BOOST_CHECK_CLOSE( *c, 0.0f, 0.01f );
  BOOST_CHECK( c != C.end()  );
  ++c;
  BOOST_CHECK_CLOSE( *c, 0.1f, 0.01f );
  BOOST_CHECK( c != C.end()  );
  ++c;
  BOOST_CHECK_CLOSE( *c, 1.1f, 0.01f );
  BOOST_CHECK( c != C.end()  );
  ++c;
  BOOST_CHECK_CLOSE( *c, 1.2f, 0.01f );
  BOOST_CHECK( c != C.end()  );
  ++c;
  BOOST_CHECK_CLOSE( *c, 1.3f, 0.01f );
  BOOST_CHECK( c != C.end()  );
  ++c;
  BOOST_CHECK_CLOSE( *c, 1.4f, 0.01f );
  BOOST_CHECK( c != C.end()  );
  ++c;
  BOOST_CHECK_CLOSE( *c, 1.5f, 0.01f );  
  BOOST_CHECK( c != C.end()  );
  ++c;
  BOOST_CHECK( c == C.end()  );
  
}








BOOST_AUTO_TEST_CASE(float4x4_test)
{	
	typedef tiny::ScalarTraits<float>                 float_traits;
  typedef tiny::detail::Container<4,4,float_traits> container4x4;
  typedef container4x4::accessor                   accessor4x4;
  BOOST_CHECK( accessor4x4::stride()    == 1 );
  BOOST_CHECK( accessor4x4::padding()   == 0 );
  BOOST_CHECK( accessor4x4::J_padded()  == 4 );
  BOOST_CHECK( accessor4x4::allocsize() == 16 );
  container4x4 C;  
  C(0,0) = 1.0f; C(0,1) = 2.0f; C(0,2) = 3.0f; C(0,3) = 4.0f;
  C(1,0) = 5.0f; C(1,1) = 6.0f; C(1,2) = 7.0f; C(1,3) = 8.0f;
  C(2,0) = 9.0f; C(2,1) = 0.0f; C(2,2) = 0.1f; C(2,3) = 1.1f;
  C(3,0) = 1.2f; C(3,1) = 1.3f; C(3,2) = 1.4f; C(3,3) = 1.5f;
  BOOST_CHECK_CLOSE( C(0,0), 1.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(0,1), 2.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(0,2), 3.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(0,3), 4.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(1,0), 5.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(1,1), 6.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(1,2), 7.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(1,3), 8.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(2,0), 9.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(2,1), 0.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(2,2), 0.1f, 0.01f );
  BOOST_CHECK_CLOSE( C(2,3), 1.1f, 0.01f );
  BOOST_CHECK_CLOSE( C(3,0), 1.2f, 0.01f );
  BOOST_CHECK_CLOSE( C(3,1), 1.3f, 0.01f );
  BOOST_CHECK_CLOSE( C(3,2), 1.4f, 0.01f );
  BOOST_CHECK_CLOSE( C(3,3), 1.5f, 0.01f );  
}

BOOST_AUTO_TEST_CASE(float1x1_test)
{	
	typedef tiny::ScalarTraits<float>                 float_traits;
  typedef tiny::detail::Container<1,1,float_traits> container1x1;
  typedef container1x1::accessor                   accessor1x1;
  BOOST_CHECK( accessor1x1::stride()    == 1 );
  BOOST_CHECK( accessor1x1::padding()   == 0 );
  BOOST_CHECK( accessor1x1::J_padded()  == 1 );
  BOOST_CHECK( accessor1x1::allocsize() == 1 );
  container1x1 C;
  C(0,0) = 1.0f;
  BOOST_CHECK_CLOSE( C(0,0), 1.0f, 0.01f );  
}

BOOST_AUTO_TEST_CASE(float2x2_test)
{	
	typedef tiny::ScalarTraits<float>                 float_traits;
  typedef tiny::detail::Container<2,2,float_traits> container2x2;
  typedef container2x2::accessor                   accessor2x2;
  BOOST_CHECK( accessor2x2::stride()    == 1 );
  BOOST_CHECK( accessor2x2::padding()   == 0 );
  BOOST_CHECK( accessor2x2::J_padded()  == 2 );
  BOOST_CHECK( accessor2x2::allocsize() == 4 );  
  container2x2  C;
  C(0,0) = 1.0f;
  C(0,1) = 2.0f;
  C(1,0) = 3.0f;
  C(1,1) = 4.0f;
  BOOST_CHECK_CLOSE( C(0,0), 1.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(0,1), 2.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(1,0), 3.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(1,1), 4.0f, 0.01f );  
}

BOOST_AUTO_TEST_CASE(float3x3_test)
{	
	typedef tiny::ScalarTraits<float>                 float_traits;
  typedef tiny::detail::Container<3,3,float_traits> container3x3;
  typedef container3x3::accessor                   accessor3x3;
  BOOST_CHECK( accessor3x3::stride()    == 1 );
  BOOST_CHECK( accessor3x3::padding()   == 0 );
  BOOST_CHECK( accessor3x3::J_padded()  == 3 );
  BOOST_CHECK( accessor3x3::allocsize() == 9 );
  container3x3  C;
  C(0,0) = 1.0f;
  C(0,1) = 2.0f;
  C(0,2) = 3.0f;
  C(1,0) = 4.0f;
  C(1,1) = 5.0f;
  C(1,2) = 6.0f;
  C(2,0) = 7.0f;
  C(2,1) = 8.0f;
  C(2,2) = 9.0f;
  BOOST_CHECK_CLOSE( C(0,0), 1.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(0,1), 2.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(0,2), 3.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(1,0), 4.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(1,1), 5.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(1,2), 6.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(2,0), 7.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(2,1), 8.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(2,2), 9.0f, 0.01f );
}


BOOST_AUTO_TEST_CASE(float1x4_test)
{	
	typedef tiny::ScalarTraits<float>                 float_traits;
  typedef tiny::detail::Container<1,4,float_traits> container1x4;
  typedef container1x4::accessor                   accessor1x4;
  BOOST_CHECK( accessor1x4::stride()    == 1 );
  BOOST_CHECK( accessor1x4::padding()   == 0 );
  BOOST_CHECK( accessor1x4::J_padded()  == 4 );
  BOOST_CHECK( accessor1x4::allocsize() == 4 );
  container1x4  C;
  C(0,0) = 1.0f;
  C(0,1) = 2.0f;
  C(0,2) = 3.0f;
  C(0,3) = 4.0f;
  BOOST_CHECK_CLOSE( C(0,0), 1.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(0,1), 2.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(0,2), 3.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(0,3), 4.0f, 0.01f );
}

BOOST_AUTO_TEST_CASE(float4x1_test)
{	
	typedef tiny::ScalarTraits<float>                 float_traits;
  typedef tiny::detail::Container<4,1,float_traits> container4x1;
  typedef container4x1::accessor                   accessor4x1;
  BOOST_CHECK( accessor4x1::stride()    == 1 );
  BOOST_CHECK( accessor4x1::padding()   == 0 );
  BOOST_CHECK( accessor4x1::J_padded()  == 1 );
  BOOST_CHECK( accessor4x1::allocsize() == 4 );
  container4x1  C;
  C(0,0) = 1.0f;
  C(1,0) = 2.0f;
  C(2,0) = 3.0f;
  C(3,0) = 4.0f;
  BOOST_CHECK_CLOSE( C(0,0), 1.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(1,0), 2.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(2,0), 3.0f, 0.01f );
  BOOST_CHECK_CLOSE( C(3,0), 4.0f, 0.01f );
}

BOOST_AUTO_TEST_SUITE_END();
