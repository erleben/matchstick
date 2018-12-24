#ifndef GEOMETRY_SPHERE_H
#define GEOMETRY_SPHERE_H

#include <types/geometry_support_mapping.h>

#include <tiny_is_number.h>
#include <tiny_is_finite.h>

#include <cmath> // needed for std::sqrt
#include <cassert>

namespace geometry
{
  
  template<typename V>
  class Sphere
  : public geometry::SupportMapping<V>

  {
  public:
    
    typedef typename V::real_type       T;
    typedef typename V::value_traits    VT;
    
  protected:
    
    V m_center;
    T m_radius;
    
  public:
    
    V const & center() const { return this->m_center; }
    T const & radius() const { return this->m_radius; }
    V       & center()       { return this->m_center; }
    T       & radius()       { return this->m_radius; }

  public:
    
    Sphere()
      : m_center( V::zero() )
      , m_radius( VT::one() )
    {
    }

    Sphere( V const & center, T const &  radius )
      : m_center( center )
      , m_radius( radius )
    {
    }

    Sphere(Sphere const & sph)
    {
      *this = sph;
    }

    Sphere & operator=(Sphere const & sph)
    {
      if( this != &sph)
      {
        this->m_center = sph.m_center;
        this->m_radius = sph.m_radius;
      }
      return *this;
    }


  public:

    V get_support_point(V const & v ) const
    {
      using std::sqrt;

      T const & vx = v(0);
      T const & vy = v(1);
      T const & vz = v(2);

      assert( is_number(vx) || !"get_support_point(): NAN encountered");
      assert( is_number(vy) || !"get_support_point(): NAN encountered");
      assert( is_number(vz) || !"get_support_point(): NAN encountered");
      assert( is_finite(vx) || !"get_support_point(): INF encountered");
      assert( is_finite(vy) || !"get_support_point(): INF encountered");
      assert( is_finite(vz) || !"get_support_point(): INF encountered");

      assert( is_number( this->m_radius)  || !"get_support_point(): NAN encountered");
      assert( is_finite( this->m_radius)  || !"get_support_point(): INF encountered");
      assert( this->m_radius > VT::zero() || !"get_support_point(): radius was non-positive");

      T const vv = vx*vx + vy*vy + vz*vz;

      assert( is_number(vv) || !"get_support_point(): NAN encountered");
      assert( is_finite(vv) || !"get_support_point(): INF encountered");

      T px = this->m_radius;
      T py = VT::zero();
      T pz = VT::zero();

      if (vv > VT::zero() )
      {
        T const tmp =  this->m_radius / sqrt(vv);

        assert( is_number(tmp) || !"get_support_point(): NAN encountered");
        assert( is_finite(tmp) || !"get_support_point(): INF encountered");

        px = vx*tmp;
        py = vy*tmp;
        pz = vz*tmp;
      }

      assert( is_number(px) || !"NAN encountered");
      assert( is_number(py) || !"NAN encountered");
      assert( is_number(pz) || !"NAN encountered");
      assert( is_finite(px) || !"INF encountered");
      assert( is_finite(py) || !"INF encountered");
      assert( is_finite(pz) || !"INF encountered");

      return V::make(px,py,pz) + this->m_center;
    }

    T get_scale() const
    {
      assert( is_number(this->m_radius)   || !"get_scale(): NAN encountered");
      assert( is_finite(this->m_radius)   || !"get_scale(): INF encountered");
      assert( this->m_radius > VT::zero() || !"get_scale(): radius was non-positive");
      
      return VT::two() *  this->m_radius ;
    }

  };
  
  template<typename V>
  inline Sphere<V> make_sphere(V const & center, typename V::real_type const &  radius)
  {
    return Sphere<V>(center,radius);
  }

}// namespace geometry

//GEOMETRY_SPHERE_H
#endif