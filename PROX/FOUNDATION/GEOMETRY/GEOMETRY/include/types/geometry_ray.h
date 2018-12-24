#ifndef GEOMETRY_RAY_H
#define GEOMETRY_RAY_H

namespace geometry
{
  
  template<typename V>
  class Ray
  {
  public:

    typedef typename V::real_type       T;
    typedef typename V::value_traits    VT;

  protected:

    V m_origin;
    V m_direction;

  public:

    V const & origin()    const { return this->m_origin; }
    V       & origin()          { return this->m_origin; }

    V const & direction() const { return this->m_direction; }

    void set_direction(V const & direction)
    {
      this->m_direction = unit(direction);
    }

  public:

    Ray()
    : m_origin( V::zero() )
    , m_direction(  V::k() )
    {}

    Ray(V const & origin, V const & direction)
    : m_origin(origin)
    , m_direction( unit( direction ) )
    {}

    Ray(Ray const & ray)
    : m_origin(ray.m_origin)
    , m_direction(ray.m_direction)
    {}

    Ray & operator=(Ray const & ray)
    {
      if(this!=&ray)
      {
        this->m_origin    = ray.m_origin;
        this->m_direction = ray.m_direction;
      }
      return *this;
    }

  };

  template<typename V>
  inline Ray<V> make_ray(V const & origin, V const & direction)
  {
    return Ray<V>(origin, direction);
  }

}// namespace geometry

// GEOMETRY_RAY_H
#endif