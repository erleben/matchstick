#ifndef PROX_VELOCITY_UPDATE_H
#define PROX_VELOCITY_UPDATE_H

namespace prox
{

  template<typename math_policy>
  inline void velocity_update( 
    typename math_policy::vector6_type const& u,
    typename math_policy::vector6_type const& Wdth,
    typename math_policy::vector6_type const& fc,
    typename math_policy::vector6_type & unew,
    math_policy const & /*math_policy_tag*/ 
    ) 
  {
    if( &u != &unew ) 
    {
      unew.resize( u.size() );
    }

    math_policy::compute_sum( u, Wdth, fc, unew );
  }

  template<typename math_policy>
  inline void velocity_update( 
    typename math_policy::vector6_type const& u,
    typename math_policy::vector6_type const& Wdth,
    typename math_policy::vector6_type & unew,
    math_policy const & /*math_policy_tag*/ 
    ) 
  {
    if( &u != &unew ) 
    {
      unew.resize( u.size( ) );
    }

    math_policy::compute_sum( u, Wdth, unew );
  }
  
} // namespace prox

// PROX_VELOCITY_UPDATE_H
#endif
