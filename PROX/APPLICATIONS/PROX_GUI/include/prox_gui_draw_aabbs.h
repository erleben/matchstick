#ifndef PROX_GUI_DRAW_AABBS_H
#define PROX_GUI_DRAW_AABBS_H

#include <prox_gui_lighting.h>
#include <prox_gui_materials.h>
#include <prox_gui_geometry.h>
#include <prox_gui_scene_manager.h>

#include <gl3_shader_program_manager.h>
#include <gl3_glm.h>

#include <content.h>
#include <tiny.h>

#include <cmath> // std::acos, std::fabs

namespace prox_gui
{
  
  inline void make_aabb_geometry(
                                 gl3::ShaderProgramManager & program_manager
                                 , GeometryManager & geometry_manager
                                 )
  {
    gl3::Program & program = program_manager.get(3);
    
    Geometry geometry;
    
    geometry.m_vbo        = gl3::make_unit_aabb_vbo();
    geometry.m_solid_vao  = gl3::make_vao(geometry.m_vbo, program, "position", "normal");
    
    geometry_manager.add("aabb", geometry);
  }
  
  inline void draw_aabbs(
                         content::API * engine
                         , util::ConfigFile const & params
                         , glm::mat4 const & projection_matrix
                         , glm::mat4 const & view_matrix
                         , std::vector<LightInfo>  & lights
                         , std::vector<MaterialInfo> & materials
                         , gl3::ShaderProgramManager  & program_manager
                         , GeometryManager  & geometry_manager
                         , SceneManager  & scene_manager
                         )
  {
    using std::acos;
    using std::fabs;
    
    gl3::check_errors("draw_aabbs() invoked");
    
    gl3::Program & program = program_manager.get(3);
    
    program.use();
    
    program.set_uniform("projection_matrix", projection_matrix );
    
    Geometry const & geometry = geometry_manager.get("aabb");
    
    std::vector<SceneObject>::const_iterator object = scene_manager.m_objects.begin();
    std::vector<SceneObject>::const_iterator end    = scene_manager.m_objects.end();
    
    for(;object!=end;++object)
    {
      float x,y,z;
      
      engine->get_rigid_body_position( object->m_rid, x, y, z );
      
      glm::mat4 const T = glm::translate(glm::mat4(1.0), glm::vec3( x, y, z) );
      
      glm::mat4 const model_view_matrix =  view_matrix * T;
      
      program.set_uniform( "scale", object->m_aabb_width, object->m_aabb_height, object->m_aabb_depth );
      
      program.set_uniform( "color", object->m_red, object->m_green, object->m_blue );
      program.set_uniform( "model_view_matrix", model_view_matrix);
      
      geometry.m_solid_vao.bind();
      geometry.m_vbo.draw();
      geometry.m_solid_vao.unbind();
    }
    
    program.stop();
  }
  
}//namespace prox_gui

// PROX_GUI_DRAW_AABBS_H
#endif
