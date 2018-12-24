#include <sparse.h>
#include <sparse_fill.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(SPARSE);

BOOST_AUTO_TEST_CASE(diag_of_prod_test)
{    
  
  typedef sparse::Block<4,6,float>  block4x6_type;
  typedef sparse::Block<6,4,float>  block6x4_type;
  typedef sparse::Block<4,4,float>  block4x4_type;
  
  sparse::TwoColumnMatrix< block4x6_type> J(2,4,4);
  sparse::fill(J(0,0), 1.0f);
  sparse::fill(J(0,1), 2.0f);
  sparse::fill(J(1,1), 3.0f);
  sparse::fill(J(1,2), 4.0f);
  
  sparse::CompressedRowMatrix<block6x4_type> JT;
  sparse::transpose(J, JT);
  
  sparse::DiagonalMatrix<block4x4_type> D(2);
  sparse::diag_of_prod(J, JT, D);
  
  BOOST_CHECK( D[0](0,0) == 230  );
  BOOST_CHECK( D[0](0,1) == 518  );
  BOOST_CHECK( D[0](0,2) == 806  );
  BOOST_CHECK( D[0](0,3) == 1094 );
  BOOST_CHECK( D[0](1,0) == 518  );
  BOOST_CHECK( D[0](1,1) == 1238 );
  BOOST_CHECK( D[0](1,2) == 1958 );
  BOOST_CHECK( D[0](1,3) == 2678 );
  BOOST_CHECK( D[0](2,0) == 806  );
  BOOST_CHECK( D[0](2,1) == 1958 );
  BOOST_CHECK( D[0](2,2) == 3110 );
  BOOST_CHECK( D[0](2,3) == 4262 );
  BOOST_CHECK( D[0](3,0) == 1094 );
  BOOST_CHECK( D[0](3,1) == 2678 );
  BOOST_CHECK( D[0](3,2) == 4262 );
  BOOST_CHECK( D[0](3,3) == 5846 );
  
  BOOST_CHECK( D[1](0,0) == 470  );
  BOOST_CHECK( D[1](0,1) == 902  );
  BOOST_CHECK( D[1](0,2) == 1334 );
  BOOST_CHECK( D[1](0,3) == 1766 );
  BOOST_CHECK( D[1](1,0) == 902  );
  BOOST_CHECK( D[1](1,1) == 1766 );
  BOOST_CHECK( D[1](1,2) == 2630 );
  BOOST_CHECK( D[1](1,3) == 3494 );
  BOOST_CHECK( D[1](2,0) == 1334 );
  BOOST_CHECK( D[1](2,1) == 2630 );
  BOOST_CHECK( D[1](2,2) == 3926 );
  BOOST_CHECK( D[1](2,3) == 5222 );
  BOOST_CHECK( D[1](3,0) == 1766 );
  BOOST_CHECK( D[1](3,1) == 3494 );
  BOOST_CHECK( D[1](3,2) == 5222 );
  BOOST_CHECK( D[1](3,3) == 6950 );
  
}

BOOST_AUTO_TEST_SUITE_END();
