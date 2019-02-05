#ifndef PROCEDURAL_MAKE_CYLINDER_ON_INCLINED_PLANE_H
#define PROCEDURAL_MAKE_CYLINDER_ON_INCLINED_PLANE_H

#include <procedural_factory.h>

#include <tiny_math_types.h>

#include <cmath> // needed for std::sin

namespace procedural
{

  /**
   *
   * @param  angle                   The inclination angle of the plane given in degrees.
   * @param  length                  The length of the plane slide part of the plane.
   * @param  structure_field_plane   The constant structure field direction of
   *                                 the plane given in the body frame coordinate system.
   * @param  structure_field_cylinder     The constant structure field direction of
   *                                 the cylinder given in the body frame coordinate system.
   */
  template<typename MT>
  inline void make_cylinder_on_inclined_plane(
                                  content::API * engine
                                  , typename MT::vector3_type const & position
                                  , typename MT::quaternion_type const & orientation
                                  , typename MT::real_type const & angle
                                  , typename MT::real_type const & length
                                  , typename MT::vector3_type const & structure_field_plane
                                  , typename MT::vector3_type const & structure_field_cylinder
                                  , MaterialInfo<typename MT::real_type> mat_info
                                  )
  {
    using std::sin;

    typedef typename MT::real_type        T;
    typedef typename MT::vector3_type     V;
    typedef typename MT::quaternion_type  Q;
    typedef typename MT::value_traits    VT;

    size_t const mid           = get_material_id<MT>(mat_info, "Stone");
    size_t const stone_density = get_material_density<MT>(mat_info, "Stone");

    T const alpha       = VT::convert_to_radians(angle);   // Plane inclination angle in radians
    T const beta        = VT::pi_half() - alpha;

    assert(alpha >= VT::zero()    || !"make_cylinder_on_inclined_plane(): inclination angle must be 0 <= alpha <= 90 degrees");
    assert(alpha <= VT::pi_half() || !"make_cylinder_on_inclined_plane(): inclination angle must be 0 <= alpha <= 90 degrees");

    T const L =  length/VT::numeric_cast(16.0);    // Cylinder height
    T const R =  length/VT::numeric_cast(32.0);    // Cylinder radius
    T const W =  length;                           // Plane width  (x)
    T const H =  length/VT::numeric_cast(32.0);    // Plane height (y)
    T const D =  length/VT::numeric_cast(8.0);     // Plane depth  (z)

    V const plane_extents   = V::make(
                                      W
                                      , H
                                      , D
                                      );

    V const place_on_ground_plane =  V::make(
                                             VT::zero()
                                             , VT::half()*(W*sin(alpha) - H*sin(beta))
                                             , VT::zero()
                                             );

    GeometryHandle<MT> plane_handle   = create_geometry_handle_box<MT>(  engine
                                                                       , plane_extents(0)
                                                                       , plane_extents(1)
                                                                       , plane_extents(2)
                                                                       );

    GeometryHandle<MT> cylinder_handle     = create_geometry_handle_cylinder<MT>(  engine
                                                                       , R
                                                                       , L
                                                                       );

    {
      V const T_b2m = plane_handle.Tb2m();
      Q const Q_b2m = plane_handle.Qb2m();

      V const T_m2l = V::make(
                              VT::zero()
                              , VT::zero()
                              , VT::zero()
                              );
      Q const Q_m2l = Q::Rz(alpha);

      V const T_l2w = position + place_on_ground_plane;
      Q const Q_l2w = orientation;

      V T_b2w;
      Q Q_b2w;

      compute_body_to_world_transform<MT>(
                                          T_b2m
                                          , Q_b2m
                                          , T_m2l
                                          , Q_m2l
                                          , T_l2w
                                          , Q_l2w
                                          , T_b2w
                                          , Q_b2w
                                          );

      size_t rid = create_rigid_body<MT>(  engine
                                         , T_b2w
                                         , Q_b2w
                                         , plane_handle
                                         , mid
                                         , stone_density
                                         , true
                                         );

      engine->set_material_structure_map(
                                         rid
                                         , structure_field_plane(0)
                                         , structure_field_plane(1)
                                         , structure_field_plane(2)
                                         );
    }

    {
      V const T_b2m = cylinder_handle.Tb2m();
      Q const Q_b2m = cylinder_handle.Qb2m();

      V const T_m2l = tiny::rotate( Q::Rz(alpha) , V::make(
                                                           W*VT::half() - L
                                                           , R + VT::half()*H
                                                           , VT::zero()
                                                           ) );

      Q const Q_m2l = tiny::prod(Q::Rz(alpha), Q::Rx(  VT::pi_half() ) );

      V const T_l2w = position + place_on_ground_plane;
      Q const Q_l2w = orientation;

      V T_b2w;
      Q Q_b2w;

      compute_body_to_world_transform<MT>(
                                          T_b2m
                                          , Q_b2m
                                          , T_m2l
                                          , Q_m2l
                                          , T_l2w
                                          , Q_l2w
                                          , T_b2w
                                          , Q_b2w
                                          );

      size_t rid = create_rigid_body<MT>(  engine
                                         , T_b2w
                                         , Q_b2w
                                         , cylinder_handle
                                         , mid
                                         , stone_density
                                         , false
                                         );

      engine->set_material_structure_map(
                                         rid
                                         , VT::zero(), VT::one(), VT::zero()  // Rotation axis
                                         , VT::one(), VT::zero(), VT::zero()  // Reference point
                                         , structure_field_cylinder(0)
                                         , structure_field_cylinder(1)
                                         , structure_field_cylinder(2)
                                         );
    }

  }

}
//namespace procedural

// PROCEDURAL_MAKE_CYLINDER_ON_INCLINED_PLANE_H
#endif
