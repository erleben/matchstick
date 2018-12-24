#ifndef PROX_NORMAL_SUB_SOLVER_H
#define PROX_NORMAL_SUB_SOLVER_H

namespace prox
{
  /**
   * Normal sub solver functor.
   */
  template<typename T>
  class NormalSubSolver
  {
  public:
    
    virtual void operator()(
                    T const & z_n
                    , T & lambda_n
                            ) const = 0;
    
  };
    
} //namespace prox

// PROX_NORMAL_SUB_SOLVER_H
#endif
