#include <procedural_factory.h>

#include <tiny_math_types.h>

#include <cmath>
#include <cassert>

namespace procedural
{
	
  template<typename MT>
	void make_dome(
                   content::API * engine
                 , typename MT::vector3_type const & position
                 , typename MT::quaternion_type const & orientation
                 , typename MT::real_type const & r_outer
                 , typename MT::real_type const & r_inner
                 , size_t const & slices
                 , size_t const & segments
                 , MaterialInfo<typename MT::real_type> mat_info
                 )
	{
		using std::cos;
		using std::sin;
    
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type  Q;
    typedef typename MT::value_traits    VT;
    
		assert(r_inner > VT::zero() || !"invalid inner radius value");
		assert(r_outer > r_inner    || !"invalid outer radius value");
		assert( slices>2u           || !"at last three slices  must be used");
		assert( segments>0u         || !"at last one segment must be used");
		
		size_t const mid = get_material_id<MT>(mat_info,"Stone");
		
		// The dome is created using spherical coordinates as defined here
		//
		// http://mathworld.wolfram.com/SphericalCoordinates.html
		//
		//   x = r cos( theta ) sin( phi )
		//   y = r sin( theta ) sin( phi )
		//   z = r cos( phi )
		//
		// Initially the up-direction of the dome corresponds to the positive z-axis.
		
		T const offset_theta  = VT::zero();
		T const delta_theta   = VT::two() * VT::pi() / VT::numeric_cast( slices );
		T const delta_phi     = VT::pi_half() / VT::numeric_cast( segments + 1u );
		T const stone_density = get_material_density<MT>(mat_info, "Stone");
		T phi = delta_phi*VT::numeric_cast( 1.5f );
		
		for( size_t v = 0u; v < segments;  ++v, phi += delta_phi )
		{
			// When we create the stone we are looking down the negative x-axis direction and having the positive y-axis as our up-direction
			T const cos_phi_upper   = cos(   phi - delta_phi*VT::half() );
			T const sin_phi_upper   = sin(   phi - delta_phi*VT::half() );
			T const cos_phi_lower   = cos(   phi + delta_phi*VT::half() );
			T const sin_phi_lower   = sin(   phi + delta_phi*VT::half() );
			T const cos_theta_left  = cos(  -delta_theta / VT::two()    );
			T const sin_theta_left  = sin(  -delta_theta / VT::two()    );
			T const cos_theta_right = cos(   delta_theta / VT::two()    );
			T const sin_theta_right = sin(   delta_theta / VT::two()    );
			
			V vertices[8];
			
			// The front face in CCW order
			vertices[0] = r_outer * V::make( cos_theta_left  * sin_phi_lower, sin_theta_left  * sin_phi_lower,  cos_phi_lower );
			vertices[1] = r_outer * V::make( cos_theta_right * sin_phi_lower, sin_theta_right * sin_phi_lower,  cos_phi_lower );
			vertices[2] = r_outer * V::make( cos_theta_right * sin_phi_upper, sin_theta_right * sin_phi_upper,  cos_phi_upper );
			vertices[3] = r_outer * V::make( cos_theta_left  * sin_phi_upper, sin_theta_left  * sin_phi_upper,  cos_phi_upper );
			
			// The back face in CCW order
			vertices[4] = r_inner * vertices[0] / r_outer;
			vertices[5] = r_inner * vertices[1] / r_outer;
			vertices[6] = r_inner * vertices[2] / r_outer;
			vertices[7] = r_inner * vertices[3] / r_outer;
			
			//--- Compute geometric center of the stone
      V const geometric_center =  ( vertices[0]
                                  + vertices[1]
                                  + vertices[2]
                                  + vertices[3]
                                  + vertices[4]
                                  + vertices[5]
                                  + vertices[6]
                                  + vertices[7] )/VT::eight();

      //--- Change geometry so geometric center is the model reference.

			for( size_t i=0; i<8; ++i )
        vertices[i] = vertices[i] -  geometric_center;
			
			GeometryHandle<MT> stone_handle = create_geometry_handle_cuboid<MT>(  engine, vertices );
			
			T theta = offset_theta;
			for( size_t h = 0u; h < slices;	++h, theta += delta_theta )
			{
        V const T_b2m = stone_handle.Tb2m();
        Q const Q_b2m = stone_handle.Qb2m();

				Q const Q_m2l = Q::Ru( theta , V::k() );
        V const T_m2l = rotate(Q_m2l, geometric_center);//V::make( x, y, z );

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
	void make_dome<MTf>(  content::API * engine
                      , MTf::vector3_type const & position
                      , MTf::quaternion_type const & orientation
                      , MTf::real_type const & r_outer
                      , MTf::real_type const & r_inner
                      , size_t const & slices
                      , size_t const & segments
                      , MaterialInfo<MTf::real_type> mat_info
                      );
  
	
} //namespace procedural
