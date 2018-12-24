#ifndef GEOMETRY_BARYCENTRIC_H
#define GEOMETRY_BARYCENTRIC_H

#include <tiny_math_types.h>
#include <tiny_is_number.h>
#include <tiny_type_traits.h>
#include <tiny_vector.h>
#include <tiny_vector_functions.h>
#include <tiny_matrix_functions.h>

#include <cmath>
#include <cassert>

namespace geometry
{
  /*
   * Compute Barycentric Coordinates.
   * This method computes the barycentric coodinates for a point p of an edge
   * given by the points x1 and x2.
   *
   * The barycentric coordinates w1 and w2 are defined such
   * that p' = w1*x1 + w2*x2, is the point on the line closest to p.
   *
   * if 0 <= w1,w2 <= 1 then the point lies inside or on the perimeter of the triangle.
   *
   * @warning  This method uses a geometric approach to compute the barycentric coordinates.
   *
   * @param x1    The first point of the edge.
   * @param x2    The second point of the edge.
   * @param p     The point for which the barycentric coordinates should be computed.
   * @param w1    Upon return this parameter contains the value of the first barycentric coordinate.
   * @param w2    Upon return this parameter contains the value of the second barycentric coordinate.
   */
  template<typename V>
  inline void barycentric(
                          V const & x1
                          , V const & x2
                          , V const & p
                          , typename V::real_type & w1
                          , typename V::real_type & w2
                          )
  {
    using std::sqrt;

    typedef typename V::real_type      T;
    typedef typename V::value_traits    VT;

    V const u  = x2-x1;
    T const uu = tiny::inner_prod(u,u);

    assert( is_number(uu)   || !"barycentric(): NaN encountered");
    assert( uu > VT::zero() || !"barycentric(): Degenerate edge encountered");

    // Project p onto edge running from x1 to x2.
    V const q  = (tiny::inner_prod(u, p-x1)/ uu )*u + x1;

    V const a  = q - x2;
    V const b  = q - x1;

    T const aa =  tiny::inner_prod(a,a) ;
    T const bb =  tiny::inner_prod(b,b) ;

    assert( is_number(aa) || !"barycentric(): NaN encountered");
    assert( is_number(bb) || !"barycentric(): NaN encountered");

    w1 = ( tiny::inner_prod(u,a) <= VT::zero() ) ? sqrt( aa  / uu ) : - sqrt( aa  / uu );
    w2 = ( tiny::inner_prod(u,b) >= VT::zero() ) ? sqrt( bb  / uu ) : - sqrt( bb  / uu );

    assert( is_number(w1) || !"barycentric(): NaN encountered");
    assert( is_number(w2) || !"barycentric(): NaN encountered");
  }

  /*
   * Compute Barycentric Coordinates.
   * This method computes the barycentric coodinates for a point p of a triangle
   * given by the points x1,x2, and x3 (in counter clockwise order).
   *
   * The barycentric coordinates w1,w2, and w3 are defined such
   * that p' = w1*x1 + w2*x2 + w3*x3, is the point in plane of the
   * triangle closest to p.
   *
   * if 0 <= w1,w2,w3 <= 1 then the point lies inside or on the perimeter of the triangle.
   *
   * @warning  This method uses a geometric approach to compute the barycentric coordinates.
   *
   * @param x1    The first point of the triangle.
   * @param x2    The second point of the triangle.
   * @param x3    The third point of the triangle.
   * @param p     The point for which the barycentric coordinates should be computed.
   * @param w1    Upon return this parameter contains the value of the first barycentric coordinate.
   * @param w2    Upon return this parameter contains the value of the second barycentric coordinate.
   * @param w3    Upon return this parameter contains the value of the third barycentric coordinate.
   */
  template<typename V>
  inline void barycentric(
                           V const & x1
                           , V const & x2
                           , V const & x3
                           , V const & p
                           , typename V::real_type & w1
                           , typename V::real_type & w2
                           , typename V::real_type & w3
                           )
  {
    using std::sqrt;

    typedef typename V::real_type       T;
    typedef typename V::value_traits    VT;
    typedef tiny::MathTypes<T>          MT;
    typedef typename MT::matrix3x3_type M;

    // Before computing the barycentric coordinates, we 'normalize' the triangle to ensure it is not a sliver
    V const b1 = x2 - x1;
    V const b2 = x3 - x1;
    V const b3 = tiny::cross(tiny::unit(b1),tiny::unit(b2));

    M const basis = M::make(  b1(0), b2(0), b3(0)
                            , b1(1), b2(1), b3(1)
                            , b1(2), b2(2), b3(2));

    // q is the point p transformed to the isoparametric system
    V const q = tiny::inverse(basis)*(p - x1);

    w1 = VT::one() - q[0] - q[1];
    w2 = q[0];
    w3 = q[1];

    assert( is_number(w1) || !"barycentric(): NaN encountered");
    assert( is_number(w2) || !"barycentric(): NaN encountered");
    assert( is_number(w3) || !"barycentric(): NaN encountered");
  }

  /*
   * Compute Barycentric Coordinates.
   * This method computes the barycentric coodinates for a point p of a tetrahedron
   * given by the points x1,x2,x3, x4 (in right-hand order).
   *
   * @warning  This method uses a geometric approach to compute the barycentric coordinates.
   *
   * @param x1    The first point of the triangle.
   * @param x2    The second point of the triangle.
   * @param x3    The third point of the triangle.
   * @param x4    The fourth point of the triangle.
   * @param p     The point for which the barycentric coordinates should be computed.
   * @param w1    Upon return this parameter contains the value of the first barycentric coordinate.
   * @param w2    Upon return this parameter contains the value of the second barycentric coordinate.
   * @param w3    Upon return this parameter contains the value of the third barycentric coordinate.
   * @param w4    Upon return this parameter contains the value of the fourth barycentric coordinate.
   */
  template<typename V>
  inline void barycentric(
                           V const & x1
                           , V const & x2
                           , V const & x3
                           , V const & x4
                           , V const & p
                           , typename V::real_type & w1
                           , typename V::real_type & w2
                           , typename V::real_type & w3
                           , typename V::real_type & w4
                           )
  {
    using std::fabs;

    typedef typename V::real_type       T;
    typedef typename V::value_traits    VT;
    typedef tiny::MathTypes<T>          MT;
    typedef typename MT::matrix3x3_type M;

    V const b1 = x2 - x1;
    V const b2 = x3 - x1;
    V const b3 = x4 - x1;

    // check if basis forms a right hand coordsys, if not exchange two base vectors
    if(tiny::inner_prod(b3,tiny::cross(b1, b2)) > 0)
    {
      M const basis = M::make(  b1(0), b2(0), b3(0)
                              , b1(1), b2(1), b3(1)
                              , b1(2), b2(2), b3(2));

      // q is the point p transformed to the isoparametric system
      V const q = tiny::inverse(basis)*(p - x1);

      w1 = VT::one() - q[0] - q[1] - q[2];
      w2 = q[0];
      w3 = q[1];
      w4 = q[2];
    }
    else
    {
      M const  basis = M::make(  b1(0), b3(0), b2(0)
                               , b1(1), b3(1), b2(1)
                               , b1(2), b3(2), b2(2));
      // q is the point p transformed to the isoparametric system
      V const q = tiny::inverse(basis)*(p - x1);

      w1 = VT::one() - q[0] - q[1] - q[2];
      w2 = q[0];
      w3 = q[2];// we alter the sequence of barycentric coords to match altered basis
      w4 = q[1];
    }

    assert( is_number(w1) || !"barycentric(): NaN encountered");
    assert( is_number(w2) || !"barycentric(): NaN encountered");
    assert( is_number(w3) || !"barycentric(): NaN encountered");
    assert( is_number(w4) || !"barycentric(): NaN encountered");
  }

}//namespace geometry

// GEOMETRY_BARYCENTRIC_H
#endif