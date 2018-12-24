#include <prox_engine.h>
#include <prox_engine_data.h>

#include <util_string_helper.h>
#include <util_config_file.h>
#include <util_log.h>

namespace prox
{

  std::string const Engine::PARAM_SOLVER                     = "solver";
  std::string const Engine::VALUE_JACOBI                     = "jacobi";
  std::string const Engine::VALUE_GAUSS_SEIDEL               = "gauss_seidel";

  std::string const Engine::PARAM_NORMAL_SOLVER              = "normal_sub_solver";
  std::string const Engine::VALUE_NONNEGATIVE                = "nonnegative";
  std::string const Engine::VALUE_ORIGIN                     = "origin";
  std::string const Engine::VALUE_INFINITY                   = "infinity";

  std::string const Engine::PARAM_FRICTION_SOLVER            = "friction_sub_solver";
  std::string const Engine::VALUE_ANALYTICAL_SPHERE          = "analytical_sphere";
  std::string const Engine::VALUE_ANALYTICAL_ELLIPSOID       = "analytical_ellipsoid";
  std::string const Engine::VALUE_NUMERICAL_ELLIPSOID        = "numerical_ellipsoid";
  std::string const Engine::VALUE_BOX_MODEL                  = "box_model";

  std::string const Engine::PARAM_TIME_STEPPER               = "time_stepper";
  std::string const Engine::VALUE_MOREAU                     = "moreau";
  std::string const Engine::VALUE_SEMI_IMPLICIT              = "semi_implicit";
  std::string const Engine::VALUE_EMPTY                      = "empty";

  std::string const Engine::PARAM_R_FACTOR                   = "r_factor_strategy";
  std::string const Engine::VALUE_LOCAL                      = "local";
  std::string const Engine::VALUE_GLOBAL                     = "global";
  std::string const Engine::VALUE_BLOCK                      = "blocked";

  std::string const Engine::PARAM_CONTACT_ALGORITHM          = "contact_algorithm";
  std::string const Engine::VALUE_OPPOSING                   = "opposing";
  std::string const Engine::VALUE_GROWTH                     = "growth";
  std::string const Engine::VALUE_CLOSEST                    = "closest";

  std::string const Engine::PARAM_PRE_STABILIZATION          = "pre_stabilization";
  std::string const Engine::PARAM_POST_STABILIZATION         = "post_stabilization";
  std::string const Engine::PARAM_COTNACT_REDUCTION          = "contact_reduction";
  std::string const Engine::PARAM_BOUNCE_ON                  = "bounce_on";
  std::string const Engine::PARAM_TETGEN_QUIET               = "tetgen_quiet_output";
  std::string const Engine::PARAM_TETGEN_SUPPRESS_SPLITTING  = "tetgen_suppress_splitting";
  std::string const Engine::PARAM_MAX_ITERATION              = "max_iteration";
  std::string const Engine::PARAM_NARROW_CHUNK_BYTES         = "narrow_chunk_bytes";
  std::string const Engine::PARAM_ABSOLUTE_TOLERANCE         = "absolute_tolerance";
  std::string const Engine::PARAM_RELATIVE_TOLERANCE         = "relative_tolerance";
  std::string const Engine::PARAM_GAP_REDUCTION              = "gap_reduction";
  std::string const Engine::PARAM_MAX_GAP                    = "max_gap";
  std::string const Engine::PARAM_MIN_GAP                    = "min_gap";
  std::string const Engine::PARAM_TETGEN_QUALITY_RATIO       = "tetgen_quality_ratio";
  std::string const Engine::PARAM_TETGEN_MAXIMUM_VOLUME      = "tetgen_maximum_volume";
  std::string const Engine::PARAM_NARROW_ENVELOPE            = "narrow_envelope";
  std::string const Engine::PARAM_COLLISION_ENVELOPE         = "collision_envelope";
  std::string const Engine::PARAM_TIME_STEP                  = "time_step";

  std::string const Engine::PARAM_BROAD_PHASE_ALGORITHM      = "broad_phase_algorithm";
  std::string const Engine::VALUE_ALL_PAIR                   = "all_pair";
  std::string const Engine::VALUE_GRID                       = "grid";


  void Engine::set_parameter(std::string const & name, std::string const & value )
  {
    assert( m_data || !"set_parameter(): internal error, null pointer");

    if (name == PARAM_SOLVER)
    {
      if (value == VALUE_JACOBI)
      {
        m_data->m_params.solver_params().set_solver(prox::jacobi);
      }
      else if (value == VALUE_GAUSS_SEIDEL)
      {
        m_data->m_params.solver_params().set_solver(prox::gauss_seidel);
      }
      else
      {
        util::Log logging;

        logging << "Engine::set_parameter(): unknown solver value = " << value << util::Log::newline();
      }
    }
    else if (name == PARAM_NORMAL_SOLVER)
    {
      if (value == VALUE_NONNEGATIVE)
      {
        m_data->m_params.solver_params().set_normal_sub_solver(prox::nonnegative);
      }
      else if (value == VALUE_ORIGIN)
      {
        m_data->m_params.solver_params().set_normal_sub_solver(prox::normal_origin);
      }
      else if (value == VALUE_INFINITY)
      {
        m_data->m_params.solver_params().set_normal_sub_solver(prox::normal_infinity);
      }
      else
      {
        util::Log logging;

        logging << "Engine::set_parameter(): unknown normal sub solver value = " << value << util::Log::newline();
      }
    }
    else if (name == PARAM_FRICTION_SOLVER)
    {
      if (value == VALUE_ANALYTICAL_SPHERE)
      {
        m_data->m_params.solver_params().set_friction_sub_solver(prox::analytical_sphere);
      }
      else if (value == VALUE_ANALYTICAL_ELLIPSOID)
      {
        m_data->m_params.solver_params().set_friction_sub_solver(prox::analytical_ellipsoid);
      }
      else if (value == VALUE_NUMERICAL_ELLIPSOID)
      {
        m_data->m_params.solver_params().set_friction_sub_solver(prox::numerical_ellipsoid);
      }
      else if (value == VALUE_BOX_MODEL)
      {
        m_data->m_params.solver_params().set_friction_sub_solver(prox::box_model);
      }
      else if (value == VALUE_INFINITY)
      {
        m_data->m_params.solver_params().set_friction_sub_solver(prox::friction_infinity);
      }
      else if (value == VALUE_ORIGIN)
      {
        m_data->m_params.solver_params().set_friction_sub_solver(prox::friction_origin);
      }
      else
      {
        util::Log logging;

        logging << "Engine::set_parameter(): unknown friction sub solver value = " << value << util::Log::newline();
      }
    }
    else if (name == PARAM_TIME_STEPPER)
    {
      if (value == VALUE_MOREAU)
      {
        m_data->m_params.stepper_params().set_stepper(prox::moreau);
      }
      else if (value == VALUE_SEMI_IMPLICIT)
      {
        m_data->m_params.stepper_params().set_stepper(prox::semi_implicit);
      }
      else if (value == VALUE_EMPTY)
      {
        m_data->m_params.stepper_params().set_stepper(prox::empty);
      }
      else
      {
        util::Log logging;

        logging << "Engine::set_parameter(): unknown time stepper value = " << value << util::Log::newline();
      }
    }
    else if (name == PARAM_R_FACTOR)
    {
      if (value == VALUE_LOCAL)
      {
        m_data->m_params.solver_params().set_r_factor_strategy(prox::local_strategy);
      }
      else if (value == VALUE_GLOBAL)
      {
        m_data->m_params.solver_params().set_r_factor_strategy(prox::global_strategy);
      }
      else if (value == VALUE_BLOCK)
      {
        m_data->m_params.solver_params().set_r_factor_strategy(prox::blocked_strategy);
      }
      else
      {
        util::Log logging;

        logging << "Engine::set_parameter(): unknown r-factor strategy value = " << value << util::Log::newline();
      }
    }
    else if (name == "contact_algorithm")
    {
      /* Allowed values of value

        VALUE_CLOSEST
        VALUE_INTERSECTION
        VALUE_VERTEX
        VALUE_CONSISTENT
        VALUE_SAT
        VALUE_RESTRICTED
        VALUE_OPPOSING
        VALUE_GROWTH
       
       */
      kdop::SelectContactPointAlgorithm::set_algorithm( value );
    }
    else if (name == PARAM_BROAD_PHASE_ALGORITHM)
    {
      if (value == VALUE_ALL_PAIR)
      {
        m_data->m_params.use_all_pair() = true;
      }
      else if (value == VALUE_GRID)
      {
        m_data->m_params.use_all_pair() = false;
      }
      else
      {
        util::Log logging;

        logging << "Engine::set_parameter(): unknown broad phase algorithm = " << value << util::Log::newline();
      }
    }
    else
    {
      util::Log logging;

      logging << "Engine::set_parameter(string): unknown paramter name = " << name << util::Log::newline();
    }
  }

  void Engine::set_parameter(std::string const & name, bool const & value )
  {
    assert( m_data || !"set_parameter(bool): internal error, null pointer");

    if (name == PARAM_PRE_STABILIZATION)
    {
      m_data->m_params.stepper_params().set_pre_stabilization(value);
    }
    else if (name == PARAM_POST_STABILIZATION)
    {
      m_data->m_params.stepper_params().set_post_stabilization(value);
    }
    else if (name == PARAM_COTNACT_REDUCTION)
    {
      m_data->m_params.stepper_params().set_contact_reduction(value);
    }
    else if (name == PARAM_BOUNCE_ON)
    {
      m_data->m_params.stepper_params().set_bounce_on(value);
    }
    else if (name == PARAM_TETGEN_QUIET)
    {
      m_data->m_tetgen_settings.m_quiet_output = value;
    }
    else if (name == PARAM_TETGEN_SUPPRESS_SPLITTING)
    {
      m_data->m_tetgen_settings.m_suppress_splitting = value;
    }
    else
    {
      util::Log logging;

      logging << "Engine::set_parameter(bool): unknown params name = " << name << util::Log::newline();
    }
  }

  void Engine::set_parameter(std::string const & name, unsigned int const & value )
  {
    assert( m_data || !"set_parameter(unsigned int): internal error, null pointer");

    if (name == PARAM_MAX_ITERATION)
    {
      m_data->m_params.solver_params().set_max_iterations(value);
    }
    else if (name == PARAM_NARROW_CHUNK_BYTES)
    {
      m_data->m_narrow.params().set_chunk_bytes( value );
    }
    else
    {
      util::Log logging;

      logging << "Engine::set_parameter(unsigned int): unknown params name = " << name <<  util::Log::newline();
    }
  }

  void Engine::set_parameter(std::string const & name, float const & value )
  {
    assert( m_data || !"set_parameter(float): internal error, null pointer");

    if (name == PARAM_ABSOLUTE_TOLERANCE)
    {
      m_data->m_params.solver_params().set_absolute_tolerance(value);
    }
    else if (name == PARAM_RELATIVE_TOLERANCE)
    {
      m_data->m_params.solver_params().set_relative_tolerance(value);
    }
    else if (name == PARAM_GAP_REDUCTION)
    {
      m_data->m_params.stepper_params().set_gap_reduction(value);
    }
    else if (name == PARAM_MAX_GAP)
    {
      m_data->m_params.stepper_params().set_max_gap(value);
    }
    else if (name == PARAM_MIN_GAP)
    {
      m_data->m_params.stepper_params().set_min_gap(value);
    }
    else if (name == PARAM_TETGEN_QUALITY_RATIO)
    {
      m_data->m_tetgen_settings.m_quality_ratio = value;
    }
    else if (name == PARAM_TETGEN_MAXIMUM_VOLUME)
    {
      m_data->m_tetgen_settings.m_maximum_volume = value;
    }
    else if (name == PARAM_NARROW_ENVELOPE || name == PARAM_COLLISION_ENVELOPE)
    {
      m_data->m_narrow.params().set_envelope( value );
    }
    else if (name == PARAM_TIME_STEP)
    {
      assert( value > 0.0f || !"set_parameter(): internal error, null pointer");

      m_data->m_time_step = value;
    }
    else
    {
      util::Log logging;

      logging << "Engine::set_parameter(float): unknown params name = " << name <<  util::Log::newline();
    }
  }

  void Engine::set_parameters_from_config_file(std::string const & cfg_file)
  {
    util::ConfigFile  settings;

    settings.load(cfg_file);

    bool         const pre_stabilization_value     = util::to_value<bool>(         settings.get_value(PARAM_PRE_STABILIZATION,         "false"  ) );
    bool         const post_stabilization_value    = util::to_value<bool>(         settings.get_value(PARAM_POST_STABILIZATION,        "false"  ) );
    bool         const contact_reduction_value     = util::to_value<bool>(         settings.get_value(PARAM_COTNACT_REDUCTION,         "true"   ) );
    bool         const tetgen_quiet_output         = util::to_value<bool>(         settings.get_value(PARAM_TETGEN_QUIET,              "true"   ) );
    bool         const tetgen_suppress_splitting   = util::to_value<bool>(         settings.get_value(PARAM_TETGEN_SUPPRESS_SPLITTING, "true"   ) );
    bool         const bounce_on_value             = util::to_value<bool>(         settings.get_value(PARAM_BOUNCE_ON,                 "true"  ) );

    set_parameter(PARAM_PRE_STABILIZATION,           pre_stabilization_value   );
    set_parameter(PARAM_POST_STABILIZATION,          post_stabilization_value  );
    set_parameter(PARAM_COTNACT_REDUCTION,           contact_reduction_value   );
    set_parameter(PARAM_TETGEN_QUIET,         tetgen_quiet_output       );
    set_parameter(PARAM_TETGEN_SUPPRESS_SPLITTING,   tetgen_suppress_splitting );
    set_parameter(PARAM_BOUNCE_ON,                   bounce_on_value           );

    unsigned int const max_iteration_value         = util::to_value<unsigned int>( settings.get_value(PARAM_MAX_ITERATION,             "1000"   ) );
    unsigned int const narrow_chunk_bytes          = util::to_value<unsigned int>( settings.get_value(PARAM_NARROW_CHUNK_BYTES,        "8000"   ) );

    set_parameter(PARAM_MAX_ITERATION,               max_iteration_value       );
    set_parameter(PARAM_NARROW_CHUNK_BYTES,          narrow_chunk_bytes        );

    float        const absolute_tolerance_value    = util::to_value<float>(        settings.get_value(PARAM_ABSOLUTE_TOLERANCE,        "0.0"    ) );
    float        const relative_tolerance_value    = util::to_value<float>(        settings.get_value(PARAM_RELATIVE_TOLERANCE,        "0.0"    ) );
    float        const gap_reduction_value         = util::to_value<float>(        settings.get_value(PARAM_GAP_REDUCTION,             "0.5"    ) );
    float        const min_gap_value               = util::to_value<float>(        settings.get_value(PARAM_MIN_GAP,                   "0.001"  ) );
    float        const max_gap_value               = util::to_value<float>(        settings.get_value(PARAM_MAX_GAP,                   "0.01"   ) );
    float        const tetgen_quality_ratio        = util::to_value<float>(        settings.get_value(PARAM_TETGEN_QUALITY_RATIO,      "2.0"    ) );
    float        const tetgen_maximum_volume       = util::to_value<float>(        settings.get_value(PARAM_TETGEN_MAXIMUM_VOLUME,     "0.1"    ) );
    float        const narrow_envelope             = util::to_value<float>(        settings.get_value(PARAM_NARROW_ENVELOPE,           "0.01"   ) );
    float        const time_step                   = util::to_value<float>(        settings.get_value(PARAM_TIME_STEP,                 "0.01"   ) );

    set_parameter(PARAM_ABSOLUTE_TOLERANCE,          absolute_tolerance_value  );
    set_parameter(PARAM_RELATIVE_TOLERANCE,          relative_tolerance_value  );
    set_parameter(PARAM_GAP_REDUCTION,               gap_reduction_value       );
    set_parameter(PARAM_MIN_GAP,                     min_gap_value             );
    set_parameter(PARAM_MAX_GAP,                     max_gap_value             );
    set_parameter(PARAM_TETGEN_QUALITY_RATIO,        tetgen_quality_ratio      );
    set_parameter(PARAM_TETGEN_MAXIMUM_VOLUME,       tetgen_maximum_volume     );
    set_parameter(PARAM_NARROW_ENVELOPE,             narrow_envelope           );
    set_parameter(PARAM_TIME_STEP,                   time_step                 );

    float        const gravity_x_value             = util::to_value<float>(        settings.get_value("gravity_x",                 "0.0"    ) );
    float        const gravity_y_value             = util::to_value<float>(        settings.get_value("gravity_y",                 "1.0"    ) );
    float        const gravity_z_value             = util::to_value<float>(        settings.get_value("gravity_z",                 "0.0"    ) );
    float        const gravity_acceleration_value  = util::to_value<float>(        settings.get_value("gravity_acceleration",      "9.82"   ) );
    float        const damping_linear_value        = util::to_value<float>(        settings.get_value("damping_linear",            "0.01"   ) );
    float        const damping_angular_value       = util::to_value<float>(        settings.get_value("damping_angular",           "0.01"   ) );

    set_gravity_acceleration(gravity_acceleration_value);
    set_gravity_up(gravity_x_value,gravity_y_value,gravity_z_value);
    set_damping_parameters(damping_linear_value,damping_angular_value);

    std::string  const friction_sub_solver         = settings.get_value(PARAM_FRICTION_SOLVER,     VALUE_ANALYTICAL_SPHERE );
    std::string  const normal_sub_solver           = settings.get_value(PARAM_NORMAL_SOLVER,       VALUE_NONNEGATIVE       );
    std::string  const solver                      = settings.get_value(PARAM_SOLVER,              VALUE_GAUSS_SEIDEL      );
    std::string  const r_factor_strategy           = settings.get_value(PARAM_R_FACTOR,            VALUE_LOCAL             );
    std::string  const time_stepper                = settings.get_value(PARAM_TIME_STEPPER,        VALUE_MOREAU            );
    std::string  const contact_algorithm           = settings.get_value(PARAM_CONTACT_ALGORITHM,   VALUE_OPPOSING          );
    std::string  const broad_phase_algorithm       = settings.get_value(PARAM_BROAD_PHASE_ALGORITHM,   VALUE_GRID          );

    set_parameter(PARAM_FRICTION_SOLVER,         friction_sub_solver       );
    set_parameter(PARAM_NORMAL_SOLVER,           normal_sub_solver         );
    set_parameter(PARAM_SOLVER,                  solver                    );
    set_parameter(PARAM_R_FACTOR,                r_factor_strategy         );
    set_parameter(PARAM_TIME_STEPPER,            time_stepper              );
    set_parameter(PARAM_CONTACT_ALGORITHM,       contact_algorithm         );
    set_parameter(PARAM_BROAD_PHASE_ALGORITHM,   broad_phase_algorithm     );
  }

}// namespace prox
