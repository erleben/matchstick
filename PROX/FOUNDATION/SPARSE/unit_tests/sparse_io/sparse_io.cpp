#include <sparse.h>
#include <sparse_fill.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

#include <iostream>  //  needed for compile testing output stream operators

BOOST_AUTO_TEST_SUITE(SPARSE);

BOOST_AUTO_TEST_CASE(io_test)
{    
  typedef sparse::Block<4,1,float>   block4_type;
  typedef sparse::Vector<block4_type> vector_type;
  
  vector_type v;
  v.resize(2);
  sparse::fill(v);
  sparse::io::write_matlab_file("vector.m", v);
  
  typedef sparse::Block<4,4,float>   block4x4_type;
  sparse::DiagonalMatrix<block4x4_type> D(5);
  sparse::fill(D);
	sparse::io::write_matlab_file("diagonal.m", D);
  
  
  typedef sparse::Block<4,6,float>   block4x6_type;
  block4x6_type b46;  
  sparse::fill(b46, 46.0f);
  
  sparse::TwoColumnMatrix<block4x6_type> TCM;
  TCM.resize(1,4,2);
  TCM(0,0) = b46;
  TCM(0,3) = b46;
 	sparse::io::write_matlab_file("two_column_matrix.m", TCM);
  
  
  typedef sparse::Block<6,4,float>   block6x4_type;
  block6x4_type b64;
  sparse::fill( b64, 64.0f );
  
  sparse::CompressedRowMatrix<block6x4_type> CRM;
  CRM.resize(5,3,4);
  CRM(0,0) = b64;
  CRM(2,1) = b64;
  CRM(4,1) = b64;
  CRM(4,2) = b64;  
	sparse::io::write_matlab_file("compressed_row_matrix.m", CRM);
  
  {
    using namespace sparse::io;
    
    std::cout << v << std::endl;
    std::cout << D << std::endl;
    std::cout << TCM << std::endl;
    std::cout << CRM << std::endl;
  }
  
}

BOOST_AUTO_TEST_CASE(string_writer_test)
{    
  typedef sparse::Block<2,1,float>                   block2x1_type;
  typedef sparse::Block<2,2,float>                   block2x2_type;
  
  sparse::Vector<block2x1_type> v;
  v.resize(2);
  sparse::fill(v);
  std::string v_expected = "Block at row: 0 => [ 0 ; 1 ] \nBlock at row: 1 => [ 0 ; 1 ] \n";
  std::string v_expected_compressed = "[ 0 ; 1 ] [ 0 ; 1 ] ";
  std::string v_to_string = sparse::io::to_string( v );
  std::string v_to_string_compressed = sparse::io::to_string( v, true );
  BOOST_CHECK( v_expected == v_to_string );
  BOOST_CHECK( v_expected_compressed == v_to_string_compressed );

  sparse::CompressedVector<block2x1_type> c(2,3);
  sparse::fill( c(0) );
  sparse::fill( c(2) );
  std::string c_expected = "Block at row: 0 => [ 0 ; 1 ] \nBlock at row: 2 => [ 0 ; 1 ] \n";
  std::string c_expected_compressed = "0: [ 0 ; 1 ] 2: [ 0 ; 1 ] ";
  std::string c_to_string = sparse::io::to_string( c );
  std::string c_to_string_compressed = sparse::io::to_string( c, true );
  BOOST_CHECK( c_expected == c_to_string );
  BOOST_CHECK( c_expected_compressed == c_to_string_compressed );
  
  sparse::DiagonalMatrix<block2x2_type> D(3);
  sparse::fill(D);
  std::string D_expected = "Block at row: 0, column: 0 => [ 0,1 ; 2,3 ] \nBlock at row: 1, column: 1 => [ 0,1 ; 2,3 ] \nBlock at row: 2, column: 2 => [ 0,1 ; 2,3 ] \n";
  std::string D_expected_compressed = "(0,0): [ 0,1 ; 2,3 ] (1,1): [ 0,1 ; 2,3 ] (2,2): [ 0,1 ; 2,3 ] ";
  std::string D_to_string = sparse::io::to_string( D );
  std::string D_to_string_compressed = sparse::io::to_string( D, true );
  BOOST_CHECK( D_expected == D_to_string );
  BOOST_CHECK( D_expected_compressed == D_to_string_compressed );
  
  sparse::CompressedRowMatrix<block2x2_type> C;
  C.resize(4,4,2);
  C(0,0) = D(0);
  C(0,3) = D(2);
  std::string C_expected = "Block at row: 0, column: 0 => [ 0,1 ; 2,3 ] \nBlock at row: 0, column: 3 => [ 0,1 ; 2,3 ] \n";
  std::string C_expected_compressed = "(0,0): [ 0,1 ; 2,3 ] (0,3): [ 0,1 ; 2,3 ] ";
  std::string C_to_string = sparse::io::to_string( C );
  std::string C_to_string_compressed = sparse::io::to_string( C, true );
  BOOST_CHECK( C_expected == C_to_string );
  BOOST_CHECK( C_expected_compressed == C_to_string_compressed );
}

BOOST_AUTO_TEST_SUITE_END();
