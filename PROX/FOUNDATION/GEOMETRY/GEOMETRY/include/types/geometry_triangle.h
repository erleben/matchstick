#ifndef GEOMETRY_TRIANGLE_H
#define GEOMETRY_TRIANGLE_H

namespace geometry
{
  
  template<typename V>
  class Triangle
  {
  public:
    
    typedef typename V::real_type       T;
    typedef typename V::value_traits    VT;
    
  protected:
    
    V m_point[3];

  public:
    
    V const & point(unsigned int const & idx ) const { return m_point[idx]; }
    V       & point(unsigned int const & idx )       { return m_point[idx]; }
    V const & p(unsigned int const & idx )     const { return m_point[idx]; }
    V       & p(unsigned int const & idx )           { return m_point[idx]; }

  public:
    
    Triangle()
    {
      m_point[0] = V::zero();
      m_point[1] = V::i();
      m_point[2] = V::j();
    }

    Triangle(
                V const & p0
                , V const & p1
                , V const & p2
                )
    {
      m_point[0] = p0;
      m_point[1] = p1;
      m_point[2] = p2;
    }

    Triangle( Triangle const & tri)
    {
      *this = tri;
    }

    Triangle & operator=(Triangle const & tri)
    {
      if (this != &tri)
      {
        this->m_point[0] = tri.m_point[0];
        this->m_point[1] = tri.m_point[1];
        this->m_point[2] = tri.m_point[2];
      }
      return *this;
    }

  };

  template<typename V>
  inline Triangle<V> make_triangle()
  {
    return Triangle<V>();
  }

  template<typename V>
  inline Triangle<V> make_triangle(
                  V const & p0
                  , V const & p1
                  , V const & p2
                  )
  {
    return Triangle<V>(p0,p1,p2);
  }

  template<typename V>
  inline typename V::real_type get_area(Triangle<V> const & tri)
  {
    typedef typename V::real_type    T;
    typedef typename V::value_traits VT;

    V const & x1 = tri.point(0);
    V const & x2 = tri.point(1);
    V const & x3 = tri.point(2);

    T const area2 = norm( cross(x2 - x1, x3 - x1) );

    assert( is_number(area2) || !"get_area(): Nan encountered");
    assert( is_finite(area2) || !"get_area(): Inf encountered");

    return area2 / VT::two();
  }

}// namespace geometry

//GEOMETRY_TRIANGLE_H
#endif