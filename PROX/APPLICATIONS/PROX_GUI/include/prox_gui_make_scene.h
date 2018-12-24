#ifndef PROX_GUI_MAKE_SCENE_H
#define PROX_GUI_MAKE_SCENE_H

#include <prox_gui_ucph_colors.h>
#include <prox_gui_make_shader_programs.h>
#include <prox_gui_scene_manager.h>
#include <prox_gui_geometry_manager.h>
#include <prox_gui_update_scene.h>
#include <prox_gui_materials.h>

#include <gl3_shader_program_manager.h>
#include <gl3_vbo.h>

#include <content.h>

#include <mesh_array_t3mesh.h>
#include <mesh_array_vertex_attribute.h>

#include <tiny_math_types.h>

#include <util_config_file.h>

namespace prox_gui
{

  inline void make_scene(
                         util::ConfigFile const & params
                         , content::API * engine
                         , std::vector<MaterialInfo> & materials
                         , std::string const & shader_path
                         , gl3::ShaderProgramManager & program_manager
                         , SceneManager & scene_manager
                         , GeometryManager & geometry_manager
                         )
  {
    gl3::check_errors("make_scene(): invoked");

    std::vector<size_t> mids;
    mids.resize(engine->get_number_of_materials());
    engine->get_material_indices( &mids[0] );
    for(size_t i = 0u; i < mids.size(); ++i)
    {
      size_t const mid = mids[i];
      std::string const material_name = engine->get_material_name( mid );

      MaterialInfo const material = make_material_from_config_file(material_name, params);

      materials.push_back(material);
    }

    make_shader_programs(shader_path, program_manager);

    gl3::Program & solid_program  = program_manager.get(0);
    gl3::Program & wire_program   = program_manager.get(1);
    gl3::Program & shadow_program = program_manager.get(4);

    std::vector<size_t> gids;
    gids.resize(engine->get_number_of_geometries());
    engine->get_geometry_indices( &gids[0] );

    for(size_t i = 0u; i < gids.size(); ++i)
    {
      size_t const gid = gids[i];

      mesh_array::T3Mesh mesh;
      mesh_array::VertexAttribute<float, mesh_array::T3Mesh> X;
      mesh_array::VertexAttribute<float, mesh_array::T3Mesh> Y;
      mesh_array::VertexAttribute<float, mesh_array::T3Mesh> Z;

      content::add_visual_mesh_of_tetrameshes( gid, engine, mesh, X, Y, Z );

      Geometry geometry;

      geometry.m_gid       = gid;
      geometry.m_vbo       = gl3::make_vbo(mesh, X, Y, Z);
      geometry.m_solid_vao = gl3::make_vao(geometry.m_vbo, solid_program, "position", "normal");
      geometry.m_wire_vao  = gl3::make_vao(geometry.m_vbo, wire_program, "position", "normal");
      geometry.m_shadow_vao  = gl3::make_vao(geometry.m_vbo, shadow_program, "position");

      geometry_manager.add( geometry );
    }

    std::vector<size_t> rids;
    rids.resize(engine->get_number_of_rigid_bodies());
    engine->get_rigid_body_indices( &rids[0] );

    for(size_t i = 0u; i < rids.size(); ++i)
    {
      size_t const rid = rids[i];
      size_t const gid = engine->get_rigid_body_collision_geometry( rid );

      SceneObject object;

      object.m_rid = rid;
      object.m_gid = gid;

      size_t      const mid           = engine->get_rigid_body_material( rid );
      std::string const material_name = engine->get_material_name( mid );

      object.m_pid  = 0;  // Currently all objects use the same shader program

      object.m_mid  = mid;

      if (engine->get_rigid_body_fixed(rid))
      {
        object.m_red   = UCPHGreenColor::red();
        object.m_green = UCPHGreenColor::green();
        object.m_blue  = UCPHGreenColor::blue();
      }
      else if (engine->has_scripted_motion(rid))
      {
        object.m_red   = UCPHBlueColor::red();
        object.m_green = UCPHBlueColor::green();
        object.m_blue  = UCPHBlueColor::blue();
      }
      else
      {
        object.m_red   = UCPHRedColor::red();
        object.m_green = UCPHRedColor::green();
        object.m_blue  = UCPHRedColor::blue();
      }

      object.m_model_matrix  = glm::mat4(1.0f);  // init to identity

      scene_manager.add( object );
    }

    update_scene( scene_manager, engine );
  }
  
}//namespace prox_gui

// PROX_GUI_MAKE_SCENE_H
#endif
