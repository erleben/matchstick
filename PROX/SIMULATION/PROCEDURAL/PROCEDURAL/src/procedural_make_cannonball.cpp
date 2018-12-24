#include <procedural.h>
#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{

  
  template<typename MT>
  void make_cannonball(  content::API * engine
                         , typename MT::real_type const & radius
                         , typename MT::vector3_type const & position
                         , typename MT::quaternion_type const & orientation
                         , typename MT::vector3_type const & velocity
                         , MaterialInfo<typename MT::real_type> mat_info
                         )
  
  {
    GeometryHandle<MT> cannon_ball = make_cannonball_geometry<MT>(engine, radius);
    make_cannonball_rigid_body(engine, cannon_ball, position, orientation, velocity, mat_info);

  }
  
  
  typedef tiny::MathTypes<float> MTf;
  
  template
  void make_cannonball<MTf>(  content::API * engine
                         , MTf::real_type const & radius
                         , MTf::vector3_type const & position
                         , MTf::quaternion_type const & orientation
                         , MTf::vector3_type const & velocity
                         , MaterialInfo<MTf::real_type> mat_info
                              );
  
} // namespace procedural
