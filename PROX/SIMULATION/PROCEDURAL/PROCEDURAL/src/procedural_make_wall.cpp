#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{
	
  template<typename MT>
	void make_wall(
                 content::API * engine
                 , typename MT::vector3_type const& position
                 , typename MT::quaternion_type const & orientation
                 , typename MT::real_type const & width
                 , typename MT::real_type const & height
                 , typename MT::real_type const & depth
                 , size_t const & layers
                 , size_t const & span
                 , MaterialInfo<typename MT::real_type> mat_info
                 )
	{
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type  Q;
    typedef typename MT::value_traits    VT;
    
		T      const stone_width	 = width/span;
		T      const stone_height  = height/layers;
		T      const stone_density = get_material_density<MT>(mat_info, "Stone");
		size_t const mid           = get_material_id<MT>(mat_info, "Stone");
    
		GeometryHandle<MT> stone_handle = create_geometry_handle_box<MT>( engine, stone_width, stone_height, depth  );

		for( size_t i = 0u; i < layers;++i )
		{
			for( size_t j = 0u; j < span;++j )
			{
				T const x = j*(stone_width) + ( ( i%2 )*VT::half()*stone_width );
				T const y = ( i+VT::half() )*(stone_height);
				T const z = VT::zero();
				
				V const T_b2m = stone_handle.Tb2m();
				Q const Q_b2m = stone_handle.Qb2m();
				
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

        create_rigid_body<MT>(  engine
                              , T_b2w
                              , Q_b2w
                              , stone_handle
                              , mid
                              , stone_density
                              );
        
			}
		}
	}
  
	typedef tiny::MathTypes<float> MTf;
  
  template
	void make_wall<MTf>(
                      content::API * engine
                      , MTf::vector3_type const& position
                      , MTf::quaternion_type const & orientation
                      , MTf::real_type const & width
                      , MTf::real_type const & height
                      , MTf::real_type const & depth
                      , size_t const & layers
                      , size_t const & span
                      , MaterialInfo<MTf::real_type> mat_info
                      );
  
} //namespace procedural
