#include <procedural_factory.h>

#include <tiny_math_types.h>

#include <util_string_helper.h>

#include <string>

namespace procedural
{
  template<typename MT>
	size_t make_cannonball_rigid_body(
                                    content::API * engine
                                    , GeometryHandle<MT> const & ball
                                    , typename MT::vector3_type const & xbf
                                    , typename MT::quaternion_type const & Qbf
                                    , typename MT::vector3_type const & vbf
                                    , MaterialInfo<typename MT::real_type> mat_info
                                    )
	{
    typedef typename MT::real_type       T;
    
    T      const density = get_material_density<MT>(mat_info, "Cannonball");
		size_t const mid     = get_material_id<MT>(mat_info, "Cannonball");
    
		static size_t counter = 0u;
		
    std::string const body_name = "ball_" + util::to_string( counter++ );
		
		size_t const rid = engine->create_rigid_body( body_name );
		
		engine->set_rigid_body_position( rid, xbf(0), xbf(1), xbf(2) );
		
    engine->set_rigid_body_orientation( rid, Qbf.real(), Qbf.imag()(0), Qbf.imag()(1), Qbf.imag()(2) );

    engine->set_rigid_body_velocity( rid, vbf(0), vbf(1), vbf(2) );
		
		engine->set_rigid_body_mass( rid, ball.m_m*density );
		
		engine->set_rigid_body_inertia(
                                   rid
                                   , ball.m_Ixx*density
                                   , ball.m_Iyy*density
                                   , ball.m_Izz*density
                                   );
		
		engine->connect_collision_geometry( rid, ball.m_gid );
		
		engine->set_rigid_body_material( rid, mid );
    
    engine->set_rigid_body_fixed(rid, false);
		
		return rid;
	}
  
  typedef tiny::MathTypes<float>  MTf;
  
  template
	size_t make_cannonball_rigid_body<MTf>(
                                         content::API * engine
                                         , GeometryHandle<MTf> const & ball
                                         , MTf::vector3_type const & xbf
                                         , MTf::quaternion_type const & Qbf
                                         , MTf::vector3_type const & vbf
                                         , MaterialInfo<MTf::real_type> mat_info
                                         );
  
} //namespace procedural
