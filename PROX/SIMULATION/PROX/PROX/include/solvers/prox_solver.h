#ifndef PROX_SOLVER_H
#define PROX_SOLVER_H

#include <solvers/strategies/prox_R_strategy.h>
#include <solvers/sub/prox_normal_sub_solver.h>
#include <solvers/sub/prox_friction_sub_solver.h>

#include <solvers/prox_solver_params.h>

namespace prox
{
  
  /**
   * A solver functor.
   */  
  template<typename M>
  class Solver
  {    
  public:
    
    virtual void operator()(
                            typename M::compressed4x6_type const &
                            , typename M::compressed6x4_type const &
                            , typename M::vector4_type const &
                            , typename M::vector4_type const &
                            , typename M::vector4_type &
                            , RStrategy<M> const &
                            , NormalSubSolver<typename M::real_type> const &
                            , FrictionSubSolver<typename M::real_type> const &
                            , SolverParams<M> const &
                            , M const & 
                            ) const = 0;
    
  };
  
} //namespace prox

// PROX_SOLVER_H
#endif
