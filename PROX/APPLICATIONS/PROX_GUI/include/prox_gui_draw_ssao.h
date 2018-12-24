#ifndef PROX_GUI_DRAW_SSAO_H
#define PROX_GUI_DRAW_SSAO_H

#include <prox_gui_lighting.h>
#include <prox_gui_materials.h>
#include <prox_gui_scene_manager.h>
#include <prox_gui_geometry_manager.h>
#include <prox_gui_shadowmap_manager.h>

#include <gl3_frame_buffer_object.h>
#include <gl3_program.h>
#include <gl3_shader_program_manager.h>
#include <gl3_vao.h>
#include <gl3_vbo.h>
#include <gl3_glm.h>

#include <random> // necessary for generation of random floats (for sample kernel and noise texture)

namespace prox_gui
{
  
  GLfloat lerp(GLfloat a, GLfloat b, GLfloat f)
  {
    return a + f * (b - a);
  }
  
  inline void geometry_draw_scene(
                                  glm::mat4 const & projection_matrix
                                  , glm::mat4 const & view_matrix
                                  , float const & z_near
                                  , float const & z_far
                                  ,  gl3::Program & program
                                  , SceneManager const & scene_manager
                                  , GeometryManager const & geometry_manager
                                  , std::vector<MaterialInfo> & materials
                                  )
  {
    program.set_uniform("z_near", z_near );
    program.set_uniform("z_far",  z_far  );
    program.set_uniform("projection", projection_matrix );
    program.set_uniform( "view", view_matrix);
    
    std::vector<SceneObject>::const_iterator object = scene_manager.m_objects.begin();
    std::vector<SceneObject>::const_iterator end    = scene_manager.m_objects.end();
    
    for(;object!=end;++object)
    {
      program.set_uniform( "model", object->m_model_matrix);

      set_material_uniforms(program, materials[object->m_mid]);
      
      Geometry           const & geometry = geometry_manager.get( object->m_gid );
      
      gl3::VBO const & vbo = geometry.m_vbo;
      gl3::VAO const & vao = gl3::make_vao( vbo, program, "position", "normal" );
      
      vao.bind();
      vbo.draw();
      vao.unbind();
    }
    
  }
  
  class SSAOBuffer
  {
  public:
    
    int         m_kernel_size;
    int         m_blur_size;
    float       m_radius;
    float       m_noise_scale_x;
    float       m_noise_scale_y;
    int         m_width;             // screen width in pixels
    int         m_height;            // screen height in pixels
    
    float       m_clear_color_red;
    float       m_clear_color_green;
    float       m_clear_color_blue;
    float       m_clear_color_alpha;
    
    gl3::Texture2D      m_position;
    gl3::Texture2D      m_depth;
    gl3::Texture2D      m_normal;
    gl3::Texture2D      m_diffuse;
    
    std::vector<glm::vec3>  m_kernel;
    gl3::Texture2D      m_noise;
    gl3::Texture2D      m_occlusion;
    gl3::Texture2D      m_blurred_occlusion;
  };
  
  inline void init_ssao_buffer(  SSAOBuffer & buffer )
  {
    int const screen_width  = buffer.m_width;
    int const screen_height = buffer.m_height;
    
    //---Deffered FBO Color 0 attachment ---------------------------------------
    buffer.m_position      =  gl3::Texture2D(
                                             GL_RGBA16F
                                             , screen_width
                                             , screen_height
                                             , GL_RGBA
                                             , GL_FLOAT
                                             , 0
                                             );
    
    buffer.m_position.set_repeating(false);
    buffer.m_position.set_nearest_filtering();
    
    //---Deffered FBO Color 1 attachment ---------------------------------------
    buffer.m_normal      =  gl3::Texture2D(
                                           GL_RGB16F
                                           , screen_width
                                           , screen_height
                                           , GL_RGBA
                                           , GL_FLOAT
                                           , 0
                                           );
    
    buffer.m_normal.set_repeating(false);
    buffer.m_normal.set_nearest_filtering();
    
    //---Deffered FBO  Color 2 attachment --------------------------------------
    buffer.m_diffuse      =  gl3::Texture2D(
                                            GL_RGBA
                                            , screen_width
                                            , screen_height
                                            , GL_RGBA
                                            , GL_FLOAT
                                            , 0
                                            );
    
    buffer.m_diffuse.set_repeating(false);
    buffer.m_diffuse.set_nearest_filtering();
    
    //---Deffered FBO depth attachment -----------------------------------------
    buffer.m_depth      =  gl3::Texture2D(
                                          GL_DEPTH_COMPONENT16
                                          , screen_width
                                          , screen_height
                                          , GL_DEPTH_COMPONENT
                                          , GL_FLOAT
                                          , 0
                                          );
    
    buffer.m_depth.set_repeating(false);
    buffer.m_depth.set_nearest_filtering();
    
    
    //--- SSAO FBO color attachment 0 ------------------------------------------
    buffer.m_occlusion      =  gl3::Texture2D(
                                              GL_RED
                                              , screen_width
                                              , screen_height
                                              , GL_RED
                                              , GL_FLOAT
                                              , 0
                                              );
    
    buffer.m_occlusion.set_repeating(false);
    buffer.m_occlusion.set_nearest_filtering();
    
    //--- SSAO BLUR FBO  color attachment 0 -----------------------------------
    buffer.m_blurred_occlusion   =  gl3::Texture2D(
                                                   GL_RED
                                                   , screen_width
                                                   , screen_height
                                                   , GL_RED
                                                   , GL_FLOAT
                                                   , 0
                                                   );
    
    buffer.m_blurred_occlusion.set_repeating(false);
    buffer.m_blurred_occlusion.set_nearest_filtering();
    
    //--- Make ssao kernel -----------------------------------------------------
    std::uniform_real_distribution<GLfloat> random_floats(0.0, 1.0); // generates random floats between 0.0 and 1.0
    std::default_random_engine generator;
    
    buffer.m_kernel.clear();
    
    for (GLuint i = 0; i < buffer.m_kernel_size; ++i)
    {
      glm::vec3 sample(
                       random_floats(generator) * 2.0 - 1.0
                       , random_floats(generator) * 2.0 - 1.0
                       , random_floats(generator)
                       );
      
      sample = glm::normalize(sample);
      sample *= random_floats(generator);
      
      GLfloat scale = GLfloat(i) / buffer.m_kernel_size;
      
      // Scale samples s.t. they're more aligned to center of kernel
      scale = lerp(0.1f, 1.0f, scale * scale);
      sample *= scale;
      
      buffer.m_kernel.push_back(sample);
    }
    
    //--- Make ssao noise  -----------------------------------------------------
    std::vector<glm::vec3> noise_values;
    
    
    for (GLuint i = 0; i < buffer.m_blur_size*buffer.m_blur_size; ++i)
    {
      //---- rotate around z-axis (in tangent space)
      glm::vec3 noise(
                      random_floats(generator) * 2.0 - 1.0
                      , random_floats(generator) * 2.0 - 1.0
                      , 0.0f
                      );
      
      noise_values.push_back(noise);
    }
    
    //--- Make ssao noise texture ----------------------------------------------
    buffer.m_noise   =  gl3::Texture2D(
                                       GL_RGB16F
                                       , buffer.m_blur_size
                                       , buffer.m_blur_size
                                       , GL_RGB
                                       , GL_FLOAT
                                       , &noise_values[0]
                                       );
    
    buffer.m_noise.set_repeating(true);
    buffer.m_noise.set_nearest_filtering();
    
  }
  
  inline void draw_ssao(
                        std::string const & shader_path
                        , glm::mat4 const & projection_matrix
                        , glm::mat4 const & view_matrix
                        , float const & z_near
                        , float const & z_far
                        , SceneManager const & scene_manager
                        , GeometryManager const & geometry_manager
                        , std::vector<LightInfo> & lights
                        , std::vector<MaterialInfo> & materials
                        , ShadowmapManager const & shadowmap_manager
                        , SSAOBuffer & buffer
                        )
  {
    gl3::check_errors("draw_ssao() invoked");
    
    //--- Remember all openGL context that we will change so we can ------------
    //--- restore it later -----------------------------------------------------
    GLint viewport_params[4];
    glGetIntegerv(GL_VIEWPORT, &viewport_params[0]);
    
    GLboolean cull_param = glIsEnabled(GL_CULL_FACE);
    GLboolean depth_param = glIsEnabled(GL_DEPTH_TEST);
    GLboolean blend_param = glIsEnabled(GL_BLEND);
    
    GLint cull_mode_param;
    glGetIntegerv(GL_CULL_FACE_MODE, &cull_mode_param);
    
    GLint depth_func_param;
    glGetIntegerv(GL_DEPTH_FUNC, &depth_func_param);
    
    //--- First render pass ----------------------------------------------------
    {
      std::string const vertex_shader_filename   = shader_path + "/ssao/geometry_vertex.glsl";
      std::string const fragment_shader_filename = shader_path + "/ssao/geometry_fragment.glsl";
      
      gl3::Shader  vs = gl3::make_shader_from_file(vertex_shader_filename, gl3::Shader::vertex_shader);
      gl3::Shader  fs = gl3::make_shader_from_file(fragment_shader_filename, gl3::Shader::fragment_shader);
      
      gl3::Program program = gl3::make_program(vs,fs);
      
      program.use();
      
      // Setup openGL context
      {
        glViewport(0, 0, buffer.m_position.width(), buffer.m_position.height());
        glDisable(GL_BLEND);
        glEnable( GL_DEPTH_TEST );
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
      }
      
      gl3::FrameBufferObject fbo;
      // Setup FBO
      {
        fbo.create();
        fbo.attach_texture( GL_COLOR_ATTACHMENT0, buffer.m_position );
        fbo.attach_texture( GL_COLOR_ATTACHMENT1, buffer.m_normal   );
        fbo.attach_texture( GL_COLOR_ATTACHMENT2, buffer.m_diffuse  );
        fbo.attach_texture( GL_DEPTH_ATTACHMENT,  buffer.m_depth    );
        fbo.set_draw_buffers(GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2);
        fbo.bind();
        fbo.is_valid();
      }
      
      GLfloat position_values[4] = {0.0f, 0.0f, 0.0f, 1.0f};
      glClearBufferfv( GL_COLOR, 0, &position_values[0] );
      
      GLfloat normal_values[4] = {0.0f, 0.0f, 0.0f, 1.0f};
      glClearBufferfv( GL_COLOR, 1, &normal_values[0] );
      
      GLfloat diffuse_values[4] = {
        buffer.m_clear_color_red
        , buffer.m_clear_color_green
        , buffer.m_clear_color_blue
        , buffer.m_clear_color_alpha
      };
      
      glClearBufferfv( GL_COLOR, 2, &diffuse_values[0] );
      
      GLfloat depth_value = 1.0;
      glClearBufferfv( GL_DEPTH, 0, &depth_value );
      
      geometry_draw_scene(
                          projection_matrix
                          , view_matrix
                          , z_near
                          , z_far
                          , program
                          , scene_manager
                          , geometry_manager
                          , materials
                          );
      
      fbo.disable();
      fbo.clear();
      program.stop();
      program.clear();
    }
    
    //--- Second render pass ---------------------------------------------------
    {
      std::string const vertex_shader_filename   = shader_path + "/ssao/ssao_vertex.glsl";
      std::string const fragment_shader_filename = shader_path + "/ssao/ssao_fragment.glsl";
      
      gl3::Shader  vs = gl3::make_shader_from_file(vertex_shader_filename, gl3::Shader::vertex_shader);
      gl3::Shader  fs = gl3::make_shader_from_file(fragment_shader_filename, gl3::Shader::fragment_shader);
      
      gl3::Program program = gl3::make_program(vs,fs);
      
      program.use();
      
      // Setup opengl context
      {
        glViewport(0, 0, buffer.m_depth.width(), buffer.m_depth.height());
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);
      }
      
      gl3::FrameBufferObject fbo;
      // Setup FBO
      {
        fbo.create();
        fbo.attach_texture( GL_COLOR_ATTACHMENT0, buffer.m_occlusion );
        fbo.set_draw_buffer( GL_COLOR_ATTACHMENT0 );
        fbo.is_valid();
        fbo.bind();
      }
      
      // Setup textures and uniforms
      {
        glActiveTexture( GL_TEXTURE0 );                     // Active texture unit 0
        buffer.m_position.bind();                           // Bind this texture to texture unit 0
        program.set_uniform( "position_depth_map", 0 );     // Tell glsl to use texture unit 0
        
        glActiveTexture( GL_TEXTURE1 );                     // Active texture unit 1
        buffer.m_normal.bind();                             // Bind this texture to texture unit 1
        program.set_uniform( "normal_map", 1 );             // Tell glsl to use texture unit 1
        
        glActiveTexture( GL_TEXTURE2 );                     // Active texture unit 2
        buffer.m_noise.bind();                              // Bind this texture to texture unit 2
        program.set_uniform( "noise_map", 2 );              // Tell glsl to use texture unit 2
        
        program.set_uniform_array("samples", buffer.m_kernel_size, &buffer.m_kernel[0]);
        
        program.set_uniform("kernel_size", buffer.m_kernel_size);
        
        program.set_uniform("radius", buffer.m_radius);
        
        program.set_uniform("noise_scale", buffer.m_noise_scale_x, buffer.m_noise_scale_y);
      }
      
      GLint clear_values[4] = {0,0,0,0};
      
      glClearBufferiv( GL_COLOR, 0, &clear_values[0] );
      
      gl3::VBO     vbo     = gl3::make_quad_vbo();
      gl3::VAO     vao     = gl3::make_vao( vbo, program, "position");
      
      vao.bind();
      vbo.draw();
      vao.unbind();
      
      buffer.m_noise.unbind();
      buffer.m_normal.unbind();
      buffer.m_position.unbind();
      
      fbo.disable();
      fbo.clear();
      
      program.stop();
      program.clear();
    }
    
    //--- Third render pass ----------------------------------------------------
    {
      std::string const vertex_shader_filename   = shader_path + "/ssao/blur_vertex.glsl";
      std::string const fragment_shader_filename = shader_path + "/ssao/blur_fragment.glsl";
      
      gl3::Shader  vs = gl3::make_shader_from_file(vertex_shader_filename, gl3::Shader::vertex_shader);
      gl3::Shader  fs = gl3::make_shader_from_file(fragment_shader_filename, gl3::Shader::fragment_shader);
      
      gl3::Program program = gl3::make_program(vs,fs);
      
      program.use();
      
      // Setup openGL context
      {
        glViewport(0, 0, buffer.m_occlusion.width(), buffer.m_occlusion.height());
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);
      }
      
      gl3::FrameBufferObject fbo;
      
      // Setup FBO
      {
        fbo.create();
        fbo.attach_texture( GL_COLOR_ATTACHMENT0, buffer.m_blurred_occlusion );
        fbo.set_draw_buffer(GL_COLOR_ATTACHMENT0);
        fbo.is_valid();
        fbo.bind();
      }
      
      // Setup textures and uniforms
      {
        glActiveTexture( GL_TEXTURE0 );       // Active texture unit 0
        buffer.m_occlusion.bind();            // Bind this texture to texture unit 0
        program.set_uniform( "input_map", 0 );    // Tell glsl to use texture unit 0
        
        program.set_uniform("blur_size", buffer.m_blur_size);
        
      }
      
      gl3::VBO     vbo     = gl3::make_quad_vbo();
      gl3::VAO     vao     = gl3::make_vao( vbo, program, "position");
      
      GLint clear_values[4] = {0,0,0,0};
      glClearBufferiv( GL_COLOR, 0, &clear_values[0] );
      
      vao.bind();
      vbo.draw();
      vao.unbind();
      
      buffer.m_occlusion.unbind();
      
      fbo.disable();
      fbo.clear();
      
      program.stop();
      program.clear();
    }
    
    //--- Fourth render pass ---------------------------------------------------
    {
      std::string const vertex_shader_filename   = shader_path + "/ssao/lighting_vertex.glsl";
      std::string const fragment_shader_filename = shader_path + "/ssao/lighting_fragment.glsl";
      
      gl3::Shader  vs = gl3::make_shader_from_file(vertex_shader_filename, gl3::Shader::vertex_shader);
      gl3::Shader  fs = gl3::make_shader_from_file(fragment_shader_filename, gl3::Shader::fragment_shader);
      
      gl3::Program program = gl3::make_program(vs,fs);
      
      program.use();
      
      // Setup openGL context
      {
        glViewport(0, 0, buffer.m_occlusion.width(), buffer.m_occlusion.height());
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);
      }
      
      // Setup textures and uniforms
      {
        glActiveTexture( GL_TEXTURE0 );
        buffer.m_position.bind();
        gl3::check_errors("bind 0");
        program.set_uniform( "position_map", 0 );
        
        glActiveTexture( GL_TEXTURE1 );
        buffer.m_normal.bind();
        gl3::check_errors("bind 1");
        program.set_uniform( "normal_map", 1 );
        
        glActiveTexture( GL_TEXTURE2 );
        buffer.m_diffuse.bind();
        gl3::check_errors("bind 2");
        program.set_uniform( "diffuse_map", 2 );
        
        glActiveTexture( GL_TEXTURE3 );
        buffer.m_blurred_occlusion.bind();
        gl3::check_errors("bind 3");
        program.set_uniform( "occlusion_map", 3 );
        
        for(int i = 0;i < lights.size(); ++i)
        {
          glActiveTexture( GL_TEXTURE4+i );
          shadowmap_manager.m_objects[i].m_depth_texture.bind();
          gl3::check_errors("bind " + util::to_string(4+i) );
          program.set_uniform( "light_depth_map" + util::to_string(i), 4+i);
        }
        
        program.set_uniform("view_matrix", view_matrix );
        
        set_lighting_uniforms(program, lights);
        
        for(int i=0u; i < lights.size(); ++i)
        {
          glm::mat4 const bias_matrix(
                                      0.5, 0.0, 0.0, 0.0,
                                      0.0, 0.5, 0.0, 0.0,
                                      0.0, 0.0, 0.5, 0.0,
                                      0.5, 0.5, 0.5, 1.0
                                      );
          
          ShadowmapObject const & shadow = shadowmap_manager.m_objects[i];
          glm::mat4 light_matrix = bias_matrix * shadow.m_projection_matrix * shadow.m_view_matrix;
          
          program.set_uniform( "light_matrix[" + util::to_string(i) + "]", light_matrix);
        }
      }
      
      gl3::VBO     vbo     = gl3::make_quad_vbo();
      gl3::VAO     vao     = gl3::make_vao( vbo, program, "position");
      
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      
      vao.bind();
      vbo.draw();
      vao.unbind();
      
      buffer.m_position.unbind();
      buffer.m_normal.unbind();
      buffer.m_diffuse.unbind();
      buffer.m_blurred_occlusion.unbind();
      for(int i=0u; i < lights.size(); ++i)
        shadowmap_manager.m_objects[i].m_depth_texture.unbind();
      
      program.stop();
      program.clear();
    }
    
    //--- Restore the old openGL context ---------------------------------------
    {
      if( cull_param)
      {
        glEnable(GL_CULL_FACE);
      }
      else
      {
        glDisable(GL_CULL_FACE);
      }
      
      if (depth_param)
        glEnable(GL_DEPTH_TEST);
      else
        glDisable(GL_DEPTH_TEST);
      
      if (blend_param)
        glEnable(GL_BLEND);
      else
        glDisable(GL_BLEND);
      
      glCullFace(cull_mode_param);
      glDepthFunc(depth_func_param);
      
      glViewport(viewport_params[0], viewport_params[1], viewport_params[2], viewport_params[3]);
    }
    
    gl3::check_errors("draw_ssao() exit");
  }
  
}//namespace prox_gui

// PROX_GUI_DRAW_SSAO_H
#endif
