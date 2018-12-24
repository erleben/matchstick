#include <sparse.h>
#include <sparse_fill.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(SPARSE);

BOOST_AUTO_TEST_CASE(product_crm_crm_test)
{    
  // scalar case
  sparse::CompressedRowMatrix<sparse::Block<1,1,float> > sA(1,1,1);
  sparse::CompressedRowMatrix<sparse::Block<1,1,float> > sB(1,1,1);
  sparse::CompressedRowMatrix<sparse::Block<1,1,float> > sC(1,1,1);
  
  sA(0,0)[0] = 5;
  sB(0,0)[0] = 2;
  sparse::prod(sA,sB,sC);
  BOOST_CHECK(sC(0,0)[0] == 5*2);
  
  sA.clear();
  sB.clear();
  sC.clear();
  
  sA.resize(4,3,5);
  sB.resize(3,4,3);
  sparse::fill(sA(0,0),1);
  sparse::fill(sA(0,2),2);
  sparse::fill(sA(2,1),3);
  sparse::fill(sA(3,0),4);
  sparse::fill(sA(3,1),5);
  sparse::fill(sB(0,0),6);
  sparse::fill(sB(1,1),7);
  sparse::fill(sB(2,3),8);
  sparse::prod(sA, sB, sC);
  BOOST_CHECK( sC(0,0)[0] == 6  );
  BOOST_CHECK( sC(0,3)[0] == 16 );
  BOOST_CHECK( sC(2,1)[0] == 21 );
  BOOST_CHECK( sC(3,0)[0] == 24 );
  BOOST_CHECK( sC(3,1)[0] == 35 );
  
  // Observe we do not need to test block case - it has been verified by product_block_test()
  
}

BOOST_AUTO_TEST_SUITE_END();
