#ifndef PROX_PARAMS_H
#define PROX_PARAMS_H

#include <solvers/prox_solver_params.h>    // Needed for SolverParams
#include <steppers/prox_stepper_params.h>  // Needed for StepperParams

#include <tiny_is_number.h>
#include <tiny_is_finite.h>

namespace prox
{    
  /**
   * Paramters for controlling time steppers, prox solvers and
   * collision detection system.
   *
   * @tparam   Math types policy
   */
  template< typename MT >
  class Params
  {
  public:
    
    typedef SolverParams<MT>            solver_params_type;
    typedef StepperParams<MT>           stepper_params_type;

  protected:
    
    solver_params_type  m_solver_params;     ///< Parameters used for prox solvers.
    stepper_params_type m_stepper_params;    ///< Parameters used for prox steppers.

    bool   m_use_all_pair;                   ///< Parameter for controlling if
                                             ///< all-pair or grid algorithm should
                                             ///< be used for broad phase collision
                                             ///< detetection. Default is false (=off).
    
  public:

    solver_params_type const & solver_params() const { return this->m_solver_params; }
    solver_params_type       & solver_params()       { return this->m_solver_params; }
    
    stepper_params_type const & stepper_params() const { return this->m_stepper_params; }
    stepper_params_type       & stepper_params()       { return this->m_stepper_params; }

    bool const & use_all_pair() const { return this->m_use_all_pair; }
    bool       & use_all_pair()       { return this->m_use_all_pair; }

  public:
    
    Params()
    : m_solver_params()
    , m_stepper_params()
    , m_use_all_pair(false)
    {}
  };

} // namespace prox

// PROX_PARAMS_H
#endif 
