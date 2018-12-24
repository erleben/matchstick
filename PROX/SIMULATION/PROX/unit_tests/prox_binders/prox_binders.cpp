#include <solvers/prox_bind_solver.h>
#include <solvers/strategies/prox_bind_R_strategy.h>
#include <solvers/sub/prox_bind_normal_sub_solver.h>
#include <solvers/sub/prox_bind_friction_sub_solver.h>
#include <steppers/prox_bind_stepper.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/test_tools.hpp>

#define SHUT_UP_COMPILER_WARNING( unused_variable )   size_t(&unused_variable) % 2 == 0


BOOST_AUTO_TEST_SUITE(prox_binders);

BOOST_AUTO_TEST_CASE(compile_testing)
{
  typedef float T;
  typedef prox::MathPolicy<T> M;

  prox::RStrategyBinder<M>         strategy1        = prox::bind_strategy<M>( prox::local_strategy );
  prox::RStrategyBinder<M>         strategy2        = prox::bind_strategy<M>( prox::global_strategy );
  prox::RStrategyBinder<M>         strategy3        = prox::bind_strategy<M>( prox::blocked_strategy );

  prox::NormalSubSolverBinder<T>   normal_solver1   = prox::bind_normal_solver<T>( prox::nonnegative );
  prox::NormalSubSolverBinder<T>   normal_solver2   = prox::bind_normal_solver<T>( prox::normal_origin );
  prox::NormalSubSolverBinder<T>   normal_solver3   = prox::bind_normal_solver<T>( prox::normal_infinity );

  prox::FrictionSubSolverBinder<T> friction_solver1 = prox::bind_friction_solver<T>( prox::analytical_sphere );
  prox::FrictionSubSolverBinder<T> friction_solver2 = prox::bind_friction_solver<T>( prox::analytical_ellipsoid );
  prox::FrictionSubSolverBinder<T> friction_solver3 = prox::bind_friction_solver<T>( prox::numerical_ellipsoid );
  prox::FrictionSubSolverBinder<T> friction_solver5 = prox::bind_friction_solver<T>( prox::box_model );
  prox::FrictionSubSolverBinder<T> friction_solver6 = prox::bind_friction_solver<T>( prox::friction_origin );
  prox::FrictionSubSolverBinder<T> friction_solver7 = prox::bind_friction_solver<T>( prox::friction_infinity );
  
  prox::SolverBinder<M>            prox_solver1     = prox::bind_solver<M>( prox::jacobi );
  prox::SolverBinder<M>            prox_solver2     = prox::bind_solver<M>( prox::gauss_seidel );

  prox::StepperBinder<M>            prox_stepper1     = prox::bind_stepper<M>( prox::moreau );
  prox::StepperBinder<M>            prox_stepper2     = prox::bind_stepper<M>( prox::semi_implicit );


  SHUT_UP_COMPILER_WARNING( strategy1 );
  SHUT_UP_COMPILER_WARNING( strategy2 );
  SHUT_UP_COMPILER_WARNING( strategy3 );

  SHUT_UP_COMPILER_WARNING( normal_solver1 );
  SHUT_UP_COMPILER_WARNING( normal_solver2 );
  SHUT_UP_COMPILER_WARNING( normal_solver3 );

  SHUT_UP_COMPILER_WARNING( friction_solver1 );
  SHUT_UP_COMPILER_WARNING( friction_solver2 );
  SHUT_UP_COMPILER_WARNING( friction_solver3 );
  SHUT_UP_COMPILER_WARNING( friction_solver5 );
  SHUT_UP_COMPILER_WARNING( friction_solver6 );
  SHUT_UP_COMPILER_WARNING( friction_solver7 );

  SHUT_UP_COMPILER_WARNING( prox_solver1 );
  SHUT_UP_COMPILER_WARNING( prox_solver2 );

  SHUT_UP_COMPILER_WARNING( prox_stepper1 );
  SHUT_UP_COMPILER_WARNING( prox_stepper2 );
}

BOOST_AUTO_TEST_SUITE_END();
