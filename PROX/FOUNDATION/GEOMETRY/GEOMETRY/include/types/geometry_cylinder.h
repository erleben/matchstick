#ifndef GEOMETRY_CYLINDER_H
#define GEOMETRY_CYLINDER_H

namespace geometry
{
  
  template<typename V>
  class Cylinder
  {
  public:
    
    typedef typename V::real_type       T;
    typedef typename V::value_traits    VT;
    
  protected:
    
    T m_radius;
    T m_height;
    V m_axis;
    V m_center;

  public:
    
    T const & radius()      const { return this->m_radius;            }
    T const & height()      const { return this->m_height;            }
    T         half_height() const { return this->m_height*VT::half(); }
    V const & axis()        const { return this->m_axis;              }
    V const & center()      const { return this->m_center;            }

    void set_radius(T const & radius)
    {
      assert(radius >= VT::zero() || !"Cylinder::set_radius(): Value was negative");

      this->m_radius = radius;
    }

    void set_height(T const & height )
    {
      assert(height >= VT::zero() || !"Cylinder::set_height(): Value was negative");

      this->m_height = height;
    }

    void set_axis(V const & axis)
    {
      assert(norm(axis) > VT::zero() || !"Cylinder::set_axis(): Axis was zero-vector");

      this->m_axis = unit(axis);
    }

    V & center()
    {
      return this->m_center;
    }

  public:
    
    Cylinder()
    : m_radius( VT::one() )
    , m_height( VT::one() )
    , m_axis( V::k() )
    , m_center( V::zero() )
    {}

    Cylinder(T const & radius, T const & height, V const & axis, V const & center)
    : m_radius( radius )
    , m_height( height )
    , m_axis( unit(axis) )
    , m_center( center )
    {
      assert(radius >= VT::zero()    || !"Cylinder(): radius was negative"  );
      assert(height >= VT::zero()    || !"Cylinder(): height was negative"  );
      assert(norm(axis) > VT::zero() || !"Cylinder(): axis was zero-vector" );
    }

    Cylinder( Cylinder const & cylinder)
    {
      *this = cylinder;
    }

    Cylinder & operator=(Cylinder const & cylinder)
    {
      if(this!=&cylinder)
      {
        this->m_radius = cylinder.m_radius;
        this->m_height = cylinder.m_height;
        this->m_axis    = cylinder.m_axis;
        this->m_center = cylinder.m_center;
      }
      return *this;
    }
  };
  
  template<typename V>
  Cylinder<V> make_cylinder(
                            typename V::real_type const & radius
                            , typename V::real_type const & height
                            , V const & axis
                            , V const & center
                            )
  {
    return Cylinder<V>(radius, height, axis, center);
  }

} //namespace geometry

// GEOMETRY_CYLINDER_H
#endif 