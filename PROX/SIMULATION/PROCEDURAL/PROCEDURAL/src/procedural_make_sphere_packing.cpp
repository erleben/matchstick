#include <procedural_factory.h>

#include <tiny_math_types.h>

#include <cmath> // needed for std::floor

namespace procedural
{

  template<typename MT>
	void make_sphere_packing(
                           content::API *  engine
                           , typename MT::vector3_type const & position
                           , typename MT::quaternion_type const & orientation
                           , typename MT::real_type const & min_radius
                           , typename MT::real_type const & max_radius
                           , typename MT::real_type const & width
                           , typename MT::real_type const & height
                           , typename MT::real_type const & depth
                           , size_t const & number_of_spheres
                           , MaterialInfo<typename MT::real_type> mat_info
                           )
	{
    using std::floor;
    using std::ceil;

    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type  Q;
    typedef typename MT::value_traits    VT;

		T      const stone_density	= get_material_density<MT>(mat_info, "Stone");
		size_t const mid		      	= get_material_id<MT>(mat_info, "Stone");

    std::vector< GeometryHandle<MT> > sphere_handle;
    sphere_handle.resize(10u);
    for (unsigned int c=0;c< 10u;++c)
    {
      T const radius =  (c+1u)*(max_radius - min_radius)/VT::numeric_cast(10.0) + min_radius;
      sphere_handle[c] = create_geometry_handle_sphere<MT>( engine, radius );
    }

    unsigned int const I = std::floor( width/(max_radius*VT::two())  );
    unsigned int const K = std::floor( depth/(max_radius*VT::two()) );
    unsigned int const J = std::ceil( 1.0f*number_of_spheres/(I*K)  );

    unsigned int sphere_count = 0u;

    for(unsigned int j=0u; j < J; ++j)
      for(unsigned int k=0u; k < K; ++k)
        for(unsigned int i=0u; i < I; ++i)
        {
          if(sphere_count > number_of_spheres)
            return;

          T const x = -width*VT::half()+max_radius  + i*max_radius*VT::two();
          T const z = -depth*VT::half()+max_radius  + k*max_radius*VT::two();
          T const y = -height*VT::half()+max_radius + j*max_radius*VT::two();

          int choice = sphere_count % 10;

          V const T_b2m = sphere_handle[choice].Tb2m();
          Q const Q_b2m = sphere_handle[choice].Qb2m();

          V const T_m2l = V::make( x, y, z );
          Q const Q_m2l = Q::Ru( - VT::pi_half(), V::i() );

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
                                , sphere_handle[choice]
                                , mid
                                , stone_density
                                );
          ++sphere_count;
        }
	}

  typedef tiny::MathTypes<float> MTf;

  template
	void make_sphere_packing<MTf>(
                                content::API *  engine
                                , MTf::vector3_type const & position
                                , MTf::quaternion_type const & orientation
                                , MTf::real_type const & min_radius
                                , MTf::real_type const & max_radius
                                , MTf::real_type const & width
                                , MTf::real_type const & height
                                , MTf::real_type const & depth
                                , size_t const & number_of_spheres
                                , MaterialInfo<MTf::real_type> mat_info
                                );
	
} //namespace procedural

