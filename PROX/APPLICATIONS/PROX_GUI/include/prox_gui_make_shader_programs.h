#ifndef PROX_GUI_MAKE_SHADER_PROGRAMS_H
#define PROX_GUI_MAKE_SHADER_PROGRAMS_H

#include <gl3_shader_program_manager.h>

#include <util_log.h>

namespace prox_gui
{
  
  inline void make_shader_programs(
                                   std::string const & shader_path
                                   , gl3::ShaderProgramManager & program_manager
                                   )
  {
    gl3::check_errors("make_shader_programs(): invoked");
    
    // Needs GL 4.3 or above to be supported
    //GLint max_uniform_locations = 0;
    //glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, &max_uniform_locations);
    
    GLint max_uniform_block_size = 0;
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &max_uniform_block_size);
    
    GLint max_vertex_uniform_components = 0;
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &max_vertex_uniform_components);
    
    GLint max_fragment_uniform_components = 0;
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &max_fragment_uniform_components);
    
    {
      util::Log logging;
      
      logging << "rigid_body::gui::make_shader_programs():" << util::Log::newline();
      logging << "\tGL_MAX_UNIFORM_BLOCK_SIZE          = " << max_uniform_block_size          << util::Log::newline();
      logging << "\tGL_MAX_VERTEX_UNIFORM_COMPONENTS   = " << max_vertex_uniform_components   << util::Log::newline();
      logging << "\tGL_MAX_FRAGMENT_UNIFORM_COMPONENTS = " << max_fragment_uniform_components << util::Log::newline();
      //logging << "\tGL_MAX_UNIFORM_LOCATIONS           = " << max_uniform_locations           << util::Log::newline();
    }
    
    gl3::check_errors("make_shader_programs(): query of hardware done");
    
    {
      std::string const vertex_shader_file   = shader_path + "solid_vertex.glsl";
      std::string const fragment_shader_file = shader_path + "solid_fragment.glsl";
      
      program_manager.load(vertex_shader_file, fragment_shader_file);
    }
    
    {
      std::string const vertex_shader_file   = shader_path + "wire_vertex.glsl";
      std::string const geometry_shader_file = shader_path + "wire_geometry.glsl";
      std::string const fragment_shader_file = shader_path + "wire_fragment.glsl";
      
      program_manager.load(vertex_shader_file, geometry_shader_file, fragment_shader_file);
    }
    
    {
      std::string const vertex_shader_file   = shader_path + "contact_vertex.glsl";
      std::string const fragment_shader_file = shader_path + "contact_fragment.glsl";
      
      program_manager.load(vertex_shader_file, fragment_shader_file);
    }
    
    {
      std::string const vertex_shader_file   = shader_path + "aabb_vertex.glsl";
      std::string const fragment_shader_file = shader_path + "aabb_fragment.glsl";
      
      program_manager.load(vertex_shader_file, fragment_shader_file);
    }
    
    {
      std::string const vertex_shader_file   = shader_path + "shadow_vertex.glsl";
      std::string const fragment_shader_file = shader_path + "shadow_fragment.glsl";
      
      program_manager.load(vertex_shader_file, fragment_shader_file);
    }
    
    gl3::check_errors("make_shader_programs(): leaving");
  }
  
}//namespace prox_gui

// PROX_GUI_MAKE_SHADER_PROGRAMS_H
#endif
