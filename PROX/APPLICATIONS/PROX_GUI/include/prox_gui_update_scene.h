#ifndef PROX_GUI_UPDATE_SCENE_H
#define PROX_GUI_UPDATE_SCENE_H

#include <prox_gui_scene_object.h>
#include <prox_gui_scene_manager.h>

#include <content.h>

#include <gl3_glm.h>

namespace prox_gui
{
  
  inline void update_scene(
                           SceneManager & scene_manager
                           , content::API * engine
                           )
  {
    std::vector<SceneObject>::iterator object = scene_manager.m_objects.begin();
    std::vector<SceneObject>::iterator end    = scene_manager.m_objects.end();
    
    for(;object!=end;++object)
    {
      float x,y,z;
      float qs,qx,qy,qz;
      
      engine->get_rigid_body_position( object->m_rid, x, y, z );
      engine->get_rigid_body_orientation( object->m_rid, qs, qx, qy, qz );
      
      glm::quat const Q = glm::quat(qs, qx, qy, qz);
      glm::mat4 const R = glm::toMat4(Q);
      glm::mat4 const T = glm::translate(glm::mat4(1.0), glm::vec3( x, y, z) );
      
      object->m_model_matrix = T*R;
      
      float min_x = 0.0f;
      float min_y = 0.0f;
      float min_z = 0.0f;
      float max_x = 0.0f;
      float max_y = 0.0f;
      float max_z = 0.0f;
      
      engine->get_rigid_body_bounding_box( object->m_rid, min_x, min_y, min_z, max_x, max_y, max_z );
      
      object->m_aabb_width  = max_x - min_x;
      object->m_aabb_height = max_y - min_y;
      object->m_aabb_depth  = max_z - min_z;
    }
  }
  
}//namespace prox_gui

// PROX_GUI_UPDATE_SCENE_H
#endif
