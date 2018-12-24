#include <sparse.h>
#include <sparse_fill.h>
#include <cstdlib>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(SPARSE);


BOOST_AUTO_TEST_CASE(sparse_conjugate_gradient_test)
{
  typedef sparse::Block<3,3,float>                    block3x3_type;
  typedef sparse::Block<3,1,float>                    block3x1_type;
  typedef sparse::Vector<block3x1_type>               block3Vector_type;
  typedef sparse::CompressedRowMatrix<block3x3_type>  compressed3x3_type;

  compressed3x3_type A;
  block3Vector_type x;
  block3Vector_type b;
  block3Vector_type expected_x;

  //set up variables
  A.resize(4,4,4);
  x.resize(4);
  b.resize(4);
  expected_x.resize(4);

  for(int i = 0; i < 4; ++i)
  {
    A(i,i) = block3x3_type::identity();
    b(i)= 1;
    expected_x(i)= 1;
  }
  //run conjugate gradient
  conjugate_gradient(A,x,b);
  //test values
  for(size_t i = 0; i < A.size(); ++i)
  {
    BOOST_CHECK(x(i) == expected_x(i));
  }
  //reset the values for use
  A.clear();
  b.clear_data();
  x.clear_data();
  expected_x.clear_data();

  //set up variables
  A.resize(4,4,4,false);
  x.resize(4);
  b.resize(4);
  expected_x.resize(4);

  for(int i = 0; i < 4; ++i)
  {
    A(i,i) = block3x3_type::identity();
    sparse::mul(2.0f,A(i,i));
    b(i)= 1;
    expected_x(i)= 0.5;
  }
  //run conjugate gradient
  conjugate_gradient(A,x,b);
  //test values
  for(size_t i = 0; i < A.size(); ++i)
  {
    BOOST_CHECK(x(i) == expected_x(i));
  }

  //reset the values for use
  A.clear();
  b.clear_data();
  x.clear_data();
  expected_x.clear_data();

  //set up variables
  A.resize(4,4,4,false);
  x.resize(4);
  b.resize(4);
  expected_x.resize(4);

  for(int i = 0; i < 4; ++i)
  {
    A(i,i) = block3x3_type::identity();
    sparse::mul(i+2.0f,A(i,i));
    expected_x(i)= i*2 % 3;
  }
  prod(A,expected_x, b);
  //run conjugate gradient
  conjugate_gradient(A,x,b);
  //test values
  for(size_t i = 0; i < A.size(); ++i)
  {
    BOOST_CHECK(x(i) == expected_x(i));
  }

  //reset the values for use
  A.clear();
  b.clear_data();
  x.clear_data();
  expected_x.clear_data();

  //set up variables
  A.resize(4,4,10);
  x.resize(4);
  b.resize(4);
  expected_x.resize(4);
  //create an upper triangle matrix (with no zeros in diagonal) as it
  //is ensured to have full rank
  for(size_t i = 0; i < 4; ++i)
  {
    for(size_t j = 0; j < 4; ++j)
    {
      sparse::mul(1.0f,block3x3_type::identity(),A(i,j));
    }
    expected_x(i)= 2;
  }
  //calculate b
  sparse::prod(A,expected_x,b);

  //run conjugate gradient
  conjugate_gradient(A,x,b);
  //test values
  for(size_t i = 0; i < A.nrows(); ++i)
  {
    BOOST_CHECK(x(i) == expected_x(i));
  }
  //reset the values for use
  A.clear();
  b.clear_data();
  x.clear_data();
  expected_x.clear_data();

  //set up variables
  A.resize(4,4,10);
  x.resize(4);
  b.resize(4);
  expected_x.resize(4);

  for(size_t i = 0; i < 4; ++i)
  {
    for(size_t j = 0; j < 4; ++j)
    {
      sparse::mul(1.0f,block3x3_type::identity(),A(i,j));
    }
    expected_x(i)= 2;
  }
  //calculate b
  sparse::prod(A,expected_x,b);

  //run conjugate gradient
  conjugate_gradient(A,x,b);
  //test values

  for(size_t i = 0; i < A.nrows(); ++i)
  {
    BOOST_CHECK(x(i) == expected_x(i));
  }

  //reset the values for use
  A.clear();
  b.clear_data();
  x.clear_data();
  expected_x.clear_data();

  A.resize(4,4,16);
  compressed3x3_type P;
  for(size_t i = 0; i < 4; ++i)
  {
    for(size_t j = 0; j < 4; ++j)
    {
      sparse::mul(2.0f,block3x3_type::identity(),A(i,j));
    }
    expected_x(i)= 2;
  }
  sparse::make_jacobi_preconditioner(A,P);
  compressed3x3_type::iterator PIter = P.begin();
  compressed3x3_type::iterator PEnd = P.end();
  for(; PIter < PEnd; ++PIter)
  {
    BOOST_CHECK(*PIter == block3x3_type::make_diag(1.0f/2.0f));
    BOOST_CHECK(row(PIter) == col(PIter));
  }
}

BOOST_AUTO_TEST_SUITE_END();
