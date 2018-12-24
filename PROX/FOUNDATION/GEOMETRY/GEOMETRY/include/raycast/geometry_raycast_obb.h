#ifndef GEOMETRY_COMPUTE_RAYCAST_OBB_H
#define GEOMETRY_COMPUTE_RAYCAST_OBB_H

#include <types/geometry_obb.h>
#include <types/geometry_ray.h>

#include <tiny_is_number.h>
#include <tiny_is_finite.h>

#include <cmath>
#include <vector>

namespace geometry 
{

  template<typename M>
  inline bool compute_raycast_obb(
                                   Ray<typename M::vector3_type> const & ray
                                   , OBB<M> const & obb
                                   , typename M::vector3_type & q
                                   , typename M::real_type & length
                                   )
  {
    typedef typename M::value_traits VT;
    typedef typename M::vector3_type V;
    typedef typename M::real_type    T;

    q      = V::zero();
    length = VT::infinity();


    V const p_obb  = transform_to_obb( ray.origin(), obb, TRANSFORM_POINT()     );
    V const r_obb  = transform_to_obb( ray.direction(), obb, TRANSFORM_VECTOR() );

    V const aabb_mincoord = -obb.half_extent();
    V const aabb_maxcoord =  obb.half_extent();

    V q_obb;
    T t_obb;

    bool const did_hit = compute_raycast_aabb(p_obb, r_obb, aabb_mincoord, aabb_maxcoord, q_obb, t_obb);

    if(did_hit)
    {
      length = t_obb;
      q      = transform_from_obb(q_obb, obb, TRANSFORM_POINT() );
      return true;
    }

    return false;
  }


} //namespace geometry

// GEOMETRY_COMPUTE_RAYCAST_OBB_H
#endif 