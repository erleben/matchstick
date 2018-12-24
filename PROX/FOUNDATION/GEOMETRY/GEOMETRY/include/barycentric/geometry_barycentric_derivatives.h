#ifndef GEOMETRY_BARYCENTRIC_DERIVATIVES_H
#define GEOMETRY_BARYCENTRIC_DERIVATIVES_H

#include <tiny_value_traits.h>
#include <tiny_vector_functions.h>

#include <cassert>

namespace geometry
{

  template<typename V>
  inline void barycentric_derivatives(
                                      V const & x1
                                      , V const & x2
                                      , V const & x3
                                      , V const & x4
                                      , V & nabla_w1
                                      , V & nabla_w2
                                      , V & nabla_w3
                                      , V & nabla_w4
                                      )
  {
    using std::fabs;

    typedef typename V::real_type          T;
    typedef typename tiny::ValueTraits<T> VT;

    T const vol6 = inner_prod(x4 - x1, cross(x2 - x1, x3 - x1 ));

    assert( vol6 > VT::zero() || !"compute_barycentric_derivatives(): tetrahedron was left oriented");

    nabla_w1 = cross( x4 - x2, x3 - x2) / vol6;
    nabla_w2 = cross( x3 - x1, x4 - x1) / vol6;
    nabla_w3 = cross( x4 - x1, x2 - x1) / vol6;
    nabla_w4 = cross( x2 - x1, x3 - x1) / vol6;
  }
  
}//namespace geometry

// GEOMETRY_BARYCENTRIC_DERIVATIVES_H
#endif