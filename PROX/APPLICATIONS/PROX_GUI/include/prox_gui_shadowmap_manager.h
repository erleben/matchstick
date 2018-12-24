#ifndef PROX_GUI_SHADOWMAP_MANAGER_H
#define PROX_GUI_SHADOWMAP_MANAGER_H

#include <prox_gui_shadowmap_object.h>

namespace prox_gui
{

  class ShadowmapManager
  {
  public:

    std::vector<ShadowmapObject> m_objects;

  public:

    void clear()
    {
      for(unsigned int i = 0u; i < m_objects.size(); ++i)
      {
        m_objects[i].m_depth_texture.clear();
      }
      m_objects.clear();
    }

    void add(ShadowmapObject const & object)
    {
      m_objects.push_back(object);
    }

  };

}//namespace prox_gui

// PROX_GUI_SHADOWMAP_MANAGER_H
#endif


