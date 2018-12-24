#ifndef SPARSE_ROW_PROD_BLAS2_H
#define SPARSE_ROW_PROD_BLAS2_H

#include <sparsefwd.h>

#include <cstring>  // for memset
#include <cassert>

namespace sparse
{
    
  /**
   * Row product between the ith row of lhs with the rhs vector
   * res += lhs_row * rhs
   */  
  template <typename B1, typename B2, typename BR>
  inline void row_prod(
                     CompressedRowMatrix<B1> const& lhs
                   , Vector<B2> const& rhs
                   , BR& res
                   , size_t row
                   )
  {
    assert(lhs.ncols() == rhs.nrows() || !"number of lhs columns must be the same as number of right hand side rows");
    
    typedef typename CompressedRowMatrix<B1>::const_row_iterator const_row_iterator;
    
    const_row_iterator iter = lhs.row_begin(row);
    const_row_iterator last = lhs.row_end(row);
    for (; iter != last; ++iter)
    {
      prod(*iter, rhs(col(iter)), res);
    }
  }

  /**
   * Row product between the ith row of lhs with the rhs vector
   * res += lhs_row * rhs
   */  
  template <typename B1, typename B2, typename BR, typename C>
  inline void row_prod(
                   CompressedRowMatrix<B1>  const& lhs
                   , CompressedVector<B2,C> const& rhs
                   , BR& res
                   , size_t row
                   )
  {
    assert(lhs.ncols() == rhs.nrows() || !"number of lhs columns must be the same as number of right hand side rows");
    
    typedef typename CompressedRowMatrix<B1>::const_row_iterator const_row_iterator;
    
    const_row_iterator iter = lhs.row_begin(row);
    const_row_iterator last = lhs.row_end(row);
    for (; iter != last; ++iter)
    {
      prod(*iter, rhs(col(iter)), res);
    }
  }

  /**
   * Row product between the ith row of lhs with the rhs vector
   * res += lhs_row * rhs
   */ 
  template <typename B1, typename B2, typename B3>
  inline void row_prod(
                    DiagonalMatrix<B1> const& lhs
                   , Vector<B2> const& rhs
                   , B3& res
                   , size_t row
                   )
  {    
    assert(lhs.ncols() == rhs.nrows() || !"number of lhs columns must be the same as number of right hand side rows");
    
    // only one element in each row of diagonal matrix, so only one block operation needed
    prod( lhs(row,row), rhs(row), res );
  }
  
  /**
   * Row product between the ith row of lhs with the rhs vector
   * res += lhs_row * rhs
   */ 
  template <typename B1, typename B2, typename B3>
  inline void row_prod(
                      TwoColumnMatrix<B1> const& lhs
                      , Vector<B2> const& rhs
                      , B3& res
                      , size_t row )
  {
    assert(lhs.ncols() == rhs.nrows() || !"number of columns is different from rows in vector ");
    
    // we exploit that there are only two elements in one lhs row
    size_t row_index = 2u * row;
    size_t col_index = lhs.col_of_idx(row_index);
    B1 const & first_block = lhs[row_index];
    prod( first_block, rhs(col_index), res );

    ++row_index;
    col_index = lhs.col_of_idx(row_index);
    B1 const & second_block = lhs[row_index];
    prod( second_block, rhs(col_index), res );
  }
  
} // namespace sparse

// SPARSE_ROW_PROD_BLAS2_H
#endif 
