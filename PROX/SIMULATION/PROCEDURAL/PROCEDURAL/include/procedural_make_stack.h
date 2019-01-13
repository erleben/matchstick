#ifndef PROCEDURAL_MAKE_STACK_H
#define PROCEDURAL_MAKE_STACK_H

#include <procedural.h>

#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{
  
  template<typename MT>
  inline void make_stack(
                         content::API * engine
                         , typename MT::vector3_type const& position
                         , typename MT::quaternion_type const & orientation
                         , typename MT::real_type const & stone_width
                         , size_t const & layers
                         , typename MT::real_type const & structure_field_x
                         , typename MT::real_type const & structure_field_y
                         , typename MT::real_type const & structure_field_z
                         , typename MT::real_type const & mu_iso
                         , typename MT::real_type const & mu_ani_x
                         , typename MT::real_type const & mu_ani_y
                         , typename MT::real_type const & mu_ani_z
                         , MaterialInfo<typename MT::real_type> mat_info
                         )
  {
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type Q;
    typedef typename MT::value_traits    VT;
    
    T const stone_density = get_material_density<MT>(mat_info, "Stone");
    size_t const mid      = get_material_id<MT>(mat_info, "Stone");
    
    engine->set_coefficient_of_isotropic_friction( mid
                                                  , mid
                                                  , mu_iso
                                                  );
    
    engine->set_coefficients_of_anisotropic_friction( mid
                                                     , mid
                                                     , mu_ani_x
                                                     , mu_ani_y
                                                     , mu_ani_z
                                                     );
    
    GeometryHandle<MT> stone_handle = create_geometry_handle_box<MT>( engine
                                                                     , stone_width
                                                                     , stone_width
                                                                     , stone_width
                                                                     );
    
    for( size_t i = 0u; i < layers;++i )
    {
      T y = ( i+VT::half() )*stone_width;
      
      V const T_b2m = stone_handle.Tb2m();
      Q const Q_b2m = stone_handle.Qb2m();
      
      V const T_m2l = V::make( VT::zero(), y, VT::zero() );
      Q const Q_m2l = Q::identity();
      
      V const T_l2w = position;
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
                                         , stone_handle
                                         , mid
                                         , stone_density
                                         );
      
      engine->set_material_structure_map(
                                         rid
                                         , structure_field_x
                                         , structure_field_y
                                         , structure_field_z
                                         );
      
    }
    
  }
  
} //namespace procedural

// PROCEDURAL_MAKE_STACK_H
#endif
