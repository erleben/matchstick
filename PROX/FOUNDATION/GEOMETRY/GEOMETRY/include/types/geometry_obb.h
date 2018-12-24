#ifndef GEOMETRY_OBB_H
#define GEOMETRY_OBB_H

#include <types/geometry_box.h>

#include <cassert>

namespace geometry 
{

  /**
   * Oriented Bounding Box (OBB).
   * This is a Box that are subjected to a rigid body transformation (rotation and translation).
   */
  template<typename MT>
  class OBB
  {
  public:
    
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type Q;
    typedef typename MT::matrix3x3_type  M;
    typedef typename MT::real_type       T;
    typedef typename MT::value_traits    VT;
    
  protected:
    
    V m_center;
    Q m_orientation;
    V m_half_extent;
    
  public:
    
    V const & center()      const   { return this->m_center; }
    Q const & orientation() const   { return this->m_orientation; }
    V const & half_extent() const   { return this->m_half_extent; }
    V       & center()              { return this->m_center; }
    Q       & orientation()         { return this->m_orientation; }
    V       & half_extent()         { return this->m_half_extent; }

  public:
    
    OBB()
    : m_center( V::zero() )
    , m_orientation( Q::identity() )
    , m_half_extent( V::make(VT::one(),VT::one(),VT::one()) )
    {}

    OBB( V const & center, Q const & orientation, V const &  half_extent )
    : m_center( center )
    , m_orientation( orientation )
    , m_half_extent( half_extent )
    {}

    OBB(OBB const & box)
    {
      *this = box;
    }

    OBB & operator=(OBB const & box)
    {
      if( this != &box)
      {
        this->m_center      = box.m_center;
        this->m_orientation = box.m_orientation;
        this->m_half_extent = box.m_half_extent;
      }
      return *this;
    }

  };
  
  template<typename MT>
  inline OBB<MT> make_obb(
               typename MT::vector3_type const & center
               , typename MT::quaternion_type const & orientation
               , typename MT::vector3_type const &  half_extent
               )
  {
    return OBB<MT>( center, orientation, half_extent);
  }


  template<typename MT>
  inline OBB<MT> make_obb(
                          typename MT::vector3_type const & center
                          , typename MT::quaternion_type const & orientation
                          , Box<typename MT::vector3_type> const & box
                          )
  {
    return OBB<MT>( center, orientation, box.half_extent() );
  }

  /**
   * Returns the local-space coordiantes of a specifed corner point.
   *
   * To retrive the corner point in global (world) space use the transfrom_from_obb function.
   */
  template<typename MT>
  inline typename MT::vector3_type get_local_corner(size_t const & idx, OBB<MT> const & box)
  {
    assert(idx < 8u || !"get_local_corner(): Idx was out of range");

    typedef typename MT::real_type    T;
    typedef typename MT::vector3_type V;

    T const x = ((idx   ) & 0x0001u) ? box.half_extent()(0) : -box.half_extent()(0);
    T const y = ((idx>>1) & 0x0001u) ? box.half_extent()(1) : -box.half_extent()(1);
    T const z = ((idx>>2) & 0x0001u) ? box.half_extent()(2) : -box.half_extent()(2);

    return V::make(x,y,z);
  }

} //namespace geometry

// GEOMETRY_OBB_H
#endif
