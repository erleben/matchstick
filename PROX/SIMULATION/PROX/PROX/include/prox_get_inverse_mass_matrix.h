#ifndef PROX_GET_INVERSE_MASS_MATRIX_H
#define PROX_GET_INVERSE_MASS_MATRIX_H

#include <prox_update_inertia_tensor.h>

#include <tiny_is_number.h>
#include <tiny_is_finite.h>
#include <tiny_matrix_functions.h>

#include <cassert>

namespace prox
{

  // 2009-08-13 Kenny code reivew: Optimization replace diagonal6x6_type with diagonal_mass_type, maybe wait to optimize until all it working
  template <typename body_iterator, typename MT >
  inline void get_inverse_mass_matrix(
                                      body_iterator begin,
                                      body_iterator end,
                                      typename MT::diagonal6x6_type & W,
                                      MT const & /*math types tag*/
                                      ) 
  {
    typedef typename MT::real_type                T;
    typedef typename MT::matrix3x3_type           M;
    typedef typename MT::block6x6_type            B6x6;
    typedef typename MT::value_traits             VT;
    
    size_t const N = std::distance(begin,end);

    W.resize( N );
    
    size_t index = 0u;
    for(body_iterator body = begin;body!=end;++body, ++index)
    {
      T inv_mass = VT::zero();
      M inv_I    = M::make(
                             VT::zero(), VT::zero(), VT::zero()
                           , VT::zero(), VT::zero(), VT::zero()
                           , VT::zero(), VT::zero(), VT::zero()
                           );

      if( !body->is_fixed() && !body->is_scripted() )
      {
        assert( fabs(body->get_mass()) > VT::zero() || !"get_inverse_mass_matrix(): Divide by zero!");
        
        inv_mass = VT::one() / body->get_mass();

        assert(is_number(inv_mass)   || !"get_inverse_mass_matrix(): Nan");
        assert(is_finite(inv_mass)   || !"get_inverse_mass_matrix(): Inf");
        assert(inv_mass > VT::zero() || !"get_inverse_mass_matrix(): Negative mass");

        M const I_bf = body->get_inertia_bf();
        M const R    = tiny::make( body->get_orientation() );

        detail::update_inertia_tensor<MT>( R, I_bf, inv_I );

        inv_I = tiny::inverse( inv_I );
      }
      
      B6x6 & b = W( index );
      
      b(0,0) = inv_mass;
      b(1,1) = inv_mass;
      b(2,2) = inv_mass;
      b(3,3) = inv_I(0,0);
      b(3,4) = inv_I(0,1);
      b(3,5) = inv_I(0,2);
      b(4,3) = inv_I(1,0);
      b(4,4) = inv_I(1,1);
      b(4,5) = inv_I(1,2);
      b(5,3) = inv_I(2,0);
      b(5,4) = inv_I(2,1);
      b(5,5) = inv_I(2,2);    
    }
  }
} // namespace prox
// PROX_GET_INVERSE_MASS_MATRIX_H
#endif 
