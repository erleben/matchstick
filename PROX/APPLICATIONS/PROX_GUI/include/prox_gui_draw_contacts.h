#ifndef PROX_GUI_DRAW_CONTACTS_H
#define PROX_GUI_DRAW_CONTACTS_H

#include <prox_gui_lighting.h>
#include <prox_gui_geometry.h>
#include <prox_gui_draw_point.h>
#include <prox_gui_draw_vector.h>

#include <gl3_shader_program_manager.h>
#include <gl3_glm.h>

#include <content.h>
#include <mesh_array.h>
#include <tiny.h>

#include <cmath> // std::acos, std::fabs

namespace prox_gui
{

  inline void draw_contacts(
                            content::API * engine
                            , util::ConfigFile const & params
                            , MaterialInfo const & point_material
                            , MaterialInfo const & normal_vector_material
                            , MaterialInfo const & structure_vector_material
                            , glm::mat4 const & projection_matrix
                            , glm::mat4 const & view_matrix
                            , std::vector<LightInfo>  & lights
                            , gl3::ShaderProgramManager  & program_manager
                            , GeometryManager  & geometry_manager
                            )
  {
    float const scale = util::to_value<float>(params.get_value("draw_vector_scaling", "1.0"));

    gl3::check_errors("draw_contacts() invoked");

    gl3::Program & program = program_manager.get(2);

    program.use();

    program.set_uniform("projection_matrix", projection_matrix );
    program.set_uniform("view_matrix", view_matrix );

    set_lighting_uniforms(program, lights);

    Geometry const & vector_geometry = geometry_manager.get("vector");
    Geometry const & point_geometry = geometry_manager.get("point");

    float x  = 0.0;
    float y  = 0.0;
    float z  = 0.0;
    float nx = 0.0;
    float ny = 0.0;
    float nz = 0.0;
    float sx = 0.0;
    float sy = 0.0;
    float sz = 0.0;

    for( size_t c = 0; c < engine->get_number_of_contacts(); ++c)
    {
      engine->get_contact_position(c, x, y, z);
      engine->get_contact_structure_direction(c, sx, sy, sz); 
      engine->get_contact_normal(c, nx, ny, nz);
      
      glm::vec3 const p = glm::vec3(x,y,z);
      glm::vec3 const n = glm::normalize( glm::vec3(nx,ny,nz) );
      glm::vec3 const s = glm::normalize( glm::vec3(sx,sy,sz) );
      
      set_material_uniforms(program, normal_vector_material);
      draw_vector(view_matrix, p, n, scale, vector_geometry, program);
      set_material_uniforms(program, structure_vector_material);
      draw_vector(view_matrix, p, s, scale, vector_geometry, program);
      set_material_uniforms(program, point_material);
      draw_point(view_matrix, p, scale, point_geometry, program);

    }

    program.stop();
  }

}//namespace prox_gui

// PROX_GUI_DRAW_CONTACTS_H
#endif
