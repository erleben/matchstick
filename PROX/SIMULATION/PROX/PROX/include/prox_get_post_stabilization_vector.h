#ifndef PROX_GET_POST_STABILIZATION_VECTOR_H
#define PROX_GET_POST_STABILIZATION_VECTOR_H

#include <steppers/prox_stepper_params.h>

#include <tiny_is_number.h>

#include <util_log.h>

#include <cassert>
#include <cmath>

namespace prox
{
  
  template<typename contact_iterator, typename M>
  inline void get_post_stabilization_vector( 
                             contact_iterator begin
                             , contact_iterator end
                             , StepperParams<M> const & params
                             , typename M::vector4_type & g
                             , M const & /*tag*/ 
                             , size_t const K
                             ) 
  {
    using std::min;
    using std::max;
    
    typedef typename M::block4x1_type    B4x1;
    typedef typename M::value_traits     VT;
    typedef typename M::real_type        T;
        
    util::Log logging;

    g.resize( K );

    logging << "get_post_stabilization_vector(): post stabilizaiton = " << params.post_stabilization() << util::Log::newline();

    if( ! params.post_stabilization() )
      return;
    
    T const & reduction = params.gap_reduction();
    T const & max_gap   = params.max_gap();
    
    assert( reduction  >= VT::zero() || !"get_post_stabilization_vector(): reduction must be non-negative"       );
    assert( reduction  <= VT::one()  || !"get_post_stabilization_vector(): reduction must less than or equal one");
    assert( max_gap    >  VT::zero() || !"get_post_stabilization_vector(): max reduction must be positive"       );
        
    size_t index = 0u;
    
    for(contact_iterator contact = begin;contact!=end; ++contact, ++index)
    {
      B4x1 & b = g( index );
      
      b(0) = max( - max_gap,  min( reduction * contact->get_depth(), VT::zero() ) );
      b(1) = VT::zero();   
      b(2) = VT::zero();
      b(3) = VT::zero();
      
      assert(is_number(b(0)) || !"get_post_stabilization_vector(): b(0) is not a number");      
    }
  }
} //namespace prox
// PROX_GET_POST_STABILIZATION_VECTOR_H
#endif 
