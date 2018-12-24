#ifndef PROX_BIND_NORMAL_SUB_SOLVER_H
#define PROX_BIND_NORMAL_SUB_SOLVER_H

#include <prox_enums.h>

#include <solvers/sub/prox_normal_sub_solver.h>

#include <solvers/sub/prox_nonnegative.h>
#include <solvers/sub/prox_origin.h>
#include <solvers/sub/prox_infinity.h>

#include <util_log.h>

#include <cassert>

namespace prox
{

  /**
   * A normal sub solver function pointer binder.
   */
  template<typename T>
  class NormalSubSolverBinder
  : public NormalSubSolver<T>
  {
  public:
    
    typedef void func_type(T const &, T &);
    
    func_type * m_solver;
    
  public:
    
    NormalSubSolverBinder()
    : m_solver(0)
    {
    }
    
    NormalSubSolverBinder(func_type * solver)
    : m_solver(solver)
    {
    }
    
  public:
    
    void operator()(
                    T const & z_n
                    , T & lambda_n
                    ) const
    {
      assert( this->m_solver || !"NormalSubSolverBinder(): sub solver was null");
      
      this->m_solver(z_n,lambda_n);
    }
    
  };
    
  /**
   *
   */     
  template<typename T>
  inline NormalSubSolverBinder<T> bind_normal_solver( normal_sub_solver_type const & type )
  {
    util::Log logging;

    switch( type )
    {
      case nonnegative:
        logging << "bind_normal_solver(): using nonnegative solver"<< util::Log::newline();
        return NormalSubSolverBinder<T>( &detail::nonnegative<T> );

      case normal_origin:
        logging << "bind_normal_solver(): using origin solver"<< util::Log::newline();
        return NormalSubSolverBinder<T>( &detail::origin1D<T> );

      case normal_infinity:
        logging << "bind_normal_solver(): using infinity solver"<< util::Log::newline();
        return NormalSubSolverBinder<T>( &detail::infinity1D<T> );

      default:
        assert(!"bind_normal_solver(): unknown normal solver type");
        break;
    };
    
    return NormalSubSolverBinder<T>();
  }
    
} //namespace prox

// PROX_BIND_NORMAL_SUB_SOLVER_H
#endif
