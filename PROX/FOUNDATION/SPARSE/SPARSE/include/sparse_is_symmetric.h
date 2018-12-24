#ifndef SPARSE_IS_SYMMETRIC_H
#define SPARSE_IS_SYMMETRIC_H

#include <sparse_compressed_row_matrix.h>
#include <sparse_block.h>

namespace sparse
{

  template<size_t N, typename T>
  inline bool is_symmetric(CompressedRowMatrix<Block<N,N,T> > const & A)
  {
    if(A.nrows() != A.ncols())
    {
      return false;
    }

    Block<N,N,T> tmp;

    for(size_t i = 0; i< A.nrows(); ++i)
    {
      for(size_t j = 0; j< A.nrows(); ++j)
      {
        transpose(A(i,j), tmp);
        
        if(tmp != A(j,i))
        {
          return false;
        }
      }
    }
    return true;
  }

}// end namespace sparse

// SPARSE_IS_SYMMETRIC_H
#endif
