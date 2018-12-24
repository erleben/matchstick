#ifndef PROX_STEPPER_H
#define PROX_STEPPER_H

#include <prox_params.h>
#include <prox_contact_point.h>
#include <prox_rigid_body.h>
#include <prox_force_callbacks.h>
#include <prox_matchstick_model.h>

#include <broad.h>
#include <narrow.h>

#include <vector>

namespace prox
{
  
  /**
   * A stepper functor.
   */
  template< typename M >
  class Stepper
  {    
  public:
    
    virtual void operator()(
                            typename M::real_type const &
                            , std::vector< RigidBody< M > > &
                            , std::vector< std::vector< MatchStickModel< M > > > const &
                            , Gravity< M > const &
                            , Damping< M > const &
                            , Params<M> const &
                            , broad::System<typename M::real_type> &
                            , narrow::System<typename M::tiny_types> &
                            , std::vector< ContactPoint<M> > &
                            , M const &
                            ) const = 0;

  };
  
} //namespace prox

// PROX_STEPPER_H
#endif
