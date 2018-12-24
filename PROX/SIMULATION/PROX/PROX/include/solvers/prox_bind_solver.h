#ifndef PROX_BIND_SOLVER_H
#define PROX_BIND_SOLVER_H

#include <prox_enums.h>

#include <solvers/prox_solver.h>

#include <solvers/prox_jacobi_solver.h>
#include <solvers/prox_gauss_seidel_solver.h>

#include <util_log.h>

#include <cassert>

namespace prox
{
  
  /**
   * Solver function pointer binder.
   */  
  template<typename M>
  class SolverBinder
  : public Solver<M>
  {
  public:
    
    typedef void func_type(
                                  typename M::compressed4x6_type const &
                                  , typename M::compressed6x4_type const &
                                  , typename M::vector4_type const &
                                  , typename M::vector4_type const &
                                  , typename M::vector4_type &
                                  , RStrategy<M> const &
                                  , NormalSubSolver<typename M::real_type> const &
                                  , FrictionSubSolver<typename M::real_type> const &
                                  , SolverParams<M> const &
                                  , M const & tag 
                                  );
    
    func_type * m_solver;
    
  public:
    
    SolverBinder()
    : m_solver(0)
    {
    }
    
    SolverBinder(func_type * solver)
    : m_solver(solver)
    {
    }
    
  public:
    
    void operator()(
                    typename M::compressed4x6_type const & J
                    , typename M::compressed6x4_type const & WJT
                    , typename M::vector4_type const & b
                    , typename M::vector4_type const & mu
                    , typename M::vector4_type & lambda
                    , RStrategy<M> const & strategy
                    , NormalSubSolver<typename M::real_type> const & normal_solver
                    , FrictionSubSolver<typename M::real_type> const & friction_solver
                    , SolverParams<M> const & params
                    , M const & tag
                    ) const
    {
      assert( this->m_solver || !"SolverBinder(): solver was null");
      
      this->m_solver( J, WJT, b, mu, lambda, strategy, normal_solver, friction_solver, params, M() );
    }
    
  };
  
  /**
   *
   */     
  template<typename M>
  inline SolverBinder<M> bind_solver( solver_type const & type )
  {
    util::Log logging;

    switch( type )
    {
      case jacobi:
        logging << "bind_solver(): using jacobi solver"<< util::Log::newline();
        return SolverBinder<M>( &jacobi_solver<M> );
        
      case gauss_seidel:
        logging << "bind_solver(): using gauss seidel solver"<< util::Log::newline();
        return SolverBinder<M>( &gauss_seidel_solver<M> );
        
      default:
        assert(!"bind_solver(): unknown solver type");
        break;
    };
    
    return SolverBinder<M>();
  }
  
} //namespace prox

// PROX_BIND_SOLVER_H
#endif
