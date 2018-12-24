#include <sparse.h>
#include <sparse_fill.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(SPARSE);

BOOST_AUTO_TEST_CASE(product_2cm_vector_test)
{    
  
  typedef sparse::Block<4,1,float> block4_type; 
  typedef sparse::Block<6,1,float> block6_type;
  typedef sparse::Block<4,6,float> block4x6_type;
    
  typedef sparse::TwoColumnMatrix<block4x6_type> matrix_type;
	typedef sparse::Vector<block6_type> vector6_type;
	typedef sparse::Vector<block4_type> vector4_type;
  
	matrix_type J(1,2,2);
  sparse::fill(J(0,0), 1.0f);
	sparse::fill(J(0,1), 2.0f);
  
	vector6_type u(2);
	sparse::fill(u(0), 0.5f);
	sparse::fill(u(1), 6.5f);
	
  vector4_type r(1);
	sparse::prod(J, u, r);
	BOOST_CHECK( r(0)(0) == 341  );
  BOOST_CHECK( r(0)(1) == 773  );
  BOOST_CHECK( r(0)(2) == 1205 );
  BOOST_CHECK( r(0)(3) == 1637 );
  
	J.resize(2,4,4);
  sparse::fill(J(1,1), 3.0f);
	sparse::fill(J(1,2), 4.0f);
	u.resize(4);
	sparse::fill(u(2), 12.5f);
	sparse::fill(u(3), 18.5f);
	r.clear();
	r.resize(2);
	sparse::prod(J, u, r);
	BOOST_CHECK( r(0)(0) == 341  );
  BOOST_CHECK( r(0)(1) == 773  );
  BOOST_CHECK( r(0)(2) == 1205 );
  BOOST_CHECK( r(0)(3) == 1637 );
	BOOST_CHECK( r(1)(0) == 917  );
  BOOST_CHECK( r(1)(1) == 1781 );
  BOOST_CHECK( r(1)(2) == 2645 );
  BOOST_CHECK( r(1)(3) == 3509 );

}

BOOST_AUTO_TEST_SUITE_END();
