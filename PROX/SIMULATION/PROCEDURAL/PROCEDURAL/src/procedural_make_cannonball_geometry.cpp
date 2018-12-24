#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{
  template<typename MT>
	GeometryHandle<MT> make_cannonball_geometry(
                                              content::API * engine
                                              , typename MT::real_type const & radius
                                              )
	{
		return create_geometry_handle_sphere<MT>( engine, radius );
	}
  
  typedef tiny::MathTypes<float> MTf;
  
  template
	GeometryHandle<MTf> make_cannonball_geometry<MTf>(
                                                    content::API * engine
                                                    , MTf::real_type const & radius
                                                    );
} // namespace procedural