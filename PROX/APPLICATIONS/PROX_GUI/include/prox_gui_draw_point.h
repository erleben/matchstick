#ifndef PROX_GUI_DRAW_POINT_H
#define PROX_GUI_DRAW_POINT_H

#include <prox_gui_lighting.h>
#include <prox_gui_geometry.h>

#include <gl3_shader_program_manager.h>
#include <gl3_glm.h>

#include <content.h>
#include <mesh_array.h>
#include <tiny.h>

#include <cmath> // std::acos, std::fabs

namespace prox_gui
{
  
  inline void make_point_geometry(
                                   gl3::ShaderProgramManager & program_manager
                                   , GeometryManager & geometry_manager
                                   )
  {
    typedef tiny::MathTypes<float>       MT;
    
    gl3::Program & program = program_manager.get(2);
    
    float radius1 = 0.25f;
    
    mesh_array::T3Mesh mesh;
    mesh_array::VertexAttribute<float, mesh_array::T3Mesh> X;
    mesh_array::VertexAttribute<float, mesh_array::T3Mesh> Y;
    mesh_array::VertexAttribute<float, mesh_array::T3Mesh> Z;
    
    mesh_array::make_sphere<MT>(radius1, 12, 12, mesh, X, Y, Z);
    
    Geometry geometry;
    
    geometry.m_vbo        = gl3::make_vbo(mesh, X, Y, Z);
    geometry.m_solid_vao  = gl3::make_vao(geometry.m_vbo, program, "position", "normal");
    
    geometry_manager.add("point", geometry);
  }
  
  
  inline void draw_point(
                          glm::mat4 const & view_matrix
                          ,  glm::vec3 const & p
                          , float const & scale
                          , Geometry const & point_geometry
                          , gl3::Program & program
                          )
  {
    using std::acos;
    using std::fabs;
    
    glm::mat4 const scale_matrix = glm::scale(glm::mat4(1.0), glm::vec3(scale,scale,scale));
    glm::mat4 const translation_matrix = glm::translate( glm::mat4(1.0), p);
    glm::mat4 const model_view_matrix =  view_matrix * translation_matrix * scale_matrix;
    
    program.set_uniform( "model_view_matrix", model_view_matrix);
    
    point_geometry.m_solid_vao.bind();
    point_geometry.m_vbo.draw();
    point_geometry.m_solid_vao.unbind();
  }
  
}//namespace prox_gui

// PROX_GUI_DRAW_POINT_H
#endif
