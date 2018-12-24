#include <procedural.h>
#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{
  template<typename MT>
  void make_cylinder(  content::API * engine
                     , typename MT::vector3_type const & position
                     , typename MT::quaternion_type const & orientation
                     , typename MT::real_type const & radius
                     , typename MT::real_type const & length
                     , typename MT::real_type const & Sa
                     , typename MT::real_type const & Sb
                     , MaterialInfo<typename MT::real_type> mat_info
                     )
  {
    typedef typename MT::real_type        T;
    typedef typename MT::vector3_type     V;
    typedef typename MT::quaternion_type  Q;
    typedef typename MT::value_traits    VT;

    size_t const mid           = get_material_id<MT>(mat_info, "Stone");
    size_t const stone_density = get_material_density<MT>(mat_info, "Stone");

    V const hill_extents    = V::make( VT::numeric_cast(32.0), VT::numeric_cast(1.0), VT::four());
    T const degree          = 10;
    T const radians_d       = degree* (M_PI/180.0);


    GeometryHandle<MT> hill   = create_geometry_handle_box<MT>(  engine
                                                               , hill_extents(0)
                                                               , hill_extents(1)
                                                               , hill_extents(2)
                                                               );

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
    // Essentially the coordinates are (x,z,y)
    if (0.0 == Sa)
    {
      // Straight down
      engine->set_material_structure_map(
                                         rid
                                         , -1.0
                                         , 0.0
                                         , 0.0
                                         );
    }
    if (1.0 == Sa)
    {
      // Skewed
      engine->set_material_structure_map(
                                         rid
                                         , std::sqrt(2.0)/2.0
                                         , 0.0
                                         , -std::sqrt(2.0)/2.0
                                         );
    }
    if( 2.0 == Sa)
    {
      // Sideways
      engine->set_material_structure_map(
                                         rid
                                         , 0.0
                                         , 0.0
                                         , 1.0
                                         );
    }


    GeometryHandle<MT> cylinder  = create_geometry_handle_cylinder<MT>(  engine, radius, length);

    // Position of hill (center)
    //Tm = V::make(VT::zero(), radius+VT::half()*hill_extents(0)*std::sin(radians_d)+hill_extents(1)/std::cos(radians_d), VT::zero());
    Tm = V::make(VT::half()*hill_extents(0)*0.9, hill_extents(0)*std::sin(radians_d)+0.5*hill_extents(1)/std::cos(radians_d)+0.27, VT::zero());

    // Orientation of hill (center)
    Qm = Q::Rx(VT::half()*M_PI);


    // Body to Model transform
    Tb = cylinder.Tb2m();
    Qu = cylinder.Qb2m();

    // Body to World transform
    Tu = rotate(Qm,Tb) + Tm;
    Qu = Qm*Qb;

    rid = create_rigid_body<MT>(  engine
                                , Tu
                                , Qu
                                , cylinder
                                , mid
                                , stone_density
                                );




    if (0.0 == Sb)
    {
      // Straight down direction
      engine->set_material_structure_map(
                                               rid
                                               , -1.0
                                               , 0.0
                                               , 0.0);
    }
    if (1.0 == Sb)
    {
      // This is a structure direction to the 'left' in the cylinder (along the cone)
      engine->set_material_structure_map(
                                         rid
                                         , 0.0
                                         , -1.0
                                         , 0.0
                                         );
    }

  }

  typedef tiny::MathTypes<float> MTf;

  template
  void make_cylinder<MTf>(  content::API * engine
                          , MTf::vector3_type const & position
                          , MTf::quaternion_type const & orientation
                          , MTf::real_type const & radius
                          , MTf::real_type const & length
                          , MTf::real_type const & Sa
                          , MTf::real_type const & Sb
                          , MaterialInfo<MTf::real_type> mat_info
                          );

}
//namespace procedural
