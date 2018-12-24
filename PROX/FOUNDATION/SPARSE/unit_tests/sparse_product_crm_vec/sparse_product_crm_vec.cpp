#include <sparse.h>
#include <sparse_fill.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(SPARSE);

BOOST_AUTO_TEST_CASE(product_crm_vector_test)
{    
  // scalar case
  
  sparse::CompressedRowMatrix<sparse::Block<1,1,float> > sA(1,1,1);
  sparse::Vector<sparse::Block<1,1,float> > su(1);
  sparse::Vector<sparse::Block<1,1,float> > sv(1);
  
  sA(0,0)[0] = 5;
  su(0)[0] = 2;
  sparse::prod(sA, su, sv);
  assert( sv(0)[0] == 2*5 );
  
  sA.clear();
  su.clear();
  sv.clear();
  sA.resize(2,4,4);
  su.resize(4);
  sv.resize(2);
  sA(0,0)[0] = 1;
  sA(0,3)[0] = 2;
  sA(1,1)[0] = 3;
  sA(1,2)[0] = 4;
  su(0) = 5;
  su(1) = 6;
  su(2) = 7;
  su(3) = 8;
  sparse::prod(sA,su,sv);
  BOOST_CHECK( sv(0)[0] == 21 );
  BOOST_CHECK( sv(1)[0] == 46 );
  
  // block case
  
  sparse::CompressedRowMatrix<sparse::Block<3,4,float> > A(1,1,1);
  sparse::Vector<sparse::Block<4,1,float> > u(1);
  sparse::Vector<sparse::Block<3,1,float> > v(1);
  
  sparse::fill(A(0,0));
  sparse::fill(u(0));
  sparse::prod(A,u,v);
  BOOST_CHECK( v(0)[0] == 14 );
  BOOST_CHECK( v(0)[1] == 38 );
  BOOST_CHECK( v(0)[2] == 62 );
  
  A.clear();
  u.clear();
  v.clear();
  A.resize(2,4,4);
  u.resize(4);
  v.resize(2);
  sparse::fill(A(0,0),1);
  sparse::fill(A(0,3),2);
  sparse::fill(A(1,1),3);
  sparse::fill(A(1,2),4);
  sparse::fill(u(0),1);
  sparse::fill(u(1),2);
  sparse::fill(u(2),3);
  sparse::fill(u(3),4);
  sparse::prod(A,u,v);
  BOOST_CHECK( v(0)[0] == 112 );
  BOOST_CHECK( v(0)[1] == 240 );
  BOOST_CHECK( v(0)[2] == 368 );
  BOOST_CHECK( v(1)[0] == 172 );
  BOOST_CHECK( v(1)[1] == 300 );
  BOOST_CHECK( v(1)[2] == 428 );
  
}

BOOST_AUTO_TEST_SUITE_END();
