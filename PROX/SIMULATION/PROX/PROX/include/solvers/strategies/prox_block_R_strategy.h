#ifndef PROX_BLOCK_R_STRATEGY_H
#define PROX_BLOCK_R_STRATEGY_H

#include <tiny_matrix_functions.h> // for tiny::inverse

#include <cassert>

namespace prox
{
  
  namespace detail
  {
    
    /**
     *
     */
    template< typename math_policy >
    inline void block_R_strategy(
                                 typename math_policy::compressed4x6_type const & J,    
                                 typename math_policy::compressed6x4_type const & WJT, 
                                 typename math_policy::diagonal4x4_type & R,
                                 typename math_policy::diagonal4x4_type & nu,
                                 math_policy const & /*tag*/   
                                 )
    {
      typedef typename math_policy::block4x4_type    block4x4_type;
      typedef typename math_policy::matrix3x3_type   matrix3x3_type;
      typedef typename math_policy::value_traits     value_traits;
      
      // Get problem size
      size_t const M = J.nrows();
      nu.resize(  M );
      R.resize(   M );
      
      // 2009-08-04 Kenny: Possible optimization; use R to store diagonal of A, then do
      // the inverse operations in place to get R. I actually think SPARSE supports
      // inversion of diagonal matrices so maybe one can just write R = sparse::inverse(R)
      // or something?
      
      typename math_policy::diagonal4x4_type A;
      math_policy::compute_A_diagonal( J, WJT, A );
      
      //
      // In the thesis of Studer he uses a block-diagonal matrix R, where
      // R_{ii} = A_{ii}^{-1}
      //
      for(size_t i=0u; i < M; ++i)
      {
        block4x4_type & A_b  = A(i);
        block4x4_type & R_b  = R(i);
        block4x4_type & nu_b = nu(i);
        
        matrix3x3_type Aff;
        Aff(0,0) = A_b(1,1);      Aff(0,1) = A_b(1,2);      Aff(0,2) = A_b(1,3);
        Aff(1,0) = A_b(2,1);      Aff(1,1) = A_b(2,2);      Aff(1,2) = A_b(2,3);
        Aff(2,0) = A_b(3,1);      Aff(2,1) = A_b(3,2);      Aff(2,2) = A_b(3,3);      
        
        Aff = tiny::inverse( Aff );   
        
        assert( fabs(A_b(0,0)) > value_traits::zero() || !"block_R_strategy(): divide by zero!");
        
        R_b(0,0) = value_traits::one() / A_b(0,0); 
        R_b(1,1) = Aff(0,0);      R_b(1,2) = Aff(0,1);      R_b(1,3) = Aff(0,2);
        R_b(2,1) = Aff(1,0);      R_b(2,2) = Aff(1,1);      R_b(2,3) = Aff(1,2);
        R_b(3,1) = Aff(2,0);      R_b(3,2) = Aff(2,1);      R_b(3,3) = Aff(2,2);
        
        nu_b(0,0) = value_traits::one();
        nu_b(1,1) = value_traits::one();
        nu_b(2,2) = value_traits::one();
        nu_b(3,3) = value_traits::one();
      }
    }
    
  } // namespace detail
} // namespace prox

// PROX_BLOCK_R_STRATEGY_H
#endif
