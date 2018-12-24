#ifndef TINY_INDEX_ITERATOR_H
#define TINY_INDEX_ITERATOR_H

#include <tiny_if_then_else.h>
#include <iterator>             // Needed for STL iterator_traits
#include <cassert>

namespace tiny
{
  namespace detail
  {
    
    /**
     * Generic Iterator Class.
     *
     * @tparam is_const    Boolean value indicating whether it is a const or non-const iterator type.
     * @tparam M           The matrix type that the index iterator ``indices'' into.
     */
    template <bool is_const, typename M>
    class IndexIterator
    : public std::iterator< std::forward_iterator_tag, typename if_then_else< is_const, typename M::real_type const, typename M::real_type >::result >
    {      
    protected:
      
      typedef typename std::iterator< std::forward_iterator_tag, typename if_then_else< is_const, typename M::real_type const, typename M::real_type >::result >  base_class_type;
      
    public:
      
      typedef typename std::iterator_traits<base_class_type>::iterator_category iterator_category;
      
      typedef typename base_class_type::value_type      value_type;
      typedef typename base_class_type::difference_type difference_type;
      typedef typename base_class_type::pointer         pointer;
      typedef typename base_class_type::reference       reference;
      
    protected:
      
      typedef typename if_then_else<is_const, M const*, M*>::result matrix_type_ptr;
      
      typedef typename if_then_else<is_const, typename M::real_type const, typename M::real_type >::result real_type;
      
    protected:
      
      matrix_type_ptr    m_src;   ///< A pointer to the matrix
      size_t             m_i;     ///< The current i index into the matrix
      size_t             m_j;     ///< The current j index into the matrix
      
    public:
      
      IndexIterator(size_t const i = 0u, size_t const j = 0u, matrix_type_ptr m = 0)
      : m_src(m)
      , m_i(i)
      , m_j(j)
      {}
      
      IndexIterator(IndexIterator const& orig)
      : m_src(orig.m_src)
      , m_i(orig.m_i)
      , m_j(orig.m_j)
      {}
      
      IndexIterator& operator=(IndexIterator const& rhs)
      {
        m_src = rhs.m_src;
        m_i = rhs.m_i;
        m_j = rhs.m_j;
        return *this;
      }
      
      reference operator*() const { return (*m_src)(m_i,m_j); }
      
      pointer operator->() const { return &operator*(); }
      
      IndexIterator& operator++()
      {
        if ( (m_j + 1u) < M::J) 
          ++m_j; 
        else if ( (m_i + 1u) < M::I  )
        {
          m_j = 0u;
          ++m_i; 
        }       
        else
        {
          // i == I-1 no more elements to iterate set to one position last element!
          m_j = M::J;
        }
        return *this;
      }
      
      IndexIterator operator++(int)
      {
        IndexIterator tmp = *this;
        this->operator++();
        return tmp;
      }
      
      bool operator == (IndexIterator const& rhs) const 
      {
        size_t const offset = m_i*M::accessor::J_padded();
        return (m_src + offset + m_j) == (rhs.m_src + offset + rhs.m_j); // Direct pointer comparision to avoid assertion
      }
      
      bool operator != (IndexIterator const& rhs) const { return ! ((*this)==( rhs)); }
      
    };
    
  } // namespace detail
} // namespace tiny

// TINY_INDEX_ITERATOR_H
#endif 
