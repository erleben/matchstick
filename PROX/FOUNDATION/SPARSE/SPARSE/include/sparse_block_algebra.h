#ifndef SPARSE_BLOCK_ALGEBRA_H
#define SPARSE_BLOCK_ALGEBRA_H

#include <sparsefwd.h>

#include <cmath>  // needed for std::sqrt
#include <cassert>

namespace sparse
{
  /**
   * General version of block product res += lhs*rhs
   */
  template <size_t A, size_t B, size_t C, typename T>
  inline void prod(Block<A,B,T> const & lhs, Block<B,C,T> const & rhs, Block<A,C,T> & res)
  {
    typedef Block<A,B,T> B1;
    typedef Block<B,C,T> B2;
    typedef Block<A,C,T> BR;
    
    size_t a = 0;
    size_t b = 0;
    
    for (size_t i = 0; i < B1::nrows(); ++i, a += BR::ncols(), b+= B1::ncols())
    {
      for (size_t j = 0; j < B2::ncols(); ++j)
      {
        size_t c = 0;
        size_t k = 0;
        size_t l = a + j;

        T dot = 0.0;  // 2009-06-30 Kenny: use proper value_traits
        for (; k < B1::ncols(); ++k, c += BR::ncols())
        {
          dot += lhs[b+k] * rhs[c+j];
        }
        res[l] += dot;
      }
    }
  }


  /**
   * General version of block product res = rhs*res for square matrices rhs and res
   */
  template <size_t N, typename T>
  inline void prod(Block<N,N,T> const & lhs, Block<N,N,T> & res)
  {

    typedef Block<N,N,T> B;
    B tmp;

     size_t a = 0;
  
     for (size_t i = 0; i < N; ++i, a += N)
     {
       for (size_t j = 0; j < N; ++j)
       {
         size_t b = 0;
         size_t k = 0;
         size_t l = a + j;

         T dot = 0.0;  // 2009-06-30 Kenny: use proper value_traits
         for (; k < N; ++k, b += N)
         {
           dot += lhs[a+k] * res[b+j];
         }
         tmp[l] = dot;
       }
     }
     res = tmp; //move semantics might help
  }
  
  /**
   * Optimised block-vector version of block product function. res += lhs*rhs
   */
  template <size_t A, size_t B, typename T>
  inline void prod(Block<A,B,T> const& lhs, Block<B,1,T> const& rhs, Block<A,1,T>& res)
  {
    typedef Block<A,B,T> B1;
    //typedef Block<B,1,T> B2;
    //typedef Block<A,1,T> BR;
    
    size_t b = 0;
    for (size_t i = 0; i < B1::nrows(); ++i, b+= B1::ncols())
    {
      T dot = 0.0; // 2009-06-30 Kenny: use proper value_traits
      for (size_t k = 0; k < B1::ncols(); ++k)
      {
        dot += lhs[b+k] * rhs[k];
      }
      res[i] += dot; 
    }
  }

  /**
   * Optimised block-vector version of block product function. res  = rhs *res
   */
  template <size_t A, typename T>
  inline void prod(Block<A,A,T> const& lhs, Block<A,1,T>& res)
  {
    size_t b = 0;
    Block<A,1,T> tmp;
     for (size_t i = 0; i < A; ++i, b+= A)
     {
       T dot = 0.0; // 2009-06-30 Kenny: use proper value_traits
       for (size_t k = 0; k < A; ++k)
       {
         dot += lhs[b+k] * res[k];
       }
       tmp[i] = dot; 
     }
     res = tmp; //move semantics might improve this
  }
  
  /**
   * Optimised scalar version of block product function.
   */
  template <typename T>
  inline void prod(Block<1,1,T> const& lhs, Block<1,1,T> const& rhs, Block<1,1,T>& res)
  {
    res = res[0] + lhs[0] * rhs[0];
  }

  /**
  * General version of scalar product res = lhs*rhs
  */
  template <size_t A, size_t B, typename T>
  inline void scalar_prod(T const & lhs, Block<A,B,T> const & rhs, Block<A,B,T> & res)
  {
    for (size_t i = 0; i < A*B; ++i)
    {
      res[i] = lhs * rhs[i];
    }
  }

  /**
  * Sensibly named version of the function above
  */
  template <size_t A, size_t B, typename T>
  inline void mul(T const & lhs, Block<A,B,T> const & rhs, Block<A,B,T> & res)
  {
    scalar_prod(lhs,rhs,res);
  }

  /**
  * Division of a block by a scalar res = rhs /lhs 
  * note the reversal of arguments
  */
  template <size_t A, size_t B, typename T>
  inline void div(T const & rhs, Block<A,B,T> const & lhs, Block<A,B,T> & res)
  {
    for (size_t i = 0; i < A*B; ++i)
    {
      res[i] = lhs[i] / rhs;
    }
  }

  /**
  * General version of scalar product res *= rhs
  */
  template <size_t A, size_t B, typename T>
  inline void scalar_prod(T const & rhs, Block<A,B,T> & res)
  {
    for (size_t i = 0; i < A*B; ++i)
    {
      res[i] *= rhs;
    }
  }

  /**
  * Division of a block by a scalar res /= rhs
  */
  template <size_t A, size_t B, typename T>
  inline void div(T const & rhs, Block<A,B,T> & res)
  {
    for (size_t i = 0; i < A*B; ++i)
    {
      res[i] /= rhs;
    }
  }

  /**
  * Sensibly named version of the function above
  */
  template <size_t A, size_t B, typename T>
  inline void mul(T const & rhs, Block<A,B,T> & res)
  {
    scalar_prod(rhs,res);
  }


  /**
  * General version of block sub res = lhs-rhs
  */
  template <size_t A, size_t B, typename T>
  inline void sub(Block<A,B,T> const & lhs, Block<A,B,T> const & rhs, Block<A,B,T> & res)
  {
    for (size_t i = 0; i < A*B; ++i)
    {
      res[i] = lhs[i] - rhs[i];
    }
  }

  /**
  * General version of block sub res -= rhs
  */
  template <size_t A, size_t B, typename T>
  inline void sub(Block<A,B,T> const & rhs, Block<A,B,T> & res)
  {
    for (size_t i = 0; i < A*B; ++i)
    {
      res[i] -= rhs[i];
    }
  }

  /**
  * Optimised vector-vector version of block sub function. res = lhs - rhs
  */
  template <size_t A, typename T>
  inline void sub(Block<A,1,T> const& lhs, Block<A,1,T> const& rhs, Block<A,1,T>& res)
  {
    for (size_t i = 0; i < A; ++i)
    {
      res[i] = lhs[i] - rhs[i];
    }
  }

  /**
   * Dot product of lhs and rhs, 
   */
  template <size_t N, typename T>
  inline void dot(Block<N,1,T> const& lhs, Block<N,1,T> const& rhs, T & res)
  {
    res = 0;
    for(size_t i = 0; i < N; ++i)
    {
      res += lhs[i] * rhs[i];
      
    }
  }

  /**
   * cross product of three dimensional vectors lhs and rhs. res = lhs x rhs 
   */
  template <typename T>
  inline void cross(Block<3,1,T> const& lhs, Block<3,1,T> const& rhs, Block<3,1,T> & res)
  {
    res[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
    res[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
    res[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
  }


  /**
  * Optimised scalar version of block sub function. res = lhs - rhs
  */
  template <typename T>
  inline void sub(Block<1,1,T> const& lhs, Block<1,1,T> const& rhs, Block<1,1,T>& res)
  {
    res = lhs[0] - rhs[0];
  }

  /**
  * Optimised scalar version of block sub function. res -= rhs
  */
  template <typename T>
  inline void sub(Block<1,1,T> const& rhs, Block<1,1,T>& res)
  {
    res[0] -= rhs[0];
  }

  /**
  * General version of block add res = lhs+rhs
  */
  template <size_t A, size_t B, typename T>
  inline void add(Block<A,B,T> const & lhs, Block<A,B,T> const & rhs, Block<A,B,T> & res)
  {
    for (size_t i = 0; i < A*B; ++i)
    {
      res[i] = lhs[i] + rhs[i];
    }
  }

  /**
  * General version of block add res += rhs
  */
  template <size_t A, size_t B, typename T>
  inline void add(Block<A,B,T> const & rhs, Block<A,B,T> & res)
  {
    for (size_t i = 0; i < A*B; ++i)
    {
      res[i] += rhs[i];
    }
  }

  /**
  * Optimised vector-vector version of block add function. res = lhs + rhs
  */
  template <size_t A, typename T>
  inline void add(Block<A,1,T> const& lhs, Block<A,1,T> const& rhs, Block<A,1,T>& res)
  {
    for (size_t i = 0; i < A; ++i)
    {
      res[i] = lhs[i] + rhs[i];
    }
  }

  /**
  * Optimised scalar version of block add function. res = lhs + rhs
  */
  template <typename T>
  inline void add(Block<1,1,T> const& lhs, Block<1,1,T> const& rhs, Block<1,1,T>& res)
  {
    res = lhs[0] + rhs[0];
  }

  /**
  * Optimised scalar version of block add function. res += rhs
  */
  template <typename T>
  inline void add(Block<1,1,T> const& rhs, Block<1,1,T>& res)
  {
    res[0] += rhs[0];
  }

  /**
   * Trace of a square block
   */
   template <size_t N, typename T>
   inline void trace(Block<N, N, T> const& rhs, T & res)
   {
     res = 0;
     for(size_t i = 0; i < N; ++i){
       res += rhs(i,i);
     }
   }

  /**
   * determinant of a 3x3  block
   */
  template <typename T>
  inline void det(Block<3, 3, T> const& lhs, T & rhs)
  {
    rhs = lhs(0,0) * (lhs(1,1)*lhs(2,2) - lhs(1,2)*lhs(2,1)) 
        - lhs(0,1) * (lhs(1,0)*lhs(2,2) - lhs(1,2)*lhs(2,0)) 
        + lhs(0,2) * (lhs(1,0)*lhs(2,1) - lhs(1,1)*lhs(2,0));
  }
  
  /**
   * Optimised scalar version of block transpose function.
   */
  template <typename T>
  inline void transpose(Block<1, 1, T> const& lhs, Block<1, 1, T>& rhs)
  {
    rhs = lhs;
  }

  /**
   * General version of block transpose function.
   */
  template <size_t M, size_t N, typename T>
  inline void transpose(Block<M, N, T> const& lhs, Block<N, M, T>& rhs)
  {
    size_t k = 0;
    for (size_t i = 0; i < M; ++i, k += N)
    {
      size_t j = 0;
      size_t l = 0;

      for (; j < N; ++j, l += M)
      {
        rhs[l+i] = lhs[k+j];
      }
    }
  }

  /**
   * General version of block transpose in place function for square matrices.
   */
  template <size_t M, typename T>
  inline void transpose(Block<M, M, T>& res)
  {
    size_t k = 0;
    for (size_t i = 0; i < M; ++i, k += M)
    {

      size_t l = k;
      for (size_t j = i; j < M; ++j, l += M)
      {
        T tmp = res[l+i];
        res[l+i] = res[k+j];
        res[k+j] = tmp;
      }
    }
  }

  /**
   * returns the euclidian length of the vector
   */
  template<size_t M, typename T>
  inline T length(Block<M,1,T> A)
  {
    using std::sqrt;

    T len = 0;
    dot(A,A,len);
    len = sqrt(len);
    return len;
  }

  /**
   * Changes A to have length 1 
   */
  template<size_t M, typename T>
  inline void unit(Block<M,1,T> &A)
  {
    div(length(A),A);
  }
  
  /**
   * returns a vector of length 1 with the same direction as A in res
   */
  template<size_t M, typename T>
  inline void unit(Block<M,1,T> const & A, Block<M,1,T> & res)
  {
    div(length(A),A,res);
  }

  /**
   * Returns the diagonal blocks of the matrix A
   * Not tested
   */
  template<size_t M, typename T>
  inline Block<M,M,T> extract_diag(Block<M,M,T> const & A)
  {
    Block<M,M,T> res(0);
    for (size_t i = 0; i<M; ++i)
    {
      res(i,i) = A(i,i);
    }
    return res;
  }
  
  /**
   * Generic Block inversion function.
   * End-user must supply own specialized block inversion routines.
   */ 
  template <size_t N,size_t M, typename T>
  inline void inverse( Block<N,M,T> & b);
  
  /**
   * Specialized scalar block inversion function.
   */
  template <typename T>
  inline void inverse( Block<1,1,T> & b)
  {
    // 2009-06-30 Kenny: Potential Divsion by zero
    // 2009-06-30 Kenny: Make proper use of value traits
    b = 1.0 / b;   
  }


  /**
   * Specialized 3x3 block inversion function. Taken from tiny
   */
  template <typename T>
  inline void inverse( Block<3,3,T> & A)
  {
    Block<3,3,T> adj;
    adj(0,0) = A(1,1)*A(2,2) - A(2,1)*A(1,2);
    adj(1,1) = A(0,0)*A(2,2) - A(2,0)*A(0,2);
    adj(2,2) = A(0,0)*A(1,1) - A(1,0)*A(0,1);
    adj(0,1) = A(1,0)*A(2,2) - A(2,0)*A(1,2);
    adj(0,2) = A(1,0)*A(2,1) - A(2,0)*A(1,1);
    adj(1,0) = A(0,1)*A(2,2) - A(2,1)*A(0,2);
    adj(1,2) = A(0,0)*A(2,1) - A(2,0)*A(0,1);
    adj(2,0) = A(0,1)*A(1,2) - A(1,1)*A(0,2);
    adj(2,1) = A(0,0)*A(1,2) - A(1,0)*A(0,2);
    T det = A(0,0)*adj(0,0) -  A(0,1)*adj(0,1) +   A(0,2)*adj(0,2);

    if(det)
    {
      adj(0,1) = -adj(0,1);
      adj(1,0) = -adj(1,0);
      adj(1,2) = -adj(1,2);
      adj(2,1) = -adj(2,1);
      sparse::transpose(adj);
      sparse::div((T)(det),adj);
      //      A = std::move(adj); // this should be faster but requires c++11
      A = adj;
    }
    assert(false || "trying to invert singular matrix" );
  }

}//namespace sparse

// SPARSE_BLOCK_ALGEBRA_H
#endif
