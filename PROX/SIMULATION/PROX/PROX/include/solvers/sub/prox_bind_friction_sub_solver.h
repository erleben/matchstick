#ifndef PROX_BIND_FRICTION_SUB_SOLVER_H
#define PROX_BIND_FRICTION_SUB_SOLVER_H

#include <prox_enums.h>

#include <solvers/sub/prox_friction_sub_solver.h>
#include <solvers/sub/prox_nonnegative.h>
#include <solvers/sub/prox_analytical_sphere.h>
#include <solvers/sub/prox_analytical_ellipsoid.h>
#include <solvers/sub/prox_numerical_ellipsoid.h>
#include <solvers/sub/prox_box_model.h>
#include <solvers/sub/prox_origin.h>
#include <solvers/sub/prox_infinity.h>

#include <util_log.h>

#include <cassert>

namespace prox
{
  /**
   * A friction sub solver function pointer binder.
   */  
  template<typename T>
  class FrictionSubSolverBinder
  : public FrictionSubSolver<T>
  {
  public:
    
    typedef void func_type(T const &,T const &,T const &,T const &,T const &,T const &,T const &,T &,T &,T &);
    
    func_type * m_solver;
    
  public:
    
    FrictionSubSolverBinder()
    : m_solver(0)
    {
    }
    
    FrictionSubSolverBinder(func_type * solver)
    : m_solver(solver)
    {
    }
    
  public:
    
    void operator()(
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
                    ) const
    {
      assert( this->m_solver || !"FrictonSubSolverBinder(): sub solver was null");
      
      this->m_solver(z_s,z_t,z_tau,mu_s,mu_t,mu_tau,lambda_n,lambda_s,lambda_t,lambda_tau);
    }
    
  };
    
  /**
   *
   */     
  template<typename T>
  inline FrictionSubSolverBinder<T> bind_friction_solver( friction_sub_solver_type const & type )
  {
    util::Log logging;

    switch( type )
    {
      case analytical_sphere:
        logging << "bind_friction_solver(): using analytical sphere solver"<< util::Log::newline();
        return FrictionSubSolverBinder<T>( &detail::analytical_sphere<T> );

      case analytical_ellipsoid:
        logging << "bind_friction_solver(): using analytical ellipsoid solver"<< util::Log::newline();
        return FrictionSubSolverBinder<T>( &detail::analytical_ellipsoid<T> );

      case numerical_ellipsoid:
        logging << "bind_friction_solver(): using numerical ellipsoid solver"<< util::Log::newline();
        return FrictionSubSolverBinder<T>( &detail::numerical_ellipsoid<T> );

      case box_model:
        logging << "bind_friction_solver(): using box model solver"<< util::Log::newline();
        return FrictionSubSolverBinder<T>( &detail::box_model<T> );

      case friction_origin:
        logging << "bind_friction_solver(): using origin solver"<< util::Log::newline();
        return FrictionSubSolverBinder<T>( &detail::origin3D<T> );

      case friction_infinity:
        logging << "bind_friction_solver(): using infinity solver"<< util::Log::newline();
        return FrictionSubSolverBinder<T>( &detail::infinity3D<T> );

      default:
        assert(!"bind_friction_solver(): unknown friction solver type");
        break;
    };
    
    return FrictionSubSolverBinder<T>();
  }
  
} //namespace prox

// PROX_BIND_FRICTION_SUB_SOLVER_H
#endif
