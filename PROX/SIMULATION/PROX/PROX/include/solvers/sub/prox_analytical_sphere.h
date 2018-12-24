#ifndef PROX_ANALYTICAL_SPHERE_H
#define PROX_ANALYTICAL_SPHERE_H

#include <tiny_is_number.h>
#include <prox_math_policy.h>
#include <cmath>
#include <cassert>

namespace prox
{
  
  namespace detail
  {

    /**
    *
    * This function computes the proximal point of a sphere
    */
    template <typename T> 
    inline static void analytical_sphere(      
                                              T const & z_s     
                                              , T const & z_t
                                              , T const & z_tau    
                                              , T const & mu
                                              , T const & lambda_n
                                              , T & lambda_s
                                              , T & lambda_t
                                              , T & lambda_tau
                                              )
    {
      using std::sqrt;
      typedef tiny::ValueTraits<T> value_traits;

      // Special case when cold starting
      if ( lambda_n <= value_traits::zero() )    
      {
        lambda_s   = value_traits::zero();
        lambda_t   = value_traits::zero();
        lambda_tau = value_traits::zero();
        return;
      }

      T const radius = mu*lambda_n;

      assert( is_number( radius ) || !"analytical_sphere(): a was not a number");
      assert( radius > value_traits::zero()  || !"analytical_sphere(): a non-positive");

      T const s = z_s*z_s + z_t*z_t + z_tau*z_tau;

      if( s <= radius*radius )
      {
        lambda_s   = z_s;
        lambda_t   = z_t;
        lambda_tau = z_tau;
        return;
      }

      assert( s > value_traits::zero() || !"analytical_sphere(): internal error");   

      T const scale = radius /sqrt(s);

      lambda_s =  scale*z_s;
      lambda_t =  scale*z_t;
      lambda_tau = scale*z_tau;
    }

    /**
     *
     * Overloaded version. The only purpose of this version is
     * such that all sub solvers have the same function signature.
     */
    template <typename T> 
    inline static void analytical_sphere(      
                                              T const & z_s     
                                              , T const & z_t
                                              , T const & z_tau    
                                              , T const & mu_s
                                              , T const & /*mu_t*/
                                              , T const & /*mu_tau*/
                                              , T const & lambda_n
                                              , T & lambda_s
                                              , T & lambda_t
                                              , T & lambda_tau
                                              )
    {
      analytical_sphere(z_s, z_t, z_tau, mu_s,lambda_n,lambda_s,lambda_t, lambda_tau);
    }
      
  } // namespace detail
} // namespace prox

// PROX_ANALYTICAL_SPHERE_H
#endif
