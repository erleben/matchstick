#ifndef PROX_GET_VELOCITY_VECTOR_H
#define PROX_GET_VELOCITY_VECTOR_H

#include <tiny_is_number.h>

namespace prox
{
  
  template<typename body_iterator, typename math_policy>
  inline void get_velocity_vector(
                                  body_iterator begin,
                                  body_iterator end,
                                  typename math_policy::vector6_type & u,
                                  math_policy const & /*math_policy_tag*/
                                  ) 
  {
    typedef typename math_policy::vector3_type     vector3_type;
    typedef typename math_policy::block6x1_type    block6x1_type;
    typedef typename math_policy::value_traits     value_traits;
    
    size_t const N = std::distance(begin,end);
    u.resize( N );
    
    size_t k = 0u;
    
    for(body_iterator body = begin;body!=end;++body, ++k)
    {
      vector3_type const & V = body->get_velocity();
      vector3_type const & W = body->get_spin();
      
      assert(is_number(V(0))     || !"get_velocity_vector(): non number encountered");
      assert(is_number(V(1))     || !"get_velocity_vector(): non number encountered");
      assert(is_number(V(2))     || !"get_velocity_vector(): non number encountered");
      assert(is_number(W(0))     || !"get_velocity_vector(): non number encountered");
      assert(is_number(W(1))     || !"get_velocity_vector(): non number encountered");
      assert(is_number(W(2))     || !"get_velocity_vector(): non number encountered");
      
      block6x1_type & b = u( k );
      
      if( body->is_fixed() ) 
      {
        b(0) = value_traits::zero();
        b(1) = value_traits::zero();
        b(2) = value_traits::zero();
        b(3) = value_traits::zero();
        b(4) = value_traits::zero();
        b(5) = value_traits::zero();
      }
      else 
      {
        b(0) = V(0);
        b(1) = V(1);
        b(2) = V(2);
        b(3) = W(0);
        b(4) = W(1);
        b(5) = W(2);
      }
    }
  }
} // namespace prox
// PROX_GET_VELOCITY_VECTOR_H
#endif
