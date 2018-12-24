#ifndef PROX_NUMERICAL_ELLIPSOID_H
#define PROX_NUMERICAL_ELLIPSOID_H

#include <tiny_is_number.h>

#include <cmath>
#include <cassert>

namespace prox 
{
  
  namespace detail   
  {
    /**
    *
    * Complete general purpose force-law.
    *
    *
    * This implementation uses a numerical root search method for
    * computing the proximal point.
    *
    */
    template <typename T> 
    inline static void numerical_ellipsoid(
      T const & z_s     
      , T const & z_t
      , T const & z_tau    
      , T const & mu_s
      , T const & mu_t
      , T const & mu_tau
      , T const & lambda_n
      , T & lambda_s
      , T & lambda_t
      , T & lambda_tau
      )
    {
      using std::sqrt;
      using std::max;
      using std::fabs;

      typedef tiny::ValueTraits<T> value_traits;

      lambda_s = value_traits::zero();            
      lambda_t = value_traits::zero();
      lambda_tau = value_traits::zero();

      if ( lambda_n <= value_traits::zero() )
        return;

      T const a = mu_s*lambda_n;
      T const b = mu_t*lambda_n;
      T const c = mu_tau*lambda_n;

      assert( is_number( a ) || !"numerical_ellipsoid(): a was not a number");
      assert( is_number( b ) || !"numerical_ellipsoid(): b was not a number");
      assert( is_number( c ) || !"numerical_ellipsoid(): c was not a number");
      assert( a > value_traits::zero()       || !"numerical_ellipsoid(): a non-positive");
      assert( b > value_traits::zero()       || !"numerical_ellipsoid(): b non-positive");
      assert( c > value_traits::zero()       || !"numerical_ellipsoid(): c non-positive");

      //
      // Let a,b,c >0 be given these parameters defines an ellipsoid surface.
      // The ellipsoid surface consists of all points x in R^3 where
      //
      //  f(x) = x^T A x - 1 = 0
      //
      // and A is a positive diagonal matrix given by
      //
      //   A = diag( 1/a^2, 1/b^2, 1/c^2 )
      //
      // Given a point z in R^3 then we wish to find the closest point,x^* on the
      // ellipsoid surface to z,
      //
      //   x^* = min_x 1/2 | z-x |^2 s.t. f(x) = 0
      //
      // The first order optimality conditions yields
      //
      //    x^* - z  - \lambda^* \nabla f(x^*)  = 0     (*1)
      //
      // where
      //
      //   \nabla f(x^*) = 2 A x^*
      //
      // Isolating x^* on the left hand-side yields
      //
      //   x^* =  ( 2 \lambda^* A + I )^{-1} z
      //
      // Making the substitution t = 2 lambda^*, we have
      //
      //   x^* = (t A + I )^{-1} z
      //
      // Further we have
      //
      //  f(x^*) = f( (t A + I )^{-1} z ) = 0
      //
      // Writting out we have
      //
      //  g(t) = f( (t A + I )^{-1} z )
      //       = (a^2*x^2)/(a^2 + t)^2 + (b^2*y^2)/(b^2 + t)^2 + (c^2*z^2)/(c^2 + t)^2 - 1
      //
      // We observe that our problem of finding the closest point has been
      // reformulated into the problem of finding a root of g(t) = 0.
      //
      // If we study equation (*1) we see that it defines a parameterized line in
      // the parameter t, going through the known point z in the direction
      // \nablaf(x).
      //
      // From geometry we know that the line can pierce an non-degenerate
      // ellipsoid at most two points. If f(z) > 0 then we are seeking the
      // intersection point with t>0
      //
      // If f(z) <= 0 then z is already in the ellipsoid and we simply can
      // return z as our solution.
      //

      T const tol = 10e-10f;  // absolute tolerance used when testing if iterate is on ellipsoid surface
      size_t const max_k = 100;   // maximum number of iterations allows to be taken in iterative method

      //
      // Scale problem to get most accurate numerical precision, that is fit
      // ellipsoid and z-point to be within unit-cube
      //
      T const scale = max( value_traits::one(), max( a, max( b, max( c, max( fabs(z_s), max( fabs(z_t), fabs(z_tau) ) ) ) ) ) );

      T const sx = z_s   / scale;
      T const sy = z_t   / scale;
      T const sz = z_tau / scale;

      T const sa = a / scale;
      T const sb = b / scale;
      T const sc = c / scale;

      //
      // First perform quick-and dirty rejection test
      //
      T const aa = sa*sa;
      T const bb = sb*sb;
      T const cc = sc*sc;

      T const xx = sx*sx;
      T const yy = sy*sy;
      T const zz = sz*sz;

      T const f0 = (xx/aa) + (yy/bb) + (zz/cc) - value_traits::one();

      if ( f0 < tol ) 
      {
        lambda_s   = z_s;
        lambda_t   = z_t;
        lambda_tau = z_tau;
        return;
      }

      //
      // To solve for the root g(t) = 0 we will use a Newton-Raphson method
      //
      // Given some t_k-value then a first order Taylor approximation yields
      //
      //  g(t_k + dt ) = g(t_k) + g'(t_k) dt
      //
      // We want to find dt such that g(t_k+dt) = 0 that means
      //
      //  0 = g(t_k) + g'(t_k) dt  =>  dt = - g(t_k) / g'(t_k)
      //
      // Next we update our t_k-value to obtain the t_{k+1} value
      //
      //  t_{k+1} = t_k + dt = t_k - g(t_k) / g'(t_k)
      //
      // A good initial value for t_0 is
      //
      //  t_0 = max{a,b,c}\norm{z}
      //

      //T t_k =  max( sa, max( sb, sc ) ) * sqrt( xx + yy + zz );
      //
      //T const aaxx = aa*xx;
      //T const bbyy = bb*yy;
      //T const cczz = cc*zz;
      //
      //for( size_t k = 0; k < max_k; ++k ) 
      //{
      //  T const aat = aa + t_k;
      //  T const bbt = bb + t_k;
      //  T const cct = cc + t_k;
      //
      //  T const g_k = aaxx/(aat*aat) + bbyy/(bbt*bbt) + cczz/(cct*cct) - 1.0f;
      //
      //  if( fabs( g_k) < tol) // absolute convergence
      //  {
      //    break;
      //  }
      //  //
      //  // The easy way to derive the gradient formula:
      //  //
      //  // syms a b c t x y z real
      //  // g = (a^2*x^2)/(a^2 + t)^2 + (b^2*y^2)/(b^2 + t)^2 + (c^2*z^2)/(c^2 + t)^2 - 1
      //  // grad_g = diff(g,t)
      //  //
      //  T const grad_g_k = -2.0f*( aaxx/(aat*aat*aat) + bbyy/(bbt*bbt*bbt) + cczz/(cct*cct*cct));
      //
      //  //
      //  // Newton method has got caught by local minimia we have to bail out
      //  //
      //  if(fabs( grad_g_k) < tol) // ocal minima
      //  {
      //    break;
      //  }
      //
      //  // Perform the Newton-update
      //  T const dt = g_k / grad_g_k;
      //  t_k -= dt;
      //
      //  if( fabs(dt) < tol ) //stagnation
      //  {
      //    break;
      //  }
      //}

      //
      // Precompute terms
      //
      T const aaxx = aa*xx;
      T const bbyy = bb*yy;
      T const cczz = cc*zz;
      //
      //    Initiate bracketing
      //
      T t0 = value_traits::zero();
      T t1 =  max( sa, max( sb, sc ) ) * sqrt( xx + yy + zz );
      T g0 = (aaxx)/((aa+t0)*(aa+t0)) + (bbyy)/((aa+t0)*(aa+t0)) + (cczz)/((aa+t0)*(aa+t0)) - value_traits::one();
      T g1 = (aaxx)/((aa+t1)*(aa+t1)) + (bbyy)/((bb+t1)*(bb+t1)) + (cczz)/((cc+t1)*(cc+t1)) - value_traits::one();

      T const expansion = value_traits::numeric_cast(1.5);
      while(g1>value_traits::zero())
      {
        t1 *= expansion;
        g1 = (aaxx)/((aa+t1)*(aa+t1)) + (bbyy)/((bb+t1)*(bb+t1)) + (cczz)/((cc+t1)*(cc+t1)) - value_traits::one();
      }

      //
      // Perform binary search for root
      //    
      T t_k = (t0+t1)*value_traits::half();
      for( size_t k = 0; k < max_k; ++k ) 
      {
        if(  fabs(t1-t0) < tol ) //stagnation test
        {
          break;
        }
        T const aat = aa + t_k;
        T const bbt = bb + t_k;
        T const cct = cc + t_k;
        T const g_k = aaxx/(aat*aat) + bbyy/(bbt*bbt) + cczz/(cct*cct) - value_traits::one();
        if( fabs( g_k) < tol) // absolute convergence test
        {
          break;
        }
        if( g_k > value_traits::zero() )
        {
          t0 = t_k;
          g0 = g_k;
        }
        else
        {
          t1 = t_k;
          g1 = g_k;
        }
        t_k = (t0+t1)*value_traits::half();
      }

      // Convert root to unscaled problem
      t_k *= scale*scale;

      // Compute closest point using
      //
      //   x^* = (t A + I )^{-1} z
      //
      lambda_s   = (a*a*z_s)  / (a*a + t_k);
      lambda_t   = (b*b*z_t)  / (b*b + t_k);
      lambda_tau = (c*c*z_tau)/ (c*c + t_k);
    }


  } // namespace detail
} // namespace prox

// PROX_NUMERICAL_ELLIPSOID_H
#endif
