#ifndef GEOMETRY_BOX_H
#define GEOMETRY_BOX_H

#include <types/geometry_support_mapping.h>

#include <cassert>

namespace geometry 
{

  template<typename V>
  class Box
  : public geometry::SupportMapping<V>
  {
  public:

    typedef typename V::real_type       T;
    typedef typename V::value_traits    VT;

  protected:

    V m_half_extent;

  public:

    V const & half_extent() const   { return this->m_half_extent; }
    V       & half_extent()         { return this->m_half_extent; }

  public:

    Box()
    : m_half_extent( V::make(VT::one(),VT::one(), VT::one()) )
    {}

    Box(V const &  half_extent )
    : m_half_extent( half_extent )
    {}

    Box(Box const & box)
    {
      *this = box;
    }

    Box & operator=(Box const & box)
    {
      if( this != &box)
      {
        this->m_half_extent = box.m_half_extent;
      }
      return *this;
    }

  public:

    V get_support_point(V const & v) const
    {
      T const & vx = v(0);
      T const & vy = v(1);
      T const & vz = v(2);

      assert( is_number(vx) || !"NAN encountered");
      assert( is_number(vy) || !"NAN encountered");
      assert( is_number(vz) || !"NAN encountered");
      assert( is_finite(vx) || !"INF encountered");
      assert( is_finite(vy) || !"INF encountered");
      assert( is_finite(vz) || !"INF encountered");

      T const & hx = this->m_half_extent(0);
      T const & hy = this->m_half_extent(1);
      T const & hz = this->m_half_extent(2);

      assert( is_number(hx)    || !"NAN encountered");
      assert( is_finite(hx)    || !"INF encountered");
      assert( hx >= VT::zero() || !"Negative half extent encountered");
      assert( is_number(hy)    || !"NAN encountered");
      assert( is_finite(hy)    || !"INF encountered");
      assert( hy >= VT::zero() || !"Negative half extent encountered");
      assert( is_number(hz)    || !"NAN encountered");
      assert( is_finite(hz)    || !"INF encountered");
      assert( hz >= VT::zero() || !"Negative half extent encountered");

      T const px = (vx > VT::zero()) ? hx : - hx;
      T const py = (vy > VT::zero()) ? hy : - hy;
      T const pz = (vz > VT::zero()) ? hz : - hz;

      assert( is_number(px) || !"NAN encountered");
      assert( is_number(py) || !"NAN encountered");
      assert( is_number(pz) || !"NAN encountered");
      assert( is_finite(px) || !"INF encountered");
      assert( is_finite(py) || !"INF encountered");
      assert( is_finite(pz) || !"INF encountered");

      return V::make( px, py, pz);
    }

    T get_scale() const
    {
      using std::min;

      T const & hx = this->m_half_extent(0);
      T const & hy = this->m_half_extent(1);
      T const & hz = this->m_half_extent(2);

      assert( is_number(hx)    || !"NAN encountered");
      assert( is_finite(hx)    || !"INF encountered");
      assert( hx >= VT::zero() || !"Negative half extent encountered");
      assert( is_number(hy)    || !"NAN encountered");
      assert( is_finite(hy)    || !"INF encountered");
      assert( hy >= VT::zero() || !"Negative half extent encountered");
      assert( is_number(hz)    || !"NAN encountered");
      assert( is_finite(hz)    || !"INF encountered");
      assert( hz >= VT::zero() || !"Negative half extent encountered");

      T const w = VT::two() * ((hx > VT::zero()) ? hx : VT::infinity());
      T const h = VT::two() * ((hy > VT::zero()) ? hy : VT::infinity());
      T const d = VT::two() * ((hz > VT::zero()) ? hz : VT::infinity());
      
      return min( w, min(h, d) );
    }

  };

  template<typename V>
  inline Box<V> make_box(V const &  half_extent )
  {
    return Box<V>( half_extent);
  }

  /**
   * Returns the coordiantes of a specifed corner point.
   */
  template<typename V>
  inline V get_corner(size_t const & idx, Box<V> const & box)
  {
    assert(idx < 8u || !"get_local_corner(): Idx was out of range");

    typedef typename V::real_type    T;

    T const x = ((idx   ) & 0x0001u) ? box.half_extent()(0) : -box.half_extent()(0);
    T const y = ((idx>>1) & 0x0001u) ? box.half_extent()(1) : -box.half_extent()(1);
    T const z = ((idx>>2) & 0x0001u) ? box.half_extent()(2) : -box.half_extent()(2);

    return V::make(x,y,z);
  }

} //namespace geometry

// GEOMETRY_BOX_H
#endif
