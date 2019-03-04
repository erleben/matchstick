#ifndef PROCEDURAL_MAKE_ARCH_H
#define PROCEDURAL_MAKE_ARCH_H

#include <procedural_factory.h>

#include <tiny_math_types.h>

#include <cmath>

namespace procedural
{
  
  template<typename MT>
  inline void make_arch(
                        content::API *  engine
                        , typename MT::vector3_type const & position
                        , typename MT::quaternion_type const & orientation
                        , typename MT::real_type const & r_outer
                        , typename MT::real_type const & r_inner
                        , typename MT::real_type const & pillar_height
                        , typename MT::real_type const & stone_depth
                        , size_t const & arch_slices
                        , size_t const & pillar_segments
                        , typename MT::real_type const & structure_field_x
                        , typename MT::real_type const & structure_field_y
                        , typename MT::real_type const & structure_field_z
                        , MaterialInfo<typename MT::real_type> mat_info
                        )
  {
    using std::cos;
    using std::sin;
    
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type Q;
    typedef typename MT::value_traits    VT;
    typedef typename MT::coordsys_type   C;

    T      const delta_theta         = VT::pi()/arch_slices;
    
    T      const pillar_stone_width  = pillar_height > VT::zero() ? pillar_height / pillar_segments : VT::one();
    T      const pillar_stone_height = r_outer - r_inner;
    
    T      const center_radius       = ( r_outer + r_inner )*VT::half();
    T      const stone_density       = get_material_density<MT>(mat_info, "Stone");
    size_t const mid                 = get_material_id<MT>(mat_info, "Stone");
        
    size_t rid = 0;
    
    std::vector<V>  arch_vertices(8u);
    
    compute_arch_stone_vertices<MT>(
                                    delta_theta
                                    , stone_depth
                                    , r_outer
                                    , r_inner
                                    , &arch_vertices[0]
                                    );
    
    GeometryHandle<MT> arch_stone   = create_geometry_handle_cuboid<MT>( engine, &arch_vertices[0] );
    GeometryHandle<MT> pillar_stone = create_geometry_handle_box<MT>( engine, pillar_stone_width, pillar_stone_height, stone_depth );
    
    // Creating the stones in the two sides
    for (size_t i = 0; i < pillar_segments; ++i)
    {
      T const y = (VT::half() + i) * pillar_stone_width;
      
      V const T_b2m = pillar_stone.Tb2m();
      Q const Q_b2m = pillar_stone.Qb2m();
      
      V const T_left_m2l = V::make(-center_radius, y, VT::zero());
      Q const Q_left_m2l = Q::Ru(VT::pi_half(), V::k());
      
      V const T_l2w = position;
      Q const Q_l2w = orientation;
      
      V T_left_b2w;
      Q Q_left_b2w;
      
      compute_body_to_world_transform<MT>(
                                          T_b2m, Q_b2m, T_left_m2l, Q_left_m2l, T_l2w, Q_l2w, T_left_b2w, Q_left_b2w
                                          );
      
      rid = create_rigid_body<MT>(
                                  engine, T_left_b2w, Q_left_b2w, pillar_stone, mid, stone_density
                                  );
      
      engine->set_material_structure_map(
                                         rid
                                         , structure_field_x
                                         , structure_field_y
                                         , structure_field_z
                                         );
      
      V const T_right_m2l = V::make(center_radius, y, VT::zero());
      Q const Q_right_m2l = Q::Ru(VT::pi_half(), V::k());
      
      V T_right_b2w;
      Q Q_right_b2w;
      
      compute_body_to_world_transform<MT>(
                                          T_b2m, Q_b2m, T_right_m2l, Q_right_m2l, T_l2w, Q_l2w, T_right_b2w, Q_right_b2w
                                          );
      
      rid = create_rigid_body<MT>(
                                  engine, T_right_b2w, Q_right_b2w, pillar_stone, mid, stone_density
                                  );
      
      engine->set_material_structure_map(
                                         rid
                                         , structure_field_x
                                         , structure_field_y
                                         , structure_field_z
                                         );
    }
    
    // Creating the round top part of the arch
    for(size_t i = 0;i<arch_slices;++i)
    {
      T const theta = delta_theta*( i+VT::half() );
      T const x     = center_radius * cos( theta );
      T const y     = center_radius * sin( theta ) + pillar_height;
      
      V const T_b2m = arch_stone.Tb2m();
      Q const Q_b2m = arch_stone.Qb2m();
      
      V const T_m2l = V::make( x, y, VT::zero() );
      Q const Q_m2l = Q::Ru(  theta - VT::pi_half() , V::k() );
      
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
      
      rid = create_rigid_body<MT>(  engine
                                  , T_b2w
                                  , Q_b2w
                                  , arch_stone
                                  , mid
                                  , stone_density
                                  );
      
      
      // 2019-03-04 Kenny: Apply X_m2b to rotational sweep! The structure
      // field rotational sweep are specified in the model frame of the arch stone.
      // The sweep parameters must be mapped into the body frame with respect to
      // which the geometry (vertex coordinates) are stored with.
      C const X_b2m = C::make(T_b2m, Q_b2m);
      C const X_m2b = tiny::inverse(X_b2m);
      
      V const axis_mf   = V::make(VT::zero(), VT::zero(), VT::one());
      V const center_mf = V::make(VT::zero(), - center_radius, VT::zero() );
      V const ref_mf    = V::make(VT::zero(), VT::one(), VT::zero() );
      V const s_mf      = V::make(structure_field_x, structure_field_y, structure_field_z);
      
      V const axis       = tiny::xform_vector(X_m2b, axis_mf);
      V const center     = tiny::xform_point(X_m2b, center_mf);
      V const ref        = tiny::xform_vector(X_m2b, ref_mf);
      V const s          = tiny::xform_vector(X_m2b, s_mf);

      engine->set_material_structure_map(
                                         rid
                                         , axis(0), axis(1), axis(2)            // rotation axis
                                         , center(0), center(1), center(2)      // center of rotation
                                         , ref(0), ref(1), ref(2)               // reference point
                                         , s(0), s(1), s(2)                     // reference structure direction
                                         );
    }
  }
  
} //namespace procedural

// PROCEDURAL_MAKE_ARCH_H∫
#endif

