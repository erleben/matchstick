#ifndef GEOMETRY_IS_VALID_H
#define GEOMETRY_IS_VALID_H

#include <types/geometry_interval.h>
#include <types/geometry_dop.h>
#include <types/geometry_line.h>
#include <types/geometry_plane.h>
#include <types/geometry_obb.h>
#include <types/geometry_ray.h>
#include <types/geometry_sphere.h>
#include <types/geometry_tetrahedron.h>
#include <types/geometry_triangle.h>
#include <types/geometry_cylinder.h>
#include <types/geometry_aabb.h>

#include <tiny_precision.h>   // needed for tiny::working_precision

#include <cmath>

namespace geometry
{
  template<typename V>
  inline bool is_valid(Capsule<V> const & capsule)
  {
    typedef typename V::value_traits  VT;

    using std::fabs;

    if( !is_number( capsule.radius() ) )
      return false;
    if(  !is_finite( capsule.radius() ) )
      return false;
    if (capsule.radius() <= VT::zero() )
      return false;


    if( !is_number( capsule.point0()(0) ) )
      return false;
    if(  !is_finite( capsule.point0()(0) ) )
      return false;
    if( !is_number( capsule.point0()(1) ) )
      return false;
    if(  !is_finite( capsule.point0()(1) ) )
      return false;
    if( !is_number( capsule.point0()(2) ) )
      return false;
    if(  !is_finite( capsule.point0()(2) ) )
      return false;

    if( !is_number( capsule.point1()(0) ) )
      return false;
    if(  !is_finite( capsule.point1()(0) ) )
      return false;
    if( !is_number( capsule.point1()(1) ) )
      return false;
    if(  !is_finite( capsule.point1()(1) ) )
      return false;
    if( !is_number( capsule.point1()(2) ) )
      return false;
    if(  !is_finite( capsule.point1()(2) ) )
      return false;
    return true;
  }

  template<typename V>
  inline bool is_valid(Cylinder<V> const & cylinder)
  {
    typedef typename V::real_type     T;
    typedef typename V::value_traits  VT;

    using std::fabs;

    if( !is_number( cylinder.radius() ) )
      return false;
    if(  !is_finite( cylinder.radius() ) )
      return false;
    if (cylinder.radius() <= VT::zero() )
      return false;

    if( !is_number( cylinder.height() ) )
      return false;
    if(  !is_finite( cylinder.height() ) )
      return false;
    if (cylinder.height() <= VT::zero() )
      return false;

    if( !is_number( cylinder.axis()(0) ) )
      return false;
    if(  !is_finite( cylinder.axis()(0) ) )
      return false;
    if( !is_number( cylinder.axis()(1) ) )
      return false;
    if(  !is_finite( cylinder.axis()(1) ) )
      return false;
    if( !is_number( cylinder.axis()(2) ) )
      return false;
    if(  !is_finite( cylinder.axis()(2) ) )
      return false;
    if (  fabs( VT::one() - norm(cylinder.axis() ) ) > tiny::working_precision<T>() )
      return false;

    if( !is_number( cylinder.center()(0) ) )
      return false;
    if(  !is_finite( cylinder.center()(0) ) )
      return false;
    if( !is_number( cylinder.center()(1) ) )
      return false;
    if(  !is_finite( cylinder.center()(1) ) )
      return false;
    if( !is_number( cylinder.center()(2) ) )
      return false;
    if(  !is_finite( cylinder.center()(2) ) )
      return false;
    return true;
  }

  template<typename T>
  inline bool is_valid(Interval<T> const & I)
  {
    if(!is_number(I.lower()))
      return false;
    if(!is_number(I.upper()))
      return false;
    if(!is_finite(I.lower()))
      return false;
    if(!is_finite(I.upper()))
      return false;
    if(I.lower() > I.upper())
      return false;
    return true;
  }

  template<typename T, size_t K>
  inline bool is_valid(DOP<T,K> const & D)
  {
    size_t const N = K/2;
    for(size_t k = 0u;k < N; ++k)
    {
      if(!is_valid(D(k)))
        return false;
    }
    return true;
  }

  template<typename V>
  inline bool is_valid(Line<V> const & line)
  {
    typedef typename V::real_type     T;
    typedef typename V::value_traits  VT;

    using std::fabs;

    if( !is_number( line.point()(0) ) )
      return false;
    if(  !is_finite( line.point()(0) ) )
      return false;
    if( !is_number( line.point()(1) ) )
      return false;
    if(  !is_finite( line.point()(1) ) )
      return false;
    if( !is_number( line.point()(2) ) )
      return false;
    if(  !is_finite( line.point()(2) ) )
      return false;
    if( !is_number( line.direction()(0) ) )
      return false;
    if(  !is_finite( line.direction()(0) ) )
      return false;
    if( !is_number( line.direction()(1) ) )
      return false;
    if(  !is_finite( line.direction()(1) ) )
      return false;
    if( !is_number( line.direction()(2) ) )
      return false;
    if(  !is_finite( line.direction()(2) ) )
      return false;
    if (  fabs( VT::one() - length(line.direction() ) ) > tiny::working_precision<T>() )
      return false;
    return true;
  }

  template<typename MT>
  inline bool is_valid(OBB<MT> const & obb)
  {
    typedef typename MT::real_type     T;
    typedef typename MT::value_traits  VT;

    using std::fabs;

    if( !is_number( obb.center()(0) ) )
      return false;
    if(  !is_finite( obb.center()(0) ) )
      return false;
    if( !is_number( obb.center()(1) ) )
      return false;
    if(  !is_finite( obb.center()(1) ) )
      return false;
    if( !is_number( obb.center()(2) ) )
      return false;
    if(  !is_finite( obb.center()(2) ) )
      return false;


    if( !is_number( obb.half_extent()(0) ) )
      return false;
    if(  !is_finite( obb.half_extent()(0) ) )
      return false;
    if( !is_number( obb.half_extent()(1) ) )
      return false;
    if(  !is_finite( obb.half_extent()(1) ) )
      return false;
    if( !is_number( obb.half_extent()(2) ) )
      return false;
    if(  !is_finite( obb.half_extent()(2) ) )
      return false;

    if( obb.half_extent()(0) <= VT::zero()  )
      return false;
    if( obb.half_extent()(1) <= VT::zero()  )
      return false;
    if( obb.half_extent()(2) <= VT::zero()  )
      return false;


    if( !is_number( obb.orientation().real() ) )
      return false;
    if(  !is_finite( obb.orientation().real() ) )
      return false;

    if( !is_number( obb.orientation().imag()(0) ) )
      return false;
    if(  !is_finite( obb.orientation().imag()(0) ) )
      return false;
    if( !is_number( obb.orientation().imag()(1) ) )
      return false;
    if(  !is_finite( obb.orientation().imag()(1) ) )
      return false;
    if( !is_number( obb.orientation().imag()(2) ) )
      return false;
    if(  !is_finite( obb.orientation().imag()(2) ) )
      return false;

    if (  fabs( VT::one() - norm(obb.orientation() ) ) > tiny::working_precision<T>() )
      return false;
    
    return true;
  }

  template<typename V>
  inline bool is_valid(Plane<V> const & plane)
  {
    typedef typename V::real_type     T;
    typedef typename V::value_traits  VT;

    using std::fabs;

    if( !is_number( plane.normal()(0) ) )
      return false;
    if(  !is_finite( plane.normal()(0) ) )
      return false;
    if( !is_number( plane.normal()(1) ) )
      return false;
    if(  !is_finite( plane.normal()(1) ) )
      return false;
    if( !is_number( plane.normal()(2) ) )
      return false;
    if(  !is_finite( plane.normal()(2) ) )
      return false;

    if (  fabs( VT::one() - norm(plane.normal() ) ) > tiny::working_precision<T>() )
      return false;

    if( !is_number( plane.offset() ) )
      return false;
    if(  !is_finite( plane.offset() ) )
      return false;

    return true;
  }

  template<typename V>
  inline bool is_valid(Ray<V> const & ray)
  {
    typedef typename V::real_type     T;
    typedef typename V::value_traits  VT;

    using std::fabs;

    if( !is_number( ray.origin()(0) ) )
      return false;
    if(  !is_finite( ray.origin()(0) ) )
      return false;
    if( !is_number( ray.origin()(1) ) )
      return false;
    if(  !is_finite( ray.origin()(1) ) )
      return false;
    if( !is_number( ray.origin()(2) ) )
      return false;
    if(  !is_finite( ray.origin()(2) ) )
      return false;

    if( !is_number( ray.direction()(0) ) )
      return false;
    if(  !is_finite( ray.direction()(0) ) )
      return false;
    if( !is_number( ray.direction()(1) ) )
      return false;
    if(  !is_finite( ray.direction()(1) ) )
      return false;
    if( !is_number( ray.direction()(2) ) )
      return false;
    if(  !is_finite( ray.direction()(2) ) )
      return false;

    if (  fabs( VT::one() - norm(ray.direction() ) ) > tiny::working_precision<T>() )
      return false;
    return true;
  }

  template<typename V>
  inline bool is_valid(Sphere<V> const & sphere)
  {
    typedef typename V::value_traits  VT;

    if( !is_number( sphere.center()(0) ) )
      return false;
    if(  !is_finite( sphere.center()(0) ) )
      return false;
    if( !is_number( sphere.center()(1) ) )
      return false;
    if(  !is_finite( sphere.center()(1) ) )
      return false;
    if( !is_number( sphere.center()(2) ) )
      return false;
    if(  !is_finite( sphere.center()(2) ) )
      return false;

    if( !is_number( sphere.radius() ) )
      return false;
    if(  !is_finite( sphere.radius() ) )
      return false;

    if(   sphere.radius() <= VT::zero() )
      return false;

    return true;
  }

  template<typename V>
  inline bool is_valid(Tetrahedron<V> const & tet)
  {
    for(unsigned int idx= 0u; idx < 4u;++idx)
    {
      if( !is_number( tet.point(idx)(0) ) )
        return false;
      if(  !is_finite( tet.point(idx)(0) ) )
        return false;

      if( !is_number( tet.point(idx)(1) ) )
        return false;
      if(  !is_finite( tet.point(idx)(1) ) )
        return false;

      if( !is_number( tet.point(idx)(2) ) )
        return false;
      if(  !is_finite( tet.point(idx)(2) ) )
        return false;
    }

    return true;
  }

  template<typename V>
  inline bool is_valid(Triangle<V> const & tet)
  {
    for(unsigned int idx= 0u; idx < 3u;++idx)
    {
      if( !is_number( tet.point(idx)(0) ) )
        return false;
      if(  !is_finite( tet.point(idx)(0) ) )
        return false;

      if( !is_number( tet.point(idx)(1) ) )
        return false;
      if(  !is_finite( tet.point(idx)(1) ) )
        return false;

      if( !is_number( tet.point(idx)(2) ) )
        return false;
      if(  !is_finite( tet.point(idx)(2) ) )
        return false;
    }

    return true;
  }

  template<typename V>
  inline bool is_valid(AABB<V> const & aabb)
  {
    if( !is_number( aabb.min()(0) ) )
      return false;
    if(  !is_finite( aabb.min()(0) ) )
      return false;

    if( !is_number( aabb.min()(1) ) )
      return false;
    if(  !is_finite( aabb.min()(1) ) )
      return false;

    if( !is_number( aabb.min()(2) ) )
      return false;
    if(  !is_finite( aabb.min()(2) ) )
      return false;

    if( !is_number( aabb.max()(0) ) )
      return false;
    if(  !is_finite( aabb.max()(0) ) )
      return false;

    if( !is_number( aabb.max()(1) ) )
      return false;
    if(  !is_finite( aabb.max()(1) ) )
      return false;

    if( !is_number( aabb.max()(2) ) )
      return false;
    if(  !is_finite( aabb.max()(2) ) )
      return false;

    if (aabb.min()(0) > aabb.max()(0))
      return false;
    if (aabb.min()(1) > aabb.max()(1))
      return false;
    if (aabb.min()(2) > aabb.max()(2))
      return false;

    return true;
  }

}// namespace geometry

// GEOMETRY_IS_VALID_H
#endif