#ifndef TINY_CONTAINER_H
#define TINY_CONTAINER_H

#include <tiny_aligned_16.h>
#include <tiny_accessor.h>
#include <tiny_value_traits.h>

#include <tiny_index_iterator.h>

#include <cstring> // Needed for memcpy
#include <cassert>

namespace tiny
{
  namespace detail
  {
    
    /**
     * Math Base Class.
     *
     *  The math base class is the foundation of TINY, and all types
     *  are derived from it.
     *
     *  The math base class provides, besides the actual container that
     *  contains all elements of the respective types, also an interface to
     *  all operations that are conceptually the same across all types.
     *
     *  @tparam I_    The number of rows. Must be a positive number.
     *  @tparam J_    The number of columns. Must be a positive number.
     */
    template<size_t I_, size_t J_, typename T>
    class Container 
      {
      public:
        
        enum { I    = I_    };
        enum { J    = J_    };
        enum { N    = I_*J_ };
        
      private:
                
        typedef Container< I , J, T>   self_type;
        
        friend class detail::Accessor< self_type >;
        
      public:
        
        typedef          detail::Accessor< self_type >    accessor;
        typedef          T                                type_traits;
        typedef typename T::real_type                     real_type;
        typedef typename T::op_type                       op_type;
        typedef          ValueTraits<real_type>           value_traits;
        
        typedef IndexIterator< false, self_type >   iterator;
        typedef IndexIterator<  true, self_type >   const_iterator;
        
        static size_t size()  { return I*J; }
        static size_t size1() { return I;   }
        static size_t size2() { return J;   }
        
      protected:
        
        enum { stride    = T::stride    };
        enum { padding   = (stride - (J % stride)) % stride };
        enum { J_padded  = J + padding };
        enum { allocsize = I * J_padded }; ///< Total number of elements including padding.
        
      private:
        
        typedef union 
          {
            real_type real[stride];
            op_type op;
          } Converter;
        
        op_type get_op_type ( size_t const & i , size_t const & j ) const 
        {
          return reinterpret_cast<Converter const *>( m_data + i*J_padded + j )->op;
        }
        
        op_type & get_op_type ( size_t const & i , size_t const & j ) 
        {
          return reinterpret_cast<Converter*>( m_data + i*J_padded + j )->op;
        }
        
      public:
        

        // 2009-07-14 Kenny: warning C4820: '4' bytes padding added after data member 'm_data'?
        ALIGNED_16 real_type m_data[ allocsize ]; 
        
      public:
        
        Container()
        {        
          for (size_t i = 0; i<I; ++i)
          {
            size_t const offset = i*J_padded;
            for (size_t j = J; j< J_padded ; ++j)
              this->m_data[offset + j] = value_traits::zero();
          }
        }
        
        explicit Container( real_type const & value )
        {
          for (size_t i = 0; i<I ; ++i)
          {
            size_t const offset = i * J_padded;
            
            for (size_t j = 0; j<J ; ++j)
              this->m_data[offset + j] = value;
            
            for (size_t j = J; j<J_padded ; ++j)
              this->m_data[offset + j] = value_traits::zero();
          }
        }
        
        Container( Container const & cpy )
        {        
          for (size_t i = 0; i<I ; ++i)
          {
            const size_t offset = i*J_padded;
            for (size_t j = 0; j<J ; ++j)
              this->m_data[offset + j] = cpy.m_data[offset+j];
            for (size_t j = J; j<J_padded ; ++j)
              this->m_data[offset + j] = value_traits::zero();
          }
        }
        
        ~Container () {}
        
        Container &  operator= (Container const & rhs)
        {
          if (this == &rhs)
            return *this;
          
          for (size_t i = 0; i<I ; ++i)
            for (size_t j = 0 ; j<J ; ++j)
              (*this)(i,j) = rhs(i,j);
          //memcpy((void*)(this),(void*)(&rhs),I*J_padded*sizeof(real_type));
          return *this;
        }
        
        void clear()
        {
          for (size_t i = 0; i< allocsize;++i)
          {
            this->m_data[i] = value_traits::zero();
          }
        }
        
        real_type const & operator()(size_t const & i, size_t const & j) const
        {
          size_t const offset = i*J_padded;
          return this->m_data[offset+j];
        }
        
        real_type & operator()(size_t const & i, size_t const & j)
        {
          size_t const offset = i*J_padded;
          return this->m_data[offset+j];
        }
        
      public:
        
        /**
         * Component index operator
         * Indexes Container as a container and return elements as an op_type.
         * If the op_type is a real-type only one element is indexed.
         * However, if the op_type is a SIMD-type, the number of
         * elements returned is dependent on the specific SIMD-type.
         * \param i the component index number.
         * \returns a reference to the op_type
         * \warning When indexing SIMD-types, ensure to index with correct
         * memory alignment, e.g. SSE must be indexed with 16 bytes alignment.
         */
        real_type & operator() ( size_t const & i )
        {
          assert( (i>=0 && i<J) || !"i was out of range");        
          return this->m_data[i];
        }
        
        real_type const & operator() ( size_t const & i ) const
        {
          assert((i>=0 && i<J) || !"i was out of range");
          return this->m_data[i];
        }
        
        real_type       & operator[] ( size_t const & i )        { return this->operator()(i); }
        real_type const & operator[] ( size_t const & i ) const  { return this->operator()(i); }
        
        iterator       begin()       { return iterator(  0, 0, this);       }
        iterator       end()         { return iterator(I-1, J, this);       }
        const_iterator begin() const { return const_iterator(  0, 0, this); }
        const_iterator end()   const { return const_iterator(I-1, J, this); }
      }; 
    
  } // namespace detail
} // namespace tiny

// TINY_CONTAINER_H
#endif 
