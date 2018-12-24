#ifndef GEOMETRY_COMPUTE_RAYCAST_DOP_H
#define GEOMETRY_COMPUTE_RAYCAST_DOP_H

#include <types/geometry_ray.h>
#include <types/geometry_plane.h>
#include <types/geometry_dop.h>

#include <geometry_inside.h>

#include <tiny.h>

#include <cmath>
#include <cassert>

namespace geometry 
{

  template<typename V, size_t K>
  inline bool compute_raycast_dop(
                                  Ray<V> const & ray
                                  , DOP<typename V::real_type,K> const & dop
                                  , V & point
                                  , typename V::real_type & length
                                  , typename V::real_type const & threshold
                                  )
  {
    typedef typename V::real_type     T;
    typedef typename V::value_traits VT;

    assert(threshold >= VT::zero() || !"compute_raycast_dop(): threhold must be non-negative");

    size_t              const N = K/2;
    DirectionTable<V,N> const D = DirectionTableHelper<V,N>::make();

    length   = VT::infinity();
    point    = V::zero();

    V const & p = ray.origin();
    V const & r = ray.direction();

    T t_min = VT::lowest();
    T t_max = VT::highest();

    for(unsigned int k =  0u; k < N; ++k)
    {
      V const d0 =   -D(k);
      T const w0 =   -dop(k).lower();
      T const f0 =   inner_prod(d0,r);

      V const d1 =   D(k);
      T const w1 =   dop(k).upper();
      T const f1 =   inner_prod(d1,r);

      if(f0 == VT::zero())
        continue;

      T t_near = (w0 - inner_prod(d0,p))  / f0;
      T t_far  = (w1 - inner_prod(d1,p))  / f1;

      assert(is_number(t_near) || !"compute_raycast_dop() NaN");
      assert(is_finite(t_near) || !"compute_raycast_dop() Inf");
      assert(is_number(t_far)  || !"compute_raycast_dop() NaN");
      assert(is_finite(t_far)  || !"compute_raycast_dop() Inf");

      if( t_near > t_far)
      {
        T const tmp = t_far;

        t_far  = t_near;
        t_near = tmp;
      }

      t_min = (t_near > t_min) ? t_near : t_min;
      t_max = (t_far  < t_max) ? t_far  : t_max;
    }

    if( t_min > VT::zero() &&  t_min <= t_max)
    {
      V const q = p + r*t_min;

      if ( outside_dop(q, dop, threshold) )
        return false;

      length = t_min;
      point  = q;

      assert(is_number(length)   || !"compute_raycast_dop() NaN");
      assert(is_finite(length)   || !"compute_raycast_dop() Inf");

      assert(is_number(point(0)) || !"compute_raycast_dop() NaN");
      assert(is_finite(point(0)) || !"compute_raycast_dop() Inf");
      assert(is_number(point(1)) || !"compute_raycast_dop() NaN");
      assert(is_finite(point(1)) || !"compute_raycast_dop() Inf");
      assert(is_number(point(2)) || !"compute_raycast_dop() NaN");
      assert(is_finite(point(2)) || !"compute_raycast_dop() Inf");
      
      return true;
    }

    return false;
  }



  template<typename V, size_t K>
  inline bool compute_raycast_dop(
                                  Ray<V> const & ray
                                  , DOP<typename V::real_type,K> const & dop
                                  , V & point
                                  , typename V::real_type & length
                                  )
  {
    typedef typename V::value_traits VT;

    return compute_raycast_dop<V,K>(ray, dop, point, length, VT::zero() );
  }


} //namespace geometry

// GEOMETRY_COMPUTE_RAYCAST_DOP_H
#endif 