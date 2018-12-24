#ifndef PROX_GAUSS_SEIDEL_SOLVER_H
#define PROX_GAUSS_SEIDEL_SOLVER_H

#include <solvers/sub/prox_normal_sub_solver.h>
#include <solvers/sub/prox_friction_sub_solver.h>
#include <solvers/strategies/prox_R_strategy.h>

#include <solvers/prox_solver_params.h>

#include <util_profiling.h>
#include <util_log.h>

namespace prox
{
  /**
   * This solver use the proximal map formulation of the constraints
   * on the motion of a rigid body system to compute the contact impulses
   * of the contact points. The system of constraints is solved using a
   * factorized Gauss-Seidel approach, where each constraint is solved in
   * turn using the updated results from previously solved constraints.
   */
  template< typename M >
  inline void gauss_seidel_solver(
                                  typename M::compressed4x6_type const& J
                                  , typename M::compressed6x4_type const& WJT
                                  , typename M::vector4_type const& b
                                  , typename M::vector4_type const& mu
                                  , typename M::vector4_type & lambda
                                  , RStrategy<M> const & strategy
                                  , NormalSubSolver<typename M::real_type> const & normal_solver
                                  , FrictionSubSolver<typename M::real_type> const & friction_solver
                                  , SolverParams<M> const& params
                                  , M const & tag
                                  )
  {
    RECORD_VECTOR_NEW("convergence");
    RECORD_VECTOR_NEW("rfactor");

    typedef typename M::block4x1_type       B4x1;
    typedef typename M::vector4_type        V4;
    typedef typename M::vector6_type        V6;
    typedef typename M::diagonal4x4_type    D4x4;
    typedef typename M::real_type           T;
    typedef typename M::value_traits        VT;
    
    START_TIMER("solver_time");
    
    size_t const K = J.nrows( ); // Number of blocks

    size_t abs_conv_in_iteration = 0u; //used for profiling, to record in what iteration we found absolute convergence
    size_t rel_conv_in_iteration = 0u; //used for profiling, to record in what iteration we found relative convergence
    size_t count_divergence      = 0u; //used for profiling, to record how many times we have discovered divergence
    
    if( !params.use_warm_starting() )
    {
      lambda.resize( K );
    }
    
    if( K == 0u )
      return;
    
    V4 x;           // Solution iterates, separate from lambda to be able to roll back
    
    x.resize( K );  // 2012-07-14 Sarah code review: x only gets initialized
                    // when using warm starting? but x(k) is used in either case
    
    if( params.use_warm_starting() )
    {
      x = lambda;   // Only in case of warm-starting
    }
    
    V4 residual;
    residual.resize( K );
    
    T last_residual_norm = VT::infinity();    // Used to detect divergence.
    
    D4x4 R;
    D4x4 nu;


    strategy(J, WJT, R, nu );

    V6 w;
    w.resize( J.ncols() );// what is in w???
    
    T residual_norm;
    
    B4x1 z_k(     VT::zero() );
    B4x1 delta_x( VT::zero() );
        
    //--- Gauss--Seidel loops
    for(size_t iteration = 0u; iteration < params.max_iterations(); ++iteration )
    {
      RECORD_VECTOR_PUSH("rfactor", R(1)(1,1));

      //--- Loop over contact points
      for(size_t k = 0u; k < K; ++k)
      {
        B4x1 const &  mu_k    = mu(k);
        B4x1       &  x_k     = x(k);
        delta_x               = x(k); // save old value

        
        M::compute_z_k( x_k, w, R(k), J, b(k), z_k, k );
        
        size_t const n   = 0u;
        size_t const s   = 1u;
        size_t const t   = 2u;
        size_t const tau = 3u;
        
        //--- Solve lambda_n = prox_{R^+}( lambda_n - r (A lambda_n + b))
        normal_solver( z_k(n), x_k(n) );
        
        //--- Solve lambda_f = prox_C( lambda_f - r (A lambda_f + b))
        friction_solver(z_k(s), z_k(t), z_k(tau), mu_k(s), mu_k(t), mu_k(tau), x_k(n), x_k(s), x_k(t), x_k(tau));
        
        //--- delta_x = x_k_new - x_k_old (saved in delta_x)
        sparse::sub(x_k, delta_x, delta_x);
        
        //--- Updating w, math_policy::update_w(WJT, J, delta_x, k, w);
        sparse::column_prod( WJT, J, delta_x, w, k);
      }
      
      //--- compute the residual, residual = lambda^k - lambda^(k+1)
      sparse::sub(lambda, x, residual);
      residual_norm = M::compute_norm_inf( residual );
      
      RECORD_VECTOR_PUSH("convergence", residual_norm );

      if( residual_norm < params.absolute_tolerance() )
      {
        util::Log logging;

        logging << "gauss_seidel_solver(): absolute convergence in "
                << iteration
                << " iterations |residual| = "
                << residual_norm
                << util::Log::newline();

        abs_conv_in_iteration = iteration;

        break;
      }
      
      if( fabs(residual_norm-last_residual_norm) < params.relative_tolerance()*last_residual_norm )
      {
        util::Log logging;

        logging << "gauss_seidel_solver(): relative convergence in "
                << iteration
                << " iterations"
                << util::Log::newline();

        rel_conv_in_iteration = iteration;

        break;
      }
      
      if( residual_norm > last_residual_norm)
      {
        util::Log logging;

        logging << "gauss_seidel_solver(): divergence in "
                << iteration
                << " iterations. |residual| = "
                << residual_norm
                << util::Log::newline();

        // Reduce R-factor and roll-back solution to last known good iterate!
        M::compute_prod( nu, R );
        x = lambda;
        ++count_divergence;
      }
      else
      {
        // save x to lambda.
        last_residual_norm = residual_norm;
        lambda = x;
      }
    }
    lambda = x;
 
    RECORD("solver_abs_conv_in",   abs_conv_in_iteration);
    RECORD("solver_rel_conv_in",   rel_conv_in_iteration);
    RECORD("solver_div_count",     count_divergence     );
    STOP_TIMER("solver_time");
  }
  
} //namespace prox

// PROX_GAUSS_SEIDEL_SOLVER_H
#endif
