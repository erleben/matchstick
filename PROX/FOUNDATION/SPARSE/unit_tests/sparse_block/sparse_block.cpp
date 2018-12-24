#include <sparse.h>
#include <sparse_fill.h>
#include <cmath>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

inline bool is_not_zero(float const & f){return f != float(0);}

BOOST_AUTO_TEST_SUITE(SPARSE);

BOOST_AUTO_TEST_CASE(block_test)
{
  
  typedef sparse::Block<4,6,float> block_type;
  typedef sparse::Block<4,1,float> block_vector_type;
  typedef sparse::Block<3,1,float> block_vector3_type;
  typedef sparse::Block<3,3,float> block_matrix3x3_type;
  block_type b(0); // zero-init
  block_matrix3x3_type const  I = block_matrix3x3_type::identity(); //identity matrix

  BOOST_CHECK( block_type::nrows() == 4);
  BOOST_CHECK( block_type::ncols() == 6 );
  BOOST_CHECK( block_type::size() == 24 );
  // 2009-07-01 Kenny: Comparision between signed and unsigned integer expressions?
  BOOST_CHECK( std::distance(b.begin(), b.end()) == block_type::size());
  // is data initialised to float(0)
  BOOST_CHECK(std::find_if(b.begin(), b.end(), is_not_zero) == b.end());
  
  b(0,0) = 0.5f;
  b(1,0) = 1.5f;
  block_type b_copy(b);

  BOOST_CHECK(b == b_copy);
	
  b_copy(0,1) = 2.5f;
  b_copy(1,1) = 3.5f;
  b_copy(3,5) = 4.5f;
	
  BOOST_CHECK( b_copy(0,0) == 0.5f);
  BOOST_CHECK( b_copy(1,0) == 1.5f);
  BOOST_CHECK( b_copy(0,1) == 2.5f);
  BOOST_CHECK( b_copy(1,1) == 3.5f);
  BOOST_CHECK( b_copy(3,5) == 4.5f);
  BOOST_CHECK( b != b_copy);
  
  b = b_copy;
	
  BOOST_CHECK(b == b_copy);
  BOOST_CHECK(b[1] == b_copy(0,1));
  BOOST_CHECK(b[block_type::ncols()] == b_copy(1,0));
  BOOST_CHECK(b[1+block_type::ncols()] == b_copy(1,1));

  // test general block add and sub
  block_type b2(0);
  add(b, b_copy, b2);
  BOOST_CHECK( b2(0,0) == 1.0f );
  BOOST_CHECK( b2(0,1) == 5.0f );
  BOOST_CHECK( b2(1,0) == 3.0f );
  BOOST_CHECK( b2(1,1) == 7.0f );
  BOOST_CHECK( b2(3,5) == 9.0f );
  BOOST_CHECK( b2(0,2) == 0.0f && b2(0,3) == 0.0f && b2(0,4) == 0.0f && b2(0,5) == 0.0f );
  BOOST_CHECK( b2(1,2) == 0.0f && b2(1,3) == 0.0f && b2(1,4) == 0.0f && b2(1,5) == 0.0f );
  BOOST_CHECK( b2(2,0) == 0.0f && b2(2,1) == 0.0f && b2(2,2) == 0.0f && b2(2,3) == 0.0f && b2(2,4) == 0.0f && b2(2,5) == 0.0f );
  BOOST_CHECK( b2(3,0) == 0.0f && b2(3,1) == 0.0f && b2(3,2) == 0.0f && b2(3,2) == 0.0f && b2(3,3) == 0.0f && b2(3,4) == 0.0f );
  sub(b2,b_copy,b2);
  BOOST_CHECK( b2 == b );
  // test block += and -=
  sub(b,b2);
  BOOST_CHECK( b2(0,0) == 0.0f && b2(0,1) == 0.0f && b2(0,2) == 0.0f && b2(0,3) == 0.0f && b2(0,4) == 0.0f && b2(0,5) == 0.0f );
  BOOST_CHECK( b2(1,0) == 0.0f && b2(1,1) == 0.0f && b2(1,2) == 0.0f && b2(1,3) == 0.0f && b2(1,4) == 0.0f && b2(1,5) == 0.0f );
  BOOST_CHECK( b2(2,0) == 0.0f && b2(2,1) == 0.0f && b2(2,2) == 0.0f && b2(2,3) == 0.0f && b2(2,4) == 0.0f && b2(2,5) == 0.0f );
  BOOST_CHECK( b2(3,0) == 0.0f && b2(3,1) == 0.0f && b2(3,2) == 0.0f && b2(3,2) == 0.0f && b2(3,3) == 0.0f && b2(3,4) == 0.0f );
  add(b,b2);
  BOOST_CHECK( b2 == b );
  // test block scalar product b2 *= 2
  scalar_prod(2.0f, b2);
  BOOST_CHECK( b2(0,0) == 1.0f );
  BOOST_CHECK( b2(0,1) == 5.0f );
  BOOST_CHECK( b2(1,0) == 3.0f );
  BOOST_CHECK( b2(1,1) == 7.0f );
  BOOST_CHECK( b2(3,5) == 9.0f );
  BOOST_CHECK( b2(0,2) == 0.0f && b2(0,3) == 0.0f && b2(0,4) == 0.0f && b2(0,5) == 0.0f );
  BOOST_CHECK( b2(1,2) == 0.0f && b2(1,3) == 0.0f && b2(1,4) == 0.0f && b2(1,5) == 0.0f );
  BOOST_CHECK( b2(2,0) == 0.0f && b2(2,1) == 0.0f && b2(2,2) == 0.0f && b2(2,3) == 0.0f && b2(2,4) == 0.0f && b2(2,5) == 0.0f );
  BOOST_CHECK( b2(3,0) == 0.0f && b2(3,1) == 0.0f && b2(3,2) == 0.0f && b2(3,2) == 0.0f && b2(3,3) == 0.0f && b2(3,4) == 0.0f );
  // test block scalar product b2 = b * 0.5
  scalar_prod( 0.5f, b, b2 );
  BOOST_CHECK( b2(0,0) == 0.25f );
  BOOST_CHECK( b2(0,1) == 1.25f );
  BOOST_CHECK( b2(1,0) == 0.75f );
  BOOST_CHECK( b2(1,1) == 1.75f );
  BOOST_CHECK( b2(3,5) == 2.25f );
  BOOST_CHECK( b2(0,2) == 0.0f && b2(0,3) == 0.0f && b2(0,4) == 0.0f && b2(0,5) == 0.0f );
  BOOST_CHECK( b2(1,2) == 0.0f && b2(1,3) == 0.0f && b2(1,4) == 0.0f && b2(1,5) == 0.0f );
  BOOST_CHECK( b2(2,0) == 0.0f && b2(2,1) == 0.0f && b2(2,2) == 0.0f && b2(2,3) == 0.0f && b2(2,4) == 0.0f && b2(2,5) == 0.0f );
  BOOST_CHECK( b2(3,0) == 0.0f && b2(3,1) == 0.0f && b2(3,2) == 0.0f && b2(3,2) == 0.0f && b2(3,3) == 0.0f && b2(3,4) == 0.0f );
  BOOST_CHECK( b == b_copy );

  // test clear of block
  b.clear_data();
  block_type zero_block(0); // zero-init
  BOOST_CHECK( b == zero_block );

  // test general block vector type add and sub
  block_vector_type v1(0);
  block_vector_type v2(0);
  v1(0) = 1.0f;
  v1(1) = 2.0f;
  v1(2) = 3.0f;
  v1(3) = 4.0f;
  v2(0) = 1.5f;
  v2(1) = 2.5f;
  v2(2) = -3.5f;
  v2(3) = 4.5f;
  add(v1,v2,v1);
  BOOST_CHECK( v1(0) == 2.5f);
  BOOST_CHECK( v1(1) == 4.5f);
  BOOST_CHECK( v1(2) == -0.5f);
  BOOST_CHECK( v1(3) == 8.5f);
  sub(v1,v2,v1);
  BOOST_CHECK( v1(0) == 1.0f);
  BOOST_CHECK( v1(1) == 2.0f);
  BOOST_CHECK( v1(2) == 3.0f);
  BOOST_CHECK( v1(3) == 4.0f);
  // test vector block += and -=
  sub(v2,v1);
  BOOST_CHECK( v1(0) == -0.5f);
  BOOST_CHECK( v1(1) == -0.5f);
  BOOST_CHECK( v1(2) == 6.5f);
  BOOST_CHECK( v1(3) == -0.5f);
  add(v2,v1);
  BOOST_CHECK( v1(0) == 1.0f);
  BOOST_CHECK( v1(1) == 2.0f);
  BOOST_CHECK( v1(2) == 3.0f);
  BOOST_CHECK( v1(3) == 4.0f);

  //test b = A*v3
  block_vector3_type v3(1);
  block_matrix3x3_type A(0);
  prod(A,v3);
  BOOST_CHECK( v3(0) == 0.0f);
  BOOST_CHECK( v3(1) == 0.0f);
  BOOST_CHECK( v3(2) == 0.0f);
  A = block_matrix3x3_type::identity();
  prod(A,v3);
  BOOST_CHECK( v3(0) == 0.0f);
  BOOST_CHECK( v3(1) == 0.0f);
  BOOST_CHECK( v3(2) == 0.0f);
  v3 = block_vector3_type(1.0f);
  prod(A,v3);
  BOOST_CHECK( v3(0) == 1.0f);
  BOOST_CHECK( v3(1) == 1.0f);
  BOOST_CHECK( v3(2) == 1.0f);
  A = block_matrix3x3_type(1.0f);
  prod(A,v3);
  BOOST_CHECK( v3(0) == 3.0f);
  BOOST_CHECK( v3(1) == 3.0f);
  BOOST_CHECK( v3(2) == 3.0f);
  //dot product
  block_vector3_type v4(1);
  float dotres;
  dot(v3,v3,dotres);
  BOOST_CHECK( dotres == 27.0f);
  dot(v3,v4,dotres);
  BOOST_CHECK( dotres == 9.0f);
  v3 = v4 = block_vector3_type::i();
  dot(v3,v4,dotres);
  BOOST_CHECK( dotres == 1.0f);
  v3 = v4 = block_vector3_type::j();
  dot(v3,v4,dotres);
  BOOST_CHECK( dotres == 1.0f);
  v3 = v4 = block_vector3_type::k();
  dot(v3,v4,dotres);
  BOOST_CHECK( dotres == 1.0f);
  v3[0] = 1; v3[1] = 2; v3[2] = 3;
  v4[0] = 4; v4[1] = 5; v4[2] = 6;
  dot(v3,v4,dotres);
  BOOST_CHECK( dotres == 32.0f);

  //cross product
  block_vector3_type crossres;
  v3 = block_vector3_type::i();
  cross(v3,v3,crossres);
  BOOST_CHECK( crossres(0) == 0.0f);
  BOOST_CHECK( crossres(1) == 0.0f);
  BOOST_CHECK( crossres(2) == 0.0f);
  v4 = block_vector3_type::j();
  cross(v3,v4,crossres);
  BOOST_CHECK( crossres(0) == 0.0f);
  BOOST_CHECK( crossres(1) == 0.0f);
  BOOST_CHECK( crossres(2) == 1.0f);
  v4 = block_vector3_type::k();
  cross(v3,v4,crossres);
  BOOST_CHECK( crossres(0) == 0.0f);
  BOOST_CHECK( crossres(1) == -1.0f);
  BOOST_CHECK( crossres(2) == 0.0f);
  v3 = block_vector3_type::j();
  cross(v3,v4,crossres);
  BOOST_CHECK( crossres(0) == 1.0f);
  BOOST_CHECK( crossres(1) == 0.0f);
  BOOST_CHECK( crossres(2) == 0.0f);
  
  //length
  BOOST_CHECK( length(v3) == 1.0f);
  BOOST_CHECK( length(v4) == 1.0f);
  v3 = block_vector3_type(1.0f);
  BOOST_CHECK( length(v3) == std::sqrt(3.0f));
  v3[0] = 3; v3[1] = 4; v3[2] = 0; 
  BOOST_CHECK( length(v3) == 5.0f);

  //unit
  unit(v3,v4);
  unit(v3);
  BOOST_CHECK( length(v3) == 1.0f);
  BOOST_CHECK( length(v4) == 1.0f);

  v3[0] = 3.5; v3[1] = 4.2; v3[2] = 2.4; 
  unit(v3);
  BOOST_CHECK( length(v3) == 1.0f);
  BOOST_CHECK( length(v4) == 1.0f);

  v3[0] = 10; v3[1] = 0; v3[2] = 0; 
  unit(v3,v4);
  unit(v3);
  BOOST_CHECK( v3(0) == 1.0f);
  BOOST_CHECK( v3(1) == 0.0f);
  BOOST_CHECK( v3(2) == 0.0f);
  BOOST_CHECK( v4(0) == 1.0f);
  BOOST_CHECK( v4(1) == 0.0f);
  BOOST_CHECK( v4(2) == 0.0f);

  v3[0] = 5; v3[1] = 5; v3[2] = 5; 
  unit(v3,v4);
  unit(v3);
  BOOST_CHECK( v3(0) == std::sqrt(3.0f)/3.0f);
  BOOST_CHECK( v3(1) == std::sqrt(3.0f)/3.0f);
  BOOST_CHECK( v3(2) == std::sqrt(3.0f)/3.0f);
  BOOST_CHECK( v4(0) == std::sqrt(3.0f)/3.0f);
  BOOST_CHECK( v4(1) == std::sqrt(3.0f)/3.0f);
  BOOST_CHECK( v4(2) == std::sqrt(3.0f)/3.0f);

  v3[0] = 9; v3[1] = 9; v3[2] = 0; 
  unit(v3,v4);
  unit(v3);
  BOOST_CHECK( v3(0) == std::sqrt(2.0f)/2.0f);
  BOOST_CHECK( v3(1) == std::sqrt(2.0f)/2.0f);
  BOOST_CHECK( v3(2) == 0.0f);
  BOOST_CHECK( v4(0) == std::sqrt(2.0f)/2.0f);
  BOOST_CHECK( v4(1) == std::sqrt(2.0f)/2.0f);
  BOOST_CHECK( v4(2) == 0.0f);


  //transpose 
  block_matrix3x3_type B;
  A (0,0) = 0;    A (0,1) = 1;    A (0,2) = 2;
  A (1,0) = 10;   A (1,1) = 11;   A (1,2) = 12;
  A (2,0) = 20;   A (2,1) = 21;   A (2,2) = 22;
  B (0,0) = 0;    B (0,1) = 10;   B (0,2) = 20;
  B (1,0) = 1;    B (1,1) = 11;   B (1,2) = 21;
  B (2,0) = 2;    B (2,1) = 12;   B (2,2) = 22;
  transpose(A);
  BOOST_CHECK( A == B);


  //div
  A(0,0) = 1;   A(0,1) = 2;   A(0,2) = 3;
  A(1,0) = 4;   A(1,1) = 5;   A(1,2) = 6;
  A(2,0) = 7;   A(2,1) = 8;   A(2,2) = 9;
  div(2.0f,A,B);
  div(2.0f,A);
  BOOST_CHECK( A(0,0) == 0.5f); BOOST_CHECK( A(0,1) == 1.0f); BOOST_CHECK( A(0,2) == 1.5f);
  BOOST_CHECK( A(1,0) == 2.0f); BOOST_CHECK( A(1,1) == 2.5f); BOOST_CHECK( A(1,2) == 3.0f);
  BOOST_CHECK( A(2,0) == 3.5f); BOOST_CHECK( A(2,1) == 4.0f); BOOST_CHECK( A(2,2) == 4.5f);
  BOOST_CHECK( B(0,0) == 0.5f); BOOST_CHECK( B(0,1) == 1.0f); BOOST_CHECK( B(0,2) == 1.5f);
  BOOST_CHECK( B(1,0) == 2.0f); BOOST_CHECK( B(1,1) == 2.5f); BOOST_CHECK( B(1,2) == 3.0f);
  BOOST_CHECK( B(2,0) == 3.5f); BOOST_CHECK( B(2,1) == 4.0f); BOOST_CHECK( B(2,2) == 4.5f);
  
  //inverse for 3x3 blocks and prod(A,B)
  A = I;
  B = A;
  inverse(A);
  prod(A,B);
  BOOST_CHECK( B == I);

  A (0,0) = 1;   A (0,1) = 1;   A (0,2) = 1;
  A (1,0) = 0;   A (1,1) = 1;   A (1,2) = 1;
  A (2,0) = 0;   A (2,1) = 0;   A (2,2) = 1;
  B = A;

  inverse(A);
  prod(A,B);
  BOOST_CHECK( B == I);


  float tol = 10e-6f;
  A (0,0) = 1;   A (0,1) = 2;   A (0,2) = 3;
  A (1,0) = 3;   A (1,1) = 2;   A (1,2) = 1;
  A (2,0) = 2;   A (2,1) = 1;   A (2,2) = 3;
  B = A;
  inverse(A);
  prod(A,B);
  BOOST_CHECK_SMALL( B(0,0) - I(0,0), tol); 
  BOOST_CHECK_SMALL( B(0,1) - I(0,1), tol); 
  BOOST_CHECK_SMALL( B(0,2) - I(0,2), tol);
  BOOST_CHECK_SMALL( B(1,0) - I(1,0), tol); 
  BOOST_CHECK_SMALL( B(1,1) - I(1,1), tol); 
  BOOST_CHECK_SMALL( B(1,2) - I(1,2), tol);
  BOOST_CHECK_SMALL( B(2,0) - I(2,0), tol); 
  BOOST_CHECK_SMALL( B(2,1) - I(2,1), tol);
  BOOST_CHECK_SMALL( B(2,2) - I(2,2), tol);


  A (0,0) = 1;   A (0,1) = 1;   A (0,2) = 1;
  A (1,0) = 0;   A (1,1) = 1;   A (1,2) = 1;
  A (2,0) = 0;   A (2,1) = 0;   A (2,2) = 1;
  B = A;
  inverse(A);
  prod(A,B);
  BOOST_CHECK_SMALL( B(0,0) - I(0,0), tol); 
  BOOST_CHECK_SMALL( B(0,1) - I(0,1), tol); 
  BOOST_CHECK_SMALL( B(0,2) - I(0,2), tol);
  BOOST_CHECK_SMALL( B(1,0) - I(1,0), tol); 
  BOOST_CHECK_SMALL( B(1,1) - I(1,1), tol); 
  BOOST_CHECK_SMALL( B(1,2) - I(1,2), tol);
  BOOST_CHECK_SMALL( B(2,0) - I(2,0), tol); 
  BOOST_CHECK_SMALL( B(2,1) - I(2,1), tol);
  BOOST_CHECK_SMALL( B(2,2) - I(2,2), tol);

  
  v1.clear_data();
  block_vector_type zero_vector(0);
  BOOST_CHECK( v1 == zero_vector );
}

BOOST_AUTO_TEST_CASE(scalar_block_test)
{  
  typedef sparse::Block<1,1,float> block_type;
  
  block_type b(0); // zero-init
  BOOST_CHECK(   block_type::nrows() == 1);
  BOOST_CHECK( block_type::ncols() == 1);
  BOOST_CHECK( block_type::size() == 1 );
  // 2009-07-01 Kenny: Comparision between signed and unsigned integer expressions?
  BOOST_CHECK(std::distance(b.begin(), b.end()) == block_type::size());
  // is data initialised to float(0)
  BOOST_CHECK(b[0] == float(0));
  b(0,0) = 0.5f;
  
  BOOST_CHECK(b(0,0) == 0.5f);
  
  block_type b_copy(b);
  
  BOOST_CHECK(b == b_copy);
  
  b_copy(0,0) = 2.5f;
  
  BOOST_CHECK(b_copy(0,0) == 2.5f);
  BOOST_CHECK(b != b_copy);
  
  // can be used as scalar
  float f = b; // rhs
  BOOST_CHECK( f == b(0,0));
  BOOST_CHECK( f == b[0]);
  BOOST_CHECK( f == b);
  
  b = 42.0f; // lhs
  BOOST_CHECK(b == 42.0f);
  
  f = b * 2;
  BOOST_CHECK(f == 84.0f);
  
  f = b / 2.0f;
  BOOST_CHECK(f == 21.0f);

  add(b, b_copy, b);
  BOOST_CHECK( b == 44.5f );
  sub(b, b_copy, b);
  BOOST_CHECK( b == 42.0f );
  add(b, b);
  BOOST_CHECK( b == 84.0f );
  sub(b_copy, b);
  BOOST_CHECK( b == 81.5f );
  
  b.clear_data();
  BOOST_CHECK( b == 0.0f );
}

BOOST_AUTO_TEST_SUITE_END();
