#ifndef PROX_GET_EXTERNAL_FORCES_VECTOR_H
#define PROX_GET_EXTERNAL_FORCES_VECTOR_H

#include <prox_force_callbacks.h>

#include <tiny_is_number.h>
#include <tiny_is_finite.h>

#include <cassert>

namespace prox
{

  /**
   *
   * @param h      Upon return this parameter contains the total external forces
   *               and torques acting on the bodies in the system
   */
  template<typename body_iterator, typename MT>
  inline void get_external_forces_vector(
                           body_iterator begin
                           , body_iterator end
                           , Gravity< MT > const & gravity
                           , Damping< MT > const & damping
                           , typename MT::vector6_type & h
                           , MT const & /*tag*/
  )
  {
    typedef typename MT::vector3_type     V;
    typedef typename MT::matrix3x3_type   M;
    typedef typename MT::block6x1_type    B6x1;
    typedef typename MT::value_traits     VT;

    size_t const N = std::distance(begin,end);
    h.resize( N );

    size_t k = 0u;
    for(body_iterator body = begin;body!=end;++body, ++k)
    {
      B6x1 & b = h( k );

      if(body->is_fixed() || body->is_scripted() )
      {
        b(0) = VT::zero();
        b(1) = VT::zero();
        b(2) = VT::zero();
        b(3) = VT::zero();
        b(4) = VT::zero();
        b(5) = VT::zero();
        continue;
      }

      V total_force  = V::zero();
      V total_torque = V::zero();

      V force  = V::zero();
      V torque = V::zero();

      //--- First we add global world gravity force ----------------------------
      gravity.compute_force_and_torque((*body), force, torque);

      total_force  += force;
      total_torque += torque;

      //--- Second we add global world damping force ---------------------------
      damping.compute_force_and_torque((*body), force, torque);

      total_force  += force;
      total_torque += torque;

      //--- Third we add any local body forces that might be applied -----------
      typename std::vector<ForceCallback<MT> * >::const_iterator callback      = body->get_force_callbacks().begin();
      typename std::vector<ForceCallback<MT> * >::const_iterator callback_end  = body->get_force_callbacks().end();
      for(;callback != callback_end;++callback)
      {
        (*callback)->compute_force_and_torque((*body), force, torque);
        total_force  += force;
        total_torque += torque;
      }

      assert(is_number(total_force(0)) || !"get_external_forces_vector(): Nan");
      assert(is_number(total_force(1)) || !"get_external_forces_vector(): Nan");
      assert(is_number(total_force(2)) || !"get_external_forces_vector(): Nan");

      assert(is_number(total_torque(0)) || !"get_external_forces_vector(): Nan");
      assert(is_number(total_torque(1)) || !"get_external_forces_vector(): Nan");
      assert(is_number(total_torque(2)) || !"get_external_forces_vector(): Nan");

      assert(is_finite(total_force(0)) || !"get_external_forces_vector(): Inf");
      assert(is_finite(total_force(1)) || !"get_external_forces_vector(): Inf");
      assert(is_finite(total_force(2)) || !"get_external_forces_vector(): Inf");

      assert(is_finite(total_torque(0)) || !"get_external_forces_vector(): Inf");
      assert(is_finite(total_torque(1)) || !"get_external_forces_vector(): Inf");
      assert(is_finite(total_torque(2)) || !"get_external_forces_vector(): Inf");

      b(0) = total_force(0);
      b(1) = total_force(1);
      b(2) = total_force(2);

      V const & w    = body->get_spin();
      M const & I_bf = body->get_inertia_bf();

      assert(is_number(w(0))|| !"get_external_forces_vector(): Nan");
      assert(is_number(w(1))|| !"get_external_forces_vector(): Nan");
      assert(is_number(w(2))|| !"get_external_forces_vector(): Nan");

      assert(is_finite(w(0))|| !"get_external_forces_vector(): Inf");
      assert(is_finite(w(1))|| !"get_external_forces_vector(): Inf");
      assert(is_finite(w(2))|| !"get_external_forces_vector(): Inf");

      M const R = tiny::make( body->get_orientation() );

      M I;
      detail::update_inertia_tensor<MT>( R, I_bf, I );

      V const wIw = tiny::cross( w , I*w);

      assert(is_number(wIw(0)) || !"get_external_forces_vector(): Nan");
      assert(is_number(wIw(1)) || !"get_external_forces_vector(): Nan");
      assert(is_number(wIw(2)) || !"get_external_forces_vector(): Nan");

      assert(is_finite(wIw(0)) || !"get_external_forces_vector(): Inf");
      assert(is_finite(wIw(1)) || !"get_external_forces_vector(): Inf");
      assert(is_finite(wIw(2)) || !"get_external_forces_vector(): Inf");

      b(3) = total_torque(0) - wIw(0);
      b(4) = total_torque(1) - wIw(1);
      b(5) = total_torque(2) - wIw(2);
    }
  }
}// namespace prox

// PROX_GET_EXTERNAL_FORCES_VECTOR_H
#endif
