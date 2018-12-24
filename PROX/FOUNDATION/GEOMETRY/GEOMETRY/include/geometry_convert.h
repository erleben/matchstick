#ifndef GEOMETRY_CONVERT_H
#define GEOMETRY_CONVERT_H

#include <types/geometry_dop.h>
#include <types/geometry_sphere.h>
#include <types/geometry_aabb.h>
#include <types/geometry_cylinder.h>
#include <types/geometry_capsule.h>

namespace geometry
{

  template<size_t K,typename V>
  inline DOP<typename V::real_type,K> convert( Sphere<V> const & S )
  {
    typedef typename V::real_type T;

    size_t              const N = K/2;
    DirectionTable<V,N> const D = DirectionTableHelper<V,N>::make();

    DOP<T, K> kdop;

    for(size_t k =  0u; k < N; ++k)
    {
      T const o = inner_prod( D(k), S.center() );

      kdop(k).lower() = o - S.radius();
      kdop(k).upper() = o + S.radius();
    }

    return kdop;
  }

  template<size_t K,typename V>
  inline DOP<typename V::real_type,K> convert( AABB<V> const & aabb )
  {
    typedef typename V::real_type T;

    size_t              const N = K/2;
    DirectionTable<V,N> const D = DirectionTableHelper<V,N>::make();

    DOP<T, K> kdop;

    for(size_t k =  0u; k < N; ++k)
    {
      T const l = inner_prod(  D(k), aabb.min() );
      T const u = inner_prod(  D(k), aabb.max() );

      kdop(k).lower() = l;
      kdop(k).upper() = u;
    }

    return kdop;
  }

  template<typename V>
  inline Capsule<V> convert( Cylinder<V> const & cylinder )
  {
    typedef typename V::real_type    T;
    typedef typename V::value_traits VT;

    T const radius = cylinder.radius();

    V const point0 = cylinder.center() - cylinder.axis()*cylinder.height()*VT::half();
    V const point1 = cylinder.center() + cylinder.axis()*cylinder.height()*VT::half();

    return make_capsule(radius, point0, point1);
  }


}// namespace geometry

// GEOMETRY_CONVERT_H
#endif