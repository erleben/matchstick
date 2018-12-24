#ifndef GEOMETRY_TRANSFORM_H
#define GEOMETRY_TRANSFORM_H

#include <types/geometry_obb.h>
#include <types/geometry_cylinder.h>

#include <tiny.h>

#include <cmath>

namespace geometry
{

  /**
   * TAG Dispatching type for selecting the proper kind of transformation.
   *
   * Use this TAG to specify that it is a "point" that is being transformed from one coordinate system to another.
   */
  struct TRANSFORM_POINT {};

  /**
   * TAG Dispatching type for selecting the proper kind of transformation.
   *
   * Use this TAG to specify that it is a "vector" that is being transformed from one coordinate system to another.
   */
  struct TRANSFORM_VECTOR {};

  template<typename MT>
  inline typename MT::vector3_type transform_from_obb( typename MT::vector3_type const & p, OBB<MT> const & box, TRANSFORM_POINT const & /*tag*/)
  {
    return rotate( box.orientation(), p ) + box.center();
  }

  template<typename MT>
  inline typename MT::vector3_type transform_from_obb( typename MT::vector3_type const & p, OBB<MT> const & box)
  {
    return transform_from_obb(p, box, TRANSFORM_POINT() );
  }

  template<typename MT>
  inline typename MT::vector3_type transform_to_obb( typename MT::vector3_type const & v, OBB<MT> const & box, TRANSFORM_VECTOR const & /*tag*/)
  {
    return rotate( conj( box.orientation() ), v );
  }

  template<typename MT>
  inline typename MT::vector3_type transform_to_obb( typename MT::vector3_type const & p, OBB<MT> const & box, TRANSFORM_POINT const & /*tag*/)
  {
    return rotate( conj( box.orientation() ), (p - box.center()) );
  }

  template<typename MT>
  inline typename MT::vector3_type transform_to_obb( typename MT::vector3_type const & p, OBB<MT> const & box)
  {
    return transform_to_obb( p, box, TRANSFORM_POINT() );
  }

  template<typename V>
  inline V transform_to_cylinder( V const & p, Cylinder<V> const & cylinder, TRANSFORM_POINT const & /*tag*/)
  {
    using std::acos;
    using std::min;
    using std::max;

    typedef typename V::value_traits    VT;
    typedef typename V::real_type        T;
    typedef          tiny::MathTypes<T>  MT;
    typedef typename MT::quaternion_type Q;


      // A cylinders default orientation is with axis aligned with V::k() axis
    V const axis        =  cross( V::k(), cylinder.axis());

    T const angle       =  acos(
                                min(
                                    VT::one()
                                    , max(
                                          - VT::one()
                                          , inner_prod( V::k(), cylinder.axis() )
                                          )
                                    )
                                );

    Q const orientation = Q::Ru( angle, axis);

    return rotate( conj( orientation ), (p - cylinder.center()) );
  }

  template<typename V>
  inline V transform_to_cylinder( V const & p, Cylinder<V> const & cylinder )
  {
    return transform_to_cylinder( p, cylinder, TRANSFORM_POINT() );
  }

  template<typename V>
  inline V transform_from_cylinder( V const & p, Cylinder<V> const & cylinder, TRANSFORM_POINT const & /*tag*/)
  {
    using std::acos;
    using std::min;
    using std::max;

    typedef typename V::value_traits    VT;
    typedef typename V::real_type        T;
    typedef          tiny::MathTypes<T>  MT;
    typedef typename MT::quaternion_type Q;


      // A cylinders default orientation is with axis aligned with V::k() axis
    V const axis        =  cross( V::k(), cylinder.axis());

    T const angle       =  acos(
                                min(
                                    VT::one()
                                    , max(
                                          - VT::one()
                                          , inner_prod( V::k(), cylinder.axis() )
                                          )
                                    )
                                );

    Q const orientation = Q::Ru( angle, axis);

    return rotate( orientation , p ) + cylinder.center();
  }

  template<typename V>
  inline V transform_from_cylinder( V const & p, Cylinder<V> const & cylinder )
  {
    return transform_from_cylinder( p, cylinder, TRANSFORM_POINT());
  }

}// namespace geometry

  // GEOMETRY_TRANSFORM_H
#endif