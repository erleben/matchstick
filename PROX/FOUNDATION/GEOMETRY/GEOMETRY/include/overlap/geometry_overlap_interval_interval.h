#ifndef GEOMETRY_OVERLAP_INTERVAL_INTERVAL_H
#define GEOMETRY_OVERLAP_INTERVAL_INTERVAL_H

#include <types/geometry_interval.h>

#include <cmath>

namespace geometry
{
  
  template<typename T>
  inline bool overlap_interval_interval(Interval<T> const & A, Interval<T> const & B)
  {
    if( A.upper() < B.lower() )
      return false;

    if( B.upper() < A.lower() )
      return false;

    return true;
  }

}// namespace geometry

// GEOMETRY_OVERLAP_INTERVAL_INTERVAL_H
#endif