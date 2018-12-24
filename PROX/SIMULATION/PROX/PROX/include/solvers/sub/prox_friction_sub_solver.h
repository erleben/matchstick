#ifndef PROX_FRICTION_SUB_SOLVER_H
#define PROX_FRICTION_SUB_SOLVER_H

namespace prox
{
  /**
   * A friction sub solver functor.
   */  
  template<typename T>
  class FrictionSubSolver
  {    
  public:
    
    virtual void operator()(
                    T const & z_s
                    , T const & z_t
                    , T const & z_tau
                    , T const & mu_s
                    , T const & mu_t
                    , T const & mu_tau
                    , T const & lambda_n
                    , T & lambda_s
                    , T & lambda_t
                    , T & lambda_tau
                    ) const = 0;
    
  };
  
} //namespace prox

// PROX_FRICTION_SUB_SOLVER_H
#endif
