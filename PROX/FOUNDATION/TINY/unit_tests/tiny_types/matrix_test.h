#ifndef TINY_MATRIX_TEST_H
#define TINY_MATRIX_TEST_H

#include <tiny_matrix.h>
#include <tiny_matrix_functions.h>

#include <boost/test/test_tools.hpp>                 // for BOOST_CHECK
#include <boost/test/floating_point_comparison.hpp>  // for BOOST_CHECK_CLOSE

namespace matrix_test
{
  
  template<size_t I, size_t J, typename T>
  inline void test()
  {
    typedef tiny::Matrix<I,J,T>   matrix_type;
    
    typedef typename matrix_type::value_traits value_traits;
    typedef typename T::real_type              real_type;
    
    real_type const zero = value_traits::zero();
    real_type const one  = value_traits::one();
    real_type const tol  = value_traits::numeric_cast(0.01); // 1/100 percent
    
    matrix_type D = matrix_type::identity();
    for (size_t i=0; i<I; ++i)
    {
      for (size_t j=0; j<J; ++j)
      {
        if (i==j)
        {
          BOOST_CHECK_CLOSE(D(i,j),one,tol);
        }
        else
        {
          BOOST_CHECK_CLOSE(D(i,j),zero,tol);
        }
      }	
    }
    
    matrix_type A(one);
    matrix_type B;
    B = tiny::equal(A,A);
    for ( typename matrix_type::iterator i = B.begin(); i!= B.end() ; ++i)
      BOOST_CHECK_CLOSE( (*i), one, tol);     
  }
  
  template<size_t I, typename T>
  inline void mul_test()
  {
    typedef tiny::Matrix<I,I,T>   matrix_type;
    
    typedef typename matrix_type::value_traits value_traits;
    typedef typename T::real_type              real_type;
    
    real_type const one  = value_traits::one();
    real_type const tol  = value_traits::numeric_cast(0.01); // 1/100 percent
    
    matrix_type D = matrix_type::identity();
    
    typedef tiny::Vector<I,T>     vector_type;
    vector_type E(one);
    vector_type a = D*E;
    for ( typename vector_type::iterator i = a.begin(); i!= a.end() ; ++i)
      BOOST_CHECK_CLOSE( (*i), one, tol); 
  }
  
  
  
  template <size_t I,size_t J>
  inline void run()
  {
    test<I,J, tiny::float_traits > ();
    test<I,J, tiny::double_traits > ();
    mul_test<I, tiny::float_traits > ();
    mul_test<I, tiny::double_traits > ();
  }
  
} // namespace matrix_test

// TINY_MATRIX_TEST_H
#endif
