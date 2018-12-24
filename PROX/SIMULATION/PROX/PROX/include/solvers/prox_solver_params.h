#ifndef PROX_SOLVER_PARAMS_H
#define PROX_SOLVER_PARAMS_H

#include <prox_enums.h>

#include <tiny_is_number.h>
#include <tiny_is_finite.h>

#include <cassert>
#include <cstdlib>

namespace prox
{    
  /**
   * Paramters for controlling prox solvers.
   *
   * @tparam   Math types policy
   */
  template< typename MT >
  class SolverParams
  {
  public:
    
    typedef typename MT::real_type           T;
    typedef typename MT::value_traits        VT;
    
  protected:
    
    size_t                   m_max_iterations;           ///< The maximum number of allowed outer iterations.
    T                        m_absolute_tolerance;       ///< The absolute tolerance value.
    T                        m_relative_tolerance;       ///< The relative tolerance value.
    bool                     m_use_warm_starting;        ///< Boolean flag that indicates whether warmstarting is used or not.
    
    solver_type              m_solver;                   ///< The solver type.
    strategy_type            m_r_factor_strategy;        ///< The R-factor strategy type.
    normal_sub_solver_type   m_normal_sub_solver;        ///< The normal sub solver type.
    friction_sub_solver_type m_friction_sub_solver;      ///< The friction sub solver type.

  public:
    
    size_t const & max_iterations()     const { return this->m_max_iterations;       }
    T      const & absolute_tolerance() const { return this->m_absolute_tolerance;   }
    T      const & relative_tolerance() const { return this->m_relative_tolerance;   }
    bool   const & use_warm_starting()  const { return this->m_use_warm_starting;    }
    
    solver_type              const & solver()               const { return this->m_solver;              }    
    strategy_type            const & r_factor_strategy()    const { return this->m_r_factor_strategy;   }
    normal_sub_solver_type   const & normal_sub_solver()    const { return this->m_normal_sub_solver;   }
    friction_sub_solver_type const & friction_sub_solver()  const { return this->m_friction_sub_solver; }
    
    
  public:      
    
    void set_max_iterations(size_t const & iterations) 
    { 
      assert(iterations> 0u || !"set_max_iterations(): number of iterations must be positive");
      this->m_max_iterations = iterations;
    }
    
    void set_absolute_tolerance(T const & value)
    {
      assert(value >= VT::zero() || !"set_absolute_tolerance(): value must be positive");
      assert(is_number(value)    || !"set_absolute_tolerance(): value must be a number");
      assert(is_finite(value)    || !"set_absolute_tolerance(): value must be a finite value");
      
      this->m_absolute_tolerance = value; 
    }

    /**
     * @note     Does not accepting relative convergence when set to VT::zero()
     */
    void set_relative_tolerance(T const & value)
    {
      assert(value >= VT::zero() || !"set_relative_tolerance(): value must be positive");
      assert(is_number(value)    || !"set_relative_tolerance(): value must be a number");
      assert(is_finite(value)    || !"set_relative_tolerance(): value must be a finite value");
      
      this->m_relative_tolerance = value; 
    }
    
    void set_use_warm_starting(bool const & value) 
    { 
      this->m_use_warm_starting = value;
    }
    
    void set_solver(solver_type const & value) 
    { 
      this->m_solver = value;
    }
    
    void set_r_factor_strategy(strategy_type const & value) 
    { 
      this->m_r_factor_strategy = value;
    }

    /**
     * @note   Does not including friction when set to friction_origin
     */
    void set_friction_sub_solver(friction_sub_solver_type const & value) 
    { 
      this->m_friction_sub_solver = value;
    }
    
    void set_normal_sub_solver(normal_sub_solver_type const & value) 
    { 
      this->m_normal_sub_solver = value;
    }
    
  public:
    
    SolverParams()
    : m_max_iterations(500)
    , m_absolute_tolerance(VT::numeric_cast(10e-5f) )
    , m_relative_tolerance(VT::zero() )
    , m_use_warm_starting(false)
    , m_solver(gauss_seidel) 
    , m_r_factor_strategy(local_strategy)
    , m_normal_sub_solver(nonnegative) 
    , m_friction_sub_solver(analytical_ellipsoid)
    {}

  };
  
} // namespace prox

// PROX_SOLVER_PARAMS_H
#endif 
