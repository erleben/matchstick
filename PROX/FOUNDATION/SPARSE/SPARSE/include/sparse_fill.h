#ifndef SPARSE_FILL_H
#define SPARSE_FILL_H

#include <sparsefwd.h>

#include <cstdlib>  // for srand and rand
#include <ctime>    // for time

#define SPARSE_DEF_MAX_FILL_RANDOM_VALUE 1000

namespace sparse 
{
  
  template <typename B>
  void fill(B& block, typename B::value_type x = 0)
  {
    typename B::iterator iter = block.begin();
    typename B::iterator last = block.end();
    
    for(; iter != last; ++iter)
    {
      *iter = x;
      x += typename B::value_type(1);    // 2009-11-25 Kenny: This is not obvious functionality! STL's fill just fill in the same value, this is more like incremental fill?
    }
  }
  
  // rand() assumed seeded by caller
  template <typename B>
  inline
  void rfill(B& block)
  {
    typename B::iterator iter = block.begin();
    typename B::iterator last = block.end();
    
    for(; iter != last; ++iter)
    {
      *iter = rand() % SPARSE_DEF_MAX_FILL_RANDOM_VALUE + 1;
    }
  }
  
  template <typename B>
  inline void fill(Vector<B>& u)
  {
    for (size_t i = 0; i < u.size(); ++i)
    {
      fill(u(i));
    }
  }
  
  template <typename B>
  inline
  void rfill(Vector<B>& u)
  {
    typedef typename Vector<B>::iterator iterator;
    iterator iter = u.begin();
    iterator last = u.end();
    srand((unsigned)time(0));
    for (; iter != last; ++iter)
    {
      rfill(*iter);
    }
  }
  
  template <typename B>
  inline void fill(DiagonalMatrix<B>& A)
  {
    for (size_t i = 0; i < A.size(); ++i)
    {
      fill( A(i) );
    }
  }
  
  template <typename B>
  inline
  void rfill(DiagonalMatrix<B>& A)
  {
    typedef typename DiagonalMatrix<B>::iterator iterator;
    iterator iter = A.begin();
    iterator last = A.end();
    
    srand((unsigned)time(0));
    
    for (; iter != last; ++iter)
    {
      rfill(*iter);
    }
  }
  
  template <typename B>
  inline void fill(TwoColumnMatrix<B> & A)
  {
    size_t col1, col2;
    
    srand((unsigned)time(0));
    
    for (size_t i = 0; i < A.nrows(); ++i)
    {
      col1 = rand() % A.ncols();
      do
      {
        col2 = rand() % A.ncols();
      }
      while (col1 == col2);
      if (col2 < col1) // (block-wise) ordered insertion
      {
        std::swap(col1, col2);
      }
      fill(A(col1, col2));
    }
  }
  
  template <typename B>
  inline void rfill(TwoColumnMatrix<B>& A)
  {
    srand((unsigned)time(0));
    size_t col1, col2;
    for (size_t i = 0; i < A.nrows(); ++i)
    {
      col1 = rand() % A.ncols();
      do
      {
        col2 = rand() % A.ncols();
      }
      while (col1 == col2);
      if (col2 < col1) // (block-wise) ordered insertion
      {
        std::swap(col1, col2);
      }
      rfill(A(i, col1));
      rfill(A(i, col2));
    }
  }
  
  inline void construct_rand_idx_sequence( std::vector<std::pair<size_t,size_t> >& idxs, size_t nrows, size_t ncols)
  {
    srand((unsigned)time(0));
    for (size_t i = 0; i < idxs.size(); ++i)
    {
      // a duplicate might creep up, but shouldn't shake results much
      idxs[i] = std::make_pair(rand() % nrows, rand() % ncols);
    }
    std::sort(idxs.begin(), idxs.end()); // ordered insert is worth a sort
  }
  
  template<typename B>
  inline void fill(CompressedRowMatrix<B>& A, size_t nnz)
  {
    std::vector<std::pair<size_t,size_t> > idxs(nnz);
    construct_rand_idx_sequence(idxs, A.nrows(), A.ncols());
    for (size_t i = 0; i < nnz; ++i)
    {
      fill(A(idxs[i].first,idxs[i].second));
    }
  }
  
  template <typename B>
  inline void rfill(CompressedRowMatrix<B>& A, size_t const nnz)
  {
    std::vector< std::pair<size_t,size_t> > idxs(nnz);
    
    construct_rand_idx_sequence(idxs, A.nrows(), A.ncols());
    srand((unsigned)time(0));
    for (size_t i = 0; i < nnz; ++i)
    {
      rfill(A(idxs[i].first,idxs[i].second));
    }
  }
  
} // namespace sparse

// SPARSE_FILL_H
#endif 
