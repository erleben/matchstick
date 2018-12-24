#ifndef GEOMETRY_MAKE_UNION_H
#define GEOMETRY_MAKE_UNION_H

#include <types/geometry_dop.h>

namespace geometry
{

  template<typename T, size_t K>
  inline DOP<T, K> make_union(DOP<T, K> const & A, DOP<T, K> const & B)
  {
    using std::min;
    using std::max;

    DOP<T, K> kdop;

    size_t const N = K/2;

    for(size_t k =  0u; k < N; ++k)
    {
      kdop(k).lower() = min( A(k).lower(), B(k).lower() );
      kdop(k).upper() = max( A(k).upper(), B(k).upper() );
    }
    return kdop;
  }

}// namespace geometry

// GEOMETRY_MAKE_UNION_H
#endif