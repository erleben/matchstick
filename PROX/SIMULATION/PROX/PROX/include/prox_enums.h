#ifndef PROX_ENUMS_H
#define PROX_ENUMS_H

namespace prox
{
  /**
   * Different time stepping methods.
   */
  typedef enum {
      moreau
    , semi_implicit
    , empty
  } stepper_type;


  /**
   * Different sovler types.
   */
  typedef enum {
    jacobi
    , gauss_seidel
  } solver_type;


  /**
   * Different R-factor strategies for PROX formulation.
   */
  typedef enum {
    local_strategy
    , global_strategy
    , blocked_strategy
  } strategy_type;


  /**
   * Normal sub solver type.
   * Numerical subroutine used for projecting the normal force onto the normal
   * cone. Different solvers allow for different behaviors of numerical methods.
   */
  typedef enum {
    nonnegative
    , normal_origin
    , normal_infinity
  } normal_sub_solver_type;

  /**
   * Frictional sub solver type.
   * Numerical subroutine used for projecting the friction force onto the friction
   * cone. Different solvers allow for different shaped friction cones.
   */
  typedef enum {
    analytical_sphere
    , analytical_ellipsoid
    , numerical_ellipsoid
    , box_model
    , friction_origin
    , friction_infinity
  } friction_sub_solver_type;


} // namespace prox

// PROX_ENUMS_H
#endif
