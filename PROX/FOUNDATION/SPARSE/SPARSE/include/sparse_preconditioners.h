#ifndef SPARSE_PRECONDITIONERS_H
#define SPARSE_PRECONDITIONERS_H

#include <sparse_compressed_row_matrix.h>
#include <sparse_block.h>
#include <sparse_block_algebra.h>
#include <sparse_index_iterator.h>
#include <sparse_row_iterator.h>

#include <cassert>
#include <cmath>   // needed for std::sqrt

namespace sparse
{

  template<size_t N, typename T>
  inline void make_identity_preconditioner( CompressedRowMatrix<Block<N,N,T> > & P)
  {
    assert(P.nrows() == P.ncols() || !"Matrix must be square");

    size_t const K = P.nrows();

    P.clear();
    P.resize(K,K,K);

    for (size_t i = 0; i < K; ++i)
    {
      P(i,i) = Block<N,N,T>::identity();
    }
  }

  /**
   * Returns inverse diagonal of the input matrix to create a jacobi
   * preconditioner
   *
   * @param A       The matrix that needs the preconditioner
   *
   * @param P        Return argument for where the inverse diagonal is returned.
   *
   * 2014-XX-XX Taus: Maybe diagnonal matrices should be utilized here. However
   * no space is wasted as there are only K blocks anyway. Not tested!
   */
  template<size_t N, typename T>
  inline void make_jacobi_preconditioner(
                                         CompressedRowMatrix<Block<N,N,T> >const & A
                                         , CompressedRowMatrix<Block<N,N,T> > & P
                                         )
  {
    assert(A.nrows() == A.ncols() || !"Matrix must be square");

    size_t const K = A.nrows();

    P.clear();
    P.resize(K,K,K);

    for (size_t i = 0; i < K; ++i)
    {
      P(i,i) = extract_diag(A(i,i));

      // 2014-XX-XX Taus: This is not final. This only works
      // for 3x3 blocks it should simply take the reciprocal
      // of the matrix as it is diagonal
      inverse(P(i,i));
    }
  }

  /**
   * Returns inverse of diagonal blocks of the input matrix to create
   * a jacobi preconditioner
   *
   * @param A       The matrix that needs the preconditioner
   *
   * @param P       Return argument for where the inverse diagonal blocks is returned.
   *
   * 2014-XX-XX Taus: Maybe diagnonal matrices should be utilized here. However no
   * space is wasted as there are only K blocks anyway. only works with 3x3 blocks Not tested!
   */
  template<typename T>
  inline void make_block_jacobi_preconditioner(
                                               CompressedRowMatrix<Block<3,3,T> > const & A
                                               , CompressedRowMatrix<Block<3,3,T> > & P
                                               )
  {
    assert(A.nrows() == A.ncols() || !"Matrix must be square");

    size_t const K = A.nrows();

    P.clear();
    P.resize(K,K,K);

    for (size_t i = 0; i < K; ++i)
    {
      P(i,i) = A(i,i);
      inverse(P(i,i));
    }
  }

}// end namespace sparse

// SPARSE_PRECONDITIONERS_H
#endif
