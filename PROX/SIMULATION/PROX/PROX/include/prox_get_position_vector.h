#ifndef PROX_GET_POSITION_VECTOR_H
#define PROX_GET_POSITION_VECTOR_H

#include <tiny_is_number.h>

namespace prox
{
  
  template<typename body_iterator, typename math_policy>
  inline void get_position_vector(
                                  body_iterator begin, 
                                  body_iterator end,
                                  typename math_policy::vector7_type & q,
                                  math_policy const & /*math_policy_tag*/ 
                                  )
  {
    typedef typename math_policy::vector3_type     vector3_type;   // 2009-08-04 Kenny: style clash with vector7_type
    typedef typename math_policy::quaternion_type  quaternion_type;
    
    typedef typename math_policy::block7x1_type    block7x1_type;
    
    size_t N = std::distance(begin,end);
    q.resize( N );
    
    size_t k = 0u;
    for(body_iterator body = begin;body!=end;++body, ++k)
    {
      vector3_type     const & r = body->get_position();
      quaternion_type  const & Q = body->get_orientation();
      
      assert(is_number(r(0))        || !"get_position_vector(): non number encountered");
      assert(is_number(r(1))        || !"get_position_vector(): non number encountered");
      assert(is_number(r(2))        || !"get_position_vector(): non number encountered");
      assert(is_number(Q.real())    || !"get_position_vector(): non number encountered");
      assert(is_number(Q.imag()(0)) || !"get_position_vector(): non number encountered");
      assert(is_number(Q.imag()(1)) || !"get_position_vector(): non number encountered");
      assert(is_number(Q.imag()(2)) || !"get_position_vector(): non number encountered");
      
      block7x1_type & b = q( k );
      
      b(0) = r(0);
      b(1) = r(1);
      b(2) = r(2);
      b(3) = Q.real();        
      b(4) = Q.imag()(0);     
      b(5) = Q.imag()(1);
      b(6) = Q.imag()(2);
      
    }
  } 
}// namespace prox
// PROX_GET_POSITION_VECTOR_H
#endif
