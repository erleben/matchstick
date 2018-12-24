#ifndef SPARSE_TRANSPOSE_H
#define SPARSE_TRANSPOSE_H

#include <sparsefwd.h>

#include <algorithm> // for fill and copy

namespace sparse
{
  
  /**
   * @note Algorithm based on code from Timothy A. Davis' Direct methods for sparse linear systems'
   */
  // 2009-07-01 Kenny: Why is it not tested that A is non-zero sized?
	template <typename B1, typename B2>
	inline void	transpose(
                          CompressedRowMatrix<B1> const & A
                        , CompressedRowMatrix<B2>& B
                        )
	{
	  typedef CompressedRowMatrix<B1> A_type;
	  typedef CompressedRowMatrix<B2> B_type;
    
	  typedef typename A_type::accessor A_A;
	  typedef typename B_type::accessor B_A;
    
	  typename A_A::row_ptrs_container_type const & A_row_ptrs = A_A::row_ptrs(A);
    typename A_A::cols_container_type     const & A_cols     = A_A::cols(A);
    typename A_A::data_container_type     const & A_data     = A_A::data(A);
    
	  typename B_A::row_ptrs_container_type       & B_row_ptrs = B_A::row_ptrs(B);
    typename B_A::cols_container_type           & B_cols     = B_A::cols(B);
    typename B_A::data_container_type           & B_data     = B_A::data(B);
    
    // set dimensions
    B_A::nrows(B) = A.ncols();
    B_A::ncols(B) = A.nrows();
    
    // allocate space
    if (!B_row_ptrs.empty() && B_row_ptrs.back() != 0)
    {
      // 2009-07-01 Kenny: Proper ADL?
      std::fill(B_row_ptrs.begin(), B_row_ptrs.end(), 0);
    }
    B_row_ptrs.resize(B.nrows()+1, 0u);
    B_cols.resize(A.size());
    B_data.resize(A.size());
    
    
    // step 1 : create row_ptrs of B
    // -----------------------------
    // count B's row-entries via A's col-entries
    for (size_t i = 0; i < A.size(); ++i)
    {
      // offset by one
      ++B_row_ptrs[A_cols[i]+1];
    }
    // accumulate row-entries to create row_ptrs
    for (size_t i = 1; i < B_row_ptrs.size(); ++i)
    {
      B_row_ptrs[i] += B_row_ptrs[i-1];
    }
    
    
    // step 2 : create cols and data of B
    // ----------------------------------
    // array used to correctly place transposed elements into B
    
    size_t * B_row_count = new size_t[B_row_ptrs.size()];   // kenny: Argh new/delete... unsafe? why not just use row_ptrs_container_type?
    std::copy(B_row_ptrs.begin(), B_row_ptrs.end(), B_row_count);
    
    for (size_t i = 0; i < A.nrows(); ++i)
    {
      size_t const row_end = A_row_ptrs[i+1];
      for (size_t j = A_row_ptrs[i]; j < row_end; ++j)
      {
        size_t const k = B_row_count[A_cols[j]]++;
        B_cols[k] = i;
        transpose(A_data[j], B_data[k]); // transpose blocks as well
      }
    }
    delete[] B_row_count;
	}
  
  // 2009-07-01 Kenny: Document what is fake transposed?
	/**
   * fake transpose does not transpose blocks
   */
  // 2009-07-01 Kenny: Why is it not tested that A is non-zero sized?
  template <typename BT>
  inline void fake_transpose(
                               CompressedRowMatrix<BT> const & A
                             , CompressedRowMatrix<BT>& B
                             )
  {
    typedef CompressedRowMatrix<BT>   MT;
    typedef typename MT::accessor     MT_A;
    
    typename MT_A::row_ptrs_container_type const & A_row_ptrs = MT_A::row_ptrs(A);
    typename MT_A::cols_container_type     const & A_cols     = MT_A::cols(A);
    typename MT_A::data_container_type     const & A_data     = MT_A::data(A);
    
    typename MT_A::row_ptrs_container_type       & B_row_ptrs = MT_A::row_ptrs(B);
    typename MT_A::cols_container_type           & B_cols     = MT_A::cols(B);
    typename MT_A::data_container_type           & B_data     = MT_A::data(B);
    
    // set dimensions
    MT_A::nrows(B) = A.ncols();
    MT_A::ncols(B) = A.nrows();
    
    // allocate space
    if (!B_row_ptrs.empty() && B_row_ptrs.back() != 0)
    {
      // 2009-07-01 Kenny: Proper ADL?
      std::fill(B_row_ptrs.begin(), B_row_ptrs.end(), 0);
    }
    B_row_ptrs.resize(B.nrows()+1, 0u);
    B_cols.resize(A.size());
    B_data.resize(A.size());
    
    // step 1 : create row_ptrs of B
    // -----------------------------
    // count B's row-entries via A's col-entries
    for (size_t i = 0; i < A.size(); ++i)
    {
      // offset by one
      ++B_row_ptrs[A_cols[i]+1];
    }
    // accumulate row-entries to create row_ptrs
    for (size_t i = 1; i < B_row_ptrs.size(); ++i)
    {
      B_row_ptrs[i] += B_row_ptrs[i-1];
    }
    
    // step 2 : create cols and data of B
    // ----------------------------------
    // array used to correctly place transposed elements into B
    size_t *B_row_count = new size_t[B_row_ptrs.size()];            // kenny: Argh new/delete... unsafe?
    std::copy(B_row_ptrs.begin(), B_row_ptrs.end(), B_row_count);
    
    for (size_t i = 0; i < A.nrows(); ++i)
    {
      size_t const row_end = A_row_ptrs[i+1];
      for (size_t j = A_row_ptrs[i]; j < row_end; ++j)
      {
        size_t const k = B_row_count[A_cols[j]]++;
        B_cols[k] = i;
        B_data[k] = A_data[j]; // don't transpose blocks
      }
    }
    delete[] B_row_count;
  }
  
  // 2009-07-01 Kenny : Why is it not tested if A is non-zero sized?
  template <typename BT>
  inline void transpose(DiagonalMatrix<BT> const& A, DiagonalMatrix<BT>& B)
  {
    if (A.size() != B.size())
    {
      B.resize(A.size());
    }
    size_t const A_size = A.size();
    for (size_t i = 0; i < A_size; ++i)
    {
      transpose(A[i], B[i]);
    }
  }
  
  /**
   * @note Algorithm based on code from Timothy A. Davis's Direct methods for sparse linear systems.
   */
  // 2009-07-01 Kenny: Why is it not tested that A is non-zero sized?
  template <typename B1, typename B2>
  inline void transpose(TwoColumnMatrix<B1> const& A, CompressedRowMatrix<B2> & B)
  {
    typedef TwoColumnMatrix<B1>       A_type;
    typedef CompressedRowMatrix<B2>   B_type;
    typedef typename A_type::accessor A_A;
    typedef typename B_type::accessor B_A;
    
    typename A_A::cols_container_type const & A_cols     = A_A::cols(A);
    typename A_A::data_container_type const & A_data     = A_A::data(A);
    
    typename B_A::row_ptrs_container_type   & B_row_ptrs = B_A::row_ptrs(B);
    typename B_A::cols_container_type       & B_cols     = B_A::cols(B);
    typename B_A::data_container_type       & B_data     = B_A::data(B);
    
    // set dimensions
    B_A::nrows(B) = A.ncols();
    B_A::ncols(B) = A.nrows();
    
    // allocate space
    if (!B_row_ptrs.empty() && B_row_ptrs.back() != 0)
    {
      // 2009-07-01 Kenny: Proper ADL?
      std::fill(B_row_ptrs.begin(), B_row_ptrs.end(), 0);
    }
    B_row_ptrs.resize(B.nrows()+1, 0u);
    B_cols.resize(A.size());
    B_data.resize(A.size());
    
    // step 1 : create row_ptrs of B
    // -----------------------------
    // count B's row-entries via A's col-entries
    for (size_t i = 0; i < A.size(); ++i)
    {
      // offset by one
      ++B_row_ptrs[A_cols[i]+1];
    }
        
    // accumulate row-entries to create row_ptrs
    for (size_t i = 1; i < B_row_ptrs.size(); ++i)
    {
      B_row_ptrs[i] += B_row_ptrs[i-1];
    }
    
    // step 2 : create cols and data of B
    // ----------------------------------
    // array used to correctly place transposed elements into B
    size_t *B_row_count = new size_t[B_row_ptrs.size()];         // kenny: Yrgh! new/delete???
    std::copy(B_row_ptrs.begin(), B_row_ptrs.end(), B_row_count);
    
    size_t A_row = 0;
    for (size_t i = 0; i < A.size(); ++i, ++A_row)
    {
      // there are two entries per row of A
      size_t k = B_row_count[A_cols[i]]++;
      B_cols[k] = A_row;
      sparse::transpose(A_data[i], B_data[k]); // transpose blocks
      ++i;
      k = B_row_count[A_cols[i]]++;
      B_cols[k] = A_row;
      transpose(A_data[i], B_data[k]); // transpose blocks
    }
    delete[] B_row_count;
  }
  
  // 2009-07-01 Kenny: Document what is fake transposed?
  /**
   * fake transpose, does not transpose blocks.
   */
  // 2009-07-01 Kenny: Why is it not tested that A is non-zero sized?
  template <typename B1>
  inline void fake_transpose(TwoColumnMatrix<B1> const& A, CompressedRowMatrix<B1>& B)
  {
    typedef TwoColumnMatrix<B1>       A_type;
    typedef CompressedRowMatrix<B1>   B_type;
    typedef typename A_type::accessor A_A;
    typedef typename B_type::accessor B_A;
    
    typename A_A::cols_container_type const & A_cols     = A_A::cols(A);
    typename A_A::data_container_type const & A_data     = A_A::data(A);
    typename B_A::row_ptrs_container_type   & B_row_ptrs = B_A::row_ptrs(B);
    typename B_A::cols_container_type       & B_cols     = B_A::cols(B);
    typename B_A::data_container_type       & B_data     = B_A::data(B);
    
    // set dimensions
    B_A::nrows(B) = A.ncols();
    B_A::ncols(B) = A.nrows();
    
    // allocate space
    if (!B_row_ptrs.empty() && B_row_ptrs.back() != 0)
    {
      // 2009-07-01 Kenny: Proper ADL?
      std::fill(B_row_ptrs.begin(), B_row_ptrs.end(), 0);
    }
    B_row_ptrs.resize(B.nrows()+1, 0u);
    B_cols.resize(A.size());
    B_data.resize(A.size());
    
    // step 1 : create row_ptrs of B
    // -----------------------------
    // count B's row-entries via A's col-entries
    for (size_t i = 0; i < A.size(); ++i)
    {
      // offset by one
      ++B_row_ptrs[A_cols[i]+1];
    }
    // accumulate row-entries to create row_ptrs
    for (size_t i = 1; i < B_row_ptrs.size(); ++i)
    {
      B_row_ptrs[i] += B_row_ptrs[i-1];
    }
    
    // step 2 : create cols and data of B
    // ----------------------------------
    // array used to correctly place transposed elements into B
    size_t *B_row_count = new size_t[B_row_ptrs.size()];                // kenny: jeeze new/delete????
    std::copy(B_row_ptrs.begin(), B_row_ptrs.end(), B_row_count);
    
    size_t A_row = 0;
    for (size_t i = 0; i < A.size(); ++i, ++A_row)
    {
      // There are two entries per row of A
      size_t k = B_row_count[A_cols[i]]++;
      B_cols[k] = A_row;
      // Don't transpose blocks
      // (B will be used as rhs, so fast col-access is wanted)
      B_data[k] = A_data[i];
      ++i;
      k = B_row_count[A_cols[i]]++;
      B_cols[k] = A_row;
      B_data[k] = A_data[i]; // don't transpose blocks
    }
    delete[] B_row_count;
  }
  
} // namespace sparse

//  SPARSE_TRANSPOSE_H
#endif 
