#ifndef PROX_GUI_MATERIALS_H
#define PROX_GUI_MATERIALS_H

#include <util_config_file.h>

#include <gl3_shader_program_manager.h>

namespace prox_gui
{
  
  class MaterialInfo
  {
  public:
    
    float m_specular_exponent;
    
    float m_Ka_red;
    float m_Ka_green;
    float m_Ka_blue;
    
    float m_Kd_red;
    float m_Kd_green;
    float m_Kd_blue;
    
    float m_Ks_red;
    float m_Ks_green;
    float m_Ks_blue;

    float m_alpha;
    
  public:
    
    MaterialInfo()
    : m_specular_exponent(100.0f)
    , m_Ka_red(1.0f)
    , m_Ka_green(1.0f)
    , m_Ka_blue(1.0f)
    , m_Kd_red(1.0f)
    , m_Kd_green(1.0f)
    , m_Kd_blue(1.0f)
    , m_Ks_red(1.0f)
    , m_Ks_green(1.0f)
    , m_Ks_blue(1.0f)
    , m_alpha(1.0f)
    {}
  };
  
  
  inline MaterialInfo make_material_from_config_file(
                                                     std::string const & material_name
                                                     , util::ConfigFile const & params
                                                     )
  {
    MaterialInfo material;
    
    material.m_specular_exponent  = util::to_value<float>( params.get_value( material_name + "_specular_exponent",   "100.0" ) );
    material.m_Ka_red       = util::to_value<float>( params.get_value( material_name + "_Ka_red"    ,   "1.0" ) );
    material.m_Ka_green     = util::to_value<float>( params.get_value( material_name + "_Ka_green"  ,   "1.0" ) );
    material.m_Ka_blue      = util::to_value<float>( params.get_value( material_name + "_Ka_blue"   ,   "1.0" ) );
    material.m_Kd_red       = util::to_value<float>( params.get_value( material_name + "_Kd_red"    ,   "1.0" ) );
    material.m_Kd_green     = util::to_value<float>( params.get_value( material_name + "_Kd_green"  ,   "1.0" ) );
    material.m_Kd_blue      = util::to_value<float>( params.get_value( material_name + "_Kd_blue"   ,   "1.0" ) );
    material.m_Ks_red       = util::to_value<float>( params.get_value( material_name + "_Ks_red"    ,   "1.0" ) );
    material.m_Ks_green     = util::to_value<float>( params.get_value( material_name + "_Ks_green"  ,   "1.0" ) );
    material.m_Ks_blue      = util::to_value<float>( params.get_value( material_name + "_Ks_blue"   ,   "1.0" ) );
    material.m_alpha        = util::to_value<float>( params.get_value( material_name + "_alpha"     ,   "1.0" ) );
    
    return material;
  }
  
  
  inline void set_material_uniforms( gl3::Program & program, MaterialInfo const & material)
  {
    program.set_uniform("material.Ka"
                        , material.m_Ka_red
                        , material.m_Ka_green
                        , material.m_Ka_blue
                        );
    
    program.set_uniform("material.Kd"
                        , material.m_Kd_red
                        , material.m_Kd_green
                        , material.m_Kd_blue
                        );
    
    program.set_uniform("material.Ks"
                        , material.m_Ks_red
                        , material.m_Ks_green
                        , material.m_Ks_blue
                        );
    
    program.set_uniform("material.specular_exponent"
                        , material.m_specular_exponent
                        );
        
    program.set_uniform("material.alpha"
                        , material.m_alpha
                        );
  }
  
}//namespace prox_gui

// PROX_GUI_MATERIALS_H
#endif
