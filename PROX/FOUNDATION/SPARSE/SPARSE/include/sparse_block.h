#ifndef SPARSE_BLOCK_H
#define SPARSE_BLOCK_H

#include <sparse_block_algebra.h>

#include <algorithm> // for std::fill, std::copy, std::equal

#include <cassert>

namespace sparse
{
    
  namespace detail
  {
    
    template <typename B>
    class BlockAccessor
      {
      public:
        typedef B block_type;
        typedef typename B::data_container_type data_container_type;
        
        static data_container_type& data(block_type& src)
        {
          return src.m_data;
        }
        
        static data_container_type const& data(block_type const& src)
        {
          return src.m_data;
        }
        
        static void copy(block_type const& orig, block_type& src)
        {
          src.copy_(orig);
        }
      };
    
  } // namespace detail
  
  
  /**
   * Generic Dense Block type.
   * To integrate with CUDA, it is necessary that the only members are
   * C-convertible data so that an array of blocks may be copied directly
   * to device memory as an array of C-convertible data.
   *
   * @tparam M   The number of rows in the block.
   * @tparam N   The number of columns in the block.
   * @tparam T   The value type of the entries in the block. Usually float or double.
   */
	template <size_t M, size_t N, typename T>
  class Block
    {
    public:
      
      typedef Block<M,N,T> block_type;
      typedef T            value_type;
      typedef T&           reference;
      typedef T const&     const_reference;
      typedef T*           pointer;
      typedef T const*     const_pointer;
      typedef T*           iterator;
      typedef T const*     const_iterator;
      
      typedef detail::BlockAccessor<block_type> accessor;
      
    protected:
      
      typedef T * data_container_type;
      
    private:
      
      friend class detail::BlockAccessor<block_type>;
      
    protected:
      
      value_type m_data[M*N];
      
    protected:
      
      void fast_copy(block_type const& orig)
      {
        // 2009-06-30 Kenny: Why not make sure that ADL works?
        // 2009-06-30 Kenny: Why do not this->size() and this->m_data?
        std::copy(orig.m_data, orig.m_data + size(), m_data);
      }
      
    public:
      
      Block() {}
      
      Block(const_reference v) { *this = v; }
      
      Block(block_type const& orig) { fast_copy(orig); }
      
      ~Block() { }
      
      block_type& operator=(block_type const& rhs)
      {
        if (&rhs != this)
        {
          fast_copy(rhs);
        }
        return *this;
      }
      
      block_type& operator=(const_reference rhs)
      {
        // 2009-06-30 Kenny: Why not make sure that ADL works?
        // 2009-06-30 Kenny: Why do not this->size() and this->m_data?
        std::fill(m_data, m_data + size(), rhs);
        return *this;
      }
      
      bool operator==(block_type const& rhs) const
      {
        // 2009-06-30 Kenny: Why not make sure that ADL works?
        // 2009-06-30 Kenny: Why do not this->begin() and this->end()?
        return std::equal(begin(), end(), rhs.begin());
      }
      
      // 2009-06-30 Kenny: Why do not this->operator==(...)?
      bool operator!=(block_type const& rhs) const { return !operator==(rhs); }
      
      // 2009-06-30 Kenny: Documentation needed. Not obvious when to use square brackets (raw linear access to data)  and when to use rounded parentheses (matrix indices). 
      value_type& operator[](size_t const i)
      {
        // 2009-06-30 Kenny: Why do not this->size()?
        assert(i < size() || !"i was too large");
        // 2009-06-30 Kenny: Why do not this->m_data?
        return m_data[i];
      }
      
      // 2009-06-30 Kenny: Documentation needed. Not obvious when to use square brackets (raw linear access to data)  and when to use rounded parentheses (matrix indices). 
      value_type const& operator[](size_t const i) const
      {
        // 2009-06-30 Kenny: Why do not this->size()?
        assert(i < size() || !"i was too large");
        // 2009-06-30 Kenny: Why do not this->m_data?
        return m_data[i];
      }
      
      // 2009-06-30 Kenny: Documentation needed. Not obvious when to use square brackets (raw linear access to data)  and when to use rounded parentheses (matrix indices). 
      value_type& operator()(size_t const i, size_t const j)
      {
        // 2009-06-30 Kenny: Why do not this->nrows() and this->ncols()?
        assert(((i < nrows()) && (j < ncols())) || !"i or j were too large");
        // 2009-06-30 Kenny: Why do not this->m_data and this->ncols()?
        return m_data[i*ncols()+j];
      }
      
      // 2009-06-30 Kenny: Documentation needed. Not obvious when to use square brackets (raw linear access to data)  and when to use rounded parentheses (matrix indices). 
      value_type const& operator()(size_t const i, size_t const j) const
      {
        // 2009-06-30 Kenny: Why do not this->nrows() and this->ncols()?
        assert(((i < nrows()) && (j < ncols())) || !"i or j were too large");
        // 2009-06-30 Kenny: Why do not this->m_data and this->ncols()?
        return m_data[i*ncols()+j];
      }

      void clear_data()
      {
        size_t const size = M*N;
//#pragma unroll
        for( size_t i = 0u; i < size; ++i )
        {
          m_data[i] = 0;  // 2010-04-03 Kenny: Use proper value traits
        }
      }
      
      // 2009-06-30 Kenny: Why not use this->m_data and this->size()?
      iterator       begin()       { return m_data;        }
      iterator       end()         { return m_data+size(); }      
      const_iterator begin() const { return m_data;        }
      const_iterator end()   const { return m_data+size(); }
      
      // 2009-06-30 Kenny: Maybe this should be emum typedefs to avoid runtime overhead?
      static size_t nrows() { return M; }
      
      // 2009-06-30 Kenny: Maybe this should be emum typedefs to avoid runtime overhead?
      static size_t ncols() { return N; }
      
      // 2009-06-30 Kenny: Maybe this should be emum typedefs to avoid runtime overhead?
      // 2009-06-30 Kenny: Is M*N done at compile time or run-time?
      static size_t size() { return M*N; }

    public:

      static Block<M,M,T> identity()
      {
        Block<M,M,T> B;
        B.clear_data();

        assert(M==N || "Not a square matrix");

        for(size_t i = 0; i < M; ++i)
        {
          B(i,i) = 1;
        }
        return B;
      }


      static Block<M,M,T> make_diag(T const & d)
      {
        Block<M,M,T> B;
        B.clear_data();

        assert(M==N || "Not a square matrix");

        for(size_t i = 0; i < M; ++i)
        {
          B(i,i) = d;
        }
        return B;
      }

    };
  
  /**
   * Specialization for a Column Block Type.
   *
   * @tparam M    The number of rows in the column block.
   * @tparam T    The value type of the entries in the block, usually float or double.
   */
	template <size_t M, typename T>
  class Block<M,1,T>
  {
  public:
    
    typedef Block<M,1,T> block_type;
    typedef T            value_type;
    typedef T&           reference;
    typedef T const&     const_reference;
    typedef T*           pointer;
    typedef T const*     const_pointer;
    typedef T*           iterator;
    typedef T const*     const_iterator;
    
  protected:
    
    typedef T*           data_container_type;
    
  private:
    
    friend class detail::BlockAccessor<block_type>;
    
  protected:
    
    value_type m_data[M];
    
  protected:
    
    void fast_copy(block_type const& orig)
    {
      // 2009-06-30 Kenny: Why not make sure that ADL works?
      // 2009-06-30 Kenny: Why do not this->size() and this->m_data?
      std::copy(orig.m_data, orig.m_data + size(), m_data);
    }
    
  public:
    
    Block(){}
    
    Block(value_type const& v) { *this = v; }
    
    Block(block_type const& orig) { fast_copy(orig); }
    
    ~Block() { }
    
    block_type& operator=(block_type const& rhs)
    {
      if (&rhs != this)
      {
        fast_copy(rhs);
      }
      return *this;
    }
    
    block_type& operator=(value_type const& rhs)
    {
      // 2009-06-30 Kenny: Why not make sure that ADL works?
      // 2009-06-30 Kenny: Why do not this->size() and this->m_data?
      std::fill(m_data, m_data + size(), rhs);
      return *this;
    }
    
    // 2009-06-30 Kenny: Why not make sure that ADL works?
    // 2009-06-30 Kenny: Why do not this->begin() and this->end()?
    bool operator==(block_type const& rhs) const {  return std::equal(begin(), end(), rhs.begin()); }
    bool operator!=(block_type const& rhs) const { return !operator==(rhs);                         }
    
    // 2009-06-30 Kenny: Documentation needed. Not obvious when to use square brackets (raw linear access to data)  and when to use rounded parentheses (matrix indices). 
    value_type& operator[](size_t const i)
    {
      // 2009-06-30 Kenny: Why do not this->size()?
      assert(i < size() || !"i was too large");
      // 2009-06-30 Kenny: Why do not this->m_data?
      return m_data[i];
    }
    
    // 2009-06-30 Kenny: Documentation needed. Not obvious when to use square brackets (raw linear access to data)  and when to use rounded parentheses (matrix indices). 
    value_type const& operator[](size_t const i) const
    {
      // 2009-06-30 Kenny: Why do not this->size()?
      assert(i < size() || !"i was too large");
      // 2009-06-30 Kenny: Why do not this->m_data?
      return m_data[i];
    }
    
    // 2009-06-30 Kenny: Documentation needed. Not obvious when to use square brackets (raw linear access to data)  and when to use rounded parentheses (matrix indices). 
    // 2009-06-30 Kenny: Why do not this->operator[]()?
    value_type&       operator()(size_t const i)       { return operator[](i); }
    value_type const& operator()(size_t const i) const { return operator[](i); }

    /*clear data in block*/
    void clear_data()
    {
//#pragma unroll
      for( size_t i = 0; i < M; ++i )
      {
        m_data[i] = 0;  // 2010-04-03 Kenny: use proper value traits
      }
    }
    
    // 2009-06-30 Kenny: Why do not this->m_data and this->size()?
    iterator       begin()       { return m_data; }
    const_iterator begin() const { return m_data; }
    iterator       end()         { return m_data+size(); }
    const_iterator end()   const { return m_data+size(); }
    
    // 2009-06-30 Kenny: Maybe this should be emum typedefs to avoid runtime overhead?
    static size_t nrows() { return M; }
    static size_t ncols() { return 1; }
    static size_t size()  { return M; }

  public:

    static Block<3,1,T> i()
    {
      Block<3,1,T> B;
      B.clear_data();

      B[0] = 1.0;

      return B;
    }

    static Block<3,1,T> j()
    {
      Block<3,1,T> B;
      B.clear_data();

      B[1] = 1.0;

      return B;
    }

    static Block<3,1,T> k()
    {
      Block<3,1,T> B;
      B.clear_data();

      B[2] = 1.0;

      return B;
    }

    static Block<3,1,T> make(T const & i, T const & j, T const & k)
    {
      Block<3,1,T> B;

      B[0] = i;
      B[1] = j;
      B[2] = k;

      return B;
    }

  };
  
  /**
   * Specialization for a Scalar Block Type.
   */
	template <typename T>
  class Block<1,1,T>
  {
  public:
    
    typedef Block<1,1,T> block_type;
    typedef T            value_type;
    typedef T&           reference;
    typedef T const&     const_reference;
    typedef T*           pointer;
    typedef T const*     const_pointer;
    typedef T*           iterator;
    typedef T const*     const_iterator;
    
  protected:
    
    typedef T data_container_type;
    
  private:
    
    friend class detail::BlockAccessor<block_type>;
    
  protected:
    
    value_type m_data;
    
  public:
    
    Block(){ }
    
    Block(value_type const& v)
    : m_data(v)
    { }
    
    Block(block_type const& orig)
    : m_data(orig.m_data)
    { }
    
    ~Block(){}
    
    // 2009-06-30 Kenny: Why do not this->m_data?
    operator value_type() { return m_data; }
    
    block_type& operator=(value_type const& rhs)
    {
      // 2009-06-30 Kenny: Why do not this->m_data?
      m_data = rhs;
      return *this;
    }
    
    // 2009-06-30 Kenny: Why do not this->m_data?
    bool operator==(block_type const& rhs) const { return m_data == rhs.m_data; }
    bool operator==(value_type const& rhs) const { return m_data == rhs;        }
    // 2009-06-30 Kenny: Why do not this->operator==()?    
    bool operator!=(value_type const& rhs) const { return !operator==(rhs);     }
    
    value_type& operator[](size_t const i)
    {
      assert((i == 0) || !"i was nonzero" );
      // 2009-06-30 Kenny: Why do not this->m_data?
      return m_data;
    }
    
    value_type const& operator[](size_t const i) const
    {
      assert((i == 0)  || !"i was nonzero");
      // 2009-06-30 Kenny: Why do not this->m_data?
      return m_data;
    }
    
    value_type& operator()(size_t const i, size_t const j)
    {
      assert(((i == 0) && (j == 0)) || !"i or j were nonzero" );
      // 2009-06-30 Kenny: Why do not this->m_data?
      return m_data;
    }
    
    value_type const& operator()(size_t const i, size_t const j) const
    {
      assert(((i == 0) && (j == 0)) || ! "i or j were nonzero");
      // 2009-06-30 Kenny: Why do not this->m_data?
      return m_data;
    }

    /*clear data in block*/
    void clear_data()
    {
      m_data = 0;
    }
    
    // 2009-06-30 Kenny: Why do not this->m_data?
    iterator       begin()       { return &m_data;        }
    const_iterator begin() const { return &m_data;        }
    // 2009-06-30 Kenny: Why do not this->m_data and this->size()?
    iterator       end()         { return &m_data+size(); }
    const_iterator end()   const { return &m_data+size(); }
    
    // 2009-06-30 Kenny: Maybe this should be emum typedefs to avoid runtime overhead?
    static size_t nrows() { return 1; }
    static size_t ncols() { return 1; }
    static size_t size()  { return 1; }
  };

} // namespace sparse

// SPARSE_BLOCK_H
#endif 
