#ifndef PROX_ENGINE_H
#define PROX_ENGINE_H

#include <content_api.h>

#include <vector>

namespace prox
{
  
  class EngineData; //forward declaration
  
  /**
   * Physics API for the PROX engine.
   */
  class Engine
  : public content::API
  {
  public:

    static std::string const PARAM_SOLVER;
    static std::string const VALUE_JACOBI;
    static std::string const VALUE_GAUSS_SEIDEL;
    static std::string const PARAM_NORMAL_SOLVER;
    static std::string const VALUE_NONNEGATIVE;
    static std::string const VALUE_ORIGIN;
    static std::string const VALUE_INFINITY;
    static std::string const PARAM_FRICTION_SOLVER;
    static std::string const VALUE_ANALYTICAL_SPHERE;
    static std::string const VALUE_ANALYTICAL_ELLIPSOID;
    static std::string const VALUE_NUMERICAL_ELLIPSOID;
    static std::string const VALUE_BOX_MODEL;
    static std::string const PARAM_TIME_STEPPER;
    static std::string const VALUE_MOREAU;
    static std::string const VALUE_SEMI_IMPLICIT;
    static std::string const VALUE_EMPTY;
    static std::string const PARAM_R_FACTOR;
    static std::string const VALUE_LOCAL;
    static std::string const VALUE_GLOBAL;
    static std::string const VALUE_BLOCK;
    static std::string const PARAM_CONTACT_ALGORITHM;
    static std::string const VALUE_OPPOSING;
    static std::string const VALUE_GROWTH;
    static std::string const VALUE_CLOSEST;
    static std::string const PARAM_PRE_STABILIZATION;
    static std::string const PARAM_POST_STABILIZATION;
    static std::string const PARAM_COTNACT_REDUCTION;
    static std::string const PARAM_BOUNCE_ON;
    static std::string const PARAM_TETGEN_QUIET;
    static std::string const PARAM_TETGEN_SUPPRESS_SPLITTING;
    static std::string const PARAM_MAX_ITERATION;
    static std::string const PARAM_NARROW_CHUNK_BYTES;
    static std::string const PARAM_ABSOLUTE_TOLERANCE;
    static std::string const PARAM_RELATIVE_TOLERANCE;
    static std::string const PARAM_GAP_REDUCTION;
    static std::string const PARAM_MAX_GAP;
    static std::string const PARAM_MIN_GAP;
    static std::string const PARAM_TETGEN_QUALITY_RATIO;
    static std::string const PARAM_TETGEN_MAXIMUM_VOLUME;
    static std::string const PARAM_NARROW_ENVELOPE;
    static std::string const PARAM_COLLISION_ENVELOPE;
    static std::string const PARAM_TIME_STEP;
    static std::string const PARAM_BROAD_PHASE_ALGORITHM;
    static std::string const VALUE_ALL_PAIR;
    static std::string const VALUE_GRID;

    void set_parameter(std::string const & name, bool         const & value );

    void set_parameter(std::string const & name, unsigned int const & value );

    void set_parameter(std::string const & name, float        const & value );

    void set_parameter(std::string const & name, std::string  const & value );

    void set_parameters_from_config_file(std::string const & cfg_file);

  protected:
    
    EngineData * m_data;
    
  public:
    
    Engine();
    
    Engine( std::string const & solver
           , std::string const & normal_solver
           , std::string const & friction_solver
           , std::string const & time_stepper
           , std::string const & r_factor_strategy
           );
    
    virtual ~Engine();
    
    bool simulate( float const & time );
    
    void clear();

    size_t create_rigid_body( std::string const & name );
    
    void set_rigid_body_position( size_t const & body_idx
                                 , float const & x
                                 , float const & y
                                 , float const & z
                                 );
    
    void set_rigid_body_orientation( size_t const & body_idx
                                    , float const & Qs
                                    , float const & Qx
                                    , float const & Qy
                                    , float const & Qz
                                    );
    
    void set_rigid_body_velocity( size_t const & body_idx
                                 , float const & vx
                                 , float const & vy
                                 , float const & vz
                                 );
    
    void set_rigid_body_spin( size_t const & body_idx
                             , float const & wx
                             , float const & wy
                             , float const & wz
                             );
    
    void set_rigid_body_mass( size_t const & body_idx, float const & mass);
    
    void set_rigid_body_inertia( size_t const & body_idx, float const & Ixx, float const & Iyy, float const & Izz);
    
    void set_rigid_body_active( size_t const & body_idx, bool const & active );
    
    void set_rigid_body_fixed( size_t const & body_idx, bool const & fixed );
    
    void set_rigid_body_material( size_t const & body_idx, size_t const & material_idx);
    
    void connect_force( size_t body_idx, size_t force_idx );
    
    void connect_collision_geometry(size_t body_idx, size_t geometry_idx );
    
    void set_gravity_up(
                        float const & x
                        , float const & y
                        , float const & z
                        );

    void set_gravity_acceleration( float const & acceleration );

    void set_damping_parameters( float const & linear, float const & angular );

    size_t create_material( std::string const & name );
    
    void create_contact_model( size_t const & first_idx, size_t const & second_idx);
    

    void set_coefficients_of_anisotropic_friction( size_t const & first_idx
                                                  , size_t const & second_idx
                                                  , float const & mu_x
                                                  , float const & mu_y
                                                  , float const & mu_z
                                                  );

    void set_coefficient_of_isotropic_friction( size_t const & first_idx
                                               , size_t const & second_idx
                                               , float const & mu
                                               );


    void set_material_structure_map(
                                    size_t const & body_idx
                                    , float const & dir_x
                                    , float const & dir_y
                                    , float const & dir_z
                                    );

    void set_material_structure_map(
                                    size_t const & body_idx
                                    , float const & rot_x
                                    , float const & rot_y
                                    , float const & rot_z
                                    , float const & cen_x
                                    , float const & cen_y
                                    , float const & cen_z
                                    , float const & ref_x
                                    , float const & ref_y
                                    , float const & ref_z
                                    , float const & dir_x
                                    , float const & dir_y
                                    , float const & dir_z
                                    );

    void set_material_structure_map(size_t const & body_idx, size_t const & N, size_t * indices, float *  data);

    void get_material_structure_map_size(size_t const & body_idx, size_t & N);

    void get_material_structure_map_data(size_t const & body_idx, size_t * indices, float *  structure_map);

    void set_coefficient_of_restitution(size_t const & first_idx
                                        , size_t const & second_idx
                                        , float const & e
                                        );
    
    size_t create_collision_geometry( std::string const & name );

    void set_box_shape(  size_t const & geometry_idx
                       , float const & width
                       , float const & height
                       , float const & depth
                       );

    void set_capsule_shape(  size_t const & geometry_idx
                           , float const & radius
                           , float const & height
                           );

    void set_cone_shape(  size_t const & geometry_idx
                        , float const & radius
                        , float const & height
                        );

    void set_convex_shape(  size_t const & geometry_idx
                          , size_t const & N
                          , float const * coordinates
                          );

    void set_cylinder_shape(  size_t const & geometry_idx
                            , float const & radius
                            , float const & height
                            );

    void set_ellipsoid_shape(  size_t const & geometry_idx
                             , float const & sx
                             , float const & sy
                             , float const & sz
                             );

    void set_sphere_shape(  size_t const & geometry_idx
                          , float const & radius
                          );

    void set_tetramesh_shape(  size_t const & geometry_idx
                             , size_t const & N
                             , size_t const & K
                             , size_t const * vertices
                             , size_t const * tetrahedra
                             , float const * coordinates
                             );
    
    float get_collision_envelope();
    
    float get_time_step();
    
    float get_time();

    void get_gravity_up(
                        float & x
                        , float & y
                        , float & z
                        );

    void get_gravity_acceleration( float & acceleration );

    void get_damping_parameters( float & linear, float & angular );

    std::string get_material_name(size_t const & material_index);
    
    size_t get_number_of_materials();
    
    void get_material_indices( size_t * index_array );
    
    size_t get_number_of_contact_models();
    
    void get_contact_model_material_indices(  size_t * first_index_array
                                       , size_t * second_index_array
                                       );

    void get_coefficients_of_anisotropic_friction( size_t const & first_index
                                                  , size_t const & second_index
                                                  , float & mu_x
                                                  , float & mu_y
                                                  , float & mu_z
                                                  );

    void get_coefficient_of_isotropic_friction( size_t const & first_index
                                               , size_t const & second_index
                                               , float & mu
                                               );

    float get_coefficient_of_restitution( size_t const & first_index
                                         , size_t const & second_index
                                         );
    
    size_t get_number_of_geometries();
    
    void get_geometry_indices( size_t * index_array );
    
    std::string get_geometry_name(size_t const & geometry_index);
    

    void get_tetramesh_shape(  size_t const & geometry_idx
                             , size_t       & N
                             , size_t       & K
                             );
    
    void get_tetramesh_shape(  size_t const & geometry_idx
                             , size_t * vertices
                             , size_t * tetrahedra
                             , float  * coordinates
                             );

    size_t get_number_of_rigid_bodies();
    
    void get_rigid_body_indices( size_t * index_array );
    
    std::string get_rigid_body_name( size_t const & body_index );
    
    void get_rigid_body_position( size_t const & body_index
                                 , float & x
                                 , float & y
                                 , float & z
                                 );
    
    void get_rigid_body_orientation( size_t const & body_index
                                    , float & Qs
                                    , float & Qx
                                    , float & Qy
                                    , float & Qz
                                    );
    
    void get_rigid_body_velocity( size_t const & body_index, float & x, float & y, float & z );
    
    void get_rigid_body_spin( size_t const & body_index, float & x, float & y, float & z);
    
    void get_rigid_body_inertia( size_t const & body_index, float & xx, float & yy, float & zz );
    
    float get_rigid_body_mass( size_t const & body_index );
    
    bool get_rigid_body_active( size_t const & body_index );
    
    bool get_rigid_body_fixed( size_t const & body_index );
    
    size_t get_rigid_body_material( size_t const & body_index );
    
    
    size_t get_rigid_body_collision_geometry(size_t const & body_index);
    
    size_t get_number_of_connected_forces(size_t const & body_index);
    
    void get_connected_force_indices(  size_t const & body_index, size_t * index_array );
    
    void get_rigid_body_bounding_box(
                                     size_t const & body_index
                                     , float & min_x
                                     , float & min_y
                                     , float & min_z
                                     , float & max_x
                                     , float & max_y
                                     , float & max_z
                                     );


    size_t get_number_of_pin_forces() const;

    void get_pin_force_indices( size_t * index_array );

    void get_pin_tau(size_t const & force_idx, float & tau );

    void get_pin_target(size_t const & force_idx, float  & x, float  & y, float  & z );

    void get_pin_anchor(size_t const & force_idx, float  & x, float  & y, float  & z );

    size_t create_pin_force() const;

    void set_pin_tau(size_t const & force_idx, float const & tau );

    void set_pin_target(size_t const & force_idx, float const & x, float const & y, float const & z );

    void set_pin_anchor(size_t const & force_idx, float const & x, float const & y, float const & z );


    void set_scripted_key_position(
                                   size_t const & motion_index
                                   , float const & time
                                   , float const & x
                                   , float const & y
                                   , float const & z
                                   );

    void set_scripted_key_orientation(
                                      size_t const & motion_index
                                      , float const & time
                                      , float const & qs
                                      , float const & qx
                                      , float const & qy
                                      , float const & qz
                                      );

    void connect_scripted_motion( size_t const & body_idx, size_t const & motion_idx );

    bool has_scripted_motion( size_t const & body_index );

    size_t get_scripted_motion( size_t const & body_index );

    size_t create_key_frame_scripted_motion();

    size_t create_oscilation_scripted_motion();

    void set_scripted_oscilation_paramters(
                                           size_t const & motion_index
                                           , float const & amplitude
                                           , float const & frequency
                                           , float const & phase
                                           , float const & dir_x
                                           , float const & dir_y
                                           , float const & dir_z
                                           , float const & ref_x
                                           , float const & ref_y
                                           , float const & ref_z
                                           );
    
    size_t get_number_of_scripted_motions();

    void get_scripted_motion_indices( size_t * index_array );

    bool is_scripted_motion_keyframe( size_t const & motion_index );

    bool is_scripted_motion_oscilation( size_t const & motion_index );

    void get_scripted_oscilation_paramters(
                                           size_t const & motion_index
                                           , float & amplitude
                                           , float & frequency
                                           , float & phase
                                           , float & dir_x
                                           , float & dir_y
                                           , float & dir_z
                                           );

    size_t get_number_of_key_frame_positions( size_t const & motion_index);

    size_t get_number_of_key_frame_orientations( size_t const & motion_index);

    void get_key_frame_positions(
                                 size_t const & motion_index
                                 , float * time_array
                                 , float * x_array
                                 , float * y_array
                                 , float * z_array
                                 );

    void get_key_frame_orientations(
                                    size_t const & motion_index
                                    , float * time_array
                                    , float * qs_array
                                    , float * qx_array
                                    , float * qy_array
                                    , float * qz_array
                                    );

    bool compute_raycast(
                         float const & p_x
                         , float const & p_y
                         , float const & p_z
                         , float const & ray_x
                         , float const & ray_y
                         , float const & ray_z
                         , size_t & body_idx
                         , float & hit_x
                         , float & hit_y
                         , float & hit_z
                         , float & distance
                         );

  public:

    size_t get_number_of_contacts();
    void get_contact_position( size_t const & contact_number, float & x, float & y, float & z);
    void get_contact_normal( size_t const & contact_number, float & x, float & y, float & z);
    void get_contact_structure_direction( size_t const & contact_number, float & x, float & y, float & z);
    void get_contact_depth( size_t const & contact_number, float & depth);

  public:

    bool write_matlab_profiling_data(std::string const & filename);
    bool write_matlab_contact_data(std::string const & filename, unsigned int const & frame_number);

  };

}// namespace prox

// PROX_ENGINE_H
#endif
