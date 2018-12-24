#ifndef PROX_GUI_DRAW_SHADOWMAPS_H
#define PROX_GUI_DRAW_SHADOWMAPS_H

#include <prox_gui_scene_manager.h>
#include <prox_gui_geometry_manager.h>
#include <prox_gui_shadowmap_manager.h>

#include <gl3_frame_buffer_object.h>
#include <gl3_shader_program_manager.h>
#include <gl3_vbo.h>
#include <gl3_glm.h>

namespace prox_gui
{
  
  inline void init_shadowmaps(
                              std::vector<LightInfo> const & lights
                              , ShadowmapManager             & shadow_manager
                              )
  {
    shadow_manager.clear();
    
    std::vector<LightInfo>::const_iterator L = lights.begin();
    for (; L!=lights.end(); ++L)
    {
      ShadowmapObject shadow;
      
      shadow.m_light_source       = *L;
      
      shadow.m_depth_texture      =  gl3::Texture2D(
                                                    GL_DEPTH_COMPONENT16
                                                    , 1024
                                                    , 1024
                                                    , GL_DEPTH_COMPONENT
                                                    , GL_FLOAT
                                                    , 0
                                                    );
      
      shadow.m_depth_texture.set_repeating(false);
      shadow.m_depth_texture.set_nearest_filtering();
      
      // Compute the MVP matrix from the light's point of view
      glm::vec3 const light_position  =  glm::vec3(L->m_position_x, L->m_position_y, L->m_position_z);
      glm::vec3 const light_target    =  glm::vec3(L->m_target_x, L->m_target_y, L->m_target_z);
      glm::vec3 const light_up_guess  =  glm::vec3(L->m_up_x, L->m_up_y, L->m_up_z);
      glm::vec3 const light_direction =  glm::normalize(light_target - light_position);
      glm::vec3 const light_right     =  glm::normalize(glm::cross(light_direction,light_up_guess));
      glm::vec3 const light_up        =  glm::normalize(glm::cross(light_right,light_direction));;
      
      float const fov    = L->m_cutoff_angle*2.0;
      float const aspect = 1.0;
      float const z_near  = L->m_z_near;
      float const z_far   = L->m_z_far;
      
      shadow.m_projection_matrix = glm::perspective<float>(glm::radians(fov), aspect, z_near, z_far);
      shadow.m_view_matrix       = glm::lookAt(light_position, light_target, light_up);
      
      shadow_manager.add(shadow);
    }
  }
  
  
  
  inline void draw_shadowmaps(
                              gl3::ShaderProgramManager & program_manager
                              , SceneManager const & scene_manager
                              , GeometryManager const & geometry_manager
                              , ShadowmapManager const & shadow_manager
                              )
  {
    gl3::check_errors("draw_shadowmaps() invoked");
    
    gl3::Program & program = program_manager.get(4);
    
    gl3::FrameBufferObject fbo;
    fbo.create();
    
    GLint viewport_params[4];
    glGetIntegerv(GL_VIEWPORT, &viewport_params[0]);
    
    GLboolean cull_enabled_param = glIsEnabled(GL_CULL_FACE);
    
    GLint cull_mode_param;
    glGetIntegerv(GL_CULL_FACE_MODE, &cull_mode_param);
    
    program.use();
    
    std::vector<ShadowmapObject>::const_iterator shadow = shadow_manager.m_objects.begin();
    
    for (; shadow!=shadow_manager.m_objects.end(); ++shadow)
    {
      fbo.attach_texture( GL_DEPTH_ATTACHMENT, shadow->m_depth_texture );
      fbo.set_no_color_buffer();
      fbo.is_valid();
      fbo.bind();
      
      GLfloat depth_value = 1.0;
      glClearBufferfv( GL_DEPTH, 0, &depth_value );
      
      glEnable(GL_CULL_FACE);
      glCullFace(GL_FRONT);
      
      glViewport(0,0,shadow->m_depth_texture.width(),shadow->m_depth_texture.height());
      
      program.set_uniform("projection_matrix", shadow->m_projection_matrix );
      
      std::vector<SceneObject>::const_iterator object = scene_manager.m_objects.begin();
      std::vector<SceneObject>::const_iterator end    = scene_manager.m_objects.end();
      
      for(;object!=end;++object)
      {
        Geometry           const & geometry = geometry_manager.get( object->m_gid );
        
        glm::mat4 const model_view_matrix =  shadow->m_view_matrix * object->m_model_matrix;
        program.set_uniform( "model_view_matrix", model_view_matrix);
        
        geometry.m_shadow_vao.bind();
        geometry.m_vbo.draw();
        geometry.m_shadow_vao.unbind();
      }
      fbo.unattach(GL_DEPTH_ATTACHMENT);
    }
    program.stop();
    fbo.disable();
    fbo.clear();
    
    glViewport(viewport_params[0], viewport_params[1], viewport_params[2], viewport_params[3]);
    
    if( cull_enabled_param)
    {
      glEnable(GL_CULL_FACE);
      glCullFace(cull_mode_param);
    }
    else
    {
      glDisable(GL_CULL_FACE);
    }
    gl3::check_errors("draw_shadowmaps() exit");
  }
  
}//namespace prox_gui

// PROX_GUI_DRAW_SHADOWMAPS_H
#endif
