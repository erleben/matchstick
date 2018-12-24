#ifndef SPARSE_TWO_COLUMN_MATRIX_H
#define SPARSE_TWO_COLUMN_MATRIX_H

#include <sparse_traits.h>            // for zero_block, volatile_zero_block
#include <sparse_index_iterator.h>

#include <vector>
#include <cassert>
#include <algorithm>               // for std::min, std::max


namespace sparse
{
  namespace detail
  {
    
    template <typename M>
    class TwoColumnMatrixAccessor
      {
      public:
        typedef M matrix_type;
        
        typedef typename M::data_container_type     data_container_type;
        typedef typename M::cols_container_type     cols_container_type;
        typedef typename M::is_init_container_type  is_init_container_type;
        
        static size_t       & ncols(matrix_type       & src) { return src.m_ncols; }
        static size_t const & ncols(matrix_type const & src) { return src.m_ncols; }
        
        static data_container_type       & data(matrix_type       & src) { return src.m_data; }
        static data_container_type const & data(matrix_type const & src) { return src.m_data; }
        static cols_container_type       & cols(matrix_type       & src) { return src.m_cols; }
        static cols_container_type const & cols(matrix_type const & src) { return src.m_cols; }
        
        static size_t       & n_not_init(matrix_type       & src) { return src.n_not_init_; }
        static size_t const & n_not_init(matrix_type const & src) { return src.m_n_not_init; }
        
        static is_init_container_type       & is_init(matrix_type       & src) { return src.m_is_init; }
        static is_init_container_type const & is_init(matrix_type const & src) { return src.m_is_init; }
        
        static typename matrix_type::reference init_block(size_t const i, size_t const j, matrix_type const& src) { return src.init_block_(i, j); }
      };
    
  }//namespace detail
  
  /**
   * Specialized Two Column Matrix Class.
   *
   * @tparam B     The block type of the matrix.
   */
	template <typename B>
	class TwoColumnMatrix
    {
    protected:
      
      typedef std::vector<B>                          data_container_type;
      typedef std::vector<size_t>                     cols_container_type;
      typedef std::vector<bool>                       is_init_container_type;
      
    public:
      
      typedef TwoColumnMatrix<B>                      matrix_type;
      typedef B                                       block_type;
      typedef block_type&                             reference;
      typedef block_type const&                       const_reference;
      typedef block_type*                             pointer;
      typedef block_type const*                       const_pointer;
      typedef IndexIterator<false, matrix_type>       iterator;
      typedef IndexIterator<true, matrix_type>        const_iterator;
      
      typedef detail::TwoColumnMatrixAccessor<matrix_type>    accessor;
      
    private:
      
      friend class detail::TwoColumnMatrixAccessor<matrix_type>;
      
    protected:
      
      size_t                 m_ncols;
      data_container_type    m_data;
      cols_container_type    m_cols;
      // 2009-07-01 Kenny: Documentation, it is not obvious to me what m_n_not_init and m_is_init are used for?
      size_t                 m_n_not_init;
      is_init_container_type m_is_init;
      
    protected:
      
      // 2009-07-01 Kenny: Why not use this-> when accessing members/methods in a template class?
      reference safe_init_block(size_t const i, size_t const j)
      {
        size_t const i_x2 = i << 1;
        
        if (!m_is_init[i_x2])
        {
          --m_n_not_init;
          m_is_init[i_x2] = true;
          m_cols[i_x2] = j;
          return m_data[i_x2];
        }
        else if (!m_is_init[i_x2+1])
        {
          assert(m_cols[i_x2] != j || !"Initialising same block multiple times not presently supported"   );
          --m_n_not_init;
          m_is_init[i_x2+1] = true;
          if (m_cols[i_x2] > j)
          {
            m_cols[i_x2+1] = m_cols[i_x2];
            m_cols[i_x2] = j;
            std::swap(m_data[i_x2],m_data[i_x2+1]);
            return m_data[i_x2];
          }
          else
          {
            m_cols[i_x2+1] = j;
            return m_data[i_x2+1];
          }
        }
        // 2009-07-14 Kenny: Warning c4715 not all control paths return a value
        assert(false && "TwoColumnMatrix.safe_init_block(i,j): invalid access");
      }
      
    private:
      
      // 2009-07-01 Kenny: Document why private?
      matrix_type& operator=(matrix_type const& orig);
      
    public:
      
      TwoColumnMatrix()
      : m_ncols(0)
      , m_data(0)
      , m_cols(0)
      , m_n_not_init(0)
      , m_is_init(0)
      {}
      
      TwoColumnMatrix(size_t const m, size_t const n)
      : m_ncols(n)
      , m_data(m << 1, zero_block<block_type>())
      , m_cols(m << 1, 0u)
      , m_n_not_init(m << 1)
      , m_is_init((m << 1), false)
      {
        // 2009-07-01 Kenny: Proper ADL?
        assert(std::min(m,n) > 0 ? (n > 1 && n % 2 == 0) : std::max(m,n) == 0  || !"Invalid Arguments");
      }
      
      TwoColumnMatrix(size_t const m, size_t const n, size_t const nnz)
			: m_ncols(n)
      , m_data(m << 1, zero_block<block_type>())
      , m_cols(m << 1, 0u)
      , m_n_not_init(m << 1)
      , m_is_init((m << 1), false)
      {
        // 2009-07-01 Kenny: Proper ADL?
        assert(std::min(m,n) > 0 ? (n > 1 && n % 2 == 0) : std::max(m,n) == 0 || !"Invalid Arguments");
        assert(nnz >= (m << 1) || !"Invalid Arguments");
        
        // 2009-07-01 Kenny: Why not use this-> when accessing members/methods in a template class?
        m_data.reserve(nnz);
        m_cols.reserve(nnz);
        m_is_init.reserve(nnz);
      }
      
      TwoColumnMatrix(matrix_type const& orig)
      : m_ncols(orig.m_ncols)
      , m_data(orig.m_data)
      , m_cols(orig.m_cols)
      , m_n_not_init(orig.m_ncols)
      , m_is_init(orig.m_is_init)
      {}
      
      ~TwoColumnMatrix(){}
      
      /**
       * @warning if block is not initialised, col-index may be garbage
       */
      // 2009-07-01 Kenny: Why not use this-> when accessing members/methods in a template class?
      // 2009-07-01 Kenny: Documentation what is difference between [] and ()?
      reference operator[](size_t const i)
      {
        assert(i < size() || !"i was too large");
        return m_data[i];
      }
      
      /**
       * @warning if block is not initialised, col-index may be garbage
       */
      // 2009-07-01 Kenny: Why not use this-> when accessing members/methods in a template class?
      // 2009-07-01 Kenny: Documentation what is difference between [] and ()?
      const_reference operator[](size_t const i) const
      {
        assert(i < size() || !"i was too large");
        return m_data[i];
      }
      
      // 2009-07-01 Kenny: Why not use this-> when accessing members/methods in a template class?
      // 2009-07-01 Kenny: Documentation what is difference between [] and ()?
      reference operator()(size_t const i, size_t const j)
      {
        assert( ((i < nrows()) && (j < ncols())) || !"i or j were too large");
       
        size_t const i_x2 = i << 1;
        if (m_n_not_init > 0)
        {
          return safe_init_block(i, j);
        }
        else // Matrix has been initialised, no setting of new block indices
        {
          if (m_cols[i_x2] == j)
          {
            return m_data[i_x2];
          }
          else if (m_cols[i_x2 + 1] == j)
          {
            return m_data[i_x2 + 1];
          }
          else
          {
            // 2009-07-14 Kenny: Warning c4715 not all control paths return a value
            assert(false && "operator(i,j) to non-existant non-zero index");
          }
        }
      }
      
      // 2009-07-01 Kenny: Why not use this-> when accessing members/methods in a template class?
      // 2009-07-01 Kenny: Documentation what is difference between [] and ()?
      const_reference operator()(size_t const i, size_t const j) const
      {
        assert(  ((i < nrows()) && (j < ncols())) || !"i or j were too large");
        
        size_t const i_x2 = i << 1;
        
        if (m_cols[i_x2] == j)
        {
          return m_data[i_x2];
        }
        else if (m_cols[i_x2 + 1] == j)
        {
          return m_data[i_x2 + 1];
        }
        return zero_block<block_type>();
      }
      
      // 2009-07-01 Kenny: Why not use this-> when accessing members/methods in a template class?
      void resize(size_t const m, size_t const n, size_t const nnz, bool preserve = true)
      {
        assert(m_n_not_init == 0  || !"matrix must be initialized before resize" );      
        assert(n == 0 || n >= 2   || !"Invalid Arguments");
        assert(nnz >= (m << 1)    || !"Invalid Arguments");
        
        if(!preserve)
        {
          assert(false || !"preseve is not supported");
        }
        
        size_t m_x2 = m << 1;
        m_data.reserve(nnz);
        m_cols.reserve(nnz);
        m_is_init.reserve(nnz);
        m_data.resize(m_x2, zero_block<block_type>());
        m_cols.resize(m_x2, 0u);
        if (m_x2 > m_is_init.size())
        {
          m_n_not_init = m_x2 - m_is_init.size();
        }
        m_is_init.resize(m_x2, false);
        m_ncols = n;
      }
      
      // 2009-07-01 Kenny: Why not use this-> when accessing members/methods in a template class?
      void clear()
      {
        m_data.clear();
        m_cols.clear();
        m_is_init.clear();
        m_ncols = 0;
      }

      /**
      * Returns the column of the ith element of the data array
      */
      const size_t col_of_idx(size_t i) const
      { 
        assert( i < m_data.size()  || !"index must be smaller than the size of data" );
        return m_cols[i]; 
      }
      
      iterator       begin()       { return iterator(0u, this);           }
      const_iterator begin() const { return const_iterator(0u, this);     }
      // 2009-07-01 Kenny: Why not use this-> when accessing members/methods in a template class?
      iterator       end()         { return iterator(size(), this);       }
      const_iterator end()   const { return const_iterator(size(), this); }
      
      // 2009-07-01 Kenny: Why not use this-> when accessing members/methods in a template class?
      size_t size()  const { return m_data.size();      }
      size_t ncols() const { return m_ncols;            }
      size_t nrows() const { return m_data.size() >> 1; }
      
    };
  
} // namespace sparse

// SPARSE_TWO_COLUMN_MATRIX_H
#endif 
