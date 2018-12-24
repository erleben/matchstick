#ifndef SPARSE_INDEX_ITERATOR_H
#define SPARSE_INDEX_ITERATOR_H

#include <sparse_if_then_else.h>
#include <sparse_property_maps.h>  // for col and row functions

#include <iterator>             // for STL iterator_traits

namespace sparse
{
  
  // 2009-07-01 Kenny: Documentation Needed, what is an index iterator? Related to [] access on matrices?  
  /**
   * Generic Index Iterator Class.
   *
   * @tparam is_const    Boolean value indicating whether it is a const or non-const iterator type.
   * @tparam M           The matrix type that the index iterator ``indices'' into.
   */
  template <bool is_const, typename M>
  class IndexIterator
    {
    private:
      
// 2010-04-19 mrtn: VS 2005 view the template friend prototype as a function declaration, causing compile error c2668
#if _MSC_VER != 1400
      template<bool b, typename T> friend size_t row(IndexIterator<b, T> const & );
      template<bool b, typename T> friend size_t col(IndexIterator<b, T> const & );
      template<bool b, typename T> friend size_t idx(IndexIterator<b, T> const & );
#else
      friend size_t row<is_const,M>(IndexIterator<is_const,M> const&);
      friend size_t col<is_const,M>(IndexIterator<is_const,M> const&);
      // 2010-04-19 mrtn: The idx friend function prototype gives a compile error
      //friend size_t idx<is_const,M>(IndexIterator<is_const,M> const&);
#endif
      
    protected:
      
      typedef M container_type;
      typedef typename detail::if_then_else<is_const, container_type const*, container_type*>::result container_type_ptr;
      
      typedef typename M::accessor::data_container_type     data_container_type;
      typedef typename data_container_type::iterator        data_iterator;
      typedef typename data_container_type::const_iterator  const_data_iterator;
      
      typedef typename detail::if_then_else<is_const, const_data_iterator, data_iterator>::result data_iterator_type;
      
    public:
      
      // 2009-07-01 Kenny: STL dependency may be problematic?
      typedef typename std::iterator_traits<data_iterator_type>::iterator_category iterator_category;
      
      typedef typename data_iterator_type::value_type      value_type;
      typedef typename data_iterator_type::difference_type difference_type;
      typedef typename data_iterator_type::pointer         pointer;
      typedef typename data_iterator_type::reference       reference;
      
    protected:
      
      container_type_ptr m_src;
      size_t             m_idx;
      
    public:
      
      // 2009-07-01 Kenny: What if i is not in ``range'' of m?
      IndexIterator(size_t const i = 0u, container_type_ptr m = 0)
      : m_src(m)
      , m_idx(i)
      {}
      
      IndexIterator(IndexIterator const& orig)
      : m_src(orig.m_src)
      , m_idx(orig.m_idx)
      {}
      
      // 2009-07-01 Kenny: Why not use this-> for member/method access in template class?
      IndexIterator& operator=(IndexIterator const& rhs)
      {
        m_src = rhs.m_src;
        m_idx = rhs.m_idx;
        return *this;
      }
      
      // 2009-07-01 Kenny: Why not use this-> for member/method access in template class?
      reference operator*() const { return (*m_src)[m_idx]; }
      
      // 2009-07-01 Kenny: Why not use this-> for member/method access in template class?
      pointer operator->() const { return &operator*(); }
      
      // 2009-07-01 Kenny: Why not use this-> for member/method access in template class?
      IndexIterator& operator++()
      {
        ++m_idx;
        return *this;
      }
      
      // 2009-07-01 Kenny: Why not use this-> for member/method access in template class?
      IndexIterator operator++(int)
      {
        IndexIterator tmp = *this;
        ++m_idx;
        return tmp;
      }
      
      // 2009-07-01 Kenny: Why not use this-> for member/method access in template class?
      IndexIterator& operator--()
      {
        --m_idx;
        return *this;
      }
      
      // 2009-07-01 Kenny: Why not use this-> for member/method access in template class?
      IndexIterator operator--(int)
      {
        IndexIterator tmp = *this;
        --m_idx;
        return tmp;
      }
      
      // 2009-07-01 Kenny: Why not use this-> for member/method access in template class?
      IndexIterator& operator+=(int n)
      {
        m_idx += n;
        return *this;
      }
      
      // 2009-07-01 Kenny: Why not use this-> for member/method access in template class?
      IndexIterator& operator-=(int n)
      {
        m_idx -= n;
        return *this;
      }
      
      // 2009-07-01 Kenny: Why not use this-> for member/method access in template class?
      IndexIterator operator+(int n) const { return IndexIterator(m_idx+n, m_src);  }
      IndexIterator operator-(int n) const { return IndexIterator(m_idx-n, m_src);  }
      
      // 2009-07-01 Kenny: Why not use this-> for member/method access in template class?
      // 2009-07-01 Kenny: Is static_cast a good thing to use?
      difference_type operator-(IndexIterator const& x) const 
      {
        return static_cast<difference_type>(m_idx) - static_cast<difference_type>(x.m_idx);
      }
      
      // 2009-07-01 Kenny: Why is it not tested/asserted that m_src are the same/different?
      bool operator == (IndexIterator const& y) const { return m_idx == y.m_idx; }
      // 2009-07-01 Kenny: Why is it not tested/asserted that m_src are the same/different?
      // 2009-07-01 Kenny: Why not implement this using !operator==()?
      bool operator != (IndexIterator const& y) const { return m_idx != y.m_idx; }
      bool operator <  (IndexIterator const& y) const { return m_idx < y.m_idx;  }
      
    };
  
  // 2009-07-01 Kenny: Documentation Needed
  template <bool is_const, typename M>
  inline size_t idx(IndexIterator<is_const, M> const & iter) { return iter.m_idx;  }
  
  // 2009-07-01 Kenny: Documentation Needed
  template <bool is_const, typename M>
  inline size_t row(IndexIterator<is_const, M> const & iter) { return row(iter.m_idx, *iter.m_src); }
  
  // 2009-07-01 Kenny: Documentation Needed
  template <bool is_const, typename M>
  inline size_t col(IndexIterator<is_const, M> const & iter) { return col(iter.m_idx, *iter.m_src); }
  
} // namespace sparse

// SPARSE_INDEX_ITERATOR_H
#endif 
