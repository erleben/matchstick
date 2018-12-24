#ifndef SPARSE_PROD_BLAS2_H
#define SPARSE_PROD_BLAS2_H

#include <sparsefwd.h>

#include <cstring>  // for memset
#include <cassert>

namespace sparse
{
    
  /**
   * Optimization for scalar case, as using a temporary in inner loop
   * delivers a huge performance gain if temporary is small
   */
  template <typename T>
  inline void prod(
                    CompressedRowMatrix<Block<1,1,T> > const& lhs
                   , Vector<Block<1,1,T> > const& rhs
                   , Vector<Block<1,1,T> >& res
                   , bool init = false
                   )
  {
    assert(lhs.ncols() == rhs.nrows() || !"number of lhs columns must be the same as number of right hand side rows");
    
    if (init)
    {
      memset(&res[0], 0, sizeof(Block<1,1,T>)*res.size());
    }
    if (res.nrows() != lhs.nrows())
    {
      res.resize(lhs.nrows());
    }
    
    typedef typename CompressedRowMatrix<Block<1,1,T> >::const_row_iterator const_row_iterator;
    
    const_row_iterator iter;
    const_row_iterator last;
    for (size_t i = 0; i < lhs.top_non_zero_row(); ++i)
    {
      Block<1,1,T> t(res(i));
      iter = lhs.row_begin(i);
      last = lhs.row_end(i);
      for (; iter != last; ++iter)
      {
        prod(*iter, rhs(col(iter)), t);
      }
      res(i) = t;
    }
  }
  
  template <typename B1, typename B2, typename BR>
  inline void prod(
                     CompressedRowMatrix<B1> const& lhs
                   , Vector<B2> const& rhs
                   , Vector<BR>& res
                   , bool init = false
                   )
  {
    assert(lhs.ncols() == rhs.nrows() || !"number of lhs columns must be the same as number of right hand side rows");
    
    if (init)
    {
      memset(&res[0], 0, sizeof(BR)*res.size());
    }
    
    if (res.nrows() != lhs.nrows())
    {
      res.resize(lhs.nrows());
    }
    
    typedef typename CompressedRowMatrix<B1>::const_row_iterator const_row_iterator;
    
    const_row_iterator iter;
    const_row_iterator last;
    for (size_t i = 0; i < lhs.top_non_zero_row(); ++i)
    {
      iter = lhs.row_begin(i);
      last = lhs.row_end(i);
      for (; iter != last; ++iter)
      {
        prod(*iter, rhs(col(iter)), res(i));
      }
    }
  }
    
  template <typename B1, typename B2>
  inline void prod(
                    DiagonalMatrix<B1> const& lhs
                   , Vector<B2> const& rhs
                   , Vector<B2>& res
                   , bool init = false
                   )
  {    
    assert(lhs.ncols() == rhs.nrows() || !"number of lhs columns must be the same as number of right hand side rows");
    
    if (init)
    {
      memset(&res[0], 0, sizeof(B2)*res.size());
    }
    
    if (res.nrows() != lhs.nrows())
    {
      res.resize(lhs.nrows());
    }
    
    typename DiagonalMatrix<B1>::const_iterator lhs_iter = lhs.begin();
    typename DiagonalMatrix<B1>::const_iterator lhs_last = lhs.end();
    typename Vector<B2>::const_iterator         rhs_iter = rhs.begin();
    typename Vector<B2>::iterator               res_iter = res.begin();
    
    for (; lhs_iter != lhs_last; ++lhs_iter, ++rhs_iter, ++res_iter)
    {
      prod(*lhs_iter, *rhs_iter, *res_iter);
    }
  }
  
  // 2009-07-01 Kenny: What is the purpose of the documentation? the other prods do not have it?
  /**
   * res += lhs*rhs
   */
  template <typename B1, typename B2, typename B3>
  inline void prod(TwoColumnMatrix<B1> const& lhs, Vector<B2> const& rhs, Vector<B3>& res, bool init = false)
  {
    assert(lhs.ncols() == rhs.nrows() || !"number of columns is different from rows in vector ");
    
    if (init)
    {
      // 2009-07-01 Kenny: Proper ADL?
      // 2009-07-01 Kenny: Why use fill and not memset here?
      std::fill(res.begin(), res.end(), zero_block<B3>());
    }
    if (res.nrows() != lhs.nrows())
    {
      res.resize(lhs.nrows());
    }
    
    typedef TwoColumnMatrix<B1> matrix_type;
    typedef Vector<B3>          vector_res;
    
    typename matrix_type::const_iterator lhs_iter = lhs.begin();
    typename matrix_type::const_iterator lhs_last = lhs.end();
    typename vector_res::iterator        res_iter = res.begin();
    
    // Every row in lhs contains two blocks
    for (; lhs_iter != lhs_last; ++res_iter)
    {
      prod(*lhs_iter, rhs(col(lhs_iter)), *res_iter);
      ++lhs_iter;
      prod(*lhs_iter, rhs(col(lhs_iter)), *res_iter);
      ++lhs_iter;
    }
  }
  
  // 2009-07-01 Kenny: Is prod a good name for this functionality? This is similar to axpy_prod?
  /**
   * res += lhs*rhs+smd
   */
  template <typename B1, typename B2, typename B3>
  inline void prod(TwoColumnMatrix<B1> const& lhs, Vector<B2> const& rhs, Vector<B3> const& smd, Vector<B3>& res, bool init = false)
  {
    assert(lhs.ncols() == rhs.nrows() || !"number of columns is different from rows in vector ");

    if (init)
    {
      // 2009-07-01 Kenny: Proper ADL?
      // 2009-07-01 Kenny: Why use fill and not memset here?
      std::fill(res.begin(), res.end(), zero_block<B3>());
    }
    
    if (res.nrows() != lhs.nrows())
    {
      res.resize(lhs.nrows());
    }
    
    typedef TwoColumnMatrix<B1> matrix_type;
    typedef Vector<B3>          vector_res;
    
    typename matrix_type::const_iterator lhs_iter = lhs.begin();
    typename matrix_type::const_iterator lhs_last = lhs.end();
    typename vector_res::const_iterator  smd_iter = smd.begin();
    typename vector_res::iterator        res_iter = res.begin();
    
    // Every row in lhs contains two blocks
    for (; lhs_iter != lhs_last; ++res_iter, ++smd_iter)
    {
      prod(*lhs_iter, rhs(col(lhs_iter)), *smd_iter, *res_iter);
      ++lhs_iter;
      prod(*lhs_iter, rhs(col(lhs_iter)), *smd_iter, *res_iter);
      ++lhs_iter;
    }
  }
  
  // 2009-07-01 Kenny: Why is there not axpy_prod versions for diagonal and compressed row matrices?
  // 2009-07-01 Kenny: Why is prod(CRM,vec)-version the only one with scalar optimization?
  
} // namespace sparse

// SPARSE_PROD_BLAS2_H
#endif 
