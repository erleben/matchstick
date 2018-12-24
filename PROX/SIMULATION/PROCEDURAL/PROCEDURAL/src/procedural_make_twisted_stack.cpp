#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{
	
  template<typename MT>
	void make_twisted_stack(
                          content::API * engine
                          , typename MT::vector3_type const& position
                          , typename MT::quaternion_type const & orientation
                          , typename MT::real_type const & stone_dim
                          , size_t const & layers
                          , typename MT::real_type const & degree
                          , MaterialInfo<typename MT::real_type> mat_info
                          )
	{
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type Q;
    typedef typename MT::value_traits    VT;
    
		T const stone_density = get_material_density<MT>(mat_info, "Stone");
		size_t const mid      = get_material_id<MT>(mat_info, "Stone");
    
		GeometryHandle<MT> stone_handle = create_geometry_handle_box<MT>( engine, stone_dim, stone_dim, stone_dim );
    T twist = degree* (VT::pi()/180);;
		for( size_t i = 0u; i < layers;++i )
		{
      T y = ( i+VT::half() )*stone_dim;
      
      V const T_b2m = stone_handle.Tb2m();
      Q const Q_b2m = stone_handle.Qb2m();
      
      V const T_m2l = V::make( VT::zero(), y, VT::zero() );
      Q const Q_m2l = Q::Ry(twist*i);
      
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
  
  typedef tiny::MathTypes<float> MTf;
  
  template
	void make_twisted_stack<MTf>(
                               content::API * engine
                               , MTf::vector3_type const & position
                               , MTf::quaternion_type const & orientation
                               , MTf::real_type const & stone_dim
                               , size_t const & layers
                               , typename MTf::real_type const & twist
                               , MaterialInfo<MTf::real_type> mat_info
                               );
  
	
} //namespace procedural
