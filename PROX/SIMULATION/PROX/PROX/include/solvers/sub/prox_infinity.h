#ifndef PROX_INFINITY_H
#define PROX_INFINITY_H

namespace prox
{
  namespace detail
  {
    
    /**
     * Projection onto the infinite space. This is the same as not changing
     * lambda_n (pass through).
     */
    template <typename T>
    inline static void infinity1D( T const & z_n, T & lambda_n )
    {
    }
    
    /**
     * Projection onto the infinite space. This is the same as not changing
     * lambda_s, lambda_t and lambda_tau (pass through).
     */
    template <typename T>
    inline static void infinity3D(
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
    }
    
  } // namespace detail
} // namespace prox

// PROX_INFINITY_H
#endif
