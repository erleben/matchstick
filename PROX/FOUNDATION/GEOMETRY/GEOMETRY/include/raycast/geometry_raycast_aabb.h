#ifndef GEOMETRY_COMPUTE_RAYCAST_AABB_H
#define GEOMETRY_COMPUTE_RAYCAST_AABB_H

#include <types/geometry_aabb.h>
#include <types/geometry_ray.h>

#include <tiny_is_number.h>
#include <tiny_is_finite.h>

#include <cmath>
#include <vector>

namespace geometry 
{

  /**
   * Compute ray and AABB intersection point.
   *
   * @param p            Ray origin
   * @param r            Ray direction
   * @param min_coord    Minimum coordinates of AABB
   * @param max_coord    Maximum coordinates of AABB
   * @param hit          The hit point of ray and AABB if any
   * @param length       The ray distance from p to hit point
   *
   * @return             If ray hits AABB then return value is true orhterwise false
   */
  template<typename V>
  inline bool compute_raycast_aabb(
                                   V const & p
                                   , V const & r
                                   , V const &  min_coord
                                   , V const &  max_coord
                                   , V & hit
                                   , typename V::real_type & length
                                   )
  {
    typedef typename V::value_traits VT;
    typedef typename V::real_type    T;

    using std::fabs;

    length = VT::infinity();
    hit    = V::zero();

    V const e =  (max_coord - min_coord)*VT::half();  //--- Compute half extents of box
    V const d =  p - (e + min_coord);                 //--- Compute vector from center of box to origin of ray

    assert(is_number(e(0)) || !"compute_raycast_aabb() NaN");
    assert(is_finite(e(0)) || !"compute_raycast_aabb() Inf");
    assert(is_number(e(1)) || !"compute_raycast_aabb() NaN");
    assert(is_finite(e(1)) || !"compute_raycast_aabb() Inf");
    assert(is_number(e(2)) || !"compute_raycast_aabb() NaN");
    assert(is_finite(e(2)) || !"compute_raycast_aabb() Inf");

    assert(is_number(d(0)) || !"compute_raycast_aabb() NaN");
    assert(is_finite(d(0)) || !"compute_raycast_aabb() Inf");
    assert(is_number(d(1)) || !"compute_raycast_aabb() NaN");
    assert(is_finite(d(1)) || !"compute_raycast_aabb() Inf");
    assert(is_number(d(2)) || !"compute_raycast_aabb() NaN");
    assert(is_finite(d(2)) || !"compute_raycast_aabb() Inf");

    //--- Test if one of three axes of the box is a separation axe
    if (fabs(d(0)) > e(0) && d(0)*r(0) >= VT::zero())
      return false;

    if (fabs(d(1)) > e(1) && d(1)*r(1) >= VT::zero())
      return false;

    if (fabs(d(2)) > e(2) && d(2)*r(2) >= VT::zero())
      return false;

    //--- Test if one of three cross producs of the axes and the ray direction is a separation axe
    V const rXd = cross(r , d);

    if ( fabs(rXd(0)) > ( e(1)*fabs(r(2)) + e(2)*fabs(r(1)) ) )
      return false;

    if ( fabs(rXd(1)) > ( e(0)*fabs(r(2)) + e(2)*fabs(r(0)) ) )
      return false;

    if ( fabs(rXd(2)) > ( e(0)*fabs(r(1)) + e(1)*fabs(r(0)) ) )
      return false;

    //--- No sepration axe exist, we must have a collision ---------------------

    //--- See if we are completely inside or on AABB with ray origin
    bool const not_outside = fabs( d(0)) <= e(0) && fabs( d(1)) <= e(1) && fabs( d(2)) <= e(2);

    if (not_outside)
    {
      hit    = p;
      length = VT::zero();
      return true;
    }

    //--- Ray origin must be outside of AABB -----------------------------------
    T t_min = VT::lowest();
    T t_max = VT::highest();

    for(unsigned int i=0u; i < 3u; ++i)
    {
      if(r(i) == VT::zero())
        continue;

      T t_near = ((min_coord(i) - p(i)) / r(i));
      T t_far  = ((max_coord(i) - p(i)) / r(i));

      assert(is_number(t_near) || !"compute_raycast_aabb() NaN");
      assert(is_finite(t_near) || !"compute_raycast_aabb() Inf");
      assert(is_number(t_far) || !"compute_raycast_aabb() NaN");
      assert(is_finite(t_far) || !"compute_raycast_aabb() Inf");

      if( t_near > t_far)
      {
        T const tmp = t_far;

        t_far = t_near;
        t_near = tmp;
      }

      t_min = (t_near > t_min) ? t_near : t_min;
      t_max = (t_far  < t_max) ? t_far  : t_max;
    }


    if( t_min > VT::zero() &&  t_min <= t_max)
    {
      length = t_min;
      hit    = p + r*length;

      assert(is_number(length) || !"compute_raycast_aabb() NaN");
      assert(is_finite(length) || !"compute_raycast_aabb() NaN");

      assert(is_number(hit(0)) || !"compute_raycast_aabb() NaN");
      assert(is_finite(hit(0)) || !"compute_raycast_aabb() Inf");
      assert(is_number(hit(1)) || !"compute_raycast_aabb() NaN");
      assert(is_finite(hit(1)) || !"compute_raycast_aabb() Inf");
      assert(is_number(hit(2)) || !"compute_raycast_aabb() NaN");
      assert(is_finite(hit(2)) || !"compute_raycast_aabb() Inf");

      return true;
    }

    return false;
  }

  template<typename V>
  inline bool compute_raycast_aabb(
                                   Ray<V> const & ray
                                   , AABB<V> const & aabb
                                   , V & hit
                                   , typename V::real_type & length
                                   )
  {
    return compute_raycast_aabb(
                                ray.origin()
                                , ray.direction()
                                , aabb.min()
                                , aabb.max()
                                , hit
                                , length
                                );
  }


} //namespace geometry

// GEOMETRY_COMPUTE_RAYCAST_AABB_H
#endif 