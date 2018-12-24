#ifndef SPARSE_COMPRESSED_ROW_MATRIX_H
#define SPARSE_COMPRESSED_ROW_MATRIX_H

#include <sparse_traits.h>              // for zero_block, volatile_zero_block
#include <sparse_index_iterator.h>
#include <sparse_row_iterator.h>

#include <vector>
#include <cassert>
#include <algorithm>  // for std::min, std::max, std::lower_bound
#include <iterator>   // for std::distance

namespace sparse
{
  
  namespace detail
  {
    
    /**
     * Compressed Row Matrix Accessor Class.
     *
     * @tparam M    The matrix type that is wanted access to.
     */
    template <typename M>
    class CompressedRowMatrixAccessor
      {
      public:
        
        typedef M matrix_type;
        typedef typename M::data_container_type     data_container_type;
        typedef typename M::cols_container_type     cols_container_type;
        typedef typename M::row_ptrs_container_type row_ptrs_container_type;
        
        static size_t       & nrows(matrix_type& src)       { return src.m_nrows; }
        static size_t const & nrows(matrix_type const& src) { return src.m_nrows; }
        static size_t       & ncols(matrix_type& src)       { return src.m_ncols; }
        static size_t const & ncols(matrix_type const& src) { return src.m_ncols; }
        
        static size_t find_row(size_t const i, matrix_type const& src)     { return src.safe_find_row(i);        }
        static size_t upd_row_ptrs(size_t const i, matrix_type const& src) { return src.safe_update_row_ptrs(i); }
        
        static data_container_type           & data(matrix_type       & src) { return src.m_data; }
        static data_container_type const     & data(matrix_type const & src) { return src.m_data; }
        static cols_container_type           & cols(matrix_type       & src) { return src.m_cols; }
        static cols_container_type const     & cols(matrix_type const & src) { return src.m_cols; }
        
        static row_ptrs_container_type       & row_ptrs(matrix_type       & src) { return src.m_row_ptrs; }
        static row_ptrs_container_type const & row_ptrs(matrix_type const & src) { return src.m_row_ptrs; }
      };
    
  } // namespace detail
  
  /**
   * Generic Blocked Compressed Row Matrix.
   *
   * @tparam B   The block type of the compressed row matrix.
   */
  template <typename B>
  class CompressedRowMatrix
    {
    public:
      
      typedef CompressedRowMatrix<B>                  matrix_type;
      typedef B                                       block_type;
      typedef block_type&                             reference;
      typedef block_type const&                       const_reference;
      typedef block_type*                             pointer;
      typedef block_type const*                       const_pointer;
      
      typedef IndexIterator<false, matrix_type>       iterator;
      typedef IndexIterator<true,  matrix_type>       const_iterator;
      typedef RowIterator<false, matrix_type>         row_iterator;
      typedef RowIterator<true,  matrix_type>         const_row_iterator;
      
      typedef detail::CompressedRowMatrixAccessor<matrix_type>  accessor;
      
    protected:
      
      typedef std::vector<block_type>                 data_container_type;
      typedef std::vector<size_t>                     cols_container_type;
      typedef std::vector<size_t>                     row_ptrs_container_type;
      
      typedef typename data_container_type::iterator            data_iterator;
      typedef typename data_container_type::const_iterator      const_data_iterator;
      typedef typename cols_container_type::iterator            cols_iterator;
      typedef typename cols_container_type::const_iterator      const_cols_iterator;
      typedef typename row_ptrs_container_type::iterator        row_ptrs_iterator;
      typedef typename row_ptrs_container_type::const_iterator  const_row_ptrs_iterator;
      
    private:
      
      friend class detail::CompressedRowMatrixAccessor<matrix_type>;
      
    protected:
      
      size_t                  m_nrows;
      size_t                  m_ncols;
      row_ptrs_container_type m_row_ptrs;
      cols_container_type     m_cols;
      data_container_type     m_data;
      
    protected:
      
      // 2009-06-30 Kenny: Documentation is needed
      void safe_update_row_ptrs(size_t const row)
      {
        // 2009-06-30 Kenny: Why not use this-> for member access in template class?
        assert(row >= m_row_ptrs.size()-2  || !"row was too large");
        
        if (row >= m_row_ptrs.size()-1)
        {
          m_row_ptrs.resize(row+2, m_row_ptrs.back());
        }
        ++m_row_ptrs.back();
      }
      
      // 2009-06-30 Kenny: Documentation is needed
      size_t safe_find_row(size_t const i) const
      {
        // 2009-06-30 Kenny: Why not use this-> for method access in template class?
        assert(i < size() || !"i was too large");
        
        // 2009-06-30 Kenny: proper ADL support
        const_row_ptrs_iterator iter = std::upper_bound(m_row_ptrs.begin(), m_row_ptrs.end(), i);
        return std::distance(m_row_ptrs.begin(), iter) - 1;
      }
      
    private:
      
      // 2009-06-30 Kenny: Document why assignment operator is not available
      matrix_type& operator=(matrix_type const& orig);
      
    public:
      
      CompressedRowMatrix()
      : m_nrows(0)
      , m_ncols(0)
      , m_row_ptrs(2, 0u)
      , m_cols(0)
      , m_data(0)
      {}
      
      CompressedRowMatrix(size_t const m, size_t const n, size_t const nnz)
      : m_nrows(m)
      , m_ncols(n)
      , m_row_ptrs(2, 0u)
      , m_cols(0)
      , m_data(0)
      {
        // 2009-06-30 Kenny: proper ADL support
        assert(std::min(m,n) > 0 ? true : std::max(m,n) == 0  || !"Invalid arguments");
        assert(nnz <= m*n || !"Invalid arguments");
        
        // 2009-06-30 Kenny: Why not use this-> for member access in template class?
        m_row_ptrs.reserve(m+1);
        m_cols.reserve(nnz);
        m_data.reserve(nnz);
      }
      
      CompressedRowMatrix(matrix_type const& orig)
      : m_nrows(orig.m_nrows)
      , m_ncols(orig.m_ncols)
      , m_row_ptrs(orig.m_row_ptrs)
      , m_cols(orig.m_cols)
      , m_data(orig.m_data)
      {}
      
      ~CompressedRowMatrix(){}
      
      // 2009-06-30 Kenny: Documentation Needed, what is difference between [] and ()?
      __attribute__((always_inline)) reference operator[](size_t const i)
      {
        assert(i < size() || !"i was too large");
        return m_data[i];
      }
      
      // 2009-06-30 Kenny: Documentation Needed, what is difference between [] and ()?
      __attribute__((always_inline)) const_reference operator[](size_t const i) const
      {
        assert(i < size() || !"i was too large");
        return m_data[i];
      }
      
      /**
       *
       * If (i,j) do not exist then a const block with all-zero entries is returned
       *
       * Time complexity is O(lg(nrows()) due to binary search
       */
      // 2009-06-30 Kenny: Documentation Needed, what is difference between [] and ()?
      // 2009-06-30 Kenny: Why not use this-> for member access in template class?
      // 2009-06-30 Kenny: proper ADL support
      __attribute__((always_inline)) const_reference operator()(size_t const i, size_t const j) const
      {
        assert(((i < nrows()) && (j < ncols())) || !"i or j were too large");
        
        if (i < m_row_ptrs.size()-1)
        {
          size_t row_first = m_row_ptrs[i];
          size_t row_last  = m_row_ptrs[i+1];
          if (row_first != row_last)
          {
            const_cols_iterator cols_first = m_cols.begin()+row_first;
            const_cols_iterator cols_last  = m_cols.begin()+row_last;
            
            const_cols_iterator cols_iter = std::lower_bound(m_cols.begin()+row_first, cols_last, j);
            
            if (cols_iter != cols_last && *cols_iter == j)
            {
              size_t const idx = row_first + std::distance(cols_first, cols_iter);
              return m_data[idx];
            }
          }
        }
        return zero_block<block_type>();
      }
      
      // 2009-06-30 Kenny: Why not use this-> for member access in template class?
      // 2009-06-30 Kenny: proper ADL support
      __attribute__((always_inline)) reference operator()(size_t const i, size_t const j)
      {
        // let (i,j) be the row and column index of the non-zero element with
        // highest row-column lex-order (e.g. 2,2 < 2,3 < 3,1) then calling
        // operator(a,b) has two semantics:
        //     IF [i,j < a,b]
        //         assume that a new element is inserted
        //         => insert element and return reference
        //     ELSE
        //         assume that an existing element is referenced
        //         => return reference
        //         (if (a,b) is non-existant, the object is in an incorrect state)
        //
        // on insert (requisite: lex-ordered by row-col): O(size()) (vector.push_back())
        //        IF [inserting into row already containing an element]
        //            a binary search on the nnz's of the row is performed before
        //            doing vector.push_back() on two vectors
        //        ELSE
        //            cost is a vector.push.back() on three vectors
        //
        // on read: O(lg(nrows()) (binary search)
        assert(((i < nrows()) && (j < ncols())) || !"i or j were too large");
        
        if (i < m_row_ptrs.size() - 2
            ||
            ( i == m_row_ptrs.size()-2 && !m_cols.empty() && j <= m_cols.back() )
            )
        {
          // case: element referenced
          size_t const row_first = m_row_ptrs[i];
          size_t const row_last  = m_row_ptrs[i+1];
          if (row_first != row_last)
          {
            cols_iterator cols_last = m_cols.begin() + row_last;
            cols_iterator cols_iter = std::lower_bound(m_cols.begin() + row_first, cols_last, j);
            if (cols_iter != cols_last && *cols_iter == j)
            {
              size_t idx = std::distance(m_cols.begin(), cols_iter);
              return m_data[idx];
            }
          }
          assert(false && "CompressedRowMatrix.operator(i,j): invalid access");
        }
        // case: insert new element
        safe_update_row_ptrs(i);
        m_cols.push_back(j);
        m_data.push_back(zero_block<block_type>());
        return m_data.back();
      }
      
      // 2009-06-30 Kenny: Why not use this-> for member access in template class?
      void resize(size_t const m, size_t const n, size_t const nnz, bool preserve = true)
      {
        assert(((nrows() <= m) && (ncols() <= n)) || !"Sorry shrinking is not supported"); 
        m_nrows = m;
        m_ncols = n;
        if (!preserve)
        {
          m_row_ptrs.erase(m_row_ptrs.begin()+2, m_row_ptrs.end());
          m_row_ptrs[1] = 0u;
          m_cols.clear();
          m_data.clear();
        }
        m_row_ptrs.reserve(m+1);
        m_cols.reserve(nnz);
        m_data.reserve(nnz);
      }
      
      // 2009-06-30 Kenny: Why not use this-> for member access in template class?
      size_t size()  const { return m_data.size(); }
      size_t ncols() const { return m_ncols;       }
      size_t nrows() const { return m_nrows;       }
      
      // 2009-06-30 Kenny: Why not use this-> for member access in template class?
      void clear()
      {
        m_nrows = 0u;
        m_ncols = 0u;
        m_row_ptrs.resize(2,0u);
        m_row_ptrs[1] = 0u;
        m_cols.clear();
        m_data.clear();
      }
      
      iterator       begin()       { return iterator(0u, this);           }
      const_iterator begin() const { return const_iterator(0u, this);     }
      // 2009-06-30 Kenny: Why not use this-> for member access in template class?
      iterator       end()         { return iterator(size(), this);       }
      const_iterator end()   const { return const_iterator(size(), this); }
      
      // 2009-06-30 Kenny: What is the difference between iterator and row_iterator? Please Document.
      // 2009-06-30 Kenny: Why not use this-> for member access in template class?
      row_iterator row_begin(size_t const r)
      {
        assert(r < nrows()  || !"r was too large");        
        return row_iterator(this, (r < m_row_ptrs.size()-1
                                   ? m_row_ptrs[r]
                                   : m_row_ptrs[m_row_ptrs.size()-1]), r);
      }
      
      // 2009-06-30 Kenny: What is the difference between iterator and row_iterator? Please Document.
      // 2009-06-30 Kenny: Why not use this-> for member access in template class?
      const_row_iterator row_begin(size_t const r) const
      {
        assert(r < nrows() || !"r was too large");
        return const_row_iterator(this, (r < m_row_ptrs.size()-1
                                         ? m_row_ptrs[r]
                                         : m_row_ptrs[m_row_ptrs.size()-1]), r);
      }
      
      // 2009-06-30 Kenny: What is the difference between iterator and row_iterator? Please Document.
      // 2009-06-30 Kenny: Why not use this-> for member access in template class?
      // 2009-06-30 Kenny: Why no assert on r-parameter?
      row_iterator row_end(size_t const r)
      {
        return row_iterator(this, (r < m_row_ptrs.size()-1
                                   ? m_row_ptrs[r+1]
                                   : m_row_ptrs[m_row_ptrs.size()-1]), r);
      }
      
      // 2009-06-30 Kenny: What is the difference between iterator and row_iterator? Please Document.
      // 2009-06-30 Kenny: Why not use this-> for member access in template class?
      // 2009-06-30 Kenny: Why no assert on r-parameter?
      const_row_iterator row_end(size_t const r) const
      {
        return const_row_iterator(this, (r < m_row_ptrs.size()-1
                                         ? m_row_ptrs[r+1]
                                         : m_row_ptrs[m_row_ptrs.size()-1]), r);
      }
      
      
      // 2009-06-30 Kenny: Why not use this-> for member access in template class?
      // 2009-06-30 Kenny: Documentation Needed.
      size_t top_non_zero_row() const { return m_row_ptrs.size()-1; }

      /**
      * Returns the column of the ith element of the data array
      */
      size_t col_of_idx(size_t i) const
      { 
        assert( i < m_data.size()  || !"index must be smaller than the size of data" );
        return m_cols[i]; 
      }

      /**
      * Returns the column of the ith element of the data array
      */
      size_t row_idx(size_t i) const
      { 
        assert( i < m_row_ptrs.size()  || !"index must be smaller than the number if rows" );
        return m_row_ptrs[i]; 
      }

      bool is_initialized(size_t const & i, size_t const & j) const
      {
        if (i < m_row_ptrs.size() - 1)
        {
          size_t row_first = m_row_ptrs[i];
          size_t row_last  = m_row_ptrs[i+1];

          if (row_first != row_last)
          {
            const_cols_iterator cols_last  = m_cols.begin()+row_last;

            const_cols_iterator cols_iter = std::lower_bound(m_cols.begin()+row_first, cols_last, j);

            if (cols_iter != cols_last && *cols_iter == j)
            {
              return true;
            }
          }
        }
        return false;
      }
      
    };

} // namespace sparse

// SPARSE_COMPRESSED_ROW_MATRIX_H
#endif 
