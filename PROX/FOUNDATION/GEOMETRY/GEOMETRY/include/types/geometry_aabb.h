#ifndef GEOMETRY_AABB_H
#define GEOMETRY_AABB_H

namespace geometry
{
  
  template<typename V>
  class AABB
  {
  public:

    typedef typename V::real_type       T;
    typedef typename V::value_traits    VT;

  protected:

    V m_min_coord;
    V m_max_coord;

  public:

    V const & min() const { return this->m_min_coord; }
    V       & min()       { return this->m_min_coord; }

    V const & max() const { return this->m_max_coord; }
    V       & max()       { return this->m_max_coord; }

  public:

    AABB()
    : m_min_coord( V::make(-VT::half(),-VT::half(),-VT::half()) )
    , m_max_coord( V::make( VT::half(), VT::half(), VT::half()) )
    {}

    AABB(V const & min_coord, V const & max_coord)
    : m_min_coord( min_coord )
    , m_max_coord( max_coord )
    {}

    AABB(AABB const & aabb)
    : m_min_coord(aabb.m_min_coord)
    , m_max_coord(aabb.m_max_coord)
    {}

    AABB & operator=(AABB const & aabb)
    {
      if(this!=&aabb)
      {
        this->m_min_coord = aabb.m_min_coord;
        this->m_max_coord = aabb.m_max_coord;
      }
      return *this;
    }

  };

  template<typename V>
  inline AABB<V> make_aabb(V const & min_coord, V const & max_coord)
  {
    return AABB<V>(min_coord, max_coord);
  }

}// namespace geometry

// GEOMETRY_AABB_H
#endif