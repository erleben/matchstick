#ifndef PROX_GUI_DRAW_STRUCTURE_FIELDS_H
#define PROX_GUI_DRAW_STRUCTURE_FIELDS_H

#include <prox_gui_lighting.h>
#include <prox_gui_geometry.h>
#include <prox_gui_draw_vector.h>

#include <gl3_shader_program_manager.h>
#include <gl3_glm.h>

#include <content.h>
#include <mesh_array.h>
#include <tiny.h>

#include <cmath> // std::acos, std::fabs

namespace prox_gui
{
  
  inline void draw_structure_fields(
                                    content::API * engine
                                    , util::ConfigFile const & params
                                    , glm::mat4 const & projection_matrix
                                    , glm::mat4 const & view_matrix
                                    , std::vector<LightInfo> & lights
                                    , MaterialInfo const & structure_vector_material
                                    , gl3::ShaderProgramManager & program_manager
                                    , SceneManager const & scene_manager
                                    , GeometryManager const & geometry_manager
                                    )
  {
    float const scale = util::to_value<float>(params.get_value("draw_vector_scaling", "1.0")) / 2.0;
    
    gl3::check_errors("draw_structure_fields() invoked");
    
    gl3::Program & program = program_manager.get(2);
    
    program.use();
    
    program.set_uniform("projection_matrix", projection_matrix );
    program.set_uniform("view_matrix", view_matrix );
    
    set_lighting_uniforms(program, lights);
    
    Geometry const & vector_geometry = geometry_manager.get("vector");
    Geometry const & point_geometry = geometry_manager.get("point");

    float  red[10]   =  { 0.65438367, 0.31988109, 0.44265486, 0.26542251, 0.69408853, 0.69311113, 0.67534252, 0.05936259, 0.54917518, 0.76056459 };
    float  green[10] =  { 0.32071453, 0.34209646, 0.15578985, 0.09783959, 0.15686464, 0.21117915, 0.89700247, 0.55887708, 0.51857144, 0.83884786 };
    float  blue[10]  =  { 0.34998893, 0.91303928, 0.21365019, 0.74112951, 0.79478672, 0.95759395, 0.76601377, 0.80043425, 0.65514907, 0.39948332 };

    float x  = 0.0;
    float y  = 0.0;
    float z  = 0.0;
    float sx = 0.0;
    float sy = 0.0;
    float sz = 0.0;
    
    std::vector<SceneObject>::const_iterator object = scene_manager.m_objects.begin();
    std::vector<SceneObject>::const_iterator end    = scene_manager.m_objects.end();
    
    for(;object!=end;++object)
    {
      glm::mat4 const model_view_matrix =  view_matrix * object->m_model_matrix;
      program.set_uniform( "model_view_matrix", model_view_matrix);
      
      MaterialInfo structure_object_material = structure_vector_material;
      
      structure_object_material.m_Kd_red   =  red[object->m_rid % 10];
      structure_object_material.m_Kd_green =  green[object->m_rid % 10];
      structure_object_material.m_Kd_blue  =  blue[object->m_rid % 10];

      set_material_uniforms(program, structure_object_material);
      
      size_t N = 0u;
      size_t K = 0u;
      engine->get_tetramesh_shape(object->m_gid, N, K);
      
      if( N==0 || K==0) // Empty tetramesh, no need to save anything
      continue;
      
      std::vector<size_t> vertices;
      std::vector<size_t> tetrahedra;
      std::vector<float>  coordinates;
      
      vertices.resize(N);
      tetrahedra.resize(4*K);
      coordinates.resize(3*N);
      
      engine->get_tetramesh_shape(object->m_gid, &vertices[0], &tetrahedra[0], &coordinates[0] );
      
      engine->get_material_structure_map_size(object->m_rid, N );
      if(N>0u)
      {
        std::vector<size_t>  indices;
        std::vector<float>   structure_map;
        
        indices.resize(N);
        structure_map.resize(3*N);
        
        engine->get_material_structure_map_data(object->m_rid, &indices[0], &structure_map[0] );
        
        for(size_t n =0u ; n < N ; ++n)
        {
          x = coordinates[3*n];
          y = coordinates[3*n+1];
          z = coordinates[3*n+2];
          
          sx = structure_map[3*n];
          sy = structure_map[3*n+1];
          sz = structure_map[3*n+2];
          
          glm::vec3 const p = glm::vec3(x,y,z);
          glm::vec3 const s = glm::normalize( glm::vec3(sx,sy,sz) );
          
          draw_vector(model_view_matrix, p, s, scale, vector_geometry, program);
          draw_point(model_view_matrix, p, scale, point_geometry, program);
        }
        
        
      }
    }
    
    program.stop();
  }
  
}//namespace prox_gui

// PROX_GUI_DRAW_STRUCTURE_FIELDS_H
#endif
