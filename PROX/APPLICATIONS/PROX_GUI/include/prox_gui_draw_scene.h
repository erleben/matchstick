#ifndef PROX_GUI_DRAW_SCENE_H
#define PROX_GUI_DRAW_SCENE_H

#include <prox_gui_lighting.h>
#include <prox_gui_materials.h>
#include <prox_gui_scene_manager.h>
#include <prox_gui_geometry_manager.h>
#include <prox_gui_shadowmap_manager.h>

#include <gl3_shader_program_manager.h>
#include <gl3_vbo.h>
#include <gl3_glm.h>

namespace prox_gui
{
  
  inline void draw_scene(
                         bool const & draw_wireframe
                         , glm::mat4 const & projection_matrix
                         , glm::mat4 const & view_matrix
                         , std::vector<LightInfo> & lights
                         , std::vector<MaterialInfo> & materials
                         , gl3::ShaderProgramManager & program_manager
                         , SceneManager const & scene_manager
                         , GeometryManager const & geometry_manager
                         , ShadowmapManager const & shadowmap_manager
                         )
  {
    gl3::check_errors("draw_scene() invoked");
    
    glm::mat4 const shadow_map_bias_matrix(
                                           0.5, 0.0, 0.0, 0.0,
                                           0.0, 0.5, 0.0, 0.0,
                                           0.0, 0.0, 0.5, 0.0,
                                           0.5, 0.5, 0.5, 1.0
                                           );
    
    gl3::Program & solid_program = program_manager.get(0);
    gl3::Program & wire_program  = program_manager.get(1);
    
    gl3::Program & program = draw_wireframe ? wire_program : solid_program;
    
    program.use();
    
    program.set_uniform("projection_matrix", projection_matrix );
    program.set_uniform("view_matrix", view_matrix );
    
    if(draw_wireframe)
      program.set_uniform("wire_color", 0.9f, 0.1f, 0.1f );  // 2014-10-6 Kenny: Consider having this as hardwired constant?
    
    set_lighting_uniforms(program, lights);
    
    int const number_of_shadows = shadowmap_manager.m_objects.size();
    if(!draw_wireframe)
    {
      for(int i=0u; i < number_of_shadows; ++i)
      {
        ShadowmapObject const & shadow = shadowmap_manager.m_objects[i];
        glActiveTexture( GL_TEXTURE1+i );                                     // Active texture unit 1+1
        gl3::check_errors("glActiveTexture( GL_TEXTURE1 + i );");
        shadow.m_depth_texture.bind();                                        // Bind this texture to texture unit 1+i
        program.set_uniform( "light_depth_map" + util::to_string(i), 1 + i);  // shadow sampler should use texture unit 1+i
      }
    }
    
    std::vector<SceneObject>::const_iterator object = scene_manager.m_objects.begin();
    std::vector<SceneObject>::const_iterator end    = scene_manager.m_objects.end();
    
    for(;object!=end;++object)
    {
      Geometry           const & geometry = geometry_manager.get( object->m_gid );

      set_material_uniforms(program, materials[object->m_mid]);
      
      glm::mat4 const model_view_matrix =  view_matrix * object->m_model_matrix;
      
      //if(!draw_wireframe)
      {
        for(int i=0u; i < number_of_shadows; ++i)
        {
          ShadowmapObject const & shadow = shadowmap_manager.m_objects[i];
          glm::mat4 light_matrix = shadow_map_bias_matrix * shadow.m_projection_matrix * shadow.m_view_matrix * object->m_model_matrix;
          program.set_uniform( "light_matrix[" + util::to_string(i) + "]", light_matrix);
        }
      }
      
      program.set_uniform( "model_view_matrix", model_view_matrix);
      
      gl3::VAO const & vao =  draw_wireframe ?  geometry.m_wire_vao : geometry.m_solid_vao;
      
      vao.bind();
      geometry.m_vbo.draw();
      vao.unbind();
      
    }
    
    for(int i=0u; i < number_of_shadows; ++i)
    {
      ShadowmapObject const & shadow = shadowmap_manager.m_objects[i];
      shadow.m_depth_texture.unbind();
    }
    
    program.stop();
  }
  
}//namespace prox_gui

// PROX_GUI_DRAW_SCENE_H
#endif
