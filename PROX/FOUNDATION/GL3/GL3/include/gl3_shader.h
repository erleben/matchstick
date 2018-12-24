#ifndef GL3_SHADER_H
#define GL3_SHADER_H

#include <gl3.h>
#include <gl3_check_errors.h>

#include <util_log.h>

#include <fstream>
#include <sstream>

namespace gl3
{

  class Shader
  {
  public:

    typedef enum {no_shader, vertex_shader, fragment_shader, geometry_shader} shader_type;

  protected:

    std::string  m_name;
    shader_type  m_type;
    std::string  m_source_code;
    unsigned int m_gl_name;

  public:

    std::string  const & name()        const { return m_name;        }
    shader_type  const & type()        const { return m_type;        }
    std::string        & source_code()       { return m_source_code; }
    std::string  const & source_code() const { return m_source_code; }
    unsigned int const & gl_name()     const { return m_gl_name;     }
    unsigned int       & gl_name()           { return m_gl_name;     }

    unsigned int gl_type( )const
    {
      switch (m_type)
      {
        case Shader::no_shader:       return GL_INVALID_VALUE;
        case Shader::vertex_shader:   return GL_VERTEX_SHADER;
        case Shader::fragment_shader: return GL_FRAGMENT_SHADER;
        case Shader::geometry_shader: return GL_GEOMETRY_SHADER;
      };

      return GL_INVALID_VALUE;
    }

  public:

    Shader()
    : m_name("")
    , m_type(no_shader)
    , m_source_code("")
    , m_gl_name(0)
    {}

    Shader(std::string const & name, shader_type const & type)
    : m_name(name)
    , m_type(type)
    , m_source_code("")
    , m_gl_name(0)
    {
    }

    Shader(Shader const & shader)
    {
      *this = shader;
    }

    Shader const & operator=(Shader const& shader)
    {
      if(this != &shader)
      {
        m_name        = shader.m_name;
        m_type        = shader.m_type;
        m_source_code = shader.m_source_code;
        m_gl_name     = shader.m_gl_name;
      }
      return *this;
    }

    virtual ~Shader()
    {
    }

    bool operator==(Shader const & shader)
    {
      return (m_name.compare(shader.m_name)==0);
    }

    void clear()
    {
      if(m_gl_name!=0)
      {
        glDeleteShader(m_gl_name);
        gl3::check_errors("glDeleteShader");
      }
      m_gl_name = 0;
    }

  };

  inline Shader make_shader_from_file(
                                      std::string const & filename
                                      , typename Shader::shader_type const & type
                                      )
  {
    gl3::check_errors("make_shader_from_file(): invoked");

    Shader shader(filename, type);

    std::ifstream file;

    file.open(filename);

    if( !file.is_open() )
    {
      util::Log logging;

      logging << "make_shader_from_file(): could not open file = " << filename << util::Log::newline();

      return shader;
    }

    std::stringstream stream;
    stream << file.rdbuf();
    shader.source_code() = stream.str();

    shader.gl_name() = glCreateShader(   shader.gl_type() );
    gl3::check_errors("make_shader_from_file(): glCreateShader");

    char const * source = shader.source_code().c_str();
    glShaderSource( shader.gl_name(), 1, &source, 0 );
    gl3::check_errors("glShaderSource");

    glCompileShader( shader.gl_name() );
    gl3::check_errors("glCompileShader");

    GLint  compiled;
    glGetShaderiv( shader.gl_name(), GL_COMPILE_STATUS, &compiled );
    gl3::check_errors("glGetShader");

    if ( !compiled )
    {
      {
        util::Log logging;

        logging << filename << " failed to compile : " << util::Log::newline();
      }

      GLint  length;
      glGetShaderiv( shader.gl_name(), GL_INFO_LOG_LENGTH, &length );
      gl3::check_errors("glGetShaderiv");

      std::string message(length, ' ');
      glGetShaderInfoLog(shader.gl_name(), length, &length, &message[0]);
      gl3::check_errors("glGetShaderInfoLog");

      {
        util::Log logging;

        logging << message << util::Log::newline();
      }
    }

    return shader;
  }

}//namespace gl3

// GL3_SHADER_H
#endif
