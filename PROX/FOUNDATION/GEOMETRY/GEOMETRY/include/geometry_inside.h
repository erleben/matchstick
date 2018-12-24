#ifndef GEOMETRY_INSIDE_H
#define GEOMETRY_INSIDE_H

#include <types/geometry_aabb.h>
#include <types/geometry_dop.h>
#include <types/geometry_obb.h>
#include <types/geometry_cylinder.h>
#include <types/geometry_triangle.h>
#include <types/geometry_plane.h>

#include <geometry_transform.h>

#include <cmath>

namespace geometry
{

  template<typename T, size_t K,typename V>
  inline bool outside_dop( V const & p, DOP<T,K> const & dop, T const & threshold )
  {
    typedef tiny::ValueTraits<T> VT;

    assert(threshold >= VT::zero()  || !"outside_dop(): threshold must be non-negative");

    size_t              const N = K/2;
    DirectionTable<V,N> const D = DirectionTableHelper<V,N>::make();

    for(size_t k =  0u; k < N; ++k)
    {
      T const o = inner_prod( D(k), p );

      if (o < dop(k).lower() - threshold )
        return true;

      if (o > dop(k).upper() + threshold)
        return true;
    }

    return false;
  }

  template<typename T, size_t K,typename V>
  inline bool outside_dop( V const & p, DOP<T,K> const & dop )
  {
    typedef tiny::ValueTraits<T> VT;

    return outside_dop(p, dop, VT::zero() );
  }

  template<typename T, size_t K,typename V>
  inline bool inside_dop( V const & p, DOP<T,K> const & dop )
  {
    return ! outside_dop(p, dop);
  }

  template<typename V>
  inline bool inside_sphere(V const & p, Sphere<V> const & sphere)
  {
    typedef typename V::real_type      T;

    V const p_local          =  p-sphere.center();
    T const distance_squared = tiny::inner_prod( p_local, p_local );

    if (distance_squared > sphere.radius()*sphere.radius() )
      return false;

    return true;
  }

  template<typename V>
  inline bool inside_aabb(V const & p, AABB<V> const & box)
  {

    if( p(0) > box.max()(0) )
      return false;

    if( p(0) < box.min()(0) )
      return false;

    if( p(1) > box.max()(1) )
      return false;

    if( p(1) < box.min()(1) )
      return false;

    if( p(2) > box.max()(2) )
      return false;

    if( p(2) < box.min()(2) )
      return false;

    return true;
  }

  template<typename MT>
  inline bool inside_obb(typename MT::vector3_type const & p, OBB<MT> const & box)
  {
    typedef typename MT::vector3_type V;

    V const p_local =  abs( transform_to_obb(p, box)  );

    if( p_local(0) > box.half_extent()(0) )
      return false;

    if( p_local(1) > box.half_extent()(1) )
      return false;

    if( p_local(2) > box.half_extent()(2) )
      return false;

    return true;
  }

  template<typename V>
  inline bool inside_cylinder(V const & p, Cylinder<V> const & cylinder)
  {
    typedef typename V::value_traits  VT;
    typedef typename V::real_type      T;

    V const p_local     =  abs( transform_to_cylinder( p, cylinder) );
    T const half_height = cylinder.height()/VT::two();

    if ( p_local(2) > half_height )
      return false;

    T const distance = tiny::norm(V::make(p_local(0), p_local(1), VT::zero()) );

    if (distance > cylinder.radius() )
      return false;

    return true;
  }

  template<typename V>
  inline bool inside_triangle(
                              V const & p
                              , Triangle<V> const & triangle
                              , bool const & test_face_plane = true
                              )
  {
    typedef typename V::value_traits   VT;

    Plane<V> const plane = make_plane(triangle);

    if (test_face_plane && get_distance(p, plane) > VT::zero() )
      return false;

    for (unsigned int k=0u; k < 3u; ++k)
    {
      V const & p0 = triangle.p(  k       );
      V const & p1 = triangle.p( (k+1)%3u );
      V const   p2 = p1 + plane.n();

      Plane<V> const wall = make_plane(p0,p1,p2);

      if( get_signed_distance(p, wall) > VT::zero() )
        return false;
    }

    return true;
  }

}// namespace geometry

// GEOMETRY_INSIDE_H
#endif