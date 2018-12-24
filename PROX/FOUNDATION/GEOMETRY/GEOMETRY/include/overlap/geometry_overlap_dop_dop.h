#ifndef GEOMETRY_OVERLAP_DOP_DOP_H
#define GEOMETRY_OVERLAP_DOP_DOP_H

#include <overlap/geometry_overlap_interval_interval.h>
#include <types/geometry_dop.h>

#include <cmath>

namespace geometry
{
  
  template<typename T, size_t K>
  inline bool overlap_dop_dop( DOP<T,K> const & A, DOP<T,K> const & B)
  {
    size_t const N = K/2;

    for(size_t k =  0u; k < N; ++k)
    {
      bool const separation = ! overlap_interval_interval( A(k), B(k) );
      if( separation )
        return false;
    }
    return true;
  }
  
}// namespace geometry

// GEOMETRY_OVERLAP_DOP_DOP_H
#endif