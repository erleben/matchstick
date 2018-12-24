#include <procedural.h>
#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{
  template<typename MT>
	void make_slide(  content::API * engine
                  , typename MT::vector3_type const & position
                  , typename MT::quaternion_type const & orientation
                  , typename MT::real_type const & degree
                  , MaterialInfo<typename MT::real_type> mat_info
                  )
	{
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type  Q;
    typedef typename MT::value_traits    VT;
    
    size_t const mid           = get_material_id<MT>(mat_info, "Stone");
    size_t const stone_density = get_material_density<MT>(mat_info, "Stone");
    
    T const radians         = degree* (VT::pi()/180);
    T const radius          = VT::half();
    V const hill_extents    = V::make( VT::numeric_cast(6.0), VT::numeric_cast(0.2), VT::four());
    V const box_extents     = V::make( radius, radius, radius)*VT::two();
    
    
    GeometryHandle<MT> hill   = create_geometry_handle_box<MT>(  engine, hill_extents(0), hill_extents(1), hill_extents(2));
    GeometryHandle<MT> stone  = create_geometry_handle_box<MT>(  engine, box_extents(0), box_extents(1), box_extents(2) );
    //  GeometryHandle<MT> sphere = create_geometry_handle_sphere<MT>( engine, radius);
    
    //BF or MF here?
    V Tm = rotate(Q::Ry(radians), V::make(hill_extents(0)*VT::half(), VT::zero(), VT::zero()));  //V::make(  std::cos(radians)*hill_extents(0)*VT::half(), std::sin(radians)*hill_extents(0)*VT::half(), VT::zero());
    Q Qm = Q::Rz(radians);
    
    // Body to Model transform
    V Tb = hill.Tb2m();
    Q Qb = hill.Qb2m();
    
    // Body to World transform
    V Tw = rotate(Qm,Tb) + Tm;
    Q Qw = Qm*Qb;
    
    // Apply any user transforms
    V Tu = rotate(orientation,Tw) + position;
    Q Qu = orientation*Qw;
    
    create_rigid_body<MT>(  engine
                          , Tu
                          , Qu
                          , hill
                          , mid
                          , stone_density
                          , true
                          );
    
    V TmB = rotate(Q::Ry(radians), V::make(hill_extents(0)-box_extents(0), (box_extents(1)+hill_extents(1)), VT::zero()));
    //V TmB = V::make(  std::cos(radians)*hill_extents(0)*VT::half()-box_extents(0)*VT::half()
      //              , std::sin(radians)*hill_extents(0)*VT::half()+box_extents(1)*VT::half()
      //              , radius*VT::two());
   // TmB   = TmB + Tm;
    
    // Body to Model transform
    Tb = stone.Tb2m();
    Qb = stone.Qb2m();
    
    // Body to World transform
    Tw = rotate(Qm, Tb) + TmB;
    Qw = Qm*Qb;
    
    // Apply any user transforms
    Tu = rotate(orientation, Tw) + position;
    Qu = orientation*Qw;
    
    create_rigid_body<MT>(  engine
                          , Tu
                          , Qu
                          , stone
                          , mid
                          , stone_density
                          );
    /*
     V TmS = V::make(  std::cos(radians)*hill_extents(0)*VT::half()-radius
     , std::sin(radians)*hill_extents(0)*VT::half()+radius
     , -radius*VT::two());
     TmS   = TmS + Tm;
     
     // Body to Model transform
     Tb = sphere.Tbf();
     Qb = sphere.Qbf();
     
     // Body to World transform
     Tw = rotate(Qm, Tb) + TmS;
     Qw = Qm*Qb;
     
     // Apply any user transforms
     Tu = rotate(orientation, Tw) + position;
     Qu = orientation*Qw;
     
     create_rigid_body<MT>(  engine
     , Tu
     , Qu
     , sphere
     , mid
     , stone_density
     );
     */
  }
	
  typedef tiny::MathTypes<float> MTf;
  
  template
	void make_slide<MTf>(  content::API * engine
                       , MTf::vector3_type const & position
                       , MTf::quaternion_type const & orientation
                       , MTf::real_type const & pi_frac
                       , MaterialInfo<MTf::real_type> mat_info
                       );
  
}
//namespace procedural
