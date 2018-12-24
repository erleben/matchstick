#include <procedural_types.h>

#include <content_api.h>

#include <mass.h>

#include <tiny_math_types.h>
#include <util_string_helper.h>

#include <string>

namespace procedural
{
  
  template<typename MT>
  MaterialInfo<typename MT::real_type> create_material_info( content::API * engine )
  {
    typedef typename MT::real_type       T;
    typedef typename MT::value_traits    VT;
    
    MaterialInfo<T> mat_info;
    
    //--- Dummy values till we get more 'correct' ones, drilling is not actually implemented
    T const drilling_dummy    = VT::numeric_cast(0.01f); // set dummy different from zero, as prox_numerical_ellipsoid requires nonzero coefficents

    //instantiating the three different material types
    mat_info.m_stone_mid      = engine->create_material("Stone");
    mat_info.m_ground_mid     = engine->create_material("Ground");
    mat_info.m_cannonball_mid = engine->create_material("Cannonball");
    
    mat_info.m_stone_density      = mass::get_density<double>(mass::BRICK); // 2011-01-27 Kenny: Bogus double to float conversion, argh!
    mat_info.m_ground_density     = mass::get_density<double>(mass::EARTH);
    mat_info.m_cannonball_density = mass::get_density<double>(mass::IRON);
    
    //--- Coupling pairs of materials, dry sliding friction
    //--- Coefficients taken from http://www.supercivilcd.com/FRICTION.htm

    //--- Brick on rock = 0.75
    engine->create_material_property(
                                     mat_info.m_stone_mid
                                     , mat_info.m_ground_mid
                                     );

    engine->set_coefficients_of_anisotropic_friction(
                                                     mat_info.m_stone_mid
                                                     , mat_info.m_ground_mid
                                                     , 0.75f
                                                     , 0.75f
                                                     , drilling_dummy
                                                     );

    engine->set_coefficient_of_isotropic_friction(
                                                  mat_info.m_stone_mid
                                                  , mat_info.m_ground_mid
                                                  , 0.75f
                                                  );

    engine->set_coefficient_of_restitution(
                                           mat_info.m_stone_mid
                                           , mat_info.m_ground_mid
                                           , 0.3f
                                           );


    //--- Concrete to steel = 0.45
    engine->create_material_property(
                                     mat_info.m_stone_mid
                                     , mat_info.m_cannonball_mid
                                     );

    engine->set_coefficient_of_isotropic_friction(
                                                  mat_info.m_stone_mid
                                                  , mat_info.m_cannonball_mid
                                                  , 0.45f
                                                  );

    engine->set_coefficients_of_anisotropic_friction(
                                                     mat_info.m_stone_mid
                                                     , mat_info.m_cannonball_mid
                                                     , 0.45f
                                                     , 0.45f
                                                     , drilling_dummy
                                                     );

    engine->set_coefficient_of_restitution(
                                           mat_info.m_stone_mid
                                           , mat_info.m_cannonball_mid
                                           , 0.8f
                                           );

    //--- Brick on brick = 0.65
    engine->create_material_property(
                                     mat_info.m_stone_mid
                                     , mat_info.m_stone_mid
                                     );

    engine->set_coefficient_of_isotropic_friction(
                                                  mat_info.m_stone_mid
                                                  , mat_info.m_stone_mid
                                                  , 0.65f
                                                  );

    engine->set_coefficients_of_anisotropic_friction(
                                                     mat_info.m_stone_mid
                                                     , mat_info.m_stone_mid
                                                     , 0.65f
                                                     , 0.65f
                                                     , drilling_dummy
                                                     );

    engine->set_coefficient_of_restitution(
                                           mat_info.m_stone_mid
                                           , mat_info.m_stone_mid
                                           , 0.7f
                                           );

    //--- Concrete to steel = 0.45
    engine->create_material_property(
                                     mat_info.m_cannonball_mid
                                     , mat_info.m_ground_mid
                                     );

    engine->set_coefficient_of_isotropic_friction(
                                                  mat_info.m_cannonball_mid
                                                  , mat_info.m_ground_mid
                                                  , 0.45f
                                                  );

    engine->set_coefficients_of_anisotropic_friction(
                                                     mat_info.m_cannonball_mid
                                                     , mat_info.m_ground_mid
                                                     , 0.45f
                                                     , 0.45f
                                                     , drilling_dummy
                                                     );

    engine->set_coefficient_of_restitution(
                                           mat_info.m_cannonball_mid
                                           , mat_info.m_ground_mid
                                           , 0.3f
                                           );

    //--- Steel to steel = 0.3
    engine->create_material_property(
                                     mat_info.m_cannonball_mid
                                     , mat_info.m_cannonball_mid
                                     );

    engine->set_coefficient_of_isotropic_friction(
                                                  mat_info.m_cannonball_mid
                                                  , mat_info.m_cannonball_mid
                                                  , 0.3f
                                                  );

    engine->set_coefficients_of_anisotropic_friction(
                                                     mat_info.m_cannonball_mid
                                                     , mat_info.m_cannonball_mid
                                                     , 0.3f
                                                     , 0.3f
                                                     , drilling_dummy
                                                     );

    engine->set_coefficient_of_restitution(
                                           mat_info.m_cannonball_mid
                                           , mat_info.m_cannonball_mid
                                           , 0.9f
                                           );

    return mat_info;
  }
  
  typedef tiny::MathTypes<float> MTf;

  template
  MaterialInfo<MTf::real_type> create_material_info<MTf>( content::API * engine );

} //namespace procedural
