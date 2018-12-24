#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{
	
  template<typename MT>
	void make_sphere_layer(
                         content::API *  engine
                         , typename MT::vector3_type const & position
                         , typename MT::quaternion_type const & orientation
                         , typename MT::real_type const & sphere_radius
                         , size_t const & spheres_width
                         , size_t const & spheres_length
                         , MaterialInfo<typename MT::real_type> mat_info
                         )
	{
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type  Q;
    typedef typename MT::value_traits    VT;
    
		T      const stone_density	= get_material_density<MT>(mat_info, "Stone");
		size_t const mid		      	= get_material_id<MT>(mat_info, "Stone");
    
    GeometryHandle<MT> sphere_handle = create_geometry_handle_sphere<MT>( engine, sphere_radius );
    
    T x = VT::one();
    T y = sphere_radius;
    T z = VT::one();
    T const offset = sphere_radius*spheres_length*VT::half();
    
    for (size_t i = 0; i < spheres_width; ++i)
    {
      x = VT::two()*i*sphere_radius - offset;
      for (size_t j = 0; j < spheres_length; ++j)
      {
        z = sphere_radius + VT::two()*j*sphere_radius;
        V const Tb = sphere_handle.Tb2m();
        Q const Qb = sphere_handle.Qb2m();
        
        V const Tm = V::make( x, y, z );
        Q const Qm = Q::Ru( - VT::pi_half(), V::i() );
        
        V const Tw = rotate(Qm,Tb) + Tm;
        Q const Qw = Qm*Qb;
        
        V const Tu = rotate(orientation,Tw) + position;
        Q const Qu = orientation*Qw;
        
        create_rigid_body<MT>(  engine
                              , Tu
                              , Qu
                              , sphere_handle
                              , mid
                              , stone_density
                              );
      }
    }
  }
  
  
  typedef tiny::MathTypes<float> MTf;
  
  template
  void make_sphere_layer<MTf>(
                                          content::API *  engine
                                          , MTf::vector3_type const & position
                                          , MTf::quaternion_type const & orientation
                                          , MTf::real_type const & sphere_radius
                                          , size_t const & spheres_width
                                          , size_t const & spheres_length
                                          , MaterialInfo<MTf::real_type> mat_info
                                          );
  
} //namespace procedural

