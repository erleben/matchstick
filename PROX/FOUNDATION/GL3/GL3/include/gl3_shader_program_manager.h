#ifndef GL3_SHADER_PROGRAM_MANAGER_H
#define GL3_SHADER_PROGRAM_MANAGER_H

#include <gl3.h>
#include <gl3_shader.h>
#include <gl3_program.h>

#include <vector>

namespace gl3
{

  class ShaderProgramManager
  {
  protected:

    std::vector<Shader>  m_shaders;
    std::vector<Program> m_programs;

  public:

    ShaderProgramManager(){}

    ~ShaderProgramManager(){}

  public:

    void clear()
    {
      for(unsigned int i=0u; i < m_shaders.size(); ++i)
      {
        m_shaders[i].clear();
      }
      for(unsigned int i=0u; i < m_programs.size(); ++i)
      {
        m_programs[i].clear();
      }
      m_shaders.clear();
      m_programs.clear();
    }

    void load(std::string const & vertex_shader_filename, std::string const & fragment_shader_filename)
    {
      gl3::check_errors("load(): invoked");

      Shader vs = make_shader_from_file(vertex_shader_filename, Shader::vertex_shader);
      Shader fs = make_shader_from_file(fragment_shader_filename, Shader::fragment_shader);

      Program p = make_program(vs,fs);

      print_program_info(p);

      m_shaders.push_back(vs);
      m_shaders.push_back(fs);
      m_programs.push_back(p);
    }

    void load(std::string const & vertex_shader_filename, std::string const & geometry_shader_filename, std::string const & fragment_shader_filename)
    {
      gl3::check_errors("load(): invoked");

      Shader vs = make_shader_from_file(vertex_shader_filename, Shader::vertex_shader);
      Shader gs = make_shader_from_file(geometry_shader_filename, Shader::geometry_shader);
      Shader fs = make_shader_from_file(fragment_shader_filename, Shader::fragment_shader);

      Program p = make_program(vs,gs,fs);

      print_program_info(p);

      m_shaders.push_back(vs);
      m_shaders.push_back(fs);
      m_programs.push_back(p);
    }


    Program const & get(unsigned int const & idx) const
    {
      return m_programs[idx];
    }

    Program & get(unsigned int const & idx)
    {
      return m_programs[idx];
    }


  };

}//namespace gl3

//GL3_SHADER_PROGRAM_MANAGER_H
#endif
