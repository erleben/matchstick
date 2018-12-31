#ifndef PROX_EMPTY_TIME_STEPPER_H
#define PROX_EMPTY_TIME_STEPPER_H

#include <prox_rigid_body.h>
#include <prox_contact_point.h>

#include <prox_collision_detection.h>

#include <prox_params.h>
#include <prox_math_policy.h>

#include <broad.h>
#include <narrow.h>

#include <util_profiling.h>
#include <util_log.h>

namespace prox
{
  /**
   * Empty time stepper.
   * This essentially does nothing, except for invoking the collision detection system.
   */
  template< typename M  >
  inline void empty_stepper(
                            typename M::real_type const & dt
                            , std::vector< RigidBody< M > > & bodies
                            , std::vector< std::vector< MatchStickModel< M > > > const &  contact_models
                            , Gravity< M > const & gravity
                            , Damping< M > const & damping
                            , Params<M> const & params
                            , broad::System<typename M::real_type> & broad_system
                            , narrow::System<typename M::tiny_types> & narrow_system
                            , std::vector< ContactPoint<M> > & contacts
                            , M const & tag
                            )
  {
    util::Log logging;

    START_TIMER("stepper_time");
        
    collision_detection(
                        bodies
                        , broad_system
                        , narrow_system
                        , contacts
                        , params
                        , tag
                        );
    
    unsigned int const number_of_contacts = contacts.size();

    logging << "empty_stepper(): Number of contacts = " << number_of_contacts << util::Log::newline();

    STOP_TIMER("stepper_time");
  }
  
} //namespace prox

// PROX_EMPTY_TIME_STEPPER_H
#endif
