#ifndef SPARSE_COMPRESSED_VECTOR_H
#define SPARSE_COMPRESSED_VECTOR_H

#include <sparse_compressed_vector_iterator.h> // for index iterator
#include <sparse_traits.h>         // for zero_block, volatile_zero_block

#include <cassert>
#include <vector>

namespace sparse
{
  
  namespace detail
  {
    /**
     * CompressedVector Accessor Class.
     * This class can be used to obtain direct access to the underlying data container type used in the vector class.
     *
     * No guarantees of any sort is given once this accessor is used by an end-user. 
     *
     * @tparam V     The vector type that the accessor should grant access to.
     */
    template <typename V>
    class CompressedVectorAccessor
      {
      public:
        
        typedef V vector_type;
        typedef typename V::data_container_type data_container_type;
        typedef typename V::row_container_type  row_container_type;
        
        static data_container_type       & data(vector_type       & src) { return src.m_data; }
        static data_container_type const & data(vector_type const & src) { return src.m_data; }
        
        //static row_container_type        & data(vector_type       & src) { return src.m_rows; }
        //static row_container_type  const & data(vector_type const & src) { return src.m_rows; }
      };
    
    /** @class BasicVectorCompressor
     * The VectorCompressor classes are used by the CompressedVector class and allows the user to 
     * specify how to handle tradeoffs between memory usage and performance of data access.
     *
     * For the BasicVectorCompressor, the memory usage is linear with the number of nonzero blocks
     * to the tune of 1 unsigned integer per nonzero blocks, but access to the block at a certain row
     * runs in O(lg n) time, where n is the number of nonzero blocks. It is recommended to use this 
     * compressor when the number of rows are very large compared to the number of nonzero blocks
     * and n is small (so lg n is small).
     */
    class BasicVectorCompressor
      {
        
      protected:
        
        typedef std::vector<size_t>   row_container_type;
        row_container_type            m_rows;              ///< The ith element contains the row number of the ith element of m_data.
        
      public:
        
        BasicVectorCompressor() 
        : m_rows(0u)
        {}
        
        BasicVectorCompressor(size_t const nnz, size_t const max_rows) 
        : m_rows(0u)
        {
          m_rows.reserve(nnz);
        }
        
        BasicVectorCompressor(BasicVectorCompressor const & orig) 
        : m_rows(orig.m_rows)
        {}
        
        ~BasicVectorCompressor(){}
        
        BasicVectorCompressor& operator=(BasicVectorCompressor const& orig) 
        { 
          this->m_rows = orig.m_rows;  
          return *this; 
        }
        
        inline size_t operator()(size_t const i) const
        {
          row_container_type::const_iterator iter = std::lower_bound<row_container_type::const_iterator, size_t>(m_rows.begin(), m_rows.end(), i);
          size_t idx = iter - m_rows.begin();
          return idx;
        }
        
        inline size_t get_row_idx(size_t const i) const
        {
          return this->m_rows[i];
        }
        
        inline void push_back(size_t const real_row, size_t const data_row)
        {
          this->m_rows.push_back(real_row);
        }
        
        inline void clear()
        {
          this->m_rows.clear(); 
        }
        
        inline void resize(size_t const nnz, size_t const max_rows)
        {
          this->m_rows.reserve(nnz);
        }
        
        inline void reserve(size_t const nnz, size_t const max_rows)
        {
          this->resize(nnz, max_rows);
        }
        
        inline bool row_exists(size_t row_idx) const
        {
          return (this->m_rows.size() > row_idx);
        }
        
      };
    
    /** @class FastVectorCompressor
     * The VectorCompressor classes are used by the CompressedVector class and allows the user to 
     * specify how to handle tradeoffs between memory usage and performance of data access.
     *
     * The FastVectorCompressor uses one unsigned int per row in the 
     * vector. However, it supports constant time access to a block at a certain row. This compressor
     * should be used if the vector is accessed often and/or if the number of nonzero blocks are smaller
     * than the number of rows and n is large.
     */
    class FastVectorCompressor
      {
      protected:
        
        typedef std::vector<size_t>   row_container_type;
        row_container_type            m_rows;     ///< the ith element contains the row number of the ith element of m_data
        row_container_type            m_row_ptrs; ///< contains the index of the ith row in m_data
        
      public:
        
        FastVectorCompressor() 
        : m_rows(0u)
        , m_row_ptrs(0u)
        {}
        
        FastVectorCompressor(size_t const nnz, size_t const max_rows) 
        : m_rows(0u)
        , m_row_ptrs(max_rows, max_rows)
        {
          m_rows.reserve(nnz);
        }
        
        FastVectorCompressor(FastVectorCompressor const & orig) 
        : m_rows(orig.m_rows), m_row_ptrs(orig.m_row_ptrs)
        {}
        
        ~FastVectorCompressor(){}
        
        FastVectorCompressor& operator=(FastVectorCompressor const& orig) 
        { 
          this->m_rows = orig.m_rows;  
          this->m_row_ptrs = orig.m_row_ptrs; 
          return *this; 
        }
        
        inline size_t operator()(size_t const i) const
        {
          return this->m_row_ptrs[i];
        }
        
        inline size_t get_row_idx(size_t const i) const
        {
          return this->m_rows[i];
        }
        
        inline void push_back(size_t const real_row, size_t const data_row)
        {
          this->m_row_ptrs[real_row] = data_row;
          this->m_rows.push_back(real_row);
        }
        
        inline void clear()
        {
          this->m_rows.clear(); 
          this->m_row_ptrs.clear();
        }
        
        inline void resize(size_t const nnz, size_t const max_rows)
        {
          this->m_rows.reserve(nnz);
          this->m_row_ptrs.resize(max_rows, max_rows); // TODO: Update old values
        }
        
        inline void reserve(size_t const nnz, size_t const max_rows)
        {
          this->resize(nnz, max_rows);
        }
        
        inline bool row_exists(size_t row_idx) const
        {
          return (row_idx < m_row_ptrs.size());
        }
        
      };
    
  }//end of namespace detail
  
  
  /** @class CompressedVector
   * Compressed Block Vector Class.
   *
   * The CompressedVector class can be used in case one needs sparse vectors, and has similarities
   * with the Compressed Row Matrix format. It is initialized using a constructor, noting the number
   * of nonzero elements and the number of rows. After the initialization, elements can be added to 
   * the vector. These elements must be added sequentially, so that the ith row element is added to
   * the vector before the jth row element if i < j. Adding elements out of order will cause the 
   * CompressedVector to malfunction.
   *
   * Elements can be accessed in three ways. The CompressedVectorRowIterator can be used to iterate 
   * over the rows in order, the () operator can be used to access the ith row, and finally the [] 
   * operator can be used to access the element at the ith position in the underlying data container.
   *
   * It should be noted that most Vector prod/sub/add functionality has not been implemented for this
   * structure yet.
   *
   * When a new CompressedVector is built, the second template parameter specifies how the vector 
   * handles the compression. The VectorCompressor classes allows the user to specify how to handle
   * tradeoffs between memory usage and performance of data access:
   *
   *  * detail::BasicVectorCompressor (default): The memory usage is linear with the number of nonzero blocks
   * to the tune of 1 unsigned integer per nonzero blocks, but access to the block at a certain row
   * runs in lg n time, where n is the number of nonzero blocks. It is recommended to use this 
   * compressor when the number of rows are very large compared to the number of nonzero blocks
   * and n is small (so lg n is small).
   *
   *  * detail::FastVectorCompressor: This compressor has a memory usage of one unsigned int per row in the 
   * vector. However, it supports constant time access to a block at a certain row. This compressor
   * should be used if the vector is accessed often and/or if the number of nonzero blocks are smaller
   * than the number of rows and n is large.
   *
   * @tparam block_type         The block type of the vector.
   * @tparam vector_compressor  This class specifies how the vector is compressed. Default value is detail::BasicVectorCompressor.
   */
  template <typename block_type, typename vector_compressor = detail::BasicVectorCompressor>
  class CompressedVector
  {
protected:
  
  typedef std::vector<block_type>                             data_container_type;
  typedef std::vector<size_t>                                 row_container_type;
  
public:
  
  typedef CompressedVector<block_type,vector_compressor>      vector_type;
  typedef Vector<block_type>                                  uncompressed_vector_type;
  typedef block_type&                                         reference;
  typedef block_type const&                                   const_reference;
  typedef block_type*                                         pointer;
  typedef block_type const*                                   const_pointer;
  typedef CompressedVectorRowIterator<false, vector_type>     iterator;
  typedef CompressedVectorRowIterator<true, vector_type>      const_iterator;
  
  typedef detail::VectorAccessor<vector_type>                 accessor;
  
private:
  
  friend class detail::VectorAccessor<vector_type>;
  
protected:
  
  data_container_type m_data;       ///< Contains data, that is, the blocks in the vector.
  size_t              m_max_rows;   ///< The max number of rows.
  vector_compressor   m_compressor; ///< Handles compression of data.
  
public:
  
  CompressedVector()
  : m_data(0u)
  , m_max_rows(0u)
  , m_compressor()
  {}
  
  CompressedVector(size_t const nnz, size_t const max_rows)
  : m_data(0u)
  , m_max_rows(max_rows)
  , m_compressor(nnz, max_rows)
  {
    m_data.reserve(nnz); 
  }
  
  CompressedVector(vector_type const & orig)
  : m_data(orig.m_data), m_max_rows(orig.m_max_rows), m_compressor(orig.m_compressor)
  {}
  
  CompressedVector(uncompressed_vector_type const & orig, size_t const start, size_t const end)
  {
    this->resize(end-start, orig.size());
    for( size_t i = start; i < end; ++i )
    {
      (*this)(i) = orig(i);
    }
  }
  
  ~CompressedVector(){}
  
  vector_type& operator=(vector_type const& orig) 
  { 
    this->m_data = orig.m_data; 
    this->m_compressor = orig.m_compressor;
    this->m_max_rows = orig.m_max_rows;
    return *this; 
  }
  
  /** 
  * The [] operator accesses the underlying data container directly. Use with caution! 
  * @param i The index of the element in the underlying data container to return.
  * @return Reference to the ith element of the underlying data container
  */
  reference operator[](size_t const i)
  {
    assert(i < size() || !"i was too large");
    return this->m_data[i];
  }
  
  /** 
  * The index operator accesses the underlying data container directly. This is the const version
  * @param i The index of the element in the underlying data container to return.
  * @return Reference to the ith element of the underlying data container
  */
  const_reference operator[](size_t const i) const
  {
    assert(i < size() || !"i was too large");
    return this->m_data[i];
  }
  
  /** 
  * The access operator () accesses the element at the ith row if any exist. 
  * If none exists, a new element is added and a ref is returned, if there is room in
  * the vector. 
  * @param i The row in the vector of the element to return
  * @return A reference to the element at the ith row in the vector
  */
  reference operator()(size_t const i)
  {
    assert( i < this->m_max_rows || !"row number must be less than the number of rows");
    size_t idx = this->m_compressor(i);
    if(this->m_compressor.row_exists(idx))
    {
      return this->m_data[idx];
    }
    else
    {
      this->m_compressor.push_back(i, this->m_data.size());
      this->m_data.push_back(zero_block<block_type>());
      return this->m_data.back();
    }
  }

  /** 
  * The const access operator () accesses the element at the ith row if any exist. 
  * If none exists, a const zero block is returned.
  * @param i The row in the vector of the element to return
  * @return A reference to the element at the ith row in the vector
  */
  const_reference operator()(size_t const i) const
  {
    assert( i < this->m_max_rows || !"row number must be less than the number of rows");
    size_t const idx = this->m_compressor(i);
    if(this->m_compressor.row_exists(idx))
    {
      return this->m_data[idx];
    }
    else
    {
      return zero_block<block_type>();
    }
  }
  
  iterator       begin()       { return iterator(0u, this);                 }
  const_iterator begin() const { return const_iterator(0u, this);           }
  iterator       end()         { return iterator(this->size(), this);       }
  const_iterator end()   const { return const_iterator(this->size(), this); }
  
  void resize(size_t const nnz, size_t const max_rows, bool preserve = true)
  {
    if (!preserve)
    {
      assert(false || !"preserve is not currently supported");
    }
    this->m_data.reserve(nnz);
    this->m_max_rows = max_rows;
    this->m_compressor.resize(nnz, max_rows);
  }
  
  void reserve(size_t const nnz, size_t max_rows) 
  { 
    this->m_data.reserve(nnz); 
    this->m_compressor.reserve(nnz, max_rows); 
  }
  
  size_t capacity() const { return this->m_data.capacity(); }
  
  void clear() 
  { 
    this->m_data.clear();
    this->m_max_rows = 0u;
    this->m_compressor.clear(); 
  }
  
  /**
  * Sets all blocks in the vector to zero while retaining its structure
  */
  void clear_data() 
  { 
    for( typename data_container_type::iterator iter = m_data.begin(); iter != m_data.end(); ++iter )
    {
      (*iter).clear_data();
    }
  }
  
  /**
  * @return The number of elements available in the vector
  */
  size_t size()  const { return m_data.size(); }
  
  /**
  * @return The number of rows in vector 
  */
  size_t nrows() const { return m_max_rows;  }
  size_t ncols() const { return 1u;            }
  
  /**
  * @return The column of the ith element of the data array 
  */
  const size_t row_idx(size_t i) const
  { 
    assert( i < this->nrows()  || !"index must be smaller than the number if rows" );
    return this->m_compressor.get_row_idx(i);
  }
};

} // namespace sparse

// SPARSE_COMPRESSED_VECTOR_H
#endif 
