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
  typedef sparse::Block<1,1,float> block1x1_type;
  typedef sparse::Block<4,1,float> block4x1_type;
  typedef sparse::Block<3,1,float> block3x1_type;
  typedef sparse::Block<3,4,float> block3x4_type;

  // scalar blocks
  sparse::CompressedRowMatrix< block1x1_type > sA(2,4,4);
  sparse::Vector< block1x1_type > su(4);
  sparse::CompressedVector< block1x1_type > csu(4,4);
  block1x1_type result(0);
  
  sA(0,0)[0] = 1.0f;
  sA(0,3)[0] = 2.0f;
  sA(1,1)[0] = 3.0f;
  sA(1,2)[0] = 4.0f;
  su(0) = 5.0f;
  su(1) = 6.0f;
  su(2) = 7.0f;
  su(3) = 8.0f;
  sparse::row_prod(sA, su, result, 0);
  BOOST_CHECK( result == 21.0f );
  sparse::row_prod(sA, su, result, 1);
  BOOST_CHECK( result == 67.0f ); // 46 + 21
  csu(0) = 5.0f;
  csu(1) = 6.0f;
  csu(2) = 7.0f;
  csu(3) = 8.0f;
  result = 0;
  sparse::row_prod(sA, csu, result, 0);
  BOOST_CHECK( result == 21.0f );
  sparse::row_prod(sA, csu, result, 1);
  BOOST_CHECK( result == 67.0f ); // 46 + 21
  
  // block case
  sparse::CompressedRowMatrix< block3x4_type > A(1,1,1);
  sparse::Vector< block4x1_type > u(1);
  sparse::CompressedVector< block4x1_type > cu(1,1);
  block3x1_type block_result(0);
  
  sparse::fill(A(0,0));
  sparse::fill(u(0));
  sparse::row_prod(A,u,block_result,0);
  BOOST_CHECK( block_result[0] == 14 );
  BOOST_CHECK( block_result[1] == 38 );
  BOOST_CHECK( block_result[2] == 62 );
  sparse::fill(cu(0));
  block_result.clear_data();
  sparse::row_prod(A,cu,block_result,0);
  BOOST_CHECK( block_result[0] == 14 );
  BOOST_CHECK( block_result[1] == 38 );
  BOOST_CHECK( block_result[2] == 62 );
  
  A.clear();
  u.clear();
  A.resize(2,4,4);
  u.resize(4);
  block_result.clear_data();

  sparse::fill(A(0,0),1);
  sparse::fill(A(0,3),2);
  sparse::fill(A(1,1),3);
  sparse::fill(A(1,2),4);
  sparse::fill(u(0),1);
  sparse::fill(u(1),2);
  sparse::fill(u(2),3);
  sparse::fill(u(3),4);
  sparse::row_prod(A, u, block_result, 0);
  BOOST_CHECK( block_result[0] == 112 );
  BOOST_CHECK( block_result[1] == 240 );
  BOOST_CHECK( block_result[2] == 368 );
  cu.clear();
  cu.resize(4,4);
  sparse::fill(cu(0),1);
  sparse::fill(cu(1),2);
  sparse::fill(cu(2),3);
  sparse::fill(cu(3),4);
  block_result.clear_data();
  sparse::row_prod(A, cu, block_result, 0);
  BOOST_CHECK( block_result[0] == 112 );
  BOOST_CHECK( block_result[1] == 240 );
  BOOST_CHECK( block_result[2] == 368 );

  block_result.clear_data();
  sparse::row_prod(A, u, block_result, 1);
  BOOST_CHECK( block_result[0] == 172 );
  BOOST_CHECK( block_result[1] == 300 );
  BOOST_CHECK( block_result[2] == 428 );
  block_result.clear_data();
  sparse::row_prod(A, cu, block_result, 1);
  BOOST_CHECK( block_result[0] == 172 );
  BOOST_CHECK( block_result[1] == 300 );
  BOOST_CHECK( block_result[2] == 428 );

  // test diagonal matrix
  sparse::DiagonalMatrix< block3x4_type > DM(2,2,2);
  sparse::fill(DM(0),1.0f);
  sparse::fill(DM(1),1.0f);
  u.resize(2);
  sparse::fill(u(0),1.0f);
  sparse::fill(u(1),1.0f);
  block_result[0] = 0.0f;
  block_result[1] = 0.0f;
  block_result[2] = 0.0f;
  sparse::row_prod(DM, u, block_result, 0);
  BOOST_CHECK( block_result[0] == 30.0f );
  BOOST_CHECK( block_result[1] == 70.0f );
  BOOST_CHECK( block_result[2] == 110.0f );
  block_result[0] = 0.0f;
  block_result[1] = 0.0f;
  block_result[2] = 0.0f;
  sparse::row_prod(DM, u, block_result, 1);
  BOOST_CHECK( block_result[0] == 30.0f );
  BOOST_CHECK( block_result[1] == 70.0f );
  BOOST_CHECK( block_result[2] == 110.0f );

  // 2 column matrix
  sparse::TwoColumnMatrix< block3x4_type > TCM(2, 4);
  sparse::Vector< block3x1_type > v(2);
  sparse::fill(TCM(0,0),1);
  sparse::fill(TCM(0,1),2);
  sparse::fill(TCM(1,1),3);
  sparse::fill(TCM(1,3),4);
  u.resize(4);
  sparse::fill(u(0),1);
  sparse::fill(u(1),2);
  sparse::fill(u(2),3);
  sparse::fill(u(3),4);
  sparse::prod(TCM,u,v); // compute "correct" data
  block_result[0] = 0.0f;
  block_result[1] = 0.0f;
  block_result[2] = 0.0f;
  sparse::row_prod(TCM,u,block_result,0);
  BOOST_CHECK( block_result == v(0) );
  block_result[0] = 0.0f;
  block_result[1] = 0.0f;
  block_result[2] = 0.0f;
  sparse::row_prod(TCM,u,block_result,1);
  BOOST_CHECK( block_result == v(1) );
}

BOOST_AUTO_TEST_SUITE_END();
