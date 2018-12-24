#ifndef GEOMETRY_CAPSULE_H
#define GEOMETRY_CAPSULE_H

namespace geometry
{
  
  template<typename V>
  class Capsule
  {
  public:
    
    typedef typename V::real_type       T;
    typedef typename V::value_traits    VT;
    
  protected:
    
    T m_radius;
    V m_point0;
    V m_point1;

  public:
    
    T const & radius() const { return this->m_radius; }
    V const & point0() const { return this->m_point0; }
    V const & point1() const { return this->m_point1; }
    V       & point0()       { return this->m_point0; }
    V       & point1()       { return this->m_point1; }

    void set_radius(T const & radius)
    {
      assert(radius >= VT::zero() || !"Capsule::set_radius(): Value was negative");

      this->m_radius = radius;
    }


  public:
    
    Capsule()
    : m_radius( VT::one() )
    , m_point0( V::zero() )
    , m_point1( V::zero() )
    {}

    Capsule(T const & radius, V const & point0, V const & point1)
    : m_radius( radius )
    , m_point0( point0 )
    , m_point1( point1 )
    {
      assert(radius >= VT::zero() || !"Capsule(): radius was negative"  );
    }

    Capsule( Capsule const & capsule)
    {
      *this = capsule;
    }

    Capsule & operator=(Capsule const & capsule)
    {
      if(this!=&capsule)
      {
        this->m_radius = capsule.m_radius;
        this->m_point0 = capsule.m_point0;
        this->m_point1 = capsule.m_point1;
      }
      return *this;
    }
  };
  
  template<typename V>
  Capsule<V> make_capsule(
                            typename V::real_type const & radius
                            , V const & point0
                            , V const & point1
                            )
  {
    return Capsule<V>(radius, point0, point1);
  }

} //namespace geometry

// GEOMETRY_CAPSULE_H
#endif 