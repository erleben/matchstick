#ifndef CONTENT_API_INPUT_H
#define CONTENT_API_INPUT_H

#include <string>

namespace content
{
  
  class Input
  {
  public:
    
    Input(){}
    virtual ~Input(){}
  public:
    
    // Creators
    
  public:
    
    /**
     * Create a new Rigid body.
     *
     * @param name          A human readable name of the new rigid body.
     * @return              An unique index value identifying the newly created rigid body.
     */
    virtual size_t create_rigid_body( std::string const & name ) = 0;

    /**
     * Connect collision geometry to rigid body.
     *
     * @param body_idx        The unique index value representing the rigid body.
     * @param geometry_idx    The unique index value representing the collision geometry.
     */
    virtual void connect_collision_geometry(size_t body_idx, size_t geometry_idx ) = 0;

    /**
     * Create a new material.
     *
     * @param name    A human readable name for the new material.
     * @return        An unique index value identifying the newly created material.
     */
    virtual size_t create_material( std::string const & name ) = 0;
    
    /**
     * Create contact model between a pair of materials.
     *
     * @param first_idx    The index value of the first material
     * @param second_idx   The index value of the second material
     */
    virtual void create_contact_model( size_t const & first_idx, size_t const & second_idx) = 0;
    
    /**
     * Create Collision Geometry.
     *
     * @param name   A human readable name for the new geometry.
     * @return       An unique index value representing the newly created collision geometry.
     */
    virtual size_t create_collision_geometry( std::string const & name ) = 0;

  public:
    
    // Destroyers
    
    /**
     * Clear all information whatsoever.
     */
    virtual void clear() = 0;
    
  public:

    // Setters
    
    /**
     * Set Parameter Value.
     *
     * @param name    The name of the parameter to set.
     * @param value   The value.
     */
    virtual void set_parameter(std::string const & name, bool        const & value ) = 0;
    virtual void set_parameter(std::string const & name, unsigned    const & value ) = 0;
    virtual void set_parameter(std::string const & name, float       const & value ) = 0;
    virtual void set_parameter(std::string const & name, std::string const & value ) = 0;

    /**
     * Set Rigid Body Position.
     *
     * @param body_idx      An unique index representing the rigid body,
     * @param x             The x-position of the center of mass.
     * @param y             The y-position of the center of mass.
     * @param z             The z-position of the center of mass.
     */
    virtual void set_rigid_body_position( size_t const & body_idx
                                         , float const & x
                                         , float const & y
                                         , float const & z
                                         ) = 0;
    
    /**
     * Set Rigid Body Orientation.
     *
     * @param body_idx      An unique index representing the rigid body,
     * @param qs            The real-part of a quaternion representation of the orientation.
     * @param qx            The x-component of the imaginary part of a quaternion representation of the orientation.
     * @param qy            The y-component of the imaginary part of a quaternion representation of the orientation.
     * @param qz            The z-component of the imaginary part of a quaternion representation of the orientation.
     */
    virtual void set_rigid_body_orientation( size_t const & body_idx
                                            , float const & qs
                                            , float const & qx
                                            , float const & qy
                                            , float const & qz
                                            ) = 0;
    
    /**
     * Set Rigid Body Velocity.
     *
     * @param body_idx      An unique index representing the rigid body,
     * @param vx            The x-component of the linear velocity of the center of mass.
     * @param vy            The y-component of the linear velocity of the center of mass.
     * @param vz            The z-component of the linear velocity of the center of mass.
     */
    virtual void set_rigid_body_velocity( size_t const & body_idx
                                         , float const & vx
                                         , float const & vy
                                         , float const & vz
                                         ) = 0;
    
    /**
     * Set Rigid Body Spin.
     *
     * @param body_idx      An unique index representing the rigid body,
     * @param wx            The x-component of the angular spin.
     * @param wy            The y-component of the angular spin.
     * @param wz            The z-component of the angular spin.
     */
    virtual void set_rigid_body_spin( size_t const & body_idx
                                     , float const & wx
                                     , float const & wy
                                     , float const & wz
                                     ) = 0;
    
    /**
     * Set Total Mass of Rigid Body.
     *
     * @param body_idx      An unique index representing the rigid body,
     * @param mass          The total mass of the rigid body.
     */
    virtual void set_rigid_body_mass( size_t const & body_idx, float const & mass) = 0;
    
    /**
     * Set Rigid Body Inertia (in body-frame space).
     *
     * @param body_idx      An unique index representing the rigid body,
     * @param Ixx           The principal moment of the inertia tensor given in the body-frame of the rigid body,
     * @param Iyy           The principal moment of the inertia tensor given in the body-frame of the rigid body,
     * @param Izz           The principal moment of the inertia tensor given in the body-frame of the rigid body,
     */
    virtual void set_rigid_body_inertia( size_t const & body_idx, float const & Ixx, float const & Iyy, float const & Izz) = 0;
    
    /**
     * Set Active Flag of Rigid Body.
     * This is usefull for preallocating rigid bodies that are supposed to be added to the configuration during simulation.
     *
     * @param body_idx      An unique index representing the rigid body,
     * @param active        A boolean flag indicating if the rigid body should be active.
     */
    virtual void set_rigid_body_active( size_t const & body_idx, bool const & active ) = 0;
    
    /**
     * Set Fixed Rigid Body,
     * A fixed rigid body is immovable, like the ground.
     *
     * @param body_idx      An unique index representing the rigid body,
     * @param fixed         A boolean flag indicating if the rigid body should be fixed.
     */
    virtual void set_rigid_body_fixed( size_t const & body_idx, bool const & fixed ) = 0;
    
    /**
     * Set Material of Rigid Body.
     *
     * @param body_idx      An unique index representing the rigid body,
     * @param material_idx  An unique index for the material of the rigid body.
     */
    virtual void set_rigid_body_material( size_t const & body_idx, size_t const & material_idx) = 0;
    
    /**
     * Set Gravity Up Direction Parameters.
     */
    virtual void set_gravity_up(
                                float const & x
                                , float const & y
                                , float const & z
                                ) = 0;

    /**
     * Set Gravity Acceleration.
     */
    virtual void set_gravity_acceleration( float const & acceleration ) = 0;

    /**
     * Set Linear and Angular Damping Parameters.
     */
    virtual void set_damping_parameters( float const & linear, float const & angular ) = 0;

    /**
     * Set Friction Coefficients.
     *
     * @param first_idx    The index value of the first material
     * @param second_idx   The index value of the second material
     * @param mu_x         Friction coefficient along x-component of contact plane.
     * @param mu_y         Friction coefficient along y-component of contact plane.
     * @param mu_z         Friction coefficient along z-component of contact plane.
     */
    virtual void set_coefficients_of_anisotropic_friction(  size_t const & first_idx
                                                          , size_t const & second_idx
                                                          , float const & mu_x
                                                          , float const & mu_y
                                                          , float const & mu_z
                                                          ) = 0;

    /**
     * Set Friction Coefficients.
     *
     * @param first_idx    The index value of the first material
     * @param second_idx   The index value of the second material
     * @param mu            Friction coefficient along all components of contact plane.
     */
    virtual void set_coefficient_of_isotropic_friction(  size_t const & first_idx
                                                       , size_t const & second_idx
                                                       , float const & mu
                                                       ) = 0;

    /**
     * Set Material Structure Map.
     *
     * @param body_idx    The index value of the body whos material direction was be set.
     * @param dir_x        The x-ccodinate of the materials characteristic direction.
     * @param dir_y        The y-ccodinate of the materials characteristic direction.
     * @param dir_z        The z-ccodinate of the materials characteristic direction.
     */
    virtual void set_material_structure_map(
                                    size_t const & body_idx
                                    , float const & dir_x
                                    , float const & dir_y
                                    , float const & dir_z
                                    ) = 0;

    /**
     * Set Material Structure Map.
     * This version sweeps a specified structure direction around a given
     * axis. The sweep starts from a given reference direction (the zero
     * angle position)
     *
     * @param body_idx    The index value of the body whos material direction was be set.
     * @param rot_x        The x-ccodinate of the rotation axis.
     * @param rot_y        The y-ccodinate of the rotation axis.
     * @param rot_z        The z-ccodinate of the rotation axis.
     * @param ref_x        The x-ccodinate of the reference direction.
     * @param ref_y        The y-ccodinate of the reference direction.
     * @param ref_z        The z-ccodinate of the reference direction.
     * @param dir_x        The x-ccodinate of the materials characteristic direction.
     * @param dir_y        The y-ccodinate of the materials characteristic direction.
     * @param dir_z        The z-ccodinate of the materials characteristic direction.
     */
    virtual void set_material_structure_map(
                                    size_t const & body_idx
                                    , float const & rot_x
                                    , float const & rot_y
                                    , float const & rot_z
                                    , float const & ref_x
                                    , float const & ref_y
                                    , float const & ref_z
                                    , float const & dir_x
                                    , float const & dir_y
                                    , float const & dir_z
                                    ) = 0;

    /**
     * Set Material Structure Map.
     * This version supports a more raw data access.
     *
     * @param  N        The number of vertices to specify the structure directions for.
     * @param indices   An array that holds the indices of the vertices.
     * @param data      An array that holds the coordinates of all the structure directions.
     */
    virtual void set_material_structure_map(size_t const & body_idx, size_t const & N, size_t * indices, float *  data) = 0;

    /**
     * Set Restitution Coefficient.
     *
     * @param first_idx    The index value of the first material
     * @param second_idx   The index value of the second material
     * @param e            The normal restitution coefficient.
     */
    virtual void set_coefficient_of_restitution(
                                                size_t const & first_idx
                                                , size_t const & second_idx
                                                , float const & e
                                                ) = 0;
    
    
    /**
     * Set Box Shape Parameters.
     *
     * @param geometry_idx    The index representing the geometry.
     * @param width           The width of the box (x-extent).
     * @param height          The height of the box (y-extent).
     * @param depth           The depth of the box (z-extent)
     */
    virtual void set_box_shape(  size_t const & geometry_idx
                               , float const & width
                               , float const & height
                               , float const & depth
                               ) = 0;

    /**
     * Set Capsule Shape Parameters.
     *
     * @param geometry_idx       The index representing the geometry.
     * @param height          The height of the capsule.
     * @pram  radius          The radius of the base of the capsule.
     */
    virtual void set_capsule_shape(  size_t const & geometry_idx
                                   , float const & radius
                                   , float const & height
                                   ) = 0;

    /**
     * Set Cone Shape Parameters.
     *
     * @param geometry_idx       The index representing the geometry.
     * @param height          The height of the cone.
     * @pram  radius          The radius of the base of the cone.
     */
    virtual void set_cone_shape(  size_t const & geometry_idx
                                , float const & radius
                                , float const & height
                                ) = 0;
    

    /**
     * Set Convex Shape Parameters.
     *
     * @param geometry_idx       The index representing the geometry.
     * @param convex_number      The convex number within the specified geometry.
     * @param N               The number of corner points in the convex shape.
     * @param coordinates     Array of coordinates in order x1 y1 z1... xN yN zN.
     */
    virtual void set_convex_shape(  size_t const & geometry_idx
                                  , size_t const & N
                                  , float const * coordinates
                                  ) = 0;
    

    /**
     * Set cylinder Shape Parameters.
     *
     * @param geometry_idx       The index representing the geometry.
     * @param height          The height of the cylinder.
     * @param radius          The radius of the cylinder.
     */
    virtual void set_cylinder_shape(  size_t const & geometry_idx
                                    , float const & radius
                                    , float const & height
                                    ) = 0;
    
    /**
     * Set Ellipsoid Shape Parameters.
     *
     * @param geometry_idx       The index representing the geometry.
     * @param sx                 The scaling of a unit-ball along the x-axis.
     * @param sy                 The scaling of a unit-ball along the y-axis.
     * @param sz                 The scaling of a unit-ball along the z-axis.
     */
    virtual void set_ellipsoid_shape(  size_t const & geometry_idx
                                     , float const & sx
                                     , float const & sy
                                     , float const & sz
                                     ) = 0;

    /**
     * Set Sphere Shape Parameters.
     *
     * @param geometry_idx    The index representing the geometry.
     * @param radius          The radius of the sphere.
     */
    virtual void set_sphere_shape(  size_t const & geometry_idx
                                  , float const & radius
                                  ) = 0;
    

    /**
     * Set Tetramesh Shape Parameters.
     * Observe that a shape can only have one tetramesh and tetrameshes live in body space by definition
     * (hence no body space transformations).
     *
     * @param geometry_idx    The index representing the geometry.
     * @param N               The number of vertices in the tetrahedra mesh.
     * @param K               The number of tetrahedra in the tetrahedra mesh.
     * @param vertices        An array of vertex indices (must be N long).
     * @param tetrahedra      An array of tetrahedra indices (must be 4*K long). Each consequtive four
     *                        tuple defines the four vertex indices of a single tetrahedron
     * @param coordinates     Array of coordinates in order x1 y1 z1... xN yN zN (must be 3 N long).
     */
    virtual void set_tetramesh_shape(  size_t const & geometry_idx
                                     , size_t const & N
                                     , size_t const & K
                                     , size_t const * vertices
                                     , size_t const * tetrahedra
                                     , float const * coordinates
                                     ) = 0;

    /**
     * Connect external forces to a rigid body.
     *
     * @param body_idx   The index value identifying the rigid body.
     * @param force_idx  The index value identifying the exernal force type.
     */
    virtual void connect_force( size_t body_idx, size_t force_idx ) = 0;

    /**
     * Creates a new pin force instance.
     * A pin force creates a critical damped spring force that drives an
     * anchor point given in body space to a world coordinate system
     * target position.
     *
     * @return   A unique force index identifying the newly created pin force.
     */
    virtual size_t create_pin_force() const = 0;

    /**
     * Set characteristic time for pin force.
     *
     * @param tau   The value of the characteristic time.
     */
    virtual void set_pin_tau(size_t const & force_idx, float const & tau ) = 0;

    /**
     * Set target position for pin force.
     *
     * @param x  The x-coordinate.
     * @param y  The y-coordinate.
     * @param z  The z-coordinate.
     */
    virtual void set_pin_target(size_t const & force_idx, float const & x, float const & y, float const & z ) = 0;

    /**
     * Set anchor position for pin force.
     *
     * @param x  The x-coordinate.
     * @param y  The y-coordinate.
     * @param z  The z-coordinate.
     */
    virtual void set_pin_anchor(size_t const & force_idx, float const & x, float const & y, float const & z ) = 0;


    /**
     * Connected a rigid body with a scripted motion.
     * The given body must be a free moving rigid body by making the
     * connection the rigid body automatically becomes a scripted body.
     * That is a rigid body whos motion are completely given by some closed
     * form solution (like keyframe interpolation or a harmonic oscillator).
     *
     * @param body_idx    The index of the body (must exist)
     * @param motion_idx  The index of the scripted motion (must have been created prior to this invocation)
     */
    virtual void connect_scripted_motion( size_t const & body_idx, size_t const & motion_idx ) = 0;

    /**
     * Create a new key frame scripted motion.
     *
     * @return    The unique scripted motion index representing this motion.
     */
    virtual size_t create_key_frame_scripted_motion() = 0;

    /**
     * Set scripted body position at given time
     */
    virtual void set_scripted_key_position(
                                           size_t const & motion_index
                                           , float const & time
                                           , float const & x
                                           , float const & y
                                           , float const & z
                                           ) = 0;

    /**
     * Set scripted body orientation at given time
     */
    virtual void set_scripted_key_orientation(
                                              size_t const & motion_index
                                              , float const & time
                                              , float const & qs
                                              , float const & qx
                                              , float const & qy
                                              , float const & qz
                                              ) = 0;


    /**
     * Create a new harmonic oscilator scripted motion.
     *
     * @return    The unique scripted motion index representing this motion.
     */
    virtual size_t create_oscilation_scripted_motion() = 0;

    /**
     * Set motion parameters of harmonic oscilator scripted motion.
     *
     * @param motion_index    The unique motion index corresponding to the motion (must be of type oscilation and not say key frame)
     * @param amplitude       The amplitude of the oscilation
     * @param frequency       The frequency of the oscilation
     * @param phase           The phase of the oscilation
     * @param dir_x           The x-component of the direction vector giving the direction of oscilation.
     * @param dir_y           The y-component of the direction vector giving the direction of oscilation.
     * @param dir_z           The z-component of the direction vector giving the direction of oscilation.
     * @param ref_x           The x-component of the origin reference point about which the motion oscilates.
     * @param ref_y           The y-component of the origin reference point about which the motion oscilates.
     * @param ref_z           The z-component of the origin reference point about which the motion oscilates.
     *
     */
    virtual void set_scripted_oscilation_paramters(
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
                                                   ) = 0;

  };
  
}// namespace content

// CONTENT_API_INPUT_H
#endif
