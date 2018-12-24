#ifndef PROX_GUI_LIGHTING_H
#define PROX_GUI_LIGHTING_H

#include <gl3_shader_program_manager.h>

#include <util_config_file.h>

namespace prox_gui
{
  
  class LightInfo
  {
  public:
    
    
    float m_position_x;
    float m_position_y;
    float m_position_z;
    
    float m_target_x;
    float m_target_y;
    float m_target_z;
    
    float m_up_x;
    float m_up_y;
    float m_up_z;
    
    float m_cutoff_angle;
    float m_attenuation;
    float m_z_near;
    float m_z_far;
    
    float m_Ia_red;
    float m_Ia_green;
    float m_Ia_blue;
    
    float m_Id_red;
    float m_Id_green;
    float m_Id_blue;
    
    float m_Is_red;
    float m_Is_green;
    float m_Is_blue;
    
  public:
    
    LightInfo()
    : m_position_x(0.0f)
    , m_position_y(100.0f)
    , m_position_z(0.0f)
    , m_target_x(0.0f)
    , m_target_y(0.0f)
    , m_target_z(0.0f)
    , m_up_x(0.0f)
    , m_up_y(1.0f)
    , m_up_z(0.0f)
    , m_cutoff_angle(45.0f)
    , m_attenuation(0.01f)
    , m_z_near(0.01f)
    , m_z_far(100.0f)
    , m_Ia_red(0.2f)
    , m_Ia_green(0.2f)
    , m_Ia_blue(0.2f)
    , m_Id_red(0.7f)
    , m_Id_green(0.7f)
    , m_Id_blue(0.7f)
    , m_Is_red(0.5f)
    , m_Is_green(0.5f)
    , m_Is_blue(0.5f)
    {}
  };
  
  inline void make_default_lighting(std::vector<LightInfo> & lights)
  {
    LightInfo L0; // Global lamp -- sun
    
    L0.m_position_x =   0.0f;
    L0.m_position_y = 100.0f;
    L0.m_position_z =   0.0f;
    L0.m_Is_red     =   1.0f;
    L0.m_Is_green   =   1.0f;
    L0.m_Is_blue    =   1.0f;
    L0.m_Id_red     =   0.7f;
    L0.m_Id_green   =   0.7f;
    L0.m_Id_blue    =   0.7f;
    L0.m_Ia_red     =   0.2f;
    L0.m_Ia_green   =   0.2f;
    L0.m_Ia_blue    =   0.2f;
    
    LightInfo L1; // Spot light
    
    L1.m_position_x =   0.0f;
    L1.m_position_y =   0.0f;
    L1.m_position_z =  10.0f;
    L1.m_Is_red     =   0.3f;
    L1.m_Is_green   =   0.3f;
    L1.m_Is_blue    =   0.3f;
    L1.m_Id_red     =   0.3f;
    L1.m_Id_green   =   0.3f;
    L1.m_Id_blue    =   0.3f;
    L1.m_Ia_red     =   0.0f;
    L1.m_Ia_green   =   0.0f;
    L1.m_Ia_blue    =   0.0f;
    
    LightInfo L2; // hitter
    
    L2.m_position_x =  -2.0f;
    L2.m_position_y =  -1.0f;
    L2.m_position_z =   8.0f;
    L2.m_Is_red     =   0.5f;
    L2.m_Is_green   =   0.5f;
    L2.m_Is_blue    =   0.5f;
    L2.m_Id_red     =   0.7f;
    L2.m_Id_green   =   0.7f;
    L2.m_Id_blue    =   0.7f;
    L2.m_Ia_red     =   0.0f;
    L2.m_Ia_green   =   0.0f;
    L2.m_Ia_blue    =   0.0f;
    
    LightInfo L3; // back light
    
    L3.m_position_x = -10.0f;
    L3.m_position_y = -10.0f;
    L3.m_position_z = -10.0f;
    L3.m_Is_red     =   0.5f;
    L3.m_Is_green   =   0.5f;
    L3.m_Is_blue    =   0.5f;
    L3.m_Id_red     =   0.7f;
    L3.m_Id_green   =   0.7f;
    L3.m_Id_blue    =   0.7f;
    L3.m_Ia_red     =   0.0f;
    L3.m_Ia_green   =   0.0f;
    L3.m_Ia_blue    =   0.0f;
    
    lights.clear();
    lights.push_back(L0);
    lights.push_back(L1);
    lights.push_back(L2);
    lights.push_back(L3);
  }
  
  inline LightInfo make_light_from_config_file(
                                               std::string const & light_name
                                               , util::ConfigFile const & params
                                               )
  {
    LightInfo light;
    
    light.m_position_x  = util::to_value<float>( params.get_value( light_name + "_position_x",   "0.0" ) );
    light.m_position_y  = util::to_value<float>( params.get_value( light_name + "_position_y", "100.0" ) );
    light.m_position_z  = util::to_value<float>( params.get_value( light_name + "_position_z",   "0.0" ) );
    
    light.m_target_x  = util::to_value<float>( params.get_value( light_name + "_target_x",   "0.0" ) );
    light.m_target_y  = util::to_value<float>( params.get_value( light_name + "_target_y",   "0.0" ) );
    light.m_target_z  = util::to_value<float>( params.get_value( light_name + "_target_z",   "0.0" ) );
    
    light.m_up_x  = util::to_value<float>( params.get_value( light_name + "_up_x",   "0.0" ) );
    light.m_up_y  = util::to_value<float>( params.get_value( light_name + "_up_y",   "1.0" ) );
    light.m_up_z  = util::to_value<float>( params.get_value( light_name + "_up_z",   "0.0" ) );
    
    light.m_cutoff_angle = util::to_value<float>( params.get_value( light_name + "_cutoff_angle"  ,   "45.0"  ) );
    light.m_attenuation  = util::to_value<float>( params.get_value( light_name + "_attenuation"   ,   "0.01"  ) );
    light.m_z_near       = util::to_value<float>( params.get_value( light_name + "_z_near"        ,   "0.01"  ) );
    light.m_z_far        = util::to_value<float>( params.get_value( light_name + "_z_far"         ,   "100.0" ) );
    
    light.m_Ia_red      = util::to_value<float>( params.get_value( light_name + "_Ia_red"    ,   "0.2" ) );
    light.m_Ia_green    = util::to_value<float>( params.get_value( light_name + "_Ia_green"  ,   "0.2" ) );
    light.m_Ia_blue     = util::to_value<float>( params.get_value( light_name + "_Ia_blue"   ,   "0.2" ) );
    light.m_Id_red      = util::to_value<float>( params.get_value( light_name + "_Id_red"    ,   "0.7" ) );
    light.m_Id_green    = util::to_value<float>( params.get_value( light_name + "_Id_green"  ,   "0.7" ) );
    light.m_Id_blue     = util::to_value<float>( params.get_value( light_name + "_Id_blue"   ,   "0.7" ) );
    light.m_Is_red      = util::to_value<float>( params.get_value( light_name + "_Is_red"    ,   "0.5" ) );
    light.m_Is_green    = util::to_value<float>( params.get_value( light_name + "_Is_green"  ,   "0.5" ) );
    light.m_Is_blue     = util::to_value<float>( params.get_value( light_name + "_Is_blue"   ,   "0.5" ) );
    
    return light;
  }
  
  inline void make_lighting_from_config_file(
                                             util::ConfigFile const & params
                                             , std::vector<LightInfo> & lights
                                             )
  {
    lights.clear();
    
    if( util::to_value<bool>( params.get_value("light0","false") ) )
      lights.push_back( make_light_from_config_file("light0", params) );
    
    if( util::to_value<bool>( params.get_value("light1","false") ) )
      lights.push_back( make_light_from_config_file("light1", params) );
    
    if( util::to_value<bool>( params.get_value("light2","false") ) )
      lights.push_back( make_light_from_config_file("light2", params) );
    
    if( util::to_value<bool>( params.get_value("light3","false") ) )
      lights.push_back( make_light_from_config_file("light3", params) );
  }
  
  inline void set_lighting_uniforms( gl3::Program & program, std::vector<LightInfo> & lights)
  {
    int const L = lights.size();
    
    program.set_uniform("number_of_lights",  L);
    
    for (unsigned int i =0; i < lights.size(); ++i)
    {
      program.set_uniform("lights[" +  util::to_string(i) +  "].position"
                          , lights[i].m_position_x
                          , lights[i].m_position_y
                          , lights[i].m_position_z
                          );
      
      program.set_uniform("lights[" +  util::to_string(i) +  "].target"
                          , lights[i].m_target_x
                          , lights[i].m_target_y
                          , lights[i].m_target_z
                          );
      
      program.set_uniform("lights[" +  util::to_string(i) +  "].cutoff_angle"
                          , lights[i].m_cutoff_angle
                          );
      
      program.set_uniform("lights[" +  util::to_string(i) +  "].attenuation"
                          , lights[i].m_attenuation
                          );
      
      program.set_uniform("lights[" +  util::to_string(i) +  "].Ia"
                          , lights[i].m_Ia_red
                          , lights[i].m_Ia_green
                          , lights[i].m_Ia_blue
                          );
      
      program.set_uniform("lights[" +  util::to_string(i) +  "].Id"
                          , lights[i].m_Id_red
                          , lights[i].m_Id_green
                          , lights[i].m_Id_blue
                          );
      
      program.set_uniform("lights[" +  util::to_string(i) +  "].Is"
                          , lights[i].m_Is_red
                          , lights[i].m_Is_green
                          , lights[i].m_Is_blue
                          );
    }
    
  }
  
}//namespace prox_gui

// PROX_GUI_LIGHTING_H
#endif
