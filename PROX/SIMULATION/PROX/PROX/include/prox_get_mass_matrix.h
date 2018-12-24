#ifndef PROX_GET_MASS_MATRIX_H
#define PROX_GET_MASS_MATRIX_H

#include <prox_update_inertia_tensor.h>

#include <tiny_is_number.h>
#include <tiny_is_finite.h>

namespace prox
{

  // 2009-08-13 Kenny: Optimization replace diagonal6x6_type with diagonal_mass_type, maybe wait to optimize until all it working
  template <typename body_iterator, typename MT>
  inline void get_mass_matrix(
                                body_iterator begin
                              , body_iterator end
                              , typename MT::diagonal6x6_type & M
                              , MT const & /*math_tag*/
                              )
  {
    typedef typename MT::real_type           T;
    typedef typename MT::matrix3x3_type      M3x3;
    typedef typename MT::block6x6_type       B6x6;
    typedef typename MT::value_traits        VT;
    
    size_t const N = std::distance(begin,end);
    M.resize( N );
    
    size_t k = 0u;
    for(body_iterator body = begin;body!=end;++body, ++k)
    {
      T const mass      = body->get_mass();
      M3x3 const I_bf   = body->get_inertia_bf();
      M3x3 const R      = tiny::make( body->get_orientation() );
      
      M3x3 I;
      detail::update_inertia_tensor<MT>( R, I_bf, I );


      assert(is_number(mass)   || !"get_mass_matrix(): Nan");
      assert(is_finite(mass)   || !"get_mass_matrix(): Inf");
      assert(mass > VT::zero() || !"get_mass_matrix(): Non-positive mass");

      if( body->is_fixed() || body->is_scripted() )
      {
        mass = VT::infinity();
      }
      
      B6x6 & b = M( k );
      
      b(0,0) = mass;
      b(1,1) = mass;
      b(2,2) = mass;
      b(0,0) = I(0,0);
      b(0,1) = I(0,1);
      b(0,2) = I(0,2);
      b(1,0) = I(1,0);
      b(1,1) = I(1,1);
      b(1,2) = I(1,2);
      b(2,0) = I(2,0);
      b(2,1) = I(2,1);
      b(2,2) = I(2,2);
    }
  }
} // namespace prox
// PROX_GET_MASS_MATRIX_H
#endif 
