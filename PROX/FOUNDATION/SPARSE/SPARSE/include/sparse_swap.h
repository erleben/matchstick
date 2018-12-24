#ifndef SPARSE_SWAP_H
#define SPARSE_SWAP_H

#include <sparsefwd.h>  

#include <algorithm> // for STL swap

namespace sparse
{  
  // 2009-07-01 Kenny: Does not appear to be tested anywhere?
  // 2009-07-01 Kenny: Should swap_data be moved into namespace detail? Or do end user have explicit usage of this?

  /**
   * @warning PRECONDITION: assumes that data of A contains a swap function that takes data of B as argument
   */ 
  template<typename M1, typename M2>
  inline void swap_data(M1& A, M2& B)  { M1::accessor::data(A).swap(M2::accessor::data(B)); }

  template<typename B>
  inline void swap_data(
                   CompressedRowMatrix<B>& A
                 , typename CompressedRowMatrix<B>::accessor::container_type& data
                 )
  {
    CompressedRowMatrix<B>::accessor::data(A).swap(data);
  }

  
  template<typename B>
  inline void swap_data(
                   DiagonalMatrix<B>& A
                 , typename DiagonalMatrix<B>::accessor::container_type& data
                )
  {
    DiagonalMatrix<B>::accessor::data(A).swap(data);
  }

  template<typename B>
  inline void swap_data(
                 Vector<B>& u
                 , typename Vector<B>::accessor::container_type& data
                 )
  {
    Vector<B>::accessor::data(u).swap(data);
  }

  template<typename B>
  inline void swap_data(
                 TwoColumnMatrix<B>& A
                 , typename TwoColumnMatrix<B>::accessor::container_type& data
                 )
  {
    TwoColumnMatrix<B>::accessor::data(A).swap(data);
  }
  
  
  template<typename B>
  inline void swap(CompressedRowMatrix<B>& C1, CompressedRowMatrix<B>& C2)
  {
    typedef typename CompressedRowMatrix<B>::accessor A;
    
    swap_data(C1, C2);
    
    A::ptr(C1).swap(A::ptr(C2));
    
    std::swap( A::ncols(C1), A::ncols(C2) );
  }
  
  template<typename B>
  inline void swap(DiagonalMatrix<B>& D1, DiagonalMatrix<B>& D2) { swap_data(D1, D2); }
  
  template<typename B>
  inline void swap(Vector<B>& u, Vector<B>& v) { swap_data(u, v); }

  template<typename B>
  inline void swap(TwoColumnMatrix<B>& T1, TwoColumnMatrix<B>& T2)
  {
    typedef typename TwoColumnMatrix<B>::accessor A;
    
    swap_data(T1, T2);
    
    std::swap( A::ncols(T1), A::ncols(T2));
  }

} // namespace sparse

// SPARSE_SWAP_H
#endif 
