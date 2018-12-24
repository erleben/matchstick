#include <sparse.h>

#include <sparse_fill.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(SPARSE);

BOOST_AUTO_TEST_CASE(fale_transpose_test)
{  
  typedef sparse::Block<4,6,float> block_type;
	typedef sparse::TwoColumnMatrix<block_type>  tcm_type;
	typedef sparse::CompressedRowMatrix<block_type>   crm_type;
  
	tcm_type J(1, 2, 2);
	block_type b1;
	block_type b2;
  
  sparse::fill(b1, 1.0f);
  sparse::fill(b2, 2.0f);
	J(0,0) = b1;
	J(0,1) = b2;
  
	crm_type JT;
  sparse::fake_transpose(J, JT);
	BOOST_CHECK( JT.size() == 2         );
	BOOST_CHECK( JT.nrows() == J.ncols());
	BOOST_CHECK( JT.ncols() == J.nrows());
	BOOST_CHECK( JT(0,0) == J(0,0)      );
  BOOST_CHECK( JT(1,0) == J(0,1)      );
  
	J.resize(2,4,4);
	block_type b3;
  block_type b4;
  
  sparse::fill(b3, 3.0f);
  sparse::fill(b4, 4.0f);
  
  J(1,0) = b1;
  J(1,3) = b2;
  sparse::fake_transpose(J, JT);
  
  BOOST_CHECK(JT.size() == 4);
  BOOST_CHECK(JT.nrows() == J.ncols());
  BOOST_CHECK(JT.ncols() == J.nrows());
  BOOST_CHECK(JT(0,0) == J(0,0));
  BOOST_CHECK(JT(1,0) == J(0,1));
  BOOST_CHECK(JT(0,1) == J(1,0));
  BOOST_CHECK(JT(3,1) == J(1,3));
  
}

BOOST_AUTO_TEST_SUITE_END();

