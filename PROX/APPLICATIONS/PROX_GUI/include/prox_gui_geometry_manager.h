#ifndef PROX_GUI_GEOMETRY_MANAGER_H
#define PROX_GUI_GEOMETRY_MANAGER_H

#include <prox_gui_geometry.h>

#include <map>
#include <string>

namespace prox_gui
{
  
  class GeometryManager
  {
  public:
    
    std::map< unsigned int, Geometry > m_indexed_geometries;
    std::map< std::string, Geometry >  m_named_geometries;
    
  public:
    
    void clear()
    {
      {
        std::map< unsigned int, Geometry >::iterator geometry = m_indexed_geometries.begin();
        std::map< unsigned int, Geometry >::iterator end      = m_indexed_geometries.end();
        
        for(;geometry!=end;++geometry)
        {
          geometry->second.m_vbo.clear();
          geometry->second.m_solid_vao.clear();
          geometry->second.m_wire_vao.clear();
        }
        
        m_indexed_geometries.clear();
      }
      
      {
        std::map< std::string, Geometry >::iterator geometry = m_named_geometries.begin();
        std::map< std::string, Geometry >::iterator end      = m_named_geometries.end();
        
        for(;geometry!=end;++geometry)
        {
          geometry->second.m_vbo.clear();
          geometry->second.m_solid_vao.clear();
          geometry->second.m_wire_vao.clear();
        }
        
        m_named_geometries.clear();
      }
      
    }
    
    void add(Geometry const & geometry)
    {
      std::map< unsigned int, Geometry >::iterator lookup = m_indexed_geometries.find( geometry.m_gid );
      
      if(lookup == m_indexed_geometries.end())
      {
        m_indexed_geometries[ geometry.m_gid] = geometry;
      }
      
    }
    
    Geometry const & get(unsigned int const & gid) const
    {
      return m_indexed_geometries.at(gid);
    }
    
    void add(std::string const & name, Geometry const & geometry)
    {
      std::map< std::string, Geometry >::iterator lookup = m_named_geometries.find( name );
      
      if(lookup == m_named_geometries.end())
      {
        m_named_geometries[ name ] = geometry;
      }
    }
    
    Geometry const & get(std::string const & name) const
    {
      return m_named_geometries.at(name);
    }
    
  };
  
}//namespace prox_gui

// PROX_GUI_GEOMETRY_MANAGER_H
#endif

