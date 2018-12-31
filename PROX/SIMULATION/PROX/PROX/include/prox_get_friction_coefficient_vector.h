#ifndef PROX_GET_FRICTION_COEFFICIENT_VECTOR_H
#define PROX_GET_FRICTION_COEFFICIENT_VECTOR_H

#include <prox_matchstick_model.h>
#include <prox_rigid_body.h>

namespace prox
{
  
  template< typename contact_iterator, typename math_policy >
  inline void get_friction_coefficient_vector( 
                                                contact_iterator begin
                                              , contact_iterator end
                                              , std::vector< std::vector< MatchStickModel< math_policy > > > const &  contact_models
                                              , typename math_policy::vector4_type & mu
                                              , math_policy const & /*tag*/
                                              , size_t const K
                                              ) 
  {

    typedef MatchStickModel< math_policy >           contact_model_type;

    typedef typename math_policy::block4x1_type      block4x1_type;
    typedef typename math_policy::real_type          real_type;
    typedef typename math_policy::value_traits       value_traits;
    typedef typename math_policy::vector3_type       vector3_type;

    mu.resize(  K );
    
    size_t k = 0u;
    for(contact_iterator contact = begin;contact!=end;++contact, ++k)
    {
      size_t const mat_i = contact->get_body_i()->get_material_idx();
      size_t const mat_j = contact->get_body_j()->get_material_idx();

      contact_model_type const * model =  &(contact_models[mat_i][mat_j]);

      vector3_type const Sa = contact->get_Sa();
      vector3_type const Sb = contact->get_Sb();

      vector3_type const mu_values = model->compute_coefficients_of_friction(Sa,Sb);
      real_type    const mu_s      = mu_values(0);
      real_type    const mu_t      = mu_values(1);
      real_type    const mu_tau    = mu_values(2);

      block4x1_type & b = mu( k );

      b(0) = value_traits::zero();
      b(1) = mu_s;
      b(2) = mu_t;
      b(3) = mu_tau;
    }
  }
  
  
} // namespace prox
// PROX_GET_FRICTION_COEFFICIENT_VECTOR_H
#endif 
