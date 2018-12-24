#ifndef PROX_GUI_SCENE_MANAGER_H
#define PROX_GUI_SCENE_MANAGER_H

#include <prox_gui_scene_object.h>

namespace prox_gui
{

  class SceneManager
  {
  public:

    std::vector<SceneObject> m_objects;
    
  public:

    void clear()
    {
      m_objects.clear();
    }

    void add(SceneObject const & object)
    {
      m_objects.push_back(object);
    }

  };

}//namespace prox_gui

// PROX_GUI_SCENE_MANAGER_H
#endif


