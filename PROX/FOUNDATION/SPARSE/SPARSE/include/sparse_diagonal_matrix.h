#ifndef SPARSE_DIAGONAL_MATRIX_H
#define SPARSE_DIAGONAL_MATRIX_H

#include <sparse_traits.h>           // for zero_block, volatile_zero_block
#include <sparse_index_iterator.h>

#include <vector>

namespace sparse
{
  
  
  namespace detail
  {
    /**
     * Diagonal Matrix Accessor Class.
     *
     * @tparam M    The matrix type that is wanted access to.
     */
    template <typename M>
    class DiagonalMatrixAccessor
      {
      public:
        
        typedef M matrix_type;
        typedef typename M::data_container_type data_container_type;
        
        static data_container_type       & data(matrix_type       & src) { return src.m_data; }
        static data_container_type const & data(matrix_type const & src) { return src.m_data; }
      };
  }//namespace detail  
  
  
  /**
   * Specialized Diagonal Matrix Class.
   *
   * @tparam B  The block type of the blocked diagonal matrix.
   */
	template <typename B>
	class DiagonalMatrix
    {
    protected:
      
      typedef std::vector<B>                          data_container_type;
      
    public:
      
      typedef DiagonalMatrix<B>                       matrix_type;
      typedef B                                       block_type;
      typedef block_type&                             reference;
      typedef block_type const&                       const_reference;
      typedef block_type*                             pointer;
      typedef block_type const*                       const_pointer;
      typedef IndexIterator<false, matrix_type>       iterator;
      typedef IndexIterator<true, matrix_type>        const_iterator;
      
      typedef detail::DiagonalMatrixAccessor<matrix_type>     accessor;
      
    protected:
      
      friend class detail::DiagonalMatrixAccessor<matrix_type>;
      
    protected:
      
      data_container_type m_data;
      
    private:
      
      // 2009-07-01 Kenny: Documentation, why is this private?
      matrix_type& operator=(matrix_type const& orig);
      
    public:
      
      DiagonalMatrix(size_t const n = 0u)
      : m_data(n, zero_block<block_type>())
      {}
      
      /**
       * @note Necessary to fulfil Matrix concept
       */
      DiagonalMatrix(size_t const m, size_t const n, size_t const nnz)
			: m_data(m, zero_block<block_type>())
      {
        // 2009-07-01 Kenny: Why not use this-> for access to member and methods in template classes?
        assert(((m == n) && (nnz >= m)) || !"Invalid arguments");
        m_data.reserve(nnz);
      }
      
      DiagonalMatrix(matrix_type const& orig)
			: m_data(orig.m_data)
      {}
      
      ~DiagonalMatrix(){}
      
      // 2009-07-01 Kenny: What do you by: ``must be unchecked''?
      /**
       * @note Must be unchecked (part of concept and IndexIterator depends on it)
       */
      // 2009-07-01 Kenny: assertion to test for valid input?
      // 2009-07-01 Kenny: Documentation what is difference between [] and ()?
      // 2009-07-01 Kenny: Why not use this-> for access to member and methods in template classes?
      reference operator[](size_t const i) { return m_data[i]; }
      
      // 2009-07-01 Kenny: What do you by: ``must be unchecked''?
      /**
       * @note Must be unchecked (part of concept and IndexIterator depends on it)
       */
      // 2009-07-01 Kenny: assertion to test for valid input?
      // 2009-07-01 Kenny: Documentation what is difference between [] and ()?
      // 2009-07-01 Kenny: Why not use this-> for access to member and methods in template classes?
      const_reference operator[](size_t const i) const { return m_data[i]; }
      
      // 2009-07-01 Kenny: assertion to test for valid input?
      // 2009-07-01 Kenny: Documentation what is difference between [] and ()?
      // 2009-07-01 Kenny: Why not use this-> for access to member and methods in template classes?
      reference operator()(size_t const i) { return m_data[i]; }
      
      // 2009-07-01 Kenny: assertion to test for valid input?
      // 2009-07-01 Kenny: Documentation what is difference between [] and ()?
      // 2009-07-01 Kenny: Why not use this-> for access to member and methods in template classes?
      const_reference operator()(size_t const i) const { return m_data[i]; }
      
      // 2009-07-01 Kenny: Documentation what is difference between [] and ()?
      // 2009-07-01 Kenny: Why not use this-> for access to member and methods in template classes?
      reference operator()(size_t const i, size_t const j)
      {
        assert(((i < nrows()) && (j < ncols())) || !"i or j were too large" );
        return (i == j) ? m_data[i] : volatile_zero_block<block_type>();
      }
      
      // 2009-07-01 Kenny: Documentation what is difference between [] and ()?
      // 2009-07-01 Kenny: Why not use this-> for access to member and methods in template classes?
      const_reference operator()(size_t const i, size_t const j) const
      {
        assert(((i < nrows()) && (j < ncols())) || !"i or j were too large");
        return (i == j) ? m_data[i] : zero_block<block_type>();
      }
      
      // 2009-07-01 Kenny: Proper ADL for std::fill?
      // 2009-07-01 Kenny: Why not use this-> for access to member and methods in template classes?
      void resize(size_t const n, bool preserve = true)
      {
        if (!preserve)
        {
          std::fill(m_data.begin(), m_data.end(), zero_block<block_type>());
        }
        m_data.resize(n, zero_block<block_type>());
      }
      
      // 2009-07-01 Kenny: Proper ADL for std::fill?
      // 2009-07-01 Kenny: Why not use this-> for access to member and methods in template classes?
      void resize(size_t const m, size_t const n, size_t const nnz, bool preserve = true)
      {
        assert(((m == n) && (nnz >= m)) || !"Invalid arguments");
        if (!preserve)
        {
          std::fill(m_data.begin(), m_data.end(), zero_block<block_type>());
        }
        m_data.reserve(nnz);
        m_data.resize(n, zero_block<block_type>());
      }
      
      // 2009-07-01 Kenny: Why not use this-> for access to member and methods in template classes?
      void clear() { m_data.clear(); }
      
      // 2009-07-01 Kenny: Why not use this-> for access to member and methods in template classes?
      size_t size()  const { return m_data.size(); }
      size_t nrows() const { return size();        }
      size_t ncols() const { return nrows();       }
      
      iterator       begin()       { return iterator(0u, this);           }
      const_iterator begin() const { return const_iterator(0u, this);     }
      // 2009-07-01 Kenny: Why not use this-> for access to member and methods in template classes?
      iterator       end()         { return iterator(size(), this);       }
      const_iterator end()   const { return const_iterator(size(), this); }
      
    };
  
} // namespace sparse

// SPARSE_DIAGONAL_MATRIX_H
#endif 
