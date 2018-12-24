#ifndef SPARSE_SUM_H
#define SPARSE_SUM_H

#include <cassert>

namespace sparse
{
  namespace detail
  {

    template <typename I1, typename I2>
    inline void add(I1 lhs, I1 lhs_last, I2 rhs)
    {
      for (; lhs != lhs_last; ++lhs, ++rhs)
      {
        *rhs += *lhs;
      }
    }
    
    template <typename I1, typename I2, typename IR>
    inline void add(I1 lhs, I1 lhs_last, I2 rhs, IR dest)
    {
      for (; lhs != lhs_last; ++lhs, ++rhs, ++dest)
      {
        *dest = *lhs + *rhs;
      }
    }
    
    template <typename I1, typename I2, typename I3, typename IR>
    inline void add(I1 lhs, I1 lhs_last, I2 rhs1, I3 rhs2, IR dest)
    {
      for (; lhs != lhs_last; ++lhs, ++rhs1, ++rhs2, ++dest)
      {
        *dest = *lhs + *rhs1 + *rhs2;
      }
    }
    
    template <typename I1, typename I2>
    inline void sub(I1 lhs, I1 lhs_last, I2 rhs)
    {
      for (; lhs != lhs_last; ++lhs, ++rhs)
      {
        *rhs -= *lhs;
      }
    }

    template <typename I1, typename I2, typename I3>
    inline void sub(I1 lhs, I1 lhs_last, I2 rhs, I3 dest)
    {
      for (; lhs != lhs_last; ++lhs, ++rhs, ++dest)
      {
        *dest = *lhs - *rhs;
      }
    }
    
  } // namespace detail
  

  /**
   * rhs += lhs
   */
  template <typename B>
  inline void add(Vector<B> const& lhs, Vector<B>& rhs)
  {
    assert(lhs.size() == rhs.size() || !"Vectors must be of same size");   
    
    detail::add(lhs.begin()->begin(), (lhs.end()-1)->begin()+B::size(), rhs.begin()->begin());
  }

  /**
   * rhs += lhs
   */
  // 2010-04-01 Kenny: I noticed that there are no sub-funcitonality with compressed vectors, nor any add(Vector,CompressedVector) version?
  template <typename B,typename VC>
  inline void add(CompressedVector<B,VC> const& lhs, Vector<B>& rhs)
  {
    assert(lhs.nrows() == rhs.size() || !"Vectors must be of same size");   
    
    typename CompressedVector<B,VC>::const_iterator iter = lhs.begin();
    typename CompressedVector<B,VC>::const_iterator end = lhs.end();
    for( ; iter != end; ++iter )
    {
      size_t const row_idx = row(iter);
      sparse::add(*iter, rhs(row_idx));
    }
  }

  /**
   * rhs += lhs
   * @warning not unit tested
   */
  template <typename B,typename VC>
  inline void add(Vector<B> const& lhs, CompressedVector<B,VC>& rhs)
  {
    assert(lhs.nrows() == rhs.size() || !"Vectors must be of same size");   
    
    typename CompressedVector<B,VC>::iterator iter = rhs.begin();
    typename CompressedVector<B,VC>::iterator end = rhs.end();
    for( ; iter != end; ++iter )
    {
      size_t const row_idx = row(iter);
      sparse::add(lhs(row_idx), *iter);
    }
  }

  /**
   * rhs += lhs, iterates over both compressed vectors, ie. linear with the longest, 
   * but still faster than noncompressed, does not expand either vector
   */
  template <typename B,typename VC1,typename VC2>
  inline void add(CompressedVector<B,VC1> const& lhs, CompressedVector<B,VC2>& rhs)
  {
    assert(lhs.nrows() == rhs.nrows() || !"Vectors must be of same size");   
    
    typename CompressedVector<B,VC1>::const_iterator lhs_iter = lhs.begin();
    typename CompressedVector<B,VC1>::const_iterator lhs_end = lhs.end();
    for( ; lhs_iter != lhs_end; ++lhs_iter )
    {
      size_t const row_idx = row(lhs_iter);
      // 2010-04-01 Kenny: Will this extend the fill pattern of rhs if lhs got non-zero blocks that do not match with rhs fill-pattern?
      sparse::add(*lhs_iter, rhs(row_idx));
    }
  }
  
  /**
   * rhs += lhs assuming that lhs and rhs has identical distribution of nonzero elements
   */
  template <typename B,typename VC1,typename VC2>
  // 2010-04-01 Kenny: I think name is misleading dimensions are the same, what you write in doxygen comment is that fill-pattern must be the same
  inline void add_same_dimension(CompressedVector<B,VC1> const& lhs, CompressedVector<B,VC2>& rhs)
  {
    assert(lhs.nrows() == rhs.nrows() || !"Vectors must be of same size");
    assert(lhs.size() == rhs.size() || !"Vectors must be of same size");
    
    // 2010-04-01 Kenny: Should there be some error testing for incompatible fill-patterns? some asserts or something?
    for(size_t i = 0u ; i < lhs.size() ; ++i )
    {
      sparse::add(lhs[i], rhs[i]);
    }
  }

  /**
   * res = lhs + rhs
   */
  template <typename B>
  inline void add(Vector<B> const& lhs, Vector<B> const& rhs, Vector<B>& res)
  {
    assert(lhs.size() == rhs.size() || !"Vectors must be of same size");
    if (res.size() != lhs.size())
    {
      res.resize(lhs.size());
    }
    detail::add(lhs.begin()->begin(), (lhs.end()-1)->begin()+B::size(), rhs.begin()->begin(), res.begin()->begin());
  }

  /**
   * res = lhs + rhs
   */
  template <typename B,typename VC>
  inline void add(CompressedVector<B,VC> const& lhs, Vector<B> const& rhs, Vector<B>& res)
  {
    assert(lhs.nrows() == rhs.size() || !"Vectors must be of same size");
    assert(lhs.nrows() == res.size() || !"Vectors must be of same size");
    
    typename CompressedVector<B,VC>::const_iterator iter = lhs.begin();
    typename CompressedVector<B,VC>::const_iterator end = lhs.end();
    for( ; iter != end; ++iter )
    {
      size_t const row_idx = row(iter);
      sparse::add(*iter, rhs(row_idx), res(row_idx));
    }
  }

  /**
   * res = lhs + rhs1 + rhs2
   */
  template <typename B>
  inline void add(Vector<B> const& lhs, Vector<B> const& rhs1, Vector<B> const& rhs2, Vector<B>& res)
  {
    assert(lhs.size() == rhs1.size() || !"Vectors must be of same size");
    assert(lhs.size() == rhs2.size() || !"Vectors must be of same size");
    if (res.size() != lhs.size())
    {
      res.resize(lhs.size());
    }
    detail::add(lhs.begin()->begin(), (lhs.end()-1)->begin()+B::size(), rhs1.begin()->begin(), rhs2.begin()->begin(), res.begin()->begin());
  }

  /**
   * rhs -= lhs
   */
  template <typename B>
  inline void sub(Vector<B> const& lhs, Vector<B>& rhs)
  {
    assert(lhs.size() == rhs.size() || !"Vectors must be of same size");
    detail::sub(lhs.begin()->begin(), (lhs.end()-1)->begin()+B::size(), rhs.begin()->begin());
  }

  /**
   * res = lhs - rhs
   */
  template <typename B>
  inline void sub(Vector<B> const& lhs, Vector<B> const& rhs, Vector<B>& res)
  {
    assert(lhs.size() == rhs.size() || !"Vectors must be of same size");
    if (res.size() != lhs.size())
    {
      res.resize(lhs.size());
    }
    detail::sub(lhs.begin()->begin(), (lhs.end()-1)->begin()+B::size(), rhs.begin()->begin(), res.begin()->begin());
  }

} // namespace sparse

// SPARSE_SUM_H
#endif 
