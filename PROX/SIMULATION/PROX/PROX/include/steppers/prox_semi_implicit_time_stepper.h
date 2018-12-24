#ifndef PROX_SEMI_IMPLICIT_TIME_STEPPER_H
#define PROX_SEMI_IMPLICIT_TIME_STEPPER_H

#include <prox_rigid_body.h> 
#include <prox_contact_point.h> 

#include <prox_get_mass_matrix.h> 
#include <prox_get_inverse_mass_matrix.h> 
#include <prox_get_jacobian_matrix.h> 
#include <prox_get_external_forces_vector.h>
#include <prox_get_position_vector.h> 
#include <prox_get_velocity_vector.h> 
#include <prox_get_pre_stabilization_vector.h> 
#include <prox_get_post_stabilization_vector.h>
#include <prox_get_restitution_vector.h>
#include <prox_get_friction_coefficient_vector.h> 

#include <prox_set_position_vector.h> 
#include <prox_set_velocity_vector.h> 

#include <prox_position_update.h> 
#include <prox_velocity_update.h> 
#include <prox_collision_detection.h> 
#include <prox_update_body_indices.h> 

#include <prox_params.h>
#include <prox_math_policy.h>

#include <solvers/prox_bind_solver.h>
#include <solvers/sub/prox_bind_normal_sub_solver.h>
#include <solvers/sub/prox_bind_friction_sub_solver.h>
#include <solvers/strategies/prox_bind_R_strategy.h>

#include <util_profiling.h>
#include <broad.h>
#include <narrow.h>

#include <vector>

namespace prox
{
  
  /**
   *
   */
  template< typename M  > 
  inline void semi_implicit_time_stepper( 
                                          typename M::real_type const & dt
                                         , std::vector< RigidBody< M > > & bodies
                                         , std::vector< std::vector< MatchStickModel< M > > > const &  properties
                                         , Gravity< M > const & gravity
                                         , Damping< M > const & damping
                                         , Params<M> const & params
                                         , broad::System<typename M::real_type> & broad_system
                                         , narrow::System<typename M::tiny_types> & narrow_system
                                         , std::vector< ContactPoint<M> > & contacts
                                         , M const & tag
                                         )
  {     
    typedef typename M::vector4_type               V4;
    typedef typename M::vector6_type               V6;
    typedef typename M::vector7_type               V7;
    typedef typename M::diagonal6x6_type           D6x6; 
    typedef typename M::compressed4x6_type         CSR4x6;
    typedef typename M::compressed6x4_type         CSR6x4;
    typedef typename M::real_type                  T;
    typedef typename M::value_traits               VT;

    util::Log logging;
    
    START_TIMER("stepper_time");

    SolverBinder<M>            prox_solver     = bind_solver<M>( params.solver_params().solver() );
    RStrategyBinder<M>         strategy        = bind_strategy<M>( params.solver_params().r_factor_strategy() );    
    NormalSubSolverBinder<T>   normal_solver   = bind_normal_solver<T>( params.solver_params().normal_sub_solver() );
    FrictionSubSolverBinder<T> friction_solver = bind_friction_solver<T>( params.solver_params().friction_sub_solver() );

    V7        q;      // position vector
    V6        u;      // velocity vector
    D6x6      W;      // inverse mass matrix.
                      // 2009-08-13 Kenny code review: Optimization replace diagonal6x6_type
                      // with diagonal_mass_type, maybe wait to optimize until all it working
    V6        h;      // external forces
    V6        Wdth;   // the product of inverse mass matrix, time step and external forces, W*dt*h
    V6        fc;     // contact forces
    V4        g;      // correction/stabilization term
    V4        e;      // restitution coefficients
    V4        mu;     // friction coefficients
    V4        lambda; // resulting forces
    V4        b;      // "right hand side vector"
    CSR4x6    J;      // Jacobian matrix
    CSR6x4    WJT;    // the product of the inverse mass matrix and the transposed Jacobian
    V4        w;      // Current contact velocities

    detail::update_body_indices(
                                bodies.begin()
                                , bodies.end()
                                );

    get_position_vector(
                        bodies.begin()
                        , bodies.end()
                        , q
                        , tag
                        );
    
    get_velocity_vector(
                        bodies.begin()
                        , bodies.end()
                        , u
                        , tag
                        );
        
    collision_detection(
                        bodies
                        , broad_system
                        , narrow_system
                        , contacts
                        , params
                        , tag
                        );
    
    unsigned int const number_of_contacts = contacts.size();

    logging << "semi_implicit_time_stepper(): Number of contacts = " << number_of_contacts << util::Log::newline();

    get_inverse_mass_matrix(
                            bodies.begin()
                            , bodies.end()
                            , W
                            , tag
                            );
    
    get_external_forces_vector(
                               bodies.begin()
                               , bodies.end()
                               , gravity
                               , damping
                               , h
                               , tag
                               );

    sparse::prod(dt, h);
    sparse::prod(W, h, Wdth);        // Wdth = dt M^{-1} f_ext
    
    if( number_of_contacts > 0u )
    {
      get_jacobian_matrix< RigidBody<M> >(
                                          contacts.begin()
                                          , contacts.end()
                                          , bodies
                                          , properties
                                          , J
                                          , tag
                                          , number_of_contacts
                                          );

      if(params.stepper_params().pre_stabilization())
      {
        sparse::prod(J, u, w);

        get_pre_stabilization_vector(
                                     contacts.begin()
                                     , contacts.end()
                                     , params.stepper_params()
                                     , dt
                                     , w
                                     , g
                                     , tag
                                     , number_of_contacts
                                     );
      }
      else
      {
        M::make_zero(g, number_of_contacts );
      }

      if(params.stepper_params().bounce_on())
      {
        get_restitution_vector(
                               contacts.begin()
                               , contacts.end()
                               , properties
                               , e
                               , tag
                               , number_of_contacts
                               );
      }
      else
      {
        M::make_zero(e, number_of_contacts );
      }

      get_friction_coefficient_vector(
                                      contacts.begin()
                                      , contacts.end()
                                      , properties
                                      , mu
                                      , tag
                                      , number_of_contacts
                                      );
      
      M::compute_WJT( W, J, WJT );            // WJT = M^{-1} J^T

      M::compute_b( J, Wdth, u, e, g, b );    // b   = (I+E)J u + J W (dt h)
      
      prox_solver(
                  J
                  , WJT
                  , b
                  , mu
                  , lambda
                  , strategy
                  , normal_solver
                  , friction_solver
                  , params.solver_params()
                  , tag
                  );
      
      fc.resize( WJT.nrows() );

      sparse::prod(WJT, lambda, fc, true);     // fc = M^{-1}*J^T*lambda
      
      velocity_update( u, Wdth, fc, u, tag );  // u = u + dt M^{-1} h + fc
      
    } 
    else 
    {
      velocity_update( u, Wdth, u, tag );      // u = u + dt M^{-1} h
    }
    
    position_update( q, u, dt, q, tag );
    
    set_position_vector( bodies.begin(), bodies.end(), q, tag ); 
    set_velocity_vector( bodies.begin(), bodies.end(), u, tag );  
    
    STOP_TIMER("stepper_time");

    if(params.stepper_params().post_stabilization())
    {
      START_TIMER("post_stabilization_time");

      normal_solver   = bind_normal_solver<T>( nonnegative );
      friction_solver = bind_friction_solver<T>( friction_origin );

      if( number_of_contacts > 0u )
      {
        get_post_stabilization_vector(
                                      contacts.begin()
                                      , contacts.end()
                                      , params.stepper_params()
                                      , g
                                      , tag
                                      , number_of_contacts
                                      );

        PREFIX("post_");
        prox_solver(
                    J
                    , WJT
                    , g
                    , mu
                    , lambda
                    , strategy
                    , normal_solver
                    , friction_solver
                    , params.solver_params()
                    , tag
                    );
        PREFIX("");

        sparse::prod(WJT, lambda, fc, true);

        position_update( q, fc, VT::one(), q, tag );

        set_position_vector( bodies.begin(), bodies.end(), q, tag );
      }

      STOP_TIMER("post_stabilization_time");
    }

  } 
  
} //namespace prox

// PROX_SEMI_IMPLICIT_TIME_STEPPER_H 
#endif
