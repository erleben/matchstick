#ifndef RPOX_GET_RESTITUTION_VECTOR_H
#define RPOX_GET_RESTITUTION_VECTOR_H

#include <util_log.h>

namespace prox
{
  
  template<
  typename contact_iterator
  , typename math_policy
  >
  inline void get_restitution_vector( 
                                     contact_iterator begin
                                     , contact_iterator end
                                     , std::vector< std::vector< MatchStickModel< math_policy > > > const &  properties
                                     , typename math_policy::vector4_type & e
                                     , math_policy const & /*tag*/ 
                                     , size_t const K
                                     )
  {
    typedef typename math_policy::block4x1_type block4x1_type;
    typedef typename math_policy::value_traits  value_traits;
    typedef typename math_policy::real_type     real_type;
    
    util::Log logging;

    e.resize( K );

    logging << "get_restitution_vector(): bounce is on" << util::Log::newline();

    size_t k = 0u;
    for(contact_iterator contact = begin;contact!=end;++contact, ++k)
    {
      size_t const material_i = contact->get_body_i()->get_material_idx();
      size_t const material_j = contact->get_body_j()->get_material_idx();
      
      real_type e_k = properties[material_i][material_j].get_coefficient_of_restitution();
      
      block4x1_type & b = e( k );
      
      b(0) = e_k;
      b(1) = value_traits::zero();
      b(2) = value_traits::zero();
      b(3) = value_traits::zero();      
    }
  }
  
} // namespace prox

// RPOX_GET_RESTITUTION_VECTOR_H
#endif 
