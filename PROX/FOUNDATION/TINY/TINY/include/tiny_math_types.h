#ifndef TINY_MATH_TYPES_H
#define TINY_MATH_TYPES_H

#include <tiny_vector.h>
#include <tiny_matrix.h>
#include <tiny_quaternion.h>
#include <tiny_coordsys.h>
#include <tiny_value_traits.h>
#include <tiny_type_traits.h>

namespace tiny
{
  
  /**
   * Math Types Container.
   * This class contains the most common used small-sized matrix
   * types. It is intended as a convience tool making. It makes it easier to
   * pass math types around as a small collection of types for instance as
   * template arguments for data structures or algorithms.
   *
   * @tparam T   The precision of the math types. Typical either float or double.
   */
  template< typename T >
  class MathTypes;
  
  template<  >
  class MathTypes<float>
  {
  public:
    
    typedef float_traits                  type_traits;
    typedef type_traits::real_type        real_type;
    typedef Vector<3,type_traits>         vector3_type;
    typedef Vector<4,type_traits>         vector4_type;
    
    typedef Quaternion<type_traits>       quaternion_type;
    typedef Matrix<3,3,type_traits>       matrix3x3_type;
    typedef Matrix<4,4,type_traits>       matrix4x4_type;
    typedef CoordSys<type_traits>         coordsys_type;
    typedef ValueTraits<real_type>        value_traits;
  };
  
  template<  >
  class MathTypes<double>
  {
  public:
    
    typedef double_traits                 type_traits;
    typedef type_traits::real_type        real_type;
    typedef Vector<3,type_traits>         vector3_type;
    typedef Vector<4,type_traits>         vector4_type;
    
    typedef Quaternion<type_traits>       quaternion_type;
    typedef Matrix<3,3,type_traits>       matrix3x3_type;
    typedef CoordSys<type_traits>         coordsys_type;
    typedef ValueTraits<real_type>        value_traits;
  };
  
} // namespace tiny

//TINY_MATH_TYPES_H
#endif
