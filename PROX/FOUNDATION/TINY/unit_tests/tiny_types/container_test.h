#ifndef TINY_CONTAINER_TEST_H
#define TINY_CONTAINER_TEST_H

#include <tiny_type_traits.h>
#include <tiny_container.h>

#include <boost/test/test_tools.hpp>                 // for BOOST_CHECK
#include <boost/test/floating_point_comparison.hpp>  // for BOOST_CHECK_CLOSE

namespace container_test
{
  
  template<size_t I, size_t J, typename T>
  inline void test()
  {
    typedef          tiny::detail::Container<I,J,T> C;
    typedef typename C::value_traits               value_traits;
    typedef typename T::real_type                  real_type;
    
    real_type const tol     = value_traits::numeric_cast(0.1); // 1/10 percent    
    real_type const zero    = value_traits::zero();
    real_type const two     = value_traits::two();

    // Verify instatiation and instantiation size
    C a;
    BOOST_CHECK( a.size1() == I );
    BOOST_CHECK( a.size2() == J );
    BOOST_CHECK( a.size() == I*J );
//    for ( typename C::iterator   i = a.begin(); i!= a.end() ; ++i)
//      BOOST_CHECK_CLOSE( (*i), zero, tol); 
    
    // single value constructor
    C b(two);
    BOOST_CHECK( b.size1() == I );
    BOOST_CHECK( b.size2() == J );
    BOOST_CHECK( b.size() == I*J );
    for ( typename C::iterator   i = b.begin(); i!= b.end() ; ++i)
      BOOST_CHECK_CLOSE( (*i), two, tol); 
    
    // copy constructor
    C c(b);
    BOOST_CHECK( c.size1() == I );
    BOOST_CHECK( c.size2() == J );
    BOOST_CHECK( c.size() == I*J );
    for ( typename C::iterator   i = c.begin(); i!= c.end() ; ++i)
      BOOST_CHECK_CLOSE( (*i), two, tol); 
    
    // assignment constructor
    C d = b;
    BOOST_CHECK( d.size1() == I );
    BOOST_CHECK( d.size2() == J );
    BOOST_CHECK( d.size() == I*J );
    for ( typename C::iterator   i = d.begin(); i!= d.end() ; ++i)
      BOOST_CHECK_CLOSE( (*i), two, tol); 
    
    // clear function
    d.clear();
    for ( typename C::iterator   i = d.begin(); i!= d.end() ; ++i)
      BOOST_CHECK_CLOSE( (*i), zero, tol); 

    //  Index operators
    for (size_t i = 0 ; i < b.size1() ; ++i)
    {
      for (size_t j = 0 ; j < b.size2() ; ++j)
      {
        b(i,j) = zero;
        BOOST_CHECK_CLOSE( b(i,j),  zero, tol ); 
      }
    }
    for ( typename C::iterator   i = b.begin(); i!= b.end() ; ++i)
      BOOST_CHECK_CLOSE( (*i), zero, tol); 
    
    // Iterator tests
    typedef typename C::iterator iterator;
    iterator begin = a.begin();
    iterator end   = a.end();
    for (iterator iter = begin ; iter!=end ;++iter)
    {
      *iter=two;
    }
    for ( typename C::iterator   i = a.begin(); i!= a.end() ; ++i)
      BOOST_CHECK_CLOSE( (*i), two, tol); 
    
    for (iterator iter = begin ; !(iter==end) ;iter++)
    {
      *iter=zero;
    }
    for ( typename C::iterator   i = a.begin(); i!= a.end() ; ++i)
      BOOST_CHECK_CLOSE( (*i), zero, tol); 
  }

  template <size_t I, size_t J>
  inline void run()
  {
    test<I,J,tiny::float_traits >();
    test<I,J,tiny::double_traits >();
  }
  
}// namespace container_test

// TINY_CONTAINER_TEST_H
#endif 
