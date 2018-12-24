#ifndef TINY_QUATERNION_TEST_H
#define TINY_QUATERNION_TEST_H

#include <tiny_type_traits.h>
#include <tiny_quaternion.h>
#include <tiny_quaternion_functions.h>

#include <boost/test/test_tools.hpp>                 // for BOOST_CHECK
#include <boost/test/floating_point_comparison.hpp>  // for BOOST_CHECK_CLOSE

namespace quaternion_test
{
  
  template <typename T>
  inline void test()
  {
    typedef          tiny::Quaternion<T>  Q;
    typedef typename Q::value_traits     value_traits;
    typedef typename Q::vector3_type     vector3_type;
    typedef typename T::real_type        real_type;
    
    real_type const tol = value_traits::numeric_cast(0.1); // 1/10 percent
    //real_type const zero = value_traits::zero();
    real_type const one  = value_traits::one();
    real_type const two  = value_traits::two();
    
    vector3_type const V(one);
    Q q;
    
    BOOST_CHECK( q.size()  == 4); 
    BOOST_CHECK( q.size1() == 1); 
    BOOST_CHECK( q.size2() == 4); 
    //BOOST_CHECK_CLOSE( q.real(),    zero, tol );
    //BOOST_CHECK_CLOSE( q.imag()(0), zero, tol );
    //BOOST_CHECK_CLOSE( q.imag()(0), zero, tol );
    //BOOST_CHECK_CLOSE( q.imag()(0), zero, tol );
    
    q = Q(two,V);  
    
    BOOST_CHECK_CLOSE( q.real(),    two, tol );
    BOOST_CHECK_CLOSE( q.imag()(0), one, tol );
    BOOST_CHECK_CLOSE( q.imag()(0), one, tol );
    BOOST_CHECK_CLOSE( q.imag()(0), one, tol );
    
    q = tiny::unit(q);
    BOOST_CHECK_CLOSE( tiny::inner_prod(q,q), one, tol );
    BOOST_CHECK_CLOSE( tiny::norm(q), one, tol );
  }
  
  inline void run()
  {
    test<tiny::float_traits >();
    test<tiny::double_traits >();
//    test<tiny::sse_float_traits >();
  }
  
}// namespace quaternion_test

// TINY_QUATERNION_TEST_H
#endif 

