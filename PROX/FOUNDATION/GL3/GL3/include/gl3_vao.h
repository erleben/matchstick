#ifndef GL3_VAO_H
#define GL3_VAO_H

#include <gl3.h>
#include <gl3_check_errors.h>

#include <gl3_vbo.h>
#include <gl3_program.h>

#define BUFFER_OFFSET(i) ((void*)(i))

namespace gl3
{

  class VAO
  {
  protected:

    unsigned int m_vao;
    unsigned int m_position_location;
    unsigned int m_normal_location;

  public:

    VAO()
    : m_vao(0)
    , m_position_location(0)
    , m_normal_location(0)
    {
    }

    virtual ~VAO()
    {
      //clear();
    }

    void clear()
    {
      if(m_vao!=0)
      {
        glBindVertexArray(m_vao);
        gl3::check_errors("VAO::clear() glBindVertexArray");

        glDisableVertexAttribArray(m_position_location);
        gl3::check_errors("VAO::clear() glDisableVertexAttribArray");

        glDisableVertexAttribArray(m_normal_location);
        gl3::check_errors("VAO::clear() glDisableVertexAttribArray");

        glDeleteVertexArrays( 1, &m_vao );
        gl3::check_errors("VAO::clear() glDeleteVertexArrays");

        glBindVertexArray(0);
        gl3::check_errors("VAO::clear() glBindVertexArray");

      }
      m_vao = 0;
      m_position_location = 0;
      m_normal_location = 0;
    }

    void bind() const
    {
      if(m_vao!=0)
      {
        glBindVertexArray(m_vao);
      }
    }

    void unbind() const
    {
      glBindVertexArray(0);
    }

    void create(
                VBO const & vbo
                , Program const & program
                , std::string const & position_variable_name
                , std::string const & normal_variable_name
                )
    {
      gl3::check_errors("VAO::create() invoked");

      clear();

      glGenVertexArrays(1, &m_vao);
      glBindVertexArray(m_vao);

      vbo.bind();

      m_position_location = glGetAttribLocation(program.gl_name(), position_variable_name.c_str() );
      gl3::check_errors("VAO::create() glGetAttribLocation for position");

      unsigned int const stride = sizeof(typename VBO::Vertex);

      glVertexAttribPointer(m_position_location, 3, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(0));
      gl3::check_errors("VAO::create() glVertexAttribPointer for normal");

      glEnableVertexAttribArray(m_position_location);
      gl3::check_errors("VAO::create() glEnableVertexAttribArray for normal");


      if(! normal_variable_name.empty() )
      {
        m_normal_location   = glGetAttribLocation(program.gl_name(), normal_variable_name.c_str() );
        gl3::check_errors("VAO::create() glGetAttribLocation for normal");

        glVertexAttribPointer(m_normal_location, 3, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(12));
        gl3::check_errors("VAO::create() glVertexAttribPointer for normal");

        glEnableVertexAttribArray(m_normal_location);
        gl3::check_errors("VAO::create() glEnableVertexAttribArray for normal");
      }

      glBindVertexArray(0);

      gl3::check_errors("VAO::create() finished");
    }

  };

  inline VAO make_vao(
                      VBO const & vbo
                      , Program const & program
                      , std::string const & position_variable_name
                      , std::string const & normal_variable_name
                      )
  {
    VAO vao;
    vao.create(vbo, program, position_variable_name, normal_variable_name);
    return vao;
  }


  inline VAO make_vao(
                      VBO const & vbo
                      , Program const & program
                      , std::string const & position_variable_name
                      )
  {
    VAO vao;
    vao.create(vbo, program, position_variable_name, "");
    return vao;
  }

} // namespace gl3

// GL3_VAO_H
#endif
