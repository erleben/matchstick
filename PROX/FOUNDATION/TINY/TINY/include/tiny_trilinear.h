#ifndef TINY_TRILINEAR_H
#define TINY_TRILINEAR_H

#include <tiny_value_traits.h> // for value_traits::numeric_cast

namespace tiny
{
  
  /**
   * Trilinear Interpolation.
   * Interpolation of values in a cubic grid.
   */
  template <typename T,typename T2>
  inline T  trilinear(
                         T const & d000
                       , T const & d001
                       , T const & d010
                       , T const & d011
                       , T const & d100
                       , T const & d101
                       , T const & d110
                       , T const & d111
                       , T2 const & s
                       , T2 const & t
                       , T2 const & u
                       ) 
  {
    typedef ValueTraits<T> value_traits;
    
    T const x00 = value_traits::numeric_cast( (d001 - d000)*s + d000 );
    T const x01 = value_traits::numeric_cast( (d011 - d010)*s + d010 );
    T const x10 = value_traits::numeric_cast( (d101 - d100)*s + d100 );
    T const x11 = value_traits::numeric_cast( (d111 - d110)*s + d110 );
    T const y0  = value_traits::numeric_cast( ( x01 -  x00)*t +  x00 );
    T const y1  = value_traits::numeric_cast( ( x11 -  x10)*t +  x10 );
    return value_traits::numeric_cast( (y1-y0)*u + y0 );
  }
  
} // namespace tiny

// TINY_TRILINEAR_H
#endif
