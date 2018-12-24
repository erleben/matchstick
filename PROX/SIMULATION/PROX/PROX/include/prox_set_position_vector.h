#ifndef PROX_SET_POSITION_VECTOR_H
#define PROX_SET_POSITION_VECTOR_H

#include <tiny_is_number.h>

namespace prox
{
  
  template<typename body_iterator, typename math_policy>
  inline void set_position_vector(
                                  body_iterator begin
                                  , body_iterator end
                                  , typename math_policy::vector7_type const & q
                                  ,	math_policy const & /*tag*/
                                  )
  {
    typedef typename math_policy::vector3_type          vector3_type;
    typedef typename math_policy::quaternion_type       quaternion_type;
    typedef typename math_policy::block7x1_type         block7x1_type;
    
    size_t const N = std::distance(begin,end);
    
    assert(q.size() == N || !"set_position_vector(): q has incorrect dimension");
    
    size_t k = 0u;
    for(body_iterator body = begin; body!=end; ++body, ++k)
    {
      if( body->is_fixed() || body->is_scripted() )
        continue;
      
      vector3_type r;
      quaternion_type Q;
      
      block7x1_type const & b = q( k );
      
      r(0) = b(0);
      r(1) = b(1);
      r(2) = b(2);
      Q.real()    = b(3);
      Q.imag()(0) = b(4);
      Q.imag()(1) = b(5);
      Q.imag()(2) = b(6);
      
      assert(is_number(r(0))        || !"set_position_vector(): non number encountered");
      assert(is_number(r(1))        || !"set_position_vector(): non number encountered");
      assert(is_number(r(2))        || !"set_position_vector(): non number encountered");
      assert(is_number(Q.real())    || !"set_position_vector(): non number encountered");
      assert(is_number(Q.imag()(0)) || !"set_position_vector(): non number encountered");
      assert(is_number(Q.imag()(1)) || !"set_position_vector(): non number encountered");
      assert(is_number(Q.imag()(2)) || !"set_position_vector(): non number encountered");
      
      body->set_position( r );
      body->set_orientation( Q );
    }
  }
} // namespace prox
// PROX_SET_POSITION_VECTOR_H
#endif
