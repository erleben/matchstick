#ifndef PROCEDURAL_MAKE_DROPPING_CAPSULES_H
#define PROCEDURAL_MAKE_DROPPING_CAPSULES_H

#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{
  
  template<typename MT>
  inline void make_dropping_capsules(
                                     content::API *  engine
                                     , typename MT::vector3_type const & position
                                     , typename MT::quaternion_type const & orientation
                                     , typename MT::real_type const & capsule_radius
                                     , typename MT::real_type const & capsule_length
                                     , typename MT::real_type const & spacing
                                     , typename MT::real_type const & structure_field_x
                                     , typename MT::real_type const & structure_field_y
                                     , typename MT::real_type const & structure_field_z
                                     , size_t const & number_in_x
                                     , size_t const & number_in_y
                                     , size_t const & number_in_z
                                     , MaterialInfo<typename MT::real_type> mat_info
                                     )
  {
    typedef typename MT::real_type          T;
    typedef typename MT::vector3_type       V;
    typedef typename MT::quaternion_type    Q;
    typedef typename MT::value_traits       VT;
    
    T      const stone_density	= get_material_density<MT>(mat_info, "Stone");
    size_t const mid		        = get_material_id<MT>(mat_info, "Stone");
    
    GeometryHandle<MT> geometry_handle  = create_geometry_handle_capsule<MT>(  engine
                                                                             , capsule_radius
                                                                             , capsule_length
                                                                             );
    
    T const H = capsule_length;
    T const R = capsule_radius;
    T const S = spacing;

    size_t const I = number_in_x;
    size_t const J = number_in_y;
    size_t const K = number_in_z;

    T const offset_x = (( I*VT::two()*R + (I-1)*S)/VT::two()) - R;
    T const offset_z = (( K*VT::two()*R + (K-1)*S)/VT::two()) - R;
    
    for (size_t i = 0; i < I; ++i)
    {
      for (size_t j = 0; j < J; ++j)
      {
        for (size_t k = 0; k < K; ++k)
        {
          T const x = i*VT::two()*R + i*S - offset_x;
          T const y = j*(H + S + VT::two()*R);
          T const z = k*VT::two()*R + k*S - offset_z;
          
          V const T_b2m = geometry_handle.Tb2m();
          Q const Q_b2m = geometry_handle.Qb2m();
          
          V const T_m2l = V::make( x, y, z );
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
                                             , geometry_handle
                                             , mid
                                             , stone_density
                                             );
          
          engine->set_material_structure_map(
                                             rid
                                             , VT::zero(), VT::one(), VT::zero()                        // rotation axis
                                             , VT::one(), VT::zero(), VT::zero()                        // reference point
                                             , structure_field_x, structure_field_y, structure_field_z  // direction at reference point
                                             );
          
        }
      }
    }
  }
  
  
} //namespace procedural

// PROCEDURAL_MAKE_DROPPING_CAPSULES_H
#endif
