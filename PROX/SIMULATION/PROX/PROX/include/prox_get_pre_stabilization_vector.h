#ifndef PROX_GET_PRE_STABILIZATION_VECTOR_H
#define PROX_GET_PRE_STABILIZATION_VECTOR_H

#include <steppers/prox_stepper_params.h>

#include <tiny_is_number.h>

#include <util_log.h>

#include <cassert>
#include <cmath>

namespace prox
{
  
  /**
   *
   * @param drift_reduction     The procentage of numerical drift reduction that should
   *                            be done. This parameter must be in the range [0..1].
   * @param time_step           The time step-length. This paramter is needed to convert
   *                            drift-correction from displacement level to velocity level.
   *
   * @param w                  Current relative contact point velocites, w = J*u
   *
   */
  template<typename contact_iterator, typename M>
  inline void get_pre_stabilization_vector(
                                           contact_iterator begin
                                           , contact_iterator end
                                           , StepperParams<M> const & params
                                           , typename M::real_type const & time_step
                                           , typename M::vector4_type const & w
                                           , typename M::vector4_type & g
                                           , M const & /*tag*/
                                           , size_t const K
                                           )
  {
    using std::max;
    
    typedef typename M::real_type        T;
    typedef typename M::block4x1_type    B4x1;
    typedef typename M::value_traits     VT;
    
    assert( time_step > VT::zero()  || !"get_pre_stabilization_vector(): time_step should be positive");

    util::Log logging;

    g.resize( K );

    logging << "get_pre_stabilization_vector(): pre stabilizaiton = " << params.pre_stabilization() << util::Log::newline();

    if( ! params.pre_stabilization() )
      return;

    
    T const & reduction = params.gap_reduction();
    T const & min_gap   = params.min_gap();
    T const & max_gap   = params.max_gap();
    
    assert( reduction >= VT::zero() || !"get_pre_stabilization_vector(): gap reduction parameter should be positive");
    assert( reduction <= VT::one()  || !"get_pre_stabilization_vector(): gap reduction parameter should be less than or equal to one");
    assert( min_gap >= VT::zero()   || !"get_pre_stabilization_vector(): min gap correction should be non negative");
    assert( max_gap > VT::zero()    || !"get_pre_stabilization_vector(): max gap correction should be positive");

    T const k       =   reduction / time_step;
    T const limit   = - max_gap / time_step;
    T const yield   = - min_gap;
    
    size_t index = 0u;
    for(contact_iterator contact = begin;contact!=end; ++contact, ++index)
    {
      B4x1       & b            = g( index );
      B4x1 const & v            = w( index );
      T    const & d            = contact->get_depth();
      T    const & v_n          = v(0);
      bool const add_correction = d <= yield && v_n <= VT::zero();

      b(0) = add_correction ? max( limit , k * d ) : VT::zero();
      b(1) = VT::zero();
      b(2) = VT::zero();
      b(3) = VT::zero();
      
      assert(is_number(b(0)) || !"get_pre_stabilization_vector(): b(0) is not a number");      
    }
  }
} //namespace prox
// PROX_GET_PRE_STABILIZATION_VECTOR_H
#endif 
