#ifndef GEOMETRY_PLANE_H
#define GEOMETRY_PLANE_H

#include <types/geometry_triangle.h>

#include <tiny_precision.h>

#include <cmath>

namespace geometry
{

  /**
   * Set of all points p such that
   *
   *  n^T (p-p0) = 0
   *
   * where p0 is a point in the plane and n is the outward plane normal.
   *
   * We define w as
   *
   *  w = n*p0
   *
   * Given that n is the unit normal then w is the distance form origin to the plane.
   */
  template<typename V>
  class Plane
  {
  protected:
    
    typedef typename V::real_type    T;
    typedef typename V::value_traits VT;

    V  m_normal;
    T  m_offset;
    
  public:
    
    void set_normal(V const & normal)
    {
      this->m_normal = unit(normal);
    }

    T       & offset()       { return this->m_offset; }
    V const & normal() const { return this->m_normal; }
    T const & offset() const { return this->m_offset; }

    void set_n(V const & n)
    {
      this->m_normal = unit(n);
    }

    T       & w()       { return this->m_offset; }
    V const & n() const { return this->m_normal; }
    T const & w() const { return this->m_offset; }

  public:
    
    Plane()
    : m_normal( V::k() )
    , m_offset( VT::zero() )
    {}
    
    ~Plane(){}
    
    Plane(V const & normal, T const & offset)
    : m_normal( unit(normal) )
    , m_offset(offset)
    {
      assert(fabs(VT::one() - norm(normal)) < tiny::working_precision<T>() || !"Plane(): Must be unit normal");
    }
    
    Plane(Plane const & plane)
    {
      *this = plane;
    }
    
    Plane& operator=(Plane const & plane)
    {
      if( this != &plane)
      {
        this->m_normal = plane.m_normal;
        this->m_offset = plane.m_offset;
      }
      return *this;
    }

    
  };
  
  template<typename V>
  inline Plane<V> make_plane(V const & normal, typename V::real_type const & offset)
  {
    typedef typename V::value_traits  VT;
    typedef typename V::real_type      T;

    assert(fabs(VT::one() - norm(normal)) < tiny::working_precision<T>() || !"make_plane(): Must be unit normal");

    return Plane<V>(normal,offset);
  }

  template<typename V>
  inline Plane<V> make_plane(V const & p0, V const & p1, V const & p2)
  {
    typedef typename V::real_type T;

    V const normal = unit( cross(p1-p0, p2-p0) );
    T const offset = inner_prod(p0,normal);

    return Plane<V>(normal,offset);
  }

  /**
   * Tries to counter numerical precision errors as much as possible.
   * This is 3 times more expensive to do that just making a plane.
   */
  template<typename V>
  inline Plane<V> make_precise_plane(V const & p0, V const & p1, V const & p2)
  {
    typedef typename V::real_type T;

    V const m0 = cross(p1-p0, p2-p0);
    V const m1 = cross(p2-p1, p0-p1);
    V const m2 = cross(p0-p2, p1-p2);

    T const l0 = inner_prod(m0,m0);
    T const l1 = inner_prod(m1,m1);
    T const l2 = inner_prod(m2,m2);

    if (l0 >= l1 && l0 >= l2)
    {
      V const normal = m0 / sqrt(l0);
      T const offset = inner_prod(p0,normal);

      return Plane<V>(normal,offset);
    }
    if (l1 >= l0 && l1 >= l2)
    {
      V const normal = m1 / sqrt(l1);
      T const offset = inner_prod(p1,normal);

      return Plane<V>(normal,offset);
    }
    if (l2 >= l1 && l2 >= l0)
    {
      V const normal = m2 / sqrt(l2);
      T const offset = inner_prod(p2,normal);

      return Plane<V>(normal,offset);
    }
    
    return Plane<V>();
  }

  template<typename V>
  inline Plane<V> make_plane(V const & normal, V const & p)
  {
    typedef typename V::real_type T;

    T const offset = inner_prod(p ,normal);

    return Plane<V>(normal,offset);
  }

  template<typename V>
  inline Plane<V> make_plane(Triangle<V> const & t)
  {
    return make_plane<V>(t.point(0),t.point(1),t.point(2));
  }

  template<typename V>
  inline typename V::real_type get_signed_distance(V const & p, Plane<V>  const & P)
  {
    return inner_prod(P.n(),p) - P.w();
  }
  
  template<typename V>
  inline typename V::real_type get_distance(V const & p, Plane<V>  const & P)
  {
    using std::fabs;
    
    return fabs( get_signed_distance(p,P));
  }


  
}// namespace geometry

// GEOMETRY_PLANE_H
#endif