#ifndef SPARSE_COLUMN_PROD_BLAS2_H
#define SPARSE_COLUMN_PROD_BLAS2_H

#include <sparsefwd.h>

#include <cstring>  // for memset
#include <cassert>

namespace sparse
{
  /**
  * @file
  * The point of these functions are to compute a column-block product, using information on how 
  * the columns are distributed.
  */
  
  /**
   * Column product between the ith column of lhs with the rhs block where a 
   * matrix that has the same form of the transposed of the compressed row 
   * matrix is known.
   * res += lhs_column * rhs
   */  
  template <typename B1, typename B2, typename B3, typename B4>
  inline void column_prod(
                     CompressedRowMatrix<B1> const& lhs
                   , CompressedRowMatrix<B2> const& lhsT
                   , B3 const& rhs
                   , Vector<B4>& res
                   , size_t const column
                   )
  {
    assert( ( (lhs.ncols() == lhsT.nrows()) && (lhs.nrows() == lhsT.ncols()) ) || !"lhsT must have the same form as lhs transposed");

    // The kth column in lhs is the kth row in lhsT. We wish to find the columns 
    // in lhsT that are the rows in lhs
    typedef typename CompressedRowMatrix<B2>::const_row_iterator const_row_iterator;
    
    const_row_iterator iter = lhsT.row_begin(column);
    const_row_iterator last = lhsT.row_end(column);
    for (; iter != last; ++iter)
    {
      size_t const row = col(iter);
      prod( lhs(row, column), rhs, res(row) ); // 2010-05-30 mrtn: This lookup is very expensive - optimize if possible
    }
  }

  /**
   * Column product between the ith column of lhs with the rhs block where a 
   * matrix that has the same form of the transposed of the compressed row 
   * matrix is known. The result is a CompressedVector, and it is assumed
   * that it is initialized with the correct size.
   * res += lhs_column * rhs
   */ 
  template <typename B1, typename B2, typename B3, typename B4, typename VC>
  inline void column_prod(
    CompressedRowMatrix<B1> const& lhs
    , CompressedRowMatrix<B2> const& lhsT
    , B3 const& rhs
    , CompressedVector<B4,VC>& res
    , size_t const column
    )
  {
    assert( ( (lhs.ncols() == lhsT.nrows()) && (lhs.nrows() == lhsT.ncols()) ) || !"lhsT must have the same form as lhs transposed");

    // The kth column in lhs is the kth row in lhsT. We wish to find the columns 
    // in lhsT that are the rows in lhs
    typedef typename CompressedRowMatrix<B2>::const_row_iterator const_row_iterator;
    const_row_iterator iter = lhsT.row_begin(column);
    const_row_iterator last = lhsT.row_end(column);
    for (; iter != last; ++iter)
    {
      size_t const row = col(iter);
      // 2010-04-10 mrtn: Note that it is assumed that the row exists in res!
      prod( lhs(row, column), rhs, res(row) ); // 2010-05-30 mrtn: This lookup is very expensive - optimize if possible
    }
  }
} // namespace sparse

// SPARSE_COLUMN_PROD_BLAS2_H
#endif 
