#ifndef PROX_NONNEGATIVE_H
#define PROX_NONNEGATIVE_H

#include <algorithm>   // for std::max
#include <tiny_value_traits.h>

namespace prox
{
  namespace detail
  {
    template <typename T>
    inline static void nonnegative( T const & z_n, T & lambda_n )
    {
      typedef tiny::ValueTraits<T> value_traits;
      
      using std::max;
      lambda_n = max( value_traits::zero(), z_n );
    }
    
  } // namespace detail
} // namespace prox

// PROX_NONNEGATIVE_H
#endif
