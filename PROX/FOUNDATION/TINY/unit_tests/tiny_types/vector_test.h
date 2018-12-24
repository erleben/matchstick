#ifndef TINY_VECTOR_TEST_H
#define TINY_VECTOR_TEST_H

#include <tiny_type_traits.h>
#include <tiny_vector.h>
#include <tiny_vector_functions.h>

#include <boost/test/test_tools.hpp>                 // for BOOST_CHECK
#include <boost/test/floating_point_comparison.hpp>  // for BOOST_CHECK_CLOSE

namespace vector_test
{
  
  template<size_t N, typename T>
  inline void test()
  {
    typedef          tiny::Vector<N,T>                   vector_type; 
    typedef typename vector_type::value_traits          value_traits;
    typedef typename T::real_type                       real_type;

    real_type const tol = value_traits::numeric_cast(0.1); // 1/10 percent

    real_type const zero = value_traits::zero();
    real_type const one  = value_traits::one();
    real_type const two  = value_traits::two();
    
    // Default Constructor
    vector_type a;
    BOOST_CHECK( a.size()  == N); 
    BOOST_CHECK( a.size1() == 1); 
    BOOST_CHECK( a.size2() == N); 
//    for ( typename vector_type::iterator   i = a.begin(); i!= a.end() ; ++i)
//      BOOST_CHECK_CLOSE( (*i), zero, tol); 
    
    // Single Valued Constructor
    vector_type b(two);
    BOOST_CHECK( b.size() == N); 
    BOOST_CHECK( b.size1() == 1); 
    BOOST_CHECK( b.size2() == N); 
    for ( typename vector_type::iterator   i = b.begin(); i!= b.end() ; ++i)
      BOOST_CHECK_CLOSE( (*i), two, tol); 
        
    // Copy constructor
    vector_type c(b);
    BOOST_CHECK( c.size() == N); 
    BOOST_CHECK( c.size1() == 1); 
    BOOST_CHECK( c.size2() == N); 
    for ( typename vector_type::iterator   i = c.begin(); i!= c.end() ; ++i)
      BOOST_CHECK_CLOSE( (*i), two, tol); 
    
    // Assignment
    vector_type d = b;
    BOOST_CHECK( d.size() == N); 
    BOOST_CHECK( d.size1() == 1); 
    BOOST_CHECK( d.size2() == N); 
    for ( typename vector_type::iterator   i = d.begin(); i!= d.end() ; ++i)
      BOOST_CHECK_CLOSE( (*i), two, tol); 
    
    //verify clear function
    d.clear();
    for ( typename vector_type::iterator i = d.begin(); i!= d.end() ; ++i)
      BOOST_CHECK_CLOSE( (*i), zero, tol); 
        
    // Verify component index operators
    for (size_t i = 0 ; i < a.size() ; ++i)
    {
      a[i] = zero;
      b(i) = zero;
    }
    for ( typename vector_type::iterator   i = a.begin(); i!= a.end() ; ++i)
      BOOST_CHECK_CLOSE( (*i), zero, tol); 
    for ( typename vector_type::iterator   i = b.begin(); i!= b.end() ; ++i)
      BOOST_CHECK_CLOSE( (*i), zero, tol); 
  
    real_type const n =  value_traits::numeric_cast( N);
    vector_type E(one);
    BOOST_CHECK_CLOSE(tiny::inner_prod(E,E), n, tol);
    
    vector_type E2(two);
    
    BOOST_CHECK_CLOSE(tiny::norm(E) ,std::sqrt(n), tol);
    BOOST_CHECK_CLOSE(tiny::norm(E2), std::sqrt(n*two*two),tol);
    
    BOOST_CHECK_CLOSE(tiny::norm(tiny::unit(E)),one,tol);
    BOOST_CHECK_CLOSE(tiny::norm(tiny::unit(E2)),one,tol);
  }

  template<typename T>
  inline void cross_test()
  {  
    typedef          tiny::Vector<3,T>                   vector_type; 
    typedef typename vector_type::value_traits          value_traits;
    typedef typename T::real_type                       real_type;
    
    real_type const zero = value_traits::zero();
    real_type const one  = value_traits::one();
    
    vector_type x;
    x[0] = one;
    x[1] = zero; 
    x[2] = zero;
    
    vector_type y;
    y[0] = zero; 
    y[1] = one; 
    y[2] = zero;
    
    vector_type z;
    z = tiny::cross(x,y);
    
    BOOST_CHECK(z[0]==zero);
    BOOST_CHECK(z[1]==zero);
    BOOST_CHECK(z[2]==one);
  }
  

  template <size_t N>
  inline void run()
  {
    test<N,tiny::float_traits>();
    test<N,tiny::double_traits>();  
  }
    
  template <>
  inline void run<3>()
  {
    test<3,tiny::float_traits>();
    test<3,tiny::double_traits>();  
    cross_test<tiny::float_traits>();
    cross_test<tiny::double_traits>();  
  }
  
} // namespace vector_test

// TINY_VECTOR_TEST_H
#endif 
