#ifndef GEOMETRY_DOP_H
#define GEOMETRY_DOP_H

#include <types/geometry_interval.h>
#include <types/geometry_direction_table.h>

#include <tiny_vector_functions.h>  // Needed for tiny::inner_prod

#include <cstddef>     // Needed for size_t
#include <algorithm>   // Needed for std::max and std::min
#include <cassert>

namespace geometry
{
  
  template<typename T, size_t K>
  class DOP
  {
  protected:
    
    Interval<T> m_slabs[K/2];
    
  public:
    
    Interval<T> const & operator()(size_t const & idx) const
    {
      assert( idx < (K/2) || !"DOP::operator(): idx was out of range");
      
      return this->m_slabs[idx];
    }
    
    Interval<T>       & operator()(size_t const & idx)
    {
      assert( idx < (K/2) || !"DOP::operator(): idx was out of range");
      
      return this->m_slabs[idx];
    }
    
    size_t size() const { return K; }

  public:
    
    DOP()
    {}

  };

  template<typename iterator, typename V, size_t N>
  inline DOP<typename V::real_type, N*2u> make_dop(
                                                   iterator const & begin
                                                   , iterator const & end
                                                   , DirectionTable<V,N> const & D
                                                   )
  {
    using std::min;
    using std::max;
    
    typedef typename V::real_type T;
    
    DOP<T,N*2> kdop;
    
    for(size_t k =  0u; k < N; ++k)
    {
      for(iterator p = begin; p!=end; ++p)
      {
        T const projection = inner_prod( D(k), (*p) );
        
        kdop(k).lower() = min( kdop(k).lower(), projection );
        kdop(k).upper() = max( kdop(k).upper(), projection );
      }
    }
    
    return kdop;
  }

}// namespace geometry

// GEOMETRY_DOP_H
#endif