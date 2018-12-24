#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{
	
  template<typename MT>
	void make_dropping_spheres(
                             content::API *  engine
                             , typename MT::vector3_type const & position
                             , typename MT::quaternion_type const & orientation
                             , typename MT::real_type const & sphere_radius
                             , size_t const & w
                             , size_t const & h
                             , size_t const & d
                             , MaterialInfo<typename MT::real_type> mat_info
                             )
	{
    typedef typename MT::real_type          T;
    typedef typename MT::vector3_type       V;
    typedef typename MT::quaternion_type    Q;
    typedef typename MT::value_traits       VT;
    
		T      const stone_density	= get_material_density<MT>(mat_info, "Stone");
		size_t const mid		    = get_material_id<MT>(mat_info, "Stone");
    
    GeometryHandle<MT> sphere_handle = create_geometry_handle_sphere<MT>( engine, sphere_radius );
    GeometryHandle<MT> plane_handle  = create_geometry_handle_box<MT>( engine, VT::two(), VT::numeric_cast(0.25f), VT::two());
    
    T x = VT::one();
    T y = VT::one();
    T z = VT::one();
    T const offset_x = sphere_radius*w;
    T const offset_z = sphere_radius*d;
   
    for (size_t i = 0; i < w; ++i)
    {
      x = VT::two()*i*sphere_radius - offset_x;
      for (size_t j = 0; j < h; ++j)
      {
        y = sphere_radius + VT::two()*j*sphere_radius + VT::four();
        
        for (size_t k = 0; k < d; ++k)
        {
          z = VT::two()*k*sphere_radius - offset_z;
          
          V const T_b2m = sphere_handle.Tb2m();
          Q const Q_b2m = sphere_handle.Qb2m();
          
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
                                , sphere_handle
                                , mid
                                , stone_density
                                );
        }
      }
    }
    
    V T_b2m = plane_handle.Tb2m();
    Q Q_b2m = plane_handle.Qb2m();
    
    V T_m2l = V::make( VT::numeric_cast(-1.25), VT::numeric_cast(2.75), VT::zero() );
    Q Q_m2l = Q::Rz(-VT::pi_quarter());
    
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
                          , plane_handle
                          , mid
                          , stone_density
                          , true
                          );
    
    T_b2w = plane_handle.Tb2m();
    Q_b2w = plane_handle.Qb2m();
    
    T_m2l = V::make( VT::numeric_cast(1.25), VT::numeric_cast(2.75), VT::zero() );
    Q_m2l = Q::Rz(VT::pi_quarter());
    
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
                          , plane_handle
                          , mid
                          , stone_density
                          , true
                          );
       
	}
  
  typedef tiny::MathTypes<float> MTf;
  
  template
	void make_dropping_spheres<MTf>(
                                  content::API *  engine
                                  ,  MTf::vector3_type const & position
                                  ,  MTf::quaternion_type const & orientation
                                  ,  MTf::real_type const & sphere_radius
                                  , size_t const & w
                                  , size_t const & h
                                  , size_t const & d
                                  , MaterialInfo< MTf::real_type> mat_info
                                  );
  	
} //namespace procedural
