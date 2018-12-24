#ifndef GL3_TRACKBALL_H
#define GL3_TRACKBALL_H

#include <gl3_glm.h>
#include <gl3_compute_normalized_coordinates.h>

#include <cmath>

namespace gl3
{

  class Trackball
  {
  protected:

    float     m_radius;
    glm::vec3 m_anchor_position;
    glm::vec3 m_current_position;
    float     m_angle;
    glm::vec3 m_axis;
    glm::mat4 m_rotation_matrix;

  public:

    float           & radius()        { return m_radius;           }
    float     const & radius()  const { return m_radius;           }
    glm::vec3       & anchor()        { return m_anchor_position;  }
    glm::vec3 const & anchor()  const { return m_anchor_position;  }
    glm::vec3       & current()       { return m_current_position; }
    glm::vec3 const & current() const { return m_current_position; }
    float           & angle()         { return m_angle;            }
    float     const & angle()   const { return m_angle;            }
    glm::vec3       & axis()          { return m_axis;             }
    glm::vec3 const & axis()    const { return m_axis;             }

    glm::mat4 const & get_rotation_matrix() const
    {
      return m_rotation_matrix;
    }

  public:

    Trackball()
    : m_radius(1.0f)
    {
      reset();
    }

    Trackball(float const  & radius)
    : m_radius(radius)
    {
      reset();
    }

    virtual ~Trackball()
    {}

    virtual void reset()
    {
      m_anchor_position   = glm::vec3(0.0, 0.0, 0.0);
      m_current_position  = glm::vec3(0.0, 0.0, 0.0);
      m_axis              = glm::vec3(0.0, 0.0, 0.0);
      m_angle             = 0.0f;
      m_rotation_matrix   = glm::mat4(1.0);

      project_onto_surface(m_anchor_position);
      project_onto_surface(m_current_position);
    }

  public:

    void mouse_down(float const & sx,float const & sy)
    {
      float nx = 0.0f;
      float ny = 0.0f;
      gl3::compute_normalized_coordinates(sx,sy,nx,ny);

      m_angle           = 0.0f;
      m_axis            = glm::vec3(0.0f, 0.0f, 0.0f);
      m_rotation_matrix = glm::mat4( 1.0 );

      m_anchor_position   = glm::vec3(nx, ny, 0.0f);
      m_current_position  = glm::vec3(nx, ny, 0.0f);

      project_onto_surface(m_anchor_position);
      project_onto_surface(m_current_position);
    }

    void mouse_up(float const & sx,float const & sy)
    {
      float nx = 0.0f;
      float ny = 0.0f;
      gl3::compute_normalized_coordinates(sx,sy,nx,ny);

      m_current_position =  glm::vec3(nx, ny, 0.0f);
      project_onto_surface(m_current_position);

      compute_rotation_matrix(m_anchor_position,m_current_position,m_rotation_matrix);
    }

    void mouse_move(float const & sx,float const & sy)
    {
      float nx = 0.0f;
      float ny = 0.0f;
      gl3::compute_normalized_coordinates(sx,sy,nx,ny);

      m_current_position = glm::vec3(nx,ny,0.0f);
      project_onto_surface(m_current_position);

      compute_rotation_matrix(m_anchor_position,m_current_position,m_rotation_matrix);
    }

  protected:

    void project_onto_surface(glm::vec3 & P)
    {
      using std::sqrt;

      static float const radius2 = m_radius * m_radius;

      float const length2 = P[0]*P[0] + P[1]*P[1];

      if (length2 <= radius2 / 2.0)
        P[2] = sqrt(radius2 - length2);
      else
      {
        P[2] = radius2 / (2.0 * sqrt(length2));
        float const length = sqrt(length2 + P[2]*P[2]);
        P /= length;
      }
      P = glm::normalize(P);
    }

    void compute_rotation_matrix(
                                 glm::vec3 const & anchor
                                 , glm::vec3 const & current
                                 , glm::mat4 & transform
                                 )
    {
      glm::vec3 const axis = glm::cross( anchor , current );

      if (glm::length(axis)>0)
      {
        m_axis  = glm::normalize( axis );
        m_angle = atan2( glm::length(axis), glm::dot(anchor, current) );

        transform = glm::rotate(glm::mat4(1.0), m_angle, m_axis);
      }
      else
      {
        m_axis    = glm::vec3( 0.0, 0.0, 0.0 );
        m_angle   = 0.0;
        transform = glm::mat4(1.0);
      }
    }

  };

} // namespace gl3

// GL3_TRACKBALL_H
#endif
