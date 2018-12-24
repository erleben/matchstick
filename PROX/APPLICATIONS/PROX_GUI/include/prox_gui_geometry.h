#ifndef PROX_GUI_GEOMETRY_H
#define PROX_GUI_GEOMETRY_H

#include <gl3_vbo.h>
#include <gl3_vao.h>

namespace prox_gui
{
  
  class Geometry
  {
  public:
    
    unsigned int     m_gid;           ///< Geometry identifier.
    gl3::VBO         m_vbo;           ///< Vertex buffer object.
    gl3::VAO         m_solid_vao;     ///< Vertex array object.
    gl3::VAO         m_wire_vao;      ///< Vertex array object.
    gl3::VAO         m_shadow_vao;    ///< Vertex array object.
    
  };
  
}//namespace prox_gui

// PROX_GUI_GEOMETRY_H
#endif
