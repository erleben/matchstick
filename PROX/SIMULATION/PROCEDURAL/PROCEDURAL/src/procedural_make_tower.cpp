#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{
	
  template<typename MT>
	void make_tower(
                    content::API * engine
                  , typename MT::vector3_type const & position
                  , typename MT::quaternion_type const & orientation
                  , typename MT::real_type const & r_outer
                  , typename MT::real_type const & r_inner
                  , typename MT::real_type const & height
                  , size_t const & slices
                  , size_t const & segments
                  , MaterialInfo<typename MT::real_type> mat_info
                  , bool const & use_cubes
                  )
	{
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type  Q;
    typedef typename MT::value_traits    VT;
    
		size_t const mid = get_material_id<MT>(mat_info, "Stone");
		
		T const stone_depth    = height/segments;
		T const delta_theta    = VT::two()* VT::pi()/ slices;
		T const center_radius  = ( r_outer + r_inner )*VT::half();
		T const stone_density  = get_material_density<MT>(mat_info, "Stone");

    std::vector<V>  vertices(8u);
    compute_arch_stone_vertices<MT>(
                                    delta_theta
                                    , stone_depth
                                    , r_outer
                                    , r_inner
                                    , &vertices[0]
                                    );

    //T const center_height    = (r_outer + r_inner)*0.5f;
    T const max_height       = r_outer*cos( delta_theta*0.5f );
    T const min_height       = r_inner*cos( delta_theta*0.5f );
    T const half_width_inner = r_inner * sin( delta_theta*0.5f );
    //T const half_width_outer = r_outer * sin( delta_theta*0.5f );
    T const half_depth       = stone_depth*0.5f;

    GeometryHandle<MT> stone_handle;

    if (use_cubes)
    {
      stone_handle = create_geometry_handle_box<MT>(
                                                                       engine
                                                                       , 1.9*half_width_inner
                                                                       , max_height - min_height
                                                                       , 2*half_depth
                                                                       );
    }
    else
    {
      stone_handle = create_geometry_handle_cuboid<MT>( engine, &vertices[0] );
    }

		for(size_t i = 0u; i < segments; ++i)
		{
			for(size_t j = 0u; j < slices; ++j)
			{
				T const theta = j*delta_theta + (i%2)*( delta_theta*VT::half() );
				T const x     = center_radius * cos(  theta  );
				T const y     = center_radius * sin(  theta  );
				T const z     = (i + VT::half()) * stone_depth;
				
				V const T_b2m = stone_handle.Tb2m();
				Q const Q_b2m = stone_handle.Qb2m();
        
				V const T_m2l = V::make( x, y, z );
        Q const Q_m2l = Q::Ru( theta - VT::pi_half(),  V::k() );
				
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

				create_rigid_body<MT>(
                              engine
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
	void make_tower<MTf>(
                       content::API * engine
                       , MTf::vector3_type const & position
                       , MTf::quaternion_type const & orientation
                       , MTf::real_type const & r_outer
                       , MTf::real_type const & r_inner
                       , MTf::real_type const & height
                       , size_t const & slices
                       , size_t const & segments
                       , MaterialInfo<MTf::real_type> mat_info
                       , bool const & use_cubes
                       );
  
} //namespace procedural
