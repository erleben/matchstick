#ifndef PROX_JACOBI_SOLVER_H
#define PROX_JACOBI_SOLVER_H

#include <solvers/prox_solver_params.h>

#include <solvers/sub/prox_normal_sub_solver.h>
#include <solvers/sub/prox_friction_sub_solver.h>
#include <solvers/strategies/prox_R_strategy.h>

#include <util_profiling.h>
#include <util_log.h>

#include <sparse.h>

namespace prox
{
  
  /**
   * This solver use the proximal map formulation of the constraints
   * on the motion of a rigid body system to compute the contact impulses
   * of the contact points. The system of constraints is solved using a
   * factorized Jacobi approach, where each constraint is solved atomically
   * in turn.
   */
  template< typename M >
  inline void jacobi_solver( 
                            typename M::compressed4x6_type const& J
                            , typename M::compressed6x4_type const& WJT
                            , typename M::vector4_type const& b 
                            , typename M::vector4_type const& mu 
                            , typename M::vector4_type & lambda 
                            , RStrategy<M> const & strategy
                            , NormalSubSolver<typename M::real_type> const & normal_solver
                            , FrictionSubSolver<typename M::real_type> const & friction_solver
                            , SolverParams<M> const & params
                            , M const & tag
                            ) 
  {
    RECORD_VECTOR_NEW("convergence");
    RECORD_VECTOR_NEW("rfactor");

    typedef typename M::real_type           T;
    typedef typename M::value_traits        VT;
    typedef typename M::block4x1_type       B4x1;
    typedef typename M::vector4_type        V4;
    typedef typename M::diagonal4x4_type    D4x4;
    
    START_TIMER("solver_time");
    
    size_t const K = J.nrows( ); // Number of blocks

    size_t abs_conv_in_iteration = 0u; //used for profiling, to record in what iteration we found absolute convergence
    size_t rel_conv_in_iteration = 0u; //used for profiling, to record in what iteration we found relative convergence
    size_t count_divergence      = 0u; //used for profiling, to record how many times we have discovered divergence

    //--- If warm starting is not used then clear the initial iterate
    if(! params.use_warm_starting() )
    {
      lambda.resize( K );
    }
    
    if( K == 0u )
      return;
    
    size_t in = 0;    // Solution vector index that remembers the input iterate to the Jacobi scheme.
    size_t out = 1;   // Solution vector index that remembers the output iterate to the Jacobi scheme.
    
    V4 x[2]; // solution iterates, needed to do flip-flopping of solution vector in the Jacobi scheme
    x[in].resize( K );
    x[out].resize( K );
    
    if(params.use_warm_starting())
    {
      x[in] = lambda;
    }
    
    V4 residual;
    residual.resize( K );
    
    T last_residual_norm = VT::infinity(); // Used to detect divergence.
    
    D4x4 R;
    D4x4 nu;
    
    strategy(J, WJT, R, nu );
    
    V4 z;
    bool last_iteration_diverged = false;
    
    //--- Jacobi loops
    for(size_t iteration = 0u; iteration < params.max_iterations(); ++iteration )
    {
      RECORD_VECTOR_PUSH("rfactor", R(1)(1,1));

      last_iteration_diverged = false;
      
      //--- Compute z = x - R(J W J^T x  + b)  = x - R( A x  + b)
      M::compute_z( x[in], R, J, WJT, b, z );
      
      for(size_t k = 0u; k < K;++k )
      {
        B4x1 const &  mu_k    = mu(k);
        B4x1 const &  z_k     = z(k);
        B4x1 const &  x_k_in  = x[in](k);
        B4x1       &  x_k_out = x[out](k);
        
        size_t const n   = 0u;
        size_t const s   = 1u;
        size_t const t   = 2u;
        size_t const tau = 3u;
        
        normal_solver( z_k(n), x_k_out(n) );
        
        friction_solver(z_k(s), z_k(t), z_k(tau), mu_k(s), mu_k(t), mu_k(tau), x_k_in(n), x_k_out(s), x_k_out(t), x_k_out(tau));
      }
      
      //--- Compute residual, residual = lambda^k - lambda^(k+1)
      sparse::sub(x[in], x[out], residual );
      
      T const residual_norm = M::compute_norm_inf( residual );
      
      RECORD_VECTOR_PUSH( "convergence", residual_norm );
      
      if( residual_norm < params.absolute_tolerance() ) 
      {
        util::Log logging;

        logging << "jacobi_solver(): absolute convergence in "
                << iteration
                << " iterations |residual| = "
                << residual_norm
                << util::Log::newline();

        abs_conv_in_iteration = iteration;

        break;
      }
      
      if( fabs(residual_norm - last_residual_norm) < params.relative_tolerance()*last_residual_norm ) 
      {
        util::Log logging;

        logging << "jacobi_solver(): relative convergence in "
                << iteration
                << " iterations"
                << util::Log::newline();

        rel_conv_in_iteration = iteration;

        break;
      }
      
      if( residual_norm > last_residual_norm ) 
      {
        util::Log logging;

        logging << "jacobi_solver(): divergence in "
                << iteration
                << " iterations. |residual| = "
                << residual_norm
                << util::Log::newline();

        // Reduce R-factor and roll-back solution to last known good
        // iterate! (same as not doing a flip-flop on x-vectors).
        M::compute_prod( nu, R );

        last_iteration_diverged = true;

        ++count_divergence;
      }
      else
      {
        last_residual_norm = residual_norm;
        std::swap( in, out );
        last_iteration_diverged = false;
      }
    }

    if( last_iteration_diverged )
      lambda = x[in];
    else
      lambda = x[out];
    
    RECORD("solver_abs_conv_in",   abs_conv_in_iteration);
    RECORD("solver_rel_conv_in",   rel_conv_in_iteration);
    RECORD("solver_div_count",     count_divergence     );
    STOP_TIMER("solver_time");
  }
  
} //namespace prox

// PROX_JACOBI_SOLVER_H
#endif
