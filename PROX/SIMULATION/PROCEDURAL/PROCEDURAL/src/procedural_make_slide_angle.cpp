#include <procedural.h>
#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{
  template<typename MT>
  void make_slide_angle(  content::API * engine
                        , typename MT::vector3_type const & position
                        , typename MT::quaternion_type const & orientation
                        , typename MT::real_type const & degree
                        , typename MT::real_type const & angle
                        , MaterialInfo<typename MT::real_type> mat_info
                        )
  {
    typedef typename MT::real_type        T;
    typedef typename MT::vector3_type     V;
    typedef typename MT::quaternion_type  Q;
    typedef typename MT::value_traits    VT;

    size_t const mid           = get_material_id<MT>(mat_info, "Stone");
    //size_t const stone_density = get_material_density<MT>(mat_info, "Stone");
    size_t const stone_density = 100;

    T const radians_d       = degree* (VT::pi()/180);
    T const radians_a       = angle * (VT::pi()/180);
    T const radius          = VT::half();
    V const hill_extents    = V::make( VT::numeric_cast(32.0), VT::numeric_cast(1.0), VT::four());
    T const a = radius; // radius*2;
    T const b = radius;
    V const box_extents     = V::make( a,b, b)*VT::two();


    GeometryHandle<MT> hill   = create_geometry_handle_box<MT>(  engine, hill_extents(0), hill_extents(1), hill_extents(2));
    GeometryHandle<MT> stone  = create_geometry_handle_box<MT>(  engine, box_extents(0), box_extents(1), box_extents(2) );

    // Position of hill (center)
    V Tm = V::make(VT::zero(), VT::half()*hill_extents(0)*std::sin(radians_d), VT::zero());
    // Orientation of hill (center)
    Q Qm = Q::Rz(radians_d);

    // Body to Model transform
    V Tb = hill.Tb2m();
    Q Qb = hill.Qb2m();

    // Body to World transform
    V Tu = rotate(Qm,Tb) + Tm;
    Q Qu = Qm*Qb;

    size_t rid = create_rigid_body<MT>(  engine
                                       , Tu
                                       , Qu
                                       , hill
                                       , mid
                                       , stone_density
                                       , true
                                       );

    engine->set_material_structure_map(
                                       rid
                                       , std::sqrt(2.0)/2.0
                                       , 0.0
                                       , std::sqrt(2.0)/2.0
                                       );

    // Calculate the 'border' box of the rotated box
    T const A =a*std::sin(M_PI/2-radians_a)+b*std::sin(radians_a);
    // Orientation of box
    Qm = Q::Rz(radians_d)*Q::Ry(radians_a); // Orientation of rigid body in WCS

    // Position of box
    Tm = V::make(VT::half()*hill_extents(0)*std::cos(radians_d)*0.85
                 , (VT::half()+0.40)*hill_extents(0)*std::sin(radians_d)+hill_extents(1)/std::cos(radians_d) + b*std::sin(M_PI/2-radians_d)+A*std::sin(radians_d)
                 , VT::zero()
                 );

    // Body to Model transform
    Tb = hill.Tb2m();
    Qb = hill.Qb2m();

    // Body to World transform//
    Tu = rotate(Qm,Tb) + Tm;
    Qu = Qm*Qb;

    rid = create_rigid_body<MT>(  engine
                                , Tu
                                , Qu
                                , stone
                                , mid
                                , stone_density
                                );

    engine->set_material_structure_map(
                                       rid
                                       , std::sqrt(2.0)/2.0
                                       , 0.0
                                       , std::sqrt(2.0)/2.0
                                       );
  }

  typedef tiny::MathTypes<float> MTf;

  template
  void make_slide_angle<MTf>(  content::API * engine
                             , MTf::vector3_type const & position
                             , MTf::quaternion_type const & orientation
                             , MTf::real_type const & pi_frac
                             , MTf::real_type const & pi_frac_angle
                             , MaterialInfo<MTf::real_type> mat_info
                             );

}
//namespace procedural
