#ifndef PROCEDURAL_MAKE_BALL_H
#define PROCEDURAL_MAKE_BALL_H

#include <procedural.h>

#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{

  
  template<typename MT>
  void make_ball(  content::API * engine
                         , typename MT::real_type const & radius
                         , typename MT::vector3_type const & position
                         , typename MT::quaternion_type const & orientation
                         , typename MT::vector3_type const & velocity
                         , MaterialInfo<typename MT::real_type> mat_info
                         )
  
  {
    typedef typename MT::real_type       T;
    typedef typename MT::value_traits    VT;

    GeometryHandle<MT> ball_handle = create_geometry_handle_sphere<MT>( engine, radius );
    
    T      const density = get_material_density<MT>(mat_info, "Stone");
    size_t const mid     = get_material_id<MT>(mat_info, "Stone");
    
    static size_t counter = 0u;
    
    std::string const body_name = "ball_" + util::to_string( counter++ );
    
    size_t const rid = engine->create_rigid_body( body_name );
    
    engine->set_rigid_body_position( rid, position(0), position(1), position(2) );
    
    engine->set_rigid_body_orientation( rid, orientation.real(), orientation.imag()(0), orientation.imag()(1), orientation.imag()(2) );
    
    engine->set_rigid_body_velocity( rid, velocity(0), velocity(1), velocity(2) );
    
    engine->set_rigid_body_mass( rid, ball_handle.m_m*density );
    
    engine->set_rigid_body_inertia(
                                   rid
                                   , ball_handle.m_Ixx*density
                                   , ball_handle.m_Iyy*density
                                   , ball_handle.m_Izz*density
                                   );
    
    engine->connect_collision_geometry( rid, ball_handle.m_gid );
    
    engine->set_rigid_body_material( rid, mid );
    
    engine->set_rigid_body_fixed(rid, false);
    
    T const structure_field_x = VT::zero();
    T const structure_field_y = VT::zero();
    T const structure_field_z = VT::zero();

    engine->set_material_structure_map(
                                       rid
                                       , structure_field_x
                                       , structure_field_y
                                       , structure_field_z
                                       );

  }
  
} // namespace procedural

// PROCEDURAL_MAKE_BALL_H∫
#endif

