#ifndef SPARSE_AMBI_VECTOR_H
#define SPARSE_AMBI_VECTOR_H

namespace sparse 
{
  namespace detail
  {
    
    // 2009-07-01 Kenny: This class is only used for matrix products and the field ``key'' seems to always be the same as column index, so why not call it col instead of key for clarity? 
    /**
     * Auxiliary Class for Sparse Matrix Products.
     * An adaption of Eigen's AmbiVector, specialised for blocks it is used for the prod() of two CompressedRowMatrix's
     *
     * Allows fast read and write of key/value pairs if pairs are inserted in key-ordered sequences, e.g: 4 values with keys: <1,3,6,9> followed by 3 values with keys: <2,4,5>
     *
     * keys must lie in {0,1, ..., size} where size is given in the constructor if <= 10% of size is used the container is sparse otherwise it is dense
     *
     * If sparse mode then an array implementation of an ordered linked list is used if dense mode then a simple array is used.
     */
    template<typename B>
    class Ambi_vector
      {
      public:
        
        typedef B block_type;
        
        class Iterator;  // forward declaration
       
      protected:
        
        // 2009-07-01 Kenny: Why creating your own data structure for this? You are already using STL vector container so would this not make the code more ``pretty''?
        
        struct ListElement
        {
          int        m_next;   // The index in array to next element in list. If next == -1 then it means null
          int        m_key;    // If dense mode  then key == -1 means null value (as checking block_type is often expensive)
          block_type m_value;
        };
        
        ListElement * m_data;     ///< Data
        // 2009-07-01 Kenny: Why not size_t?
        int  m_size;            ///< Number of elements in data
        int  m_allocated_size;  ///< Number of elements allocated
        bool m_is_sparse;      ///< Mode
        
        // Used only when in linked list mode
        // 2009-07-01 Kenny: Why not size_t?
        int m_lcurrent;        ///< Current listindex if m_lcurrent_ == -1 then it means null
        int m_lsize;           ///< Number of elements in the list
        
      private:
      
        // 2009-07-01 Kenny: Why not copy-constructable?
        Ambi_vector(Ambi_vector const&);

      public:
        
        // 2009-07-01 Kenny: Why no use this-> for member/method access in a template class?
        Ambi_vector(int size)
        : m_data(0)
        , m_size(0)
        , m_allocated_size(0)
        , m_is_sparse(true)
        {
          resize(size);
        }
        
        // 2009-07-01 Kenny: Why no use this-> for member/method access in a template class?
        ~Ambi_vector()
        {
          // 2009-07-01 Kenny: delete unsafe?
          delete[] m_data;
        }
        
        // 2009-07-01 Kenny: Why no use this-> for member/method access in a template class?
        block_type & operator()(int i)
        {
          assert(i >= 0 && i < m_size && "index into Ambi_vector out of bounds");
          
          if (!m_is_sparse)
          {
            if (m_data[i].m_key == -1)
            {
              // new element
              m_data[i].m_value = zero_block<block_type>();
              m_data[i].m_key = i;
            }
            return m_data[i].m_value;
          }
          else
          {
            if (m_lsize == 0)
            {
              // first element
              m_lcurrent = 0;
              ++m_lsize;
              m_data[0].m_value = sparse::zero_block<block_type>();
              m_data[0].m_key   = i;
              m_data[0].m_next  = -1;
              return m_data[0].m_value;
            }
            else
            {
              int next_elem = m_data[m_lcurrent].m_next;
              while (next_elem >= 0 && m_data[next_elem].m_key <= i)
              {
                m_lcurrent = next_elem;
                next_elem = m_data[next_elem].m_next;
              }
              
              if (m_data[m_lcurrent].m_key == i)
              {
                // return existing element
                return m_data[m_lcurrent].m_value;
              }
              else
              {
                // insert new element
                if (m_lsize >= m_allocated_size )
                {
                  reallocate();
                }
                ListElement & e = m_data[m_lsize];
                e.m_value = sparse::zero_block<block_type>();
                e.m_key = i;
                if(m_data[m_lcurrent].m_key < i)
                {
                  e.m_next = next_elem;
                  m_data[m_lcurrent].m_next = m_lsize;
                }
                else
                {
                  e.m_next = m_lcurrent;
                }
                ++m_lsize;
                return e.m_value;
              }
            }
          }
        }
        
        // 2009-07-01 Kenny: Why no use this-> for member/method access in a template class?
        void init(float const density)
        {
          m_is_sparse = density > 0.1 ? false : true;
          if (m_is_sparse)
          {
            m_lcurrent = -1;
            m_lsize = 0;
          }
        }
        
        // 2009-07-01 Kenny: Why no use this-> for member/method access in a template class?
        // 2009-07-01 Kenny: This is a really bad name I think... it did not make sense before I reviewed all the code to understand what the class does.
        void zero_out()
        {
          if (!m_is_sparse)
          {
            for (int i = 0; i < m_size; ++i)
            {
              m_data[i].m_key = -1;
            }
          }
          else
          {
            m_lsize = 0;
          }
        }
        
        // 2009-07-01 Kenny: Why no use this-> for member/method access in a template class?
        /**
         * if mode is sparse then this method must be called before inserting elements with key > m_lcurrent.
         */
        void restart() { m_lcurrent = ( m_lsize == 0 ) ? -1 : 0; }
        
        // 2009-07-01 Kenny: Why no use this-> for member/method access in a template class?
        void resize(int size)
        {
          if (m_allocated_size < size)
          {
            // 2009-07-01 Kenny: new/delete unsafe?
            delete[] m_data;
            m_data = size > 0 ? new ListElement[size] : 0;
            m_allocated_size = size;
          }
          m_size = size;
        }
        
        // 2009-07-01 Kenny: Why no use this-> for member/method access in a template class?
        int size() const { return m_size; }
        
      protected:
        
        // 2009-07-01 Kenny: Why no use this-> for member/method access in a template class?
        void reallocate()
        {
          m_allocated_size = (m_allocated_size == 0) ? 100 : m_allocated_size << 1; // doubling strategy
          // 2009-07-01 Kenny: new/delete unsafe?
          ListElement * new_buffer = new ListElement[m_allocated_size];
          // 2009-07-01 Kenny: Where do  you get memcpy from? Missing include header?
          memcpy(new_buffer,  m_data,  m_size * sizeof(ListElement));
          delete[] m_data;
          m_data = new_buffer;
        }
        
      };
    
    
    template<typename B>
    class Ambi_vector<B>::Iterator
    {
    protected:

      // 2009-07-01 Kenny: Why use space on m_is_sparse? Why not just access the same value from the vector reference when needed?
      
      Ambi_vector & m_vec_ref;       ///< The target vector
      int           m_current;       ///< The current index into m_vec_ref
      bool          m_is_sparse;     ///< The mode of the vector
      
    public:
      
      typedef B block_type;
      
      // 2009-07-01 Kenny: Why no use this-> for member/method access in a template class?
      Iterator(Ambi_vector& vec)
      : m_vec_ref(vec)
      {
        m_is_sparse = m_vec_ref.m_is_sparse;
        if (!m_is_sparse)
        {
          m_current = -1;
          ++(*this);
        }
        else
        {
          m_current = m_vec_ref.m_lsize == 0 ? -1 : 0;
        }
      }
      
      // 2009-07-01 Kenny: Why no use this-> for member/method access in a template class?
      Iterator& operator++()
      {
        if (!m_is_sparse)
        {
          // linear search for next non-null entry
          do
          {
            ++m_current;
          }
          while ( m_current < m_vec_ref.m_size && m_vec_ref.m_data[m_current].m_key < 0);
          
          if (m_current >= m_vec_ref.m_size)
          {
            m_current = -1;
          }
        }
        else
        {
          assert( m_current >= 0 && m_current < m_vec_ref.size() && "Iterator index into Ambi_vector out of bounds");
          m_current = m_vec_ref.m_data[m_current].m_next;
        }
        return *this;
      }
      
      // 2009-07-01 Kenny: Why no use this-> for member/method access in a template class?
      int key() const
      {
        assert( m_current >= 0 && m_current < m_vec_ref.size() && "Iterator index into Ambi_vector out of bounds");
        return m_vec_ref.m_data[m_current].m_key;
      }
      
      // 2009-07-01 Kenny: Why no use this-> for member/method access in a template class?
      block_type & value()
      {
        assert( m_current >= 0 && m_current < m_vec_ref.size() && "Iterator index into Ambi_vector out of bounds");
        return m_vec_ref.m_data[m_current].m_value;
      }
      
      operator bool() const { return m_current >= 0; }
      
    };
    
  } // namespace detail
} // namespace sparse

// SPARSE_AMBI_VECTOR_H
#endif 
