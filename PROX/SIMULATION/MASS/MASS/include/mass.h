#ifndef MASS_H
#define MASS_H

#include <string>
#include <iosfwd>  // needed for std::ostream forward declaration

namespace mass
{
  
  /**
   * A Wrapper class for values of mass properties.
   * This is a convenience class that makes it easier to pass parameters to
   * various functions in this library or obtain results from these.
   *
   * One should be carefull whether mass properties live in the body space
   * frame or in some model frame. If the properties live in body space then
   * the center of mass values are always zero, the quaternion will be the
   * identity rotation and all products of inertia will be zero. Only the
   * mass and moments of inertia will be positive.
   */
  template<typename T>
  class Properties
  {
  public:
    
    T m_m;     ///< Total mass 
    T m_Ixx;   ///< The xx moment of inertia
    T m_Iyy;   ///< The yy moment of inertia
    T m_Izz;   ///< The zz moment of inertia
    T m_Ixy;   ///< The xy product of inertia
    T m_Ixz;   ///< The xz product of inertia
    T m_Iyz;   ///< The yz product of inertia
    T m_x;     ///< The x coordinate of the center of mass position 
    T m_y;     ///< The y coordinate of the center of mass position
    T m_z;     ///< The z coordinate of the center of mass position
    T m_Qs;    ///< The scalar part of the unit quaternion corresponding to rotation from body frame to model frame.
    T m_Qx;    ///< The i imaginary part of the unit quaternion corresponding to rotation from body frame to model frame.
    T m_Qy;    ///< The j imaginary part of the unit quaternion corresponding to rotation from body frame to model frame.
    T m_Qz;    ///< The k imaginary part of the unit quaternion corresponding to rotation from body frame to model frame.
    
  public:
    
    Properties();
    virtual ~Properties();

    Properties( Properties const & cpy );
    Properties & operator=( Properties const & lhs );

    bool operator==( Properties const & lhs ) const ;

  public:
    
    /**
     * This method tests if the properties represent mass properties in a body
     * space. This implies a diagonal intertia tensor and that the center of mass
     * is at zero and that the orientation is the identity rotation.
     *
     * This is a rather strict test and it verfies the entire mass property values
     * not just whether the inertia tensor values are in body frame or model frame.
     */
    bool is_body_space() const;
    bool is_model_space() const;
    
    void clear();
        
  };
  
  /**
   * Output stream operator.
   */
  template<typename T>
  std::ostream & operator<< (std::ostream & o, Properties<T> const & P);

  /**
   * Various known materials. 
   */
  typedef enum
    {
      AIR
      , ALUMINUM
      , BALSA_WOOD
      , BRICK
      , COPPER
      , CORK
      , DIAMOND
      , EARTH
      , EARTH_CORE
      , EARTH_CRUST
      , GLASS
      , GOLD
      , HELIUM
      , HYDROGEN
      , ICE
      , IRON
      , LEAD
      , MERCURY
      , NICKEL
      , OLIVE_OIL
      , OXYGEN
      , PLATINUM
      , SILVER
      , STYROFOAM
      , TUNGSTEN
      , URANIUM
      , WATER
      , SEAWATER
    } material_type;
  
  /**
   * Get SI units for the material densities known by this library.  
   */
  inline std::string get_SI_units() {return "kg/m^3";}
  
  /**
   * Retrive the density value of a specified known material.
   *
   * @param material      A material constant specifying the material. 
   * @return              The density value.
   */
  template<typename T>
  inline T get_density( material_type const & material );
  
  template<>
  inline double get_density<double>( material_type const & material )
  {
    switch(material)
    {
      case AIR:         return     1.21;
      case ALUMINUM:    return  2700.0;
      case BALSA_WOOD:  return   120.0;
      case BRICK:       return  2000.0;
      case COPPER:      return  8900.0;
      case CORK:        return   250.0;
      case DIAMOND:     return  3300.0;
      case EARTH:       return  5500.0;
      case EARTH_CORE:  return  9500.0;
      case EARTH_CRUST: return  2800.0;
      case GLASS:       return  2500.0;
      case GOLD:        return 19300.0;
      case HELIUM:      return     0.178;
      case HYDROGEN:    return     0.090;
      case ICE:         return   917.0;
      case IRON:        return  7900.0;
      case LEAD:        return 11300.0;
      case MERCURY:     return 13600.0;
      case NICKEL:      return  8800.0;
      case OLIVE_OIL:   return   920.0;
      case OXYGEN:      return     1.43;
      case PLATINUM:    return 21500.0;
      case SILVER:      return 10500.0;
      case STYROFOAM:   return   100.0;
      case TUNGSTEN:    return 19300.0;
      case URANIUM:     return 18700.0;
      case WATER:       return   998.0;
      case SEAWATER:    return   1024.0;
    };
    return   0.0;
  }
  
  /**
   * Compute body frame of box shape.
   *
   * @param density       The density value for the box shape.
   * @param half_width    The half width (the extent along the local x-axis) of the box.
   * @param half_height   The half height (the extent along the local y-axis) of the box.
   * @param half_depth    The half depth (the extent along the local z-axis) of the box.
   */
  template<typename T>
  Properties<T> compute_box(
                   T const & density
                   , T const & half_width
                   , T const & half_height
                   , T const & half_depth
                   );
  
  /**
   * Compute body frame of capsule shape.
   *
   * @param density       The density value for the shape.
   * @param radius        The radius of the capsule.
   * @param half_height   Half height of the cylindrical part (y-axis) of the capsule.
   */
  template<typename T>
  Properties<T> compute_capsule(
                       T const & density
                       , T const & radius
                       , T const & half_height
                       );
    
  /**
   * Compute model frame of cone.
   *
   * @param density       The density value for the shape.
   * @param base_radius   The radius of the base of the cone.
   * @param height        The height of the cone along the y-axis.
   */
  template<typename T>
  Properties<T> compute_cone(
                    T const & density
                    , T const & base_radius
                    , T const & height
                    );
  
  /**
   * Compute body frame of cylinder shape.
   * The cylinder is assumed to have its center of mass at
   * the origin and the cylinder axis along the y-axis.
   *
   * @param density       The density value for the shape.
   * @param radius        The radius of the cylinder.
   * @param half_height   The half height of the cylinder along the y-axis.
   */
  template<typename T>
  Properties<T> compute_cylinder(
                        T const & density
                        , T const & radius
                        , T const & half_height
                        );
  
 /**
   * Compute model frame of conical solid.
   * The conical is assumed to have its center at the base. The conical axis is along the y-axis.
   *
   * @param density       The density value for the shape.
   * @param bottom_radius The bottom radius of the base.
   * @param top_radius    The top radius of the top.
   * @param height        The height of the frustum along the y-axis.
   * @param mass          Upon return this argument holds the value of total mass.
   */
  template<typename T>
  Properties<T> compute_conical_solid(
                        T const & density
                        , T const & bottom_radius
                        , T const & top_radius
                        , T const & height
                        );
  
  /**
   * Compute body frame of ellipsoid.
   * The ellipsoid is defined as
   *
   *   1 = x^2/a^2 + y^2/b^2  + z^2/c^2  
   *
   * where a,b,c are the lengths of the semiaxes of the ellipsoid. Put differently if
   * the ellipsoid is obtained by scaling a unit-sphere then the scaling along
   * the x-axis would be a, along y would be b, and z-axis would be scaled by c.
   *
   * @param density       The density value for the shape.
   * @param a             The scaling/length of the first major axis (x-axis) of  the ellipsoid.
   * @param b             The scaling/length of the second major axis (y-axis) of  the ellipsoid.
   * @param c             The scaling/length of the third major axis (z-axis) of  the ellipsoid.   
   */
  template<typename T>
  Properties<T> compute_ellipsoid(
                         T const & density
                         , T const & a
                         , T const & b
                         , T const & c
                         );
    
  /**
   * Face Callback Interace.
   * This callback Interface used to retrieve vertex coordinate information about a specified face.
   * Faces are assumed to be numbered in some consecutive manner from 1 to K, where K is the
   * total number of faces in the mesh. These face numbers are purely a logical entity used
   * by our mass library. One may have to create ones own book keeping/conversion to match
   * ones own mesh data structures whatever they might be.
   */
  template<typename T>
  class FaceCallbackInterface
    {
    public:
      
      virtual ~FaceCallbackInterface(){}
      
    public:
      
      /**
       * Retrieves the total number of vertices of the specified face.
       *
       * @return The number of vertices of the face.
       */
      virtual size_t get_number_of_vertices(size_t const & face_no) const = 0;
      
      /**
       * Retrieves a pointer to the x-coordinates of the face. The pointer is expected
       * to point to an array (callee is responsible for this storage) holding at
       * least 'get_number_of_vertices' entries. Each entry holds an x coordinate of
       * a vertex. Further it is assumed that the order of vertex information is given
       * in counter clock wise order.
       *
       * @param face_no    The face number.
       * @param coords     A pointer to an array of coordinates holding the requested
       *                   information. 
       */
      virtual void get_x_coords(size_t const & face_no, T * coords) const = 0;
      virtual void get_y_coords(size_t const & face_no, T * coords) const = 0;
      virtual void get_z_coords(size_t const & face_no, T * coords) const = 0;
      
    };
  
  /**
   * Compute the model frame of arbitary closed mesh.
   *
   * @param density    The density value for the shape.
   * @param K          The total number of faces in the mesh.
   * @param callback   A pointer to a callback interface which is
   *                   used to query geometry information about
   *                   faces of the mesh.
   */
  template<typename T>
  Properties<T> compute_mesh(
                     T const & density
                     , size_t const K
                     , FaceCallbackInterface<T> const * callback
                     );
    
  /**
   * Rotate Inertia Tensor.
   * This overloaded function also rotates the inertia tensor, but rather than
   * using a rotation matrix this routine uses a quaternion to specify the rotation.
   */
  template<typename T>
  Properties<T> rotate(
                        T const & qs
                      , T const & qx
                      , T const & qy
                      , T const & qz
                      , Properties<T> const & I         
                      );
  
  /**
   * Find orientation of body frame.
   * Finds the orientation of body frame such that I = Q J Q^T where J is
   * a diagonal intertia tensor. This is usefull for finding an orientation
   * of a body frame relative to some model frame. That is the rotation Q
   * from body frame to model frame.
   */
  template<typename T>
  Properties<T> rotate_to_body_frame( Properties<T> const & Imodel );
  
  
  /**
   * Compute body frame of sphere.
   *
   * @param density    The density value for the shape.
   * @param radius     The radius of the sphere.
   */
  template<typename T>
  Properties<T> compute_sphere(T const & density, T const & radius);
  
  /**
   * Compute model frame inertia tensor of a tetrahedron.
   *
   * @param density    The density value for the tetrahedron.
   * @param x1         The x coordinate of first vertex.
   * @param y1         The y coordinate of first vertex.
   * @param z1         The z coordinate of first vertex.
   * @param x2         The x coordinate of second vertex.
   * @param y2         The y coordinate of second vertex.
   * @param z2         The z coordinate of second vertex.
   * @param x3         The x coordinate of third vertex.
   * @param y3         The y coordinate of third vertex.
   * @param z3         The z coordinate of third vertex.
   * @param x4         The x coordinate of fourth vertex.
   * @param y4         The y coordinate of fourth vertex.
   * @param z4         The z coordinate of fourth vertex.
   */
  template<typename T>
  Properties<T>  compute_tetrahedron(
                           T const & density
                           , T const & x1, T const & y1, T const & z1
                           , T const & x2, T const & y2, T const & z2
                           , T const & x3, T const & y3, T const & z3
                           , T const & x4, T const & y4, T const & z4
                           );
    
  /**
   * Translate inertia tensor from body frame.
   *
   * @param dx    The amount of translation a long the x-axis.
   * @param dy    The amount of translation a long the y-axis.
   * @param dz    The amount of translation a long the z-axis.
   */
  template<typename T>
  Properties<T> translate_to_model_frame(
                           T const & dx
                         , T const & dy
                         , T const & dz
                         , Properties<T> const & Ibody
                         );
  
  /**
   * Translate inertia tensor to body frame center.
   */
  template<typename T>
  Properties<T>  translate_to_body_frame( Properties<T> const & Imodel );
    
} // namespace mass

//MASS_H
#endif
