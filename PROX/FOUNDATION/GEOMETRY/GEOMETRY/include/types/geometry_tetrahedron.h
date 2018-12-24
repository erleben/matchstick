#ifndef GEOMETRY_TETRAHEDRON_H
#define GEOMETRY_TETRAHEDRON_H

#include <types/geometry_support_mapping.h>
#include <types/geometry_triangle.h>

#include <tiny_vector_functions.h>
#include <tiny_is_number.h>
#include <tiny_is_finite.h>

#include <cassert>
#include <cmath>     // needed for std::fabs

namespace geometry
{
  
  template<typename V>
  class Tetrahedron
  : public SupportMapping<V>
  {
  public:
    
    typedef typename V::real_type       T;
    typedef typename V::value_traits    VT;
    
  protected:
    
    V m_point[4];

  public:
    
    V const & point(unsigned int const & idx ) const { return m_point[idx]; }
    V       & point(unsigned int const & idx )       { return m_point[idx]; }
    V const & p(unsigned int const & idx )     const { return m_point[idx]; }
    V       & p(unsigned int const & idx )           { return m_point[idx]; }

  public:
    
    Tetrahedron()
    {
      m_point[0] = V::zero();
      m_point[1] = V::i();
      m_point[2] = V::j();
      m_point[3] = V::k();
    }

    Tetrahedron(
                V const & p0
                , V const & p1
                , V const & p2
                , V const & p3
                )
    {
      m_point[0] = p0;
      m_point[1] = p1;
      m_point[2] = p2;
      m_point[3] = p3;
    }

    Tetrahedron( Tetrahedron const & tet)
    {
      *this = tet;
    }

    Tetrahedron & operator=(Tetrahedron const & tet)
    {
      if (this != &tet)
      {
        this->m_point[0] = tet.m_point[0];
        this->m_point[1] = tet.m_point[1];
        this->m_point[2] = tet.m_point[2];
        this->m_point[3] = tet.m_point[3];
      }
      return *this;
    }

  public:

    V get_support_point( V const & v ) const
    {
      assert( is_number(v(0)) || !"get_support_point(): NAN encountered");
      assert( is_number(v(1)) || !"get_support_point(): NAN encountered");
      assert( is_number(v(2)) || !"get_support_point(): NAN encountered");
      assert( is_finite(v(0)) || !"get_support_point(): INF encountered");
      assert( is_finite(v(1)) || !"get_support_point(): INF encountered");
      assert( is_finite(v(2)) || !"get_support_point(): INF encountered");

      V p = this->m_point[0];

      assert( is_number(p(0)) || !"get_support_point(): NAN encountered");
      assert( is_number(p(1)) || !"get_support_point(): NAN encountered");
      assert( is_number(p(2)) || !"get_support_point(): NAN encountered");
      assert( is_finite(p(0)) || !"get_support_point(): INF encountered");
      assert( is_finite(p(1)) || !"get_support_point(): INF encountered");
      assert( is_finite(p(2)) || !"get_support_point(): INF encountered");

      T max_val = tiny::inner_prod(p,v);

      for(size_t i =1u; i < 4u;)
      {
        V const & q = this->m_point[i++];

        assert( is_number(q(0)) || !"get_support_point(): NAN encountered");
        assert( is_number(q(1)) || !"get_support_point(): NAN encountered");
        assert( is_number(q(2)) || !"get_support_point(): NAN encountered");
        assert( is_finite(q(0)) || !"get_support_point(): INF encountered");
        assert( is_finite(q(1)) || !"get_support_point(): INF encountered");
        assert( is_finite(q(2)) || !"get_support_point(): INF encountered");

        T const val = tiny::inner_prod(q,v);

        if(val > max_val)
        {
          max_val = val;
          p = q;
        }
      }
      assert( is_number(p(0)) || !"get_support_point(): NAN encountered");
      assert( is_number(p(1)) || !"get_support_point(): NAN encountered");
      assert( is_number(p(2)) || !"get_support_point(): NAN encountered");
      assert( is_finite(p(0)) || !"get_support_point(): INF encountered");
      assert( is_finite(p(1)) || !"get_support_point(): INF encountered");
      assert( is_finite(p(2)) || !"get_support_point(): INF encountered");
      
      return p;
    }


    V get_center() const
    {
      V const & p0 = this->m_point[0];
      V const & p1 = this->m_point[1];
      V const & p2 = this->m_point[2];
      V const & p3 = this->m_point[3];

      return (p0 + p1 + p2 + p3)/ VT::four();
    }

    T get_scale() const
    {
      using std::max;
      using std::sqrt;

      V const & c  = this->get_center();
      V const & p0 = this->m_point[0];
      V const & p1 = this->m_point[1];
      V const & p2 = this->m_point[2];
      V const & p3 = this->m_point[3];

      T const dot0 = tiny::inner_prod(p0-c, p0-c);
      T const dot1 = tiny::inner_prod(p1-c, p1-c);
      T const dot2 = tiny::inner_prod(p2-c, p2-c);
      T const dot3 = tiny::inner_prod(p3-c, p3-c);

      T const max_val = max( dot0, max( dot1, max( dot2, dot3) ) );

      T const diameter = VT::two() * sqrt( max_val );

      assert(diameter > VT::zero() || !"get_scale(): Internal error");
      assert(is_finite(diameter)   || !"get_scale(): Inf");
      assert(is_number(diameter)   || !"get_scale(): Nan");

      return diameter;
    }

  };

  template<typename V>
  inline Tetrahedron<V> make_tetrahedron()
  {
    return Tetrahedron<V>();
  }

  template<typename V>
  inline Tetrahedron<V> make_tetrahedron(
                  V const & p0
                  , V const & p1
                  , V const & p2
                  , V const & p3
                  )
  {
    return Tetrahedron<V>(p0,p1,p2,p3);
  }


  template<typename V>
  inline Tetrahedron<V> uniform_scale( typename  V::real_type const & scale, Tetrahedron<V> const & A)
  {
    V const & p0 = A.p(0u);
    V const & p1 = A.p(1u);
    V const & p2 = A.p(2u);
    V const & p3 = A.p(3u);
    V const & c  = A.get_center();

    return Tetrahedron<V>(
                            (scale*(p0-c)) + c
                          , (scale*(p1-c)) + c
                          , (scale*(p2-c)) + c
                          , (scale*(p3-c)) + c
                          );
  }

  template<typename V>
  inline typename V::real_type get_signed_volume(Tetrahedron<V> const & tet)
  {
    typedef typename V::real_type    T;
    typedef typename V::value_traits VT;

    V const & x1 = tet.point(0);
    V const & x2 = tet.point(1);
    V const & x3 = tet.point(2);
    V const & x4 = tet.point(3);

    T const vol6 = inner_prod( x4 - x1, cross(x2 - x1, x3 - x1) );

    assert( is_number(vol6) || !"get_signed_volume(): Nan encountered");
    assert( is_finite(vol6) || !"get_signed_volume(): Inf encountered");

    return vol6 / VT::numeric_cast(6.0);
  }

  template<typename V>
  inline typename V::real_type get_volume(Tetrahedron<V> const & tet)
  {
    using std::fabs;

    return fabs( get_signed_volume(tet) );
  }

  template<typename V>
  inline Triangle<V> get_opposite_face(unsigned int const & idx, Tetrahedron<V> const & tet)
  {
    assert( idx < 4u || !"get_opposite_face(): idx out of range");

    //
    // We assume that tetrahedron vertices are given with positive orientation
    //
    // That is vertex 3 is on the front side of triangle given by the ccw order
    // of vertices 0, 1, 2
    //
    //  face opposite vertex 0:  123
    //  face opposite vertex 1:  032
    //  face opposite vertex 2:  013
    //  face opposite vertex 3:  021


    if (idx==0u)
      return Triangle<V>( tet.point(1), tet.point(2), tet.point(3) );
    if (idx==1u)
      return Triangle<V>( tet.point(0), tet.point(3), tet.point(2) );
    if (idx==2u)
      return Triangle<V>( tet.point(0), tet.point(1), tet.point(3) );
    if (idx==3u)
      return Triangle<V>( tet.point(0), tet.point(2), tet.point(1) );

    return Triangle<V>( );
  }

}// namespace geometry

//GEOMETRY_TETRAHEDRON_H
#endif