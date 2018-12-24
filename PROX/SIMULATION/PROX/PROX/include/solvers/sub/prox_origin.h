#ifndef PROX_ORIGIN_H
#define PROX_ORIGIN_H

#include <prox_math_policy.h>

namespace prox
{
  namespace detail
  {
    
    /**
     * Projection onto the origin. This is the same as always forcing lambda_n to be zero,
     */
    template <typename T>
    inline static void origin1D( T const & z_n, T & lambda_n )
    {
      typedef          prox::MathPolicy<T>    M;
      typedef typename M::value_traits        VT;
      
      lambda_n = VT::zero();
    }
    
    /**
     * Projection onto the origin. This is the same as always forcing
     * lambda_s, lambda_t and lambda_tau to be zero,
     */
    template <typename T>
    inline static void origin3D(
                              T const & z_s,   
                              T const & z_t,
                              T const & z_tau,
                              T const & mu_s,
                              T const & mu_t,
                              T const & mu_tau,
                              T const & lambda_n,
                              T & lambda_s,
                              T & lambda_t,
                              T & lambda_tau
                              )
    {
      typedef          prox::MathPolicy<T>    M;
      typedef typename M::value_traits        VT;
      
      lambda_s   = VT::zero();
      lambda_t   = VT::zero();
      lambda_tau = VT::zero();
    }
    
  } // namespace detail
} // namespace prox

// PROX_ORIGIN_H
#endif
