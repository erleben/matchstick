#ifndef NARROW_SYSTEM_H
#define NARROW_SYSTEM_H

#include <narrow_geometry.h>
#include <narrow_object.h>
#include <narrow_params.h>

#include <cassert>
#include <vector>

namespace narrow
{

  template<typename M>
  class System
  {
  protected:

    std::vector< Geometry<M> > m_geometries;
    Params<M>                  m_params;

  public:

    typedef typename std::vector< Geometry<M> >::iterator geometry_iterator;

    geometry_iterator begin()
    {
      return m_geometries.begin();
    }

    geometry_iterator end()
    {
      return m_geometries.end();
    }


    Params<M> const & params() const { return this->m_params; }
    Params<M>       & params()       { return this->m_params; }

  public:

    void clear()
    {
      m_geometries.clear();
    }

    size_t size() const
    {
      return this->m_geometries.size();
    }
    
    size_t create_geometry()
    {
      Geometry<M> G = Geometry<M>();
      this->m_geometries.push_back(G);

      return this->m_geometries.size() - 1u;
    }

    Geometry<M> const & get_geometry( Object<M> const & obj ) const
    {
      size_t const & idx = obj.get_geometry_idx();

      assert( idx < m_geometries.size() || !"get_geometry(): illegal idx");

      return this->m_geometries[idx];
    }

    Geometry<M> & get_geometry( Object<M> const & obj )
    {
      size_t const & idx = obj.get_geometry_idx();

      assert( idx < m_geometries.size() || !"get_geometry(): illegal idx");

      return this->m_geometries[idx];
    }

    Geometry<M> const & get_geometry( size_t const & idx ) const
    {
      assert( idx < m_geometries.size() || !"get_geometry(): illegal idx");

      return this->m_geometries[idx];
    }

    Geometry<M> & get_geometry( size_t const & idx  )
    {
      assert( idx < m_geometries.size() || !"get_geometry(): illegal idx");

      return this->m_geometries[idx];
    }

  };
  
} // namespace narrow

// NARROW_SYSTEM_H
#endif
