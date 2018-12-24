#ifndef SPARSE_PROPERTY_MAPS_H
#define SPARSE_PROPERTY_MAPS_H

// 2009-07-01: File name may not reflect content?

#include <sparsefwd.h>

#include <cassert>

namespace sparse
{    
  
  // 2009-07-01 Kenny: Documentation is needed, it appears that functions return row and col values corresponding to a given []-type index on a matrix?
  
  template<typename B>
  __attribute__((always_inline)) size_t row(size_t const i, CompressedRowMatrix<B> const& src)
  {
    typedef typename CompressedRowMatrix<B>::accessor A;
    assert(i < src.size()  || !"i was too large");
    return A::find_row(i, src);
  }
  
  template<typename B>
  __attribute__((always_inline)) size_t col(size_t const i, CompressedRowMatrix<B> const& src)
  {
    typedef typename CompressedRowMatrix<B>::accessor A;
    assert(i < src.size() || !"i was too large");
    return A::cols(src)[i];
  }
  
  template<typename B>
  __attribute__((always_inline)) size_t row(size_t const i, DiagonalMatrix<B> const& src)
  {
    assert(i < src.size() || !"i was too large");
    return i;
  }
  
  template<typename B>
  __attribute__((always_inline)) size_t col(size_t const i, DiagonalMatrix<B> const& src)
  {
    assert(i < src.size() || !"i was too large");
    return i;
  }
  
  template<typename B>
  __attribute__((always_inline)) size_t row(size_t const i, Vector<B> const& src)
  {
    assert(i < src.size() || !"i was too large");
    return i;
  }
  
  template<typename B>
  __attribute__((always_inline)) size_t col(size_t const i, Vector<B> const& src)
  {
    assert(i < src.size() || !"i was too large");
    return 1u;
  }
    
  template<typename B>
  __attribute__((always_inline)) size_t row(size_t const i, TwoColumnMatrix<B> const& src)
  {
    assert(i < src.size() || !"i was too large");
    return i >> 1;
  }
  
  template<typename B>
  __attribute__((always_inline)) size_t col(size_t const i, TwoColumnMatrix<B> const& src)
  {
    typedef typename TwoColumnMatrix<B>::accessor A;
    assert(i < src.size() || !"i was too large" );
    return A::cols(src)[i];
  }
  
} // namespace sparse

// SPARSE_PROPERTY_MAPS_H
#endif 
