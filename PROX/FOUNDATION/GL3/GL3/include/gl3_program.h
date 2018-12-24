#ifndef GL3_PROGRAM_H
#define GL3_PROGRAM_H

#include <gl3.h>
#include <gl3_glm.h>
#include <gl3_check_errors.h>
#include <gl3_shader.h>

#include <util_log.h>


#include <string>
#include <fstream>
#include <sstream>

namespace gl3
{

  class Program
  {
  protected:

    unsigned int m_gl_name;

  public:

    unsigned int const & gl_name()     const { return m_gl_name;     }
    unsigned int       & gl_name()           { return m_gl_name;     }

  public:

    int get_uniform_location(std::string const & name)
    {
      if(m_gl_name==0)
        return -1;

      int location = glGetUniformLocation(m_gl_name, name.c_str() );

      if (location < 0)
      {
        util::Log logging;

        logging << "get_uniform_location("+name+") oups no location?" << util::Log::newline();
        return -1;
      }

      gl3::check_errors("get_uniform_location("+name+")");

      return location;
    }

    void set_uniform_array(std::string const & name,  unsigned int const & count, glm::vec3 const * array)
    {
      int const location = this->get_uniform_location(name);
      if( location >= 0)
      {
        glUniform3fv( location, count, glm::value_ptr(*array));
        gl3::check_errors("set_uniform("+name+") vec3[]");
      }
    }

    void set_uniform(std::string const & name, int const & value)
    {
      int const location = this->get_uniform_location(name);
      if( location >= 0)
      {
        glUniform1i( location, value );
        gl3::check_errors("set_uniform("+name+") int");
      }
    }

    void set_uniform(std::string const & name, float const & value)
    {
      int const location = this->get_uniform_location(name);
      if( location >= 0)
      {
        glUniform1f( location, value );
        gl3::check_errors("set_uniform("+name+") float");
      }
    }

    void set_uniform(std::string const & name, float const & value0, float const & value1)
    {
      int const location = this->get_uniform_location(name);
      if( location >= 0)
      {
        glUniform2f(location, value0, value1);
        gl3::check_errors("set_uniform("+name+") vec2");
      }
    }

    void set_uniform(std::string const & name, float const & value0, float const & value1, float const & value2)
    {
      int const location = this->get_uniform_location(name);
      if( location >= 0)
      {
        glUniform3f(location, value0, value1, value2);
        gl3::check_errors("set_uniform("+name+") vec3");
      }
    }

    void set_uniform(std::string const & name, float const & value0, float const & value1, float const & value2, float const & value3)
    {
      int const location = this->get_uniform_location(name);
      if( location >= 0)
      {
        glUniform4f(location, value0, value1, value2, value3);
        gl3::check_errors("set_uniform("+name+") vec4");
      }
    }

    void set_uniform(std::string const & name, glm::mat4 const & matrix)
    {
      int const location = this->get_uniform_location(name);
      if( location >= 0)
      {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr( matrix ) );
        gl3::check_errors("set_uniform("+name+") matrix");
      }
    }

  public:

    Program()
    : m_gl_name(0)
    {}

    Program(Program const & program)
    {
      *this = program;
    }

    Program const & operator=(Program const& program)
    {
      if(this != &program)
      {
        m_gl_name = program.m_gl_name;
      }
      return *this;
    }

    virtual ~Program()
    {
    }

    bool operator==(Program const & program)
    {
      return (m_gl_name==program.m_gl_name);
    }

  public:

    void clear()
    {
      if(m_gl_name!=0)
      {
        glDeleteProgram(m_gl_name);
        gl3::check_errors("glDeleteProgram");
      }
      m_gl_name = 0;
    }

    bool validate() const
    {
      if(m_gl_name==0)
      {
        util::Log logging;

        logging << "no program to validate" << util::Log::newline();
        return false;
      }

      glValidateProgram( m_gl_name );
      gl3::check_errors("glValidateProgram");

      GLint  status;
      glGetProgramiv( m_gl_name,  GL_VALIDATE_STATUS, &status );
      gl3::check_errors("glCetProgram");

      if ( status )
        return true;

      {
        util::Log logging;

        logging << "Shader program failed to validate" << util::Log::newline();
      }

      GLint  length;
      glGetProgramiv( m_gl_name, GL_INFO_LOG_LENGTH, &length);
      gl3::check_errors("glGetProgram");

      std::string message(length,' ');
      glGetProgramInfoLog( m_gl_name, length, 0, &message[0] );
      gl3::check_errors("glGetProgramInfoLog");

      {
        util::Log logging;

        logging << message << util::Log::newline();
      }
      return false;
    }

    void use() const
    {
      if(m_gl_name!=0)
      {
        glUseProgram(m_gl_name);
        gl3::check_errors("glUSeProgram");
      }
    }

    void stop() const
    {
      glUseProgram(0);
    }

  };

  inline Program make_program(Shader const & A,Shader const & B)
  {
    Program program;

    program.gl_name() = glCreateProgram();
    gl3::check_errors("glCreateProgram");

    glAttachShader( program.gl_name(), A.gl_name() );
    gl3::check_errors("glAttachShader");

    glAttachShader( program.gl_name(), B.gl_name() );
    gl3::check_errors("glAttachShader");

    glLinkProgram(program.gl_name());
    gl3::check_errors("glLinkProgram");

    GLint  linked;
    glGetProgramiv( program.gl_name(), GL_LINK_STATUS, &linked );
    gl3::check_errors("glCetProgram");

    if ( !linked )
    {
      {
        util::Log logging;

        logging << "Shader program failed to link" << util::Log::newline();
      }

      GLint  length;
      glGetProgramiv( program.gl_name(), GL_INFO_LOG_LENGTH, &length);
      gl3::check_errors("glGetProgram");

      std::string message(length,' ');
      glGetProgramInfoLog( program.gl_name(), length, 0, &message[0] );
      gl3::check_errors("glGetProgramInfoLog");

      {
        util::Log logging;

        logging << message << util::Log::newline();
      }
    }

    return program;
  }


  inline Program make_program(Shader const & A,Shader const & B,Shader const & C)
  {
    Program program;

    program.gl_name() = glCreateProgram();
    gl3::check_errors("glCreateProgram");

    glAttachShader( program.gl_name(), A.gl_name() );
    gl3::check_errors("glAttachShader");

    glAttachShader( program.gl_name(), B.gl_name() );
    gl3::check_errors("glAttachShader");

    glAttachShader( program.gl_name(), C.gl_name() );
    gl3::check_errors("glAttachShader");

    glLinkProgram(program.gl_name());
    gl3::check_errors("glLinkProgram");

    GLint  linked;
    glGetProgramiv( program.gl_name(), GL_LINK_STATUS, &linked );
    gl3::check_errors("glCetProgram");

    if ( !linked )
    {
      {
        util::Log logging;

        logging << "Shader program failed to link" << util::Log::newline();
      }

      GLint  length;
      glGetProgramiv( program.gl_name(), GL_INFO_LOG_LENGTH, &length);
      gl3::check_errors("glGetProgram");

      std::string message(length,' ');
      glGetProgramInfoLog( program.gl_name(), length, 0, &message[0] );
      gl3::check_errors("glGetProgramInfoLog");

      {
        util::Log logging;

        logging << message << util::Log::newline();
      }
    }

    return program;
  }



  inline void print_program_info(Program const & program)
  {
    gl3::check_errors("print_program_info(): invoked");

    util::Log logging;

    logging << "print_program_info(" << program.gl_name() << "):" << util::Log::newline();

    GLint attached_shaders;
    glGetProgramiv( program.gl_name(), GL_ATTACHED_SHADERS, &attached_shaders );
    gl3::check_errors("GL_ATTACHED_SHADERS");
    logging << "\tGL_ATTACHED_SHADERS = " << attached_shaders << util::Log::newline();

    GLint active_attributes;
    glGetProgramiv( program.gl_name(), GL_ACTIVE_ATTRIBUTES, &active_attributes );
    gl3::check_errors("GL_ACTIVE_ATTRIBUTES");
    logging << "\tGL_ACTIVE_ATTRIBUTES = " << active_attributes << util::Log::newline();

    GLint active_attribute_max_length;
    glGetProgramiv( program.gl_name(), GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &active_attribute_max_length );
    gl3::check_errors("GL_ACTIVE_ATTRIBUTE_MAX_LENGTH");
    logging << "\tGL_ACTIVE_ATTRIBUTE_MAX_LENGTH = " << active_attribute_max_length << util::Log::newline();

    GLint active_uniforms;
    glGetProgramiv( program.gl_name(), GL_ACTIVE_UNIFORMS, &active_uniforms );
    gl3::check_errors("GL_ACTIVE_UNIFORMS");
    logging << "\tGL_ACTIVE_UNIFORMS = " << active_uniforms << util::Log::newline();

    GLint active_uniform_bloacks;
    glGetProgramiv( program.gl_name(), GL_ACTIVE_UNIFORM_BLOCKS, &active_uniform_bloacks );
    gl3::check_errors("GL_ACTIVE_UNIFORM_BLOCKS");
    logging << "\tGL_ACTIVE_UNIFORM_BLOCKS = " << active_uniform_bloacks << util::Log::newline();

    //GLint compute_Work_group_size;
    //glGetProgramiv( program.gl_name(), GL_COMPUTE_WORK_GROUP_SIZE, &compute_Work_group_size );
    //gl3::check_errors("GL_COMPUTE_WORK_GROUP_SIZE");
    //logging << "\tGL_COMPUTE_WORK_GROUP_SIZE = " << compute_Work_group_size << util::Log::newline();
  }

}//namespace gl3

// GL3_PROGRAM_H
#endif
