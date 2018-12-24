#ifndef SPARSE_CONJUGATE_GRADIENT_H
#define SPARSE_CONJUGATE_GRADIENT_H

#include <sparse_block.h>
#include <sparse_block_algebra.h>
#include <sparse_prod_blas1.h>
#include <sparse_prod_blas2.h>
#include <sparse_preconditioners.h>

#include <sparse_index_iterator.h>
#include <sparse_row_iterator.h>

#include <util_log.h>

#include <cassert>

namespace sparse
{

  // 2015-03-09 Kenny code review: Interfaces should accept somekind of params structure (monitor) allowing user control over max_iteartions, restarts, absolute/relevative tolerances, as well as any verbose mode

  // 2015-03-09 Kenny code review: Interfaces should support linear operators (functors) rather than taking explicit matrix types as argumetns.

  // 2015-03-09 Kenny code review: Valute traits should be used to set contact values etc internally in the methods to avoid problems of type conversion.

  template<size_t N, typename T>
  inline void conjugate_gradient(
                                   CompressedRowMatrix<Block<N,N,T> > const & A
                                 , Vector<Block<N,1,T> >                    & x
                                 , Vector<Block<N,1,T> >              const & b
                                 )
  {
    CompressedRowMatrix<Block<N,N,T> > P(A.nrows(),A.nrows(),A.nrows());

    make_identity_preconditioner(P);

    preconditioned_conjugate_gradient(P,A,x,b);
  }

  /**
   * Solve linear system using preconditioned conjugate gradient method.
   *
   * @param P    The Preconditioner to use
   * @param A    The system (coefficient) matrix.
   * @param x    Upon  return holds the approximate solution to A x = b.
   * @param b    The right hand side vector
   *
   * 2014-XX-XX Taus comment: Only tested with unit preconditioner
   */
  template<size_t N, typename T>
  inline void preconditioned_conjugate_gradient(
                                                  CompressedRowMatrix<Block<N,N,T> > const & P
                                                , CompressedRowMatrix<Block<N,N,T> > const & A
                                                , Vector<Block<N,1,T> >                    & x
                                                , Vector<Block<N,1,T> >              const & b
                                                )
  {
    typedef  Block<N,1,T>                           vector_block_type;
    typedef  Vector<vector_block_type>              V;

    assert(P.nrows() == A.nrows() && P.ncols() ==  A.ncols() || !"preconditioned_conjugate_gradient(): incompatable dimensions");
    assert(A.nrows() == A.ncols() && A.nrows() == b.size()   || !"preconditioned_conjugate_gradient(): incompatable dimensions");

    //--- If x has right size we just use it as initial guess. Otherwise we
    //--  set x to the zero vector.
    if(x.size() != A.nrows())
    {
      x.clear();
      x.resize(A.nrows(),true);
    }

    V tmp;

    V y;
    // y <- Ax
    prod(A,x,y);

    // r <- b - A*x
    V r;
    sub(b,y,r);

    // z <- P*r
    V z;
    prod(P,r,z);

    // p <- z
    V d = z;

    // rz = <r^H, z>
    T rz = 0.0;
    inner_prod(r,z,rz);

    int i = 0;
    while(i < 1000 &&  rz> 10e-9)
    {
      i++;

      y.clear_data();
      prod(A,d,y);

      // alpha <- <r,z>/<y,p>
      T yd;
      inner_prod(y,d, yd);
      T alpha = rz / yd;

      // x <- x + alpha * p
      prod(alpha, d, x);

      // r <- r - alpha * y
      if(i % 50 == 0)
      {
        y.clear_data();
        prod(A,x,y);
        sub(b,y,r);
      }
      else
      {
        prod(-alpha, y, r);
      }

      //z <- M*r
      z.clear_data();
      prod(P,r,z);
      T rz_old = rz;

      // rz = <r^H, z>
      inner_prod(r,z,rz);

      //beta <- <r_{i+1},r_{i+1}>/<r,r>
      T beta = rz / rz_old;

      //p <- r + beta*p
      prod(beta,d);
      prod(1.0f,z,d);
    }

    util::Log logging;

    logging << "finished in " << i << " iterations, with a residual of " << rz << util::Log::newline();
  }

}// end namespace sparse

// SPARSE_CONJUGATE_GRADIENT_H
#endif
