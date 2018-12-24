#ifndef SPARSE_ROW_ITERATOR_H
#define SPARSE_ROW_ITERATOR_H

#include <sparse_if_then_else.h> 

#include <iterator>     // for iterator_traits

namespace sparse
{
  
  // 2009-07-01 Kenny: What is a row iterator? What is it used for? Why is it different than the index iterator? It appears to be the same as index iterator only difference is that it got a row member which is never changed? So shy not just have an ``annotated'' version of index iterator?
  /**
   * Row Iterator Class.
   *
   * @tparam is_const     Compile time boolean indicating it the iterator should const or non-const.
   * @tparam M            The matrix type the iterator shouls work on.
   */
  template <bool is_const, typename M>
  class RowIterator
    {
    private:

// 2010-04-19 mrtn: VS 2005 view the template friend prototype as a function declaration, causing compile error c2668
#if _MSC_VER != 1400
      template<bool b, typename T> friend size_t row(RowIterator<b, T> const&);
      template<bool b, typename T> friend size_t col(RowIterator<b, T> const&);
#else
      friend size_t row<is_const,M>(RowIterator<is_const,M> const&);
      friend size_t col<is_const,M>(RowIterator<is_const,M> const&);
#endif
      
    private:
      
      typedef M matrix_type;
      
      typedef typename detail::if_then_else<is_const, matrix_type const*, matrix_type*>::result    matrix_type_ptr;
      typedef typename matrix_type::accessor::data_container_type                          data_container_type;
      typedef typename data_container_type::iterator                                       data_iterator;
      typedef typename data_container_type::const_iterator                                 const_data_iterator;
      typedef typename detail::if_then_else<is_const, const_data_iterator, data_iterator>::result  data_iterator_type;
      
    public:
      
      typedef typename std::iterator_traits<data_iterator_type>::iterator_category iterator_category;
      
      typedef typename data_iterator_type::value_type       value_type;
      typedef typename data_iterator_type::difference_type  difference_type;
      typedef typename data_iterator_type::pointer          pointer;
      typedef typename data_iterator_type::reference        reference;
      
    protected:
      
      matrix_type_ptr m_src;
      size_t          m_idx;
      size_t          m_row;
      
    public:
      
      RowIterator()
      : m_src(0)
      , m_idx(0u)
      , m_row(0u)
      {}
      
      RowIterator(matrix_type_ptr m, size_t const i, size_t const r)
      : m_src(m)
      , m_idx(i)
      , m_row(r)
      {}
      
      RowIterator(RowIterator const& orig)
      : m_src(orig.m_src)
      , m_idx(orig.m_idx)
      , m_row(orig.m_row)
      {}
      
      // 2009-07-01 Kenny: Why not ue this-> for accessing members/methods in a template class?
      RowIterator& operator=(RowIterator const & rhs)
      {
        m_src = rhs.m_src;
        m_idx = rhs.m_idx;
        m_row = rhs.m_row;
        return *this;
      }
      
      // 2009-07-01 Kenny: Why not ue this-> for accessing members/methods in a template class?
      reference operator*() const
      {
        typedef typename matrix_type::accessor A;
        return A::data(*m_src)[m_idx];
      }
      
      // 2009-07-01 Kenny: Why not ue this-> for accessing members/methods in a template class?
      pointer operator->() const { return &operator*(); }
      
      // 2009-07-01 Kenny: Why not ue this-> for accessing members/methods in a template class?
      RowIterator & operator++()
      {
        ++m_idx;
        return *this;
      }
      
      // 2009-07-01 Kenny: Why not ue this-> for accessing members/methods in a template class?
      RowIterator operator++(int)
      {
        RowIterator tmp = *this;
        ++m_idx;
        return tmp;
      }
      
      // 2009-07-01 Kenny: Why not ue this-> for accessing members/methods in a template class?
      RowIterator& operator--()
      {
        --m_idx;
        return *this;
      }
      
      // 2009-07-01 Kenny: Why not ue this-> for accessing members/methods in a template class?
      RowIterator operator--(int)
      {
        RowIterator tmp = *this;
        --m_idx;
        return tmp;
      }
      
      // 2009-07-01 Kenny: Why not ue this-> for accessing members/methods in a template class?
      RowIterator& operator+=(int n)
      {
        m_idx += n;
        return *this;
      }
      
      // 2009-07-01 Kenny: Why not ue this-> for accessing members/methods in a template class?
      RowIterator& operator-=(int n)
      {
        m_idx -= n;
        return *this;
      }
      
      // 2009-07-01 Kenny: Why not ue this-> for accessing members/methods in a template class?
      RowIterator operator+(int n)       { return RowIterator(m_idx + n); }    
      RowIterator operator-(int n) const { return RowIterator(m_idx - n); }
      
      // 2009-07-01 Kenny: Why not ue this-> for accessing members/methods in a template class?
      difference_type operator-(RowIterator const & x) const
      {
        // 2009-07-01 Kenny: Static_cast type safe?
        return   static_cast<difference_type>(m_idx) - static_cast<difference_type>(x.m_idx);
      }
      
      // 2009-07-01 Kenny: Why not ue this-> for accessing members/methods in a template class?
      bool operator != (RowIterator const & y) const { return !operator == (y); }
      
      // 2009-07-01 Kenny: Why not ue this-> for accessing members/methods in a template class?
      // 2009-07-01 Kenny: Why not test /assert if src's are the same?
      bool operator < (RowIterator const & y) const
      {
        assert(m_row == y.m_row  || !"rows did not match");
        return m_idx < y.m_idx;
      }
      
      // 2009-07-01 Kenny: Why not ue this-> for accessing members/methods in a template class?
      // 2009-07-01 Kenny: Why not test /assert if src's are the same?
      bool operator == (RowIterator const & y) const
      {
        return m_row == y.m_row && m_idx == y.m_idx;
      }
      
    };
  
  template <bool is_const, typename M>
  __attribute__((always_inline)) size_t row(RowIterator<is_const, M> const& iter) { return iter.m_row; }
  
  template <bool is_const, typename M>
  __attribute__((always_inline)) size_t col(RowIterator<is_const, M> const& iter) { return col(iter.m_idx, *iter.m_src); }
  
} // namespace sparse

// SPARSE_ROW_ITERATOR_H
#endif 
