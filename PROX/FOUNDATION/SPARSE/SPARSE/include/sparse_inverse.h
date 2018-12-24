#ifndef SPARSE_INV_H
#define SPARSE_INV_H

#include <sparsefwd.h>

namespace sparse
{
  /**
   * Inversion of Block Diagonal Matrix.
   *
   * @tparam B    The block type.
   */
	template <typename B>
	void inverse(DiagonalMatrix<B> & D)
	{
    typename DiagonalMatrix<B>::iterator b = D.begin();
    typename DiagonalMatrix<B>::iterator end = D.end();
    for( ; b != end; ++b)
      inverse( (*b) );
	}
  
} // namespace sparse

// SPARSE_INV_H
#endif 
