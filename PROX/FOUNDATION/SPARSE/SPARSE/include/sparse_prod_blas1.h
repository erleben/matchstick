#ifndef SPARSE_PROD_BLAS1_H
#define SPARSE_PROD_BLAS1_H

#include <sparsefwd.h>

#include <cstring>  // for memset
#include <cassert>

namespace sparse
{

  /**
   * Scalar product, also called dot product or inner product.
   */
  template <typename B>
  inline void inner_prod(
                   Vector<B> const& lhs
                 , Vector<B> const& rhs
                 , typename B::value_type& res)
  {
    assert(lhs.size() == rhs.size() || !"vectors must be of same size");

    // 2009-07-01 Kenny: Documentation, how to use this?
#ifdef SPARSE_LAPACK
    res = cblas_sdot(lhs.size()*B::ncols(), &lhs[0][0], 1, &rhs[0][0], 1);
#else
    typedef typename Vector<B>::const_iterator     const_vector_iterator;
    typedef typename B::const_iterator             const_block_iterator;
    const_vector_iterator lhs_iter = lhs.begin();
    const_vector_iterator lhs_last = lhs.end();
    const_vector_iterator rhs_iter = rhs.begin();
    
    const_block_iterator lhs_block_iter;
    const_block_iterator lhs_block_last;
    const_block_iterator rhs_block_iter;

    // 2009-07-01 Kenny: Use value traits?
    res = 0; 
    for (; lhs_iter != lhs_last; ++lhs_iter, ++rhs_iter)
    {
      lhs_block_iter = lhs_iter->begin();
      lhs_block_last = lhs_iter->end();
      rhs_block_iter = rhs_iter->begin();
//#pragma unroll
      for(; lhs_block_iter != lhs_block_last; ++lhs_block_iter, ++rhs_block_iter)
      {
        res += *lhs_block_iter * *rhs_block_iter;
      }
    }
#endif // SPARSE_LAPACK
  }

  /**
   * Elementwise product.
   */
  template <typename B>
  inline void element_prod(
                           Vector<B> const & lhs
                         , Vector<B> const & rhs
                         , Vector<B>       & res)
  {
    assert(lhs.size() == rhs.size() || !"vectors must be of same size");
    assert(rhs.size() == res.size() || !"vectors must be of same size");
    
    typedef typename Vector<B>::const_iterator     const_vector_iterator;
    typedef typename B::const_iterator             const_block_iterator;
    typedef typename Vector<B>::iterator           vector_iterator;
    typedef typename B::iterator                   block_iterator;
    
    const_vector_iterator lhs_iter = lhs.begin();
    const_vector_iterator lhs_last = lhs.end();
    const_vector_iterator rhs_iter = rhs.begin();
    vector_iterator       res_iter = res.begin();
    
    const_block_iterator lhs_block_iter;
    const_block_iterator lhs_block_last;
    const_block_iterator rhs_block_iter;
    block_iterator       res_block_iter;
    
    for (; lhs_iter != lhs_last; ++lhs_iter, ++rhs_iter,++res_iter)
    {
      res_block_iter = res_iter->begin();
      lhs_block_iter = lhs_iter->begin();
      lhs_block_last = lhs_iter->end();
      rhs_block_iter = rhs_iter->begin();
//#pragma unroll
      for(; lhs_block_iter != lhs_block_last; ++lhs_block_iter, ++rhs_block_iter,++res_block_iter)
      {
        *res_block_iter = *lhs_block_iter * *rhs_block_iter;
      }
    }
  }
  
  /**
   *
   * y += alpha * x
   * scale (axpy)
   */
  // 2009-07-01 Kenny: What is init paramter used for?
  // 2009-07-01 Kenny: inner and scalar prods assert on valid vector sizes why is the behaviour of this prod different?
  template <typename B>
  inline void prod(
                     typename B::value_type const& alpha
                   , Vector<B> const& x
                   , Vector<B>& y
                   , bool init = false
                   )
  {
    if(init)
    {
      memset(&y[0], 0, sizeof(B)*y.size());
    }
    
    if(y.size() != x.size())
    {
      y.resize(x.size());
    }
    
    // 2009-07-01 Kenny: Documentation, how to use this?
#ifdef SPARSE_LAPACK
    cblas_saxpy(rhs.size()*B::ncols(), alpha, &x[0][0], 1, &y[0][0], 1);
#else
    
    typedef typename Vector<B>::iterator        vector_iterator;
    typedef typename Vector<B>::const_iterator  const_vector_iterator;
    typedef typename B::iterator                block_iterator;
    typedef typename B::const_iterator          const_block_iterator;

    const_vector_iterator x_iter = x.begin();
    const_vector_iterator x_last = x.end();
    vector_iterator       y_iter = y.begin();
    
    const_block_iterator  x_block_iter;
    const_block_iterator  x_block_last;
    block_iterator        y_block_iter;
    
    for(; x_iter != x_last; ++x_iter, ++y_iter)
    {
      x_block_iter = x_iter->begin();
      x_block_last = x_iter->end();
      y_block_iter = y_iter->begin();
//#pragma unroll
      for(; x_block_iter != x_block_last;
          ++x_block_iter, ++y_block_iter)
      {
        *y_block_iter = alpha * *x_block_iter+ *y_block_iter;      
      }
    }
#endif // SPARSE_LAPACK
  }

  
  /**
   * scale: alpha*x
   */
  // 2009-07-01 Kenny: Why is it not tested/asserted if x is non-zero?
  template <typename B>
  inline void prod( typename B::value_type const& alpha, Vector<B>& x)
  {
    
    // 2009-07-01 Kenny: Documentation, how to use this?
#ifdef SPARSE_LAPACK
    cblas_sscal(x.size()*B::ncols(), alpha, &x[0][0], 1);
#else
    
    typedef typename Vector<B>::iterator        vector_iterator;
    //typedef typename Vector<B>::const_iterator  const_vector_iterator;
    typedef typename B::iterator                block_iterator;
    //typedef typename B::const_iterator          const_block_iterator;

    vector_iterator x_iter = x.begin();
    vector_iterator x_last = x.end();
    
    block_iterator x_block_iter;
    block_iterator x_block_last;
    
    for(; x_iter != x_last; ++x_iter)
    {
      x_block_iter = x_iter->begin();
      x_block_last = x_iter->end();
//#pragma unroll
      for(; x_block_iter != x_block_last; ++x_block_iter)
      {
        *x_block_iter *= alpha;
      }
    }
#endif // SPARSE_LAPACK
  }
  
} // namespace sparse

// SPARSE_PROD_BLAS1_H
#endif 
