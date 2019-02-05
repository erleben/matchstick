#ifndef PROX_GUI_DRAW_VECTOR_H
#define PROX_GUI_DRAW_VECTOR_H

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
  
  inline void make_vector_geometry(
                                   gl3::ShaderProgramManager & program_manager
                                   , GeometryManager & geometry_manager
                                   )
  {
    typedef tiny::MathTypes<float>       MT;
    typedef typename MT::vector3_type    V;
    
    gl3::Program & program = program_manager.get(2);
    
    float height  = 0.5f;
    float radius1 = 0.25f;
    float radius2 = 0.15f;
    
    //mesh_array::T3Mesh base;
    //mesh_array::VertexAttribute<float, mesh_array::T3Mesh> baseX;
    //mesh_array::VertexAttribute<float, mesh_array::T3Mesh> baseY;
    //mesh_array::VertexAttribute<float, mesh_array::T3Mesh> baseZ;
    
    mesh_array::T3Mesh shaft;
    mesh_array::VertexAttribute<float, mesh_array::T3Mesh> shaftX;
    mesh_array::VertexAttribute<float, mesh_array::T3Mesh> shaftY;
    mesh_array::VertexAttribute<float, mesh_array::T3Mesh> shaftZ;
    
    mesh_array::T3Mesh head;
    mesh_array::VertexAttribute<float, mesh_array::T3Mesh> headX;
    mesh_array::VertexAttribute<float, mesh_array::T3Mesh> headY;
    mesh_array::VertexAttribute<float, mesh_array::T3Mesh> headZ;
    
    //mesh_array::make_sphere<MT>(radius1, 12, 12, base, baseX, baseY, baseZ);
    mesh_array::make_cone<MT>(radius1, height, 12, head, headX, headY, headZ);
    mesh_array::make_cylinder<MT>(radius2, height, 12, shaft, shaftX, shaftY, shaftZ);
    
    mesh_array::translate<MT>(V::make(0, height/2, 0), shaft, shaftX, shaftY, shaftZ);
    mesh_array::translate<MT>(V::make(0, height, 0), head, headX, headY, headZ);
    
    mesh_array::T3Mesh mesh;
    mesh_array::VertexAttribute<float, mesh_array::T3Mesh> X;
    mesh_array::VertexAttribute<float, mesh_array::T3Mesh> Y;
    mesh_array::VertexAttribute<float, mesh_array::T3Mesh> Z;
    
    //mesh_array::concatenation<MT>( base, baseX, baseY, baseZ, mesh, X, Y, Z );
    mesh_array::concatenation<MT>( shaft, shaftX, shaftY, shaftZ, mesh, X, Y, Z );
    mesh_array::concatenation<MT>( head, headX, headY, headZ, mesh, X, Y, Z );
    
    Geometry geometry;
    
    geometry.m_vbo        = gl3::make_vbo(mesh, X, Y, Z);
    geometry.m_solid_vao  = gl3::make_vao(geometry.m_vbo, program, "position", "normal");
    
    geometry_manager.add("vector", geometry);
  }
  
  
  inline void draw_vector(
                          glm::mat4 const & view_matrix
                          ,  glm::vec3 const & p
                          , glm::vec3 const & v
                          , float const & scale
                          , Geometry const & arrow_geometry
                          , gl3::Program & program
                          )
  {
    using std::acos;
    using std::fabs;
    
    glm::vec3 const up =glm::vec3(0,1,0);
    
    float const cos_theta = glm::dot(up, v);
    
    glm::vec3 const axis = (fabs(cos_theta) < 0.99) ? glm::cross(up, v) : glm::vec3(1,0,0);
    
    float const radians = acos(cos_theta);
    
    glm::mat4 const scale_matrix = glm::scale(glm::mat4(1.0), glm::vec3(scale,scale,scale));
    glm::mat4 const rotation_matrix = glm::rotate( glm::mat4(1.0), radians, axis);
    glm::mat4 const translation_matrix = glm::translate( glm::mat4(1.0), p);
    glm::mat4 const model_view_matrix =  view_matrix * translation_matrix * rotation_matrix * scale_matrix;
    
    program.set_uniform( "model_view_matrix", model_view_matrix);
    
    arrow_geometry.m_solid_vao.bind();
    arrow_geometry.m_vbo.draw();
    arrow_geometry.m_solid_vao.unbind();
  }
  
}//namespace prox_gui

// PROX_GUI_DRAW_VECTOR_H
#endif
