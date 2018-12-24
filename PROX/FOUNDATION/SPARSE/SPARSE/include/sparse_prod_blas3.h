#ifndef SPARSE_PROD_BLAS3_H
#define SPARSE_PROD_BLAS3_H

#include <sparsefwd.h>          
#include <sparse_ambi_vector.h> 

#include <cstring>  // for memset
#include <cassert>

namespace sparse
{
  // 2009-07-01 Kenny: Why is it not tested/asserted that input matrices have compartible dimensions?
	template <size_t M, size_t R, size_t N, typename T>
	inline void prod(
                     CompressedRowMatrix<Block<M, R, T> > const& lhs
                   , CompressedRowMatrix<Block<R, N, T> > const& rhs
                   , CompressedRowMatrix<Block<M, N, T> >& res
                   , bool init = false
                   )
	{
	  typedef typename CompressedRowMatrix<Block<M, R, T> >::const_row_iterator const_row_iter_lhs;
    typedef typename CompressedRowMatrix<Block<R, N, T> >::const_row_iterator const_row_iter_rhs;

    if (init)
    {
      memset(&res[0], 0, sizeof(Block<M, N, T>)*res.size());
    }

    // 2009-07-01 Kenny: ``unsafe'' casting?
    float const ratio_lhs           = float(lhs.size()) / (float(lhs.nrows())*float(lhs.ncols()));
    float const avg_nnz_per_rhs_col = float(rhs.size()) / float(rhs.ncols());
    float const ratio_res           = std::min(ratio_lhs * avg_nnz_per_rhs_col, 1.f);

    res.resize(lhs.nrows(), rhs.ncols(), ratio_res*lhs.nrows()*rhs.ncols());
    
    // tmp_row holds the currently computed row of C in sorted order
    detail::Ambi_vector<Block<M, N, T> > tmp_row(res.ncols());
    
    tmp_row.init(ratio_res);

    const_row_iter_lhs lhs_row_iter;
    const_row_iter_lhs lhs_row_last;
    const_row_iter_rhs rhs_row_iter;
    const_row_iter_rhs rhs_row_last;
    
    for (size_t i = 0; i < lhs.top_non_zero_row(); ++i)
    {
      tmp_row.zero_out();
    
      lhs_row_iter = lhs.row_begin(i);
      lhs_row_last = lhs.row_end(i);
      
      for (; lhs_row_iter != lhs_row_last; ++lhs_row_iter)
      {
        tmp_row.restart(); // set internal current pointer to start

        size_t const j = col(lhs_row_iter);
        
        rhs_row_iter = rhs.row_begin(j);
        rhs_row_last = rhs.row_end(j);
        
        for (; rhs_row_iter != rhs_row_last; ++rhs_row_iter)
        {
          prod(*lhs_row_iter, *rhs_row_iter, tmp_row(col(rhs_row_iter)));
        }
      }
      
      typename detail::Ambi_vector<Block<M, N, T> >::Iterator it(tmp_row);
      for (; it; ++it)
      {
        res(i, it.key()) = it.value();
      }
    }
	}
  
  template <typename B>
  inline void prod(
                     DiagonalMatrix<B> const & lhs
                   , DiagonalMatrix<B> const & rhs
                   , DiagonalMatrix<B> & res
                   , bool init = false
                   )
  {
    assert(lhs.ncols() == rhs.nrows() || !"number of lhs columns must be the same as number of right hand side rows");
    if(init)
    {
      memset(&res[0], 0, sizeof(B)*res.size());
    }
    if (res.nrows() != lhs.nrows())
    {
      res.resize(lhs.size());
    }

    typename DiagonalMatrix<B>::const_iterator lhs_iter = lhs.begin();
    typename DiagonalMatrix<B>::const_iterator lhs_last = lhs.end();
    typename DiagonalMatrix<B>::const_iterator rhs_iter = rhs.begin();
    typename DiagonalMatrix<B>::iterator       res_iter = res.begin();
    
    for (; lhs_iter != lhs_last; ++lhs_iter, ++rhs_iter, ++res_iter)
    {
      prod(*lhs_iter, *rhs_iter, *res_iter);
    }
  }

  /**
  * res *= rhs
  * @warning not unittested
  */
  template <typename B>
  inline void prod(
                     DiagonalMatrix<B> const & rhs
                   , DiagonalMatrix<B>       & res
                   )
  {
    assert(res.ncols() == rhs.nrows() || !"number of lhs columns must be the same as number of right hand side rows");

    typename DiagonalMatrix<B>::iterator       res_iter = res.begin();
    typename DiagonalMatrix<B>::iterator       res_last = res.end();
    typename DiagonalMatrix<B>::const_iterator rhs_iter = rhs.begin();
    B temp(0);
    for (; res_iter != res_last; ++rhs_iter, ++res_iter)
    {
      temp = *res_iter;
      res_iter->clear_data(); // because prod compute +=, not just =
      prod(*rhs_iter, temp, *res_iter);
    }
  }
  
  /**
   * @warning not tested
   */
  template <typename B1, typename B2, typename BR>
  inline void prod(
                     CompressedRowMatrix<B1> const & lhs
                   , DiagonalMatrix<B2> const & rhs
                   , CompressedRowMatrix<B1> & res
                   , bool init = false
                   )
  {
    assert(lhs.ncols() == rhs.nrows() || !"number of lhs columns must be the same as number of right hand side rows");

    typedef typename CompressedRowMatrix<B1>::accessor A_lhs;
    typedef typename CompressedRowMatrix<BR>::accessor A_res;

    typedef CompressedRowMatrix<B1> Crs_lhs;
    typedef CompressedRowMatrix<B1> Crs_res;

    if (init)
    {
      memset(&res[0], 0, sizeof(B1)*res.size());
    }

    // res is structurally equivalent to lhs
    A_res::nrows(res)    = A_lhs::nrows(lhs);
    A_res::ncols(res)    = A_lhs::ncols(lhs);
    A_res::cols(res)     = A_lhs::cols(lhs);
    A_res::row_ptrs(res) = A_lhs::row_ptrs(lhs);
    A_res::data(res).resize(lhs.size());

    typename Crs_lhs::const_row_iterator lhs_iter;
    typename Crs_lhs::const_row_iterator lhs_last;
    typename Crs_res::row_iterator       res_iter;
    
    for (size_t i = 0; i < lhs.top_non_zero_row(); ++i)
    {
      lhs_iter = lhs.row_begin(i);
      lhs_last = lhs.row_end(i);
      res_iter = res.row_begin(i);
      for (; lhs_iter != lhs_last; ++lhs_iter, ++res_iter)
      {
        prod(*lhs_iter, rhs[col(lhs_iter)],*res_iter);
      }
    }
  }

  template <typename B1, typename B2>
  inline void prod(
                     DiagonalMatrix<B1> const& lhs
                   , CompressedRowMatrix<B2> const& rhs
                   , CompressedRowMatrix<B2>& res
                   , bool init = false
                   )
  {
    assert(lhs.ncols() == rhs.nrows() || !"number of lhs columns must be the same as number of right hand side rows");
   
    typedef typename CompressedRowMatrix<B2>::accessor A;
    typedef CompressedRowMatrix<B2> BCR;
  
    if (init)
    {
      memset(&res[0], 0, sizeof(B2)*res.size());
    }

    // res is structurally equivalent to rhs
    A::nrows(res)    = A::nrows(rhs);
    A::ncols(res)    = A::ncols(rhs);
    A::cols(res)     = A::cols(rhs);
    A::row_ptrs(res) = A::row_ptrs(rhs);
    A::data(res).resize(rhs.size(), zero_block<B2>());

    typename BCR::const_row_iterator rhs_iter;
    typename BCR::const_row_iterator rhs_last;
    typename BCR::row_iterator       res_iter;
    
    for (size_t i = 0; i < rhs.top_non_zero_row(); ++i)
    {
      B1 const& b = lhs[i];
      rhs_iter = rhs.row_begin(i);
      rhs_last = rhs.row_end(i);
      res_iter = res.row_begin(i);
      for (; rhs_iter != rhs_last; ++rhs_iter, ++res_iter)
      {
        prod(b, *rhs_iter, *res_iter);
      }
    }
  }
  
} // namespace sparse

// SPARSE_PROD_BLAS3_H
#endif 
