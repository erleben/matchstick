#ifndef PROX_LOCAL_R_STRATEGY_H
#define PROX_LOCAL_R_STRATEGY_H

#include <cmath>

namespace prox
{
  
  namespace detail
  {
    
    /**
     *
     */
    template< typename math_policy >
    inline void local_R_strategy(
                                 typename math_policy::compressed4x6_type const & J,
                                 typename math_policy::compressed6x4_type const & WJT,
                                 typename math_policy::diagonal4x4_type & R,
                                 typename math_policy::diagonal4x4_type & nu,
                                 math_policy const & /*tag*/
                                 )
    {
      using std::fabs;
      
      typedef typename math_policy::block4x4_type    block4x4_type;
      typedef typename math_policy::diagonal4x4_type diagonal4x4_type;
      typedef typename math_policy::value_traits     value_traits;
      
      // Get problem size
      size_t const M = J.nrows();
      nu.resize( M );
      R.resize(  M );
      
      // From theory we want to have regularization
      // parameter selected as 
      //
      // R = diag(r)
      //
      // where r_i = 1 / (J W J^T)_{ii}
      // In case of divergence then for the i'th index     
      //
      //  r_i =   c_i r_i    if A_ii <= sum_{j \neq i} A_ij 
      //              r_i    else
      //
      // where 0<c_i < 1 is some reduction paramter.
      //
      //
      // Apparently even for simple stacks all entries are diagonal
      // dominant and in such systems no reduction will happen. Thus
      // divergence is the only possibility. Therefor we are using a
      // global reduction.
      
      diagonal4x4_type A;
      math_policy::compute_A_diagonal( J, WJT, A );   // 2009-08-04 Kenny: we could save storage by reusing R to hold A, and the do inplace updating of R.
      
      for(size_t i=0u;i < M;++i)
      {
        block4x4_type & A_b  = A( i);
        block4x4_type & R_b  = R( i);
        block4x4_type & nu_b = nu( i);
        
        assert( fabs(A_b(0,0)) > value_traits::zero() || !"local_R_strategy(): divide by zero!");
        assert( fabs(A_b(1,1)) > value_traits::zero() || !"local_R_strategy(): divide by zero!");
        assert( fabs(A_b(2,2)) > value_traits::zero() || !"local_R_strategy(): divide by zero!");
        assert( fabs(A_b(3,3)) > value_traits::zero() || !"local_R_strategy(): divide by zero!");
        
        R_b(0,0) = value_traits::one() / A_b(0,0);
        R_b(1,1) = value_traits::one() / A_b(1,1); 
        R_b(2,2) = value_traits::one() / A_b(2,2); 
        R_b(3,3) = value_traits::one() / A_b(3,3); 
        
        //assert off diagonals are indeed zero
        
        nu_b(0,0) = value_traits::numeric_cast(0.9);   
        nu_b(1,1) = value_traits::numeric_cast(0.9);
        nu_b(2,2) = value_traits::numeric_cast(0.9);
        nu_b(3,3) = value_traits::numeric_cast(0.9);
      } 
    }
    
  } // namespace detail
} // namespace prox

// PROX_LOCAL_R_STRATEGY_H
#endif
