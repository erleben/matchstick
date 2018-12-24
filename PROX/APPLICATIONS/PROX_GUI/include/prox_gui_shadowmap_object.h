#ifndef PROX_GUI_SHADOWMAP_OBJECT_H
#define PROX_GUI_SHADOWMAP_OBJECT_H

#include <prox_gui_lighting.h>

#include <gl3_glm.h>
#include <gl3_texture_2d.h>

namespace prox_gui
{
  class ShadowmapObject
  {
  public:
    
    LightInfo              m_light_source;
    gl3::Texture2D     m_depth_texture;
    
    glm::mat4              m_projection_matrix;
    glm::mat4              m_view_matrix;
    
  };
  
}//namespace prox_gui

// PROX_GUI_SHADOWMAP_OBJECT_H
#endif
