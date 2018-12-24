#ifndef PROX_GLOBAL_R_STRATEGY_H
#define PROX_GLOBAL_R_STRATEGY_H

namespace prox
{
  
  namespace detail
  {
    
    /**
     * Computes the diagonal matrix of R factors and a nu diagonal matrix. The
     * nu matrix is a user-specified reduction parameter. If
     * divergence is detected, R should be multiplied with nu. Note that unlike
     * the article on r-factor strategies, the diagonal of R is initialized to 
     * the value 5.0, instead of using the largest and smallest eigenvalue
     * of A to compute r = 1/(|alpha_max| + |alpha_min| )
     */
    template< typename math_policy >
    inline void global_R_strategy(
                                  typename math_policy::compressed4x6_type const & J,
                                  typename math_policy::compressed6x4_type const & /*WJT*/,
                                  typename math_policy::diagonal4x4_type & R,
                                  typename math_policy::diagonal4x4_type & nu,
                                  math_policy const & /*tag*/
                                  )
    {
      typedef typename math_policy::block4x4_type block4x4_type;
      typedef typename math_policy::value_traits  value_traits;

      // Get problem size
      size_t const M = J.nrows();
      nu.resize( M );
      R.resize( M );
      
      // R-factor is some large positive scalar value. This should be 1/(|alpha_max| + |alpha_min| )
      // The reduction parameter is set to some user-specified positive value less than one.
      for(size_t i=0u;i < M; ++i)
      {
        block4x4_type & R_b = R(i );
        block4x4_type & nu_b = nu( i );
        
        for( size_t j = 0u; j < 4; ++j ) 
        {
          R_b(j,j)  = value_traits::numeric_cast(5.0f);
          nu_b(j,j) = value_traits::numeric_cast(0.1f);
        }
      } 
    }
    
    /**
     * Computes the diagonal matrix of R factors and a reduction parameter nu.
     * If divergence is detected, R should be multiplied with nu. Note that unlike
     * the article on r-factor strategies, the diagonal of R is initialized to 
     * the value 5.0, instead of using the largest and smallest eigenvalue
     * of A to compute r = 1/(|alpha_max| + |alpha_min| )
     */
    template< typename math_policy >
    inline void global_R_strategy(
                                  typename math_policy::compressed4x6_type const & J,
                                  typename math_policy::diagonal4x4_type & R,
                                  typename math_policy::real_type        & nu,
                                  math_policy const & /*tag*/
                                  )
    {
      typedef typename math_policy::block4x4_type block4x4_type;
      
      nu = math_policy::value_traits::numeric_cast(0.5f);
      // Get problem size
      size_t const M = J.nrows();
      R.resize( M );
      
      // R-factor is some large positive scalar value. This should be 1/(|alpha_max| + |alpha_min| )
      // The reduction parameter is set to some user-specified positive value less than one.
      for(size_t i=0u;i < M; ++i)
      {
        block4x4_type & R_b = R(i);
        for( size_t j = 0u; j < 4; ++j ) 
        {
          R_b(j,j)  = math_policy::value_traits::numeric_cast(5.0f);
        }
      } 
    }
    
  } // namespace detail
} // namespace prox
// PROX_GLOBAL_R_STRATEGY_H
#endif
