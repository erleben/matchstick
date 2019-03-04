#ifndef PROCEDURAL_MAKE_OBJ_H
#define PROCEDURAL_MAKE_OBJ_H

#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{
  
  template<typename MT>
  inline void make_obj(
                       content::API * engine
                       , std::string const & name
                       , typename MT::real_type const & scale
                       , typename MT::vector3_type const& position
                       , typename MT::quaternion_type const & orientation
                       , typename MT::real_type const & structure_field_x
                       , typename MT::real_type const & structure_field_y
                       , typename MT::real_type const & structure_field_z
                       , MaterialInfo<typename MT::real_type> mat_info
                       , bool const fixed = false
                       , std::string const & material = "Stone"
                       , mesh_array::TetGenSettings tetset = mesh_array::tetgen_default_settings()
                       )
  {
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type Q;
    typedef typename MT::value_traits    VT;
    typedef typename MT::coordsys_type   C;

    T const stone_density = get_material_density<MT>(mat_info, material);
    size_t const mid      = get_material_id<MT>(mat_info, material);
    
    GeometryHandle<MT> obj_handle = create_geometry_handle_obj<MT>(engine, name, scale, tetset);
    
    V const T_b2m = obj_handle.Tb2m();
    Q const Q_b2m = obj_handle.Qb2m();
    
    V const T_m2l = V::make( VT::zero(), VT::zero(), VT::zero() );
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
                                       , obj_handle
                                       , mid
                                       , stone_density
                                       , fixed
                                       );
    
    // 2019-03-04 Kenny: Apply X_m2b to rotational sweep! The structure
    // field rotational sweep are specified in the model frame of the arch stone.
    // The sweep parameters must be mapped into the body frame with respect to
    // which the geometry (vertex coordinates) are stored with.
    C const X_b2m = C::make(T_b2m, Q_b2m);
    C const X_m2b = tiny::inverse(X_b2m);
    
    V const axis_mf   = V::make(VT::zero(), VT::one(),  VT::zero() );
    V const center_mf = V::make(VT::zero(), VT::zero(), VT::zero() );
    V const ref_mf    = V::make(VT::one(),  VT::zero(), VT::zero() );
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
  
} //namespace procedural

// PROCEDURAL_MAKE_OBJ_H
#endif
