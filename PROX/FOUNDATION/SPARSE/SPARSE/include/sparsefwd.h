#ifndef SPARSE_SPARSEFWD_H
#define SPARSE_SPARSEFWD_H

#include <cstddef>  // for size_t definition

namespace sparse
{
  
  template <size_t M, size_t N, typename T> class Block;  // Forward declaration of matrix block.
  template <typename B> class CompressedRowMatrix;        // Forward declaration of compresed row matrix.
  template <typename B> class DiagonalMatrix;             // Forward declaration of diagonal matrix.
  template <typename B> class Vector;                     // Forward declaration of vector.
  template <typename B> class TwoColumnMatrix;            // Forward declaration of two column matrix.
  
} // namespace sparse

// SPARSE_SPARSEFWD_H
#endif 
