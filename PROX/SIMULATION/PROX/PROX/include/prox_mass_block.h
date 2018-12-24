#ifndef PROX_MASS_BLOCK_H
#define PROX_MASS_BLOCK_H

#include <sparse_block.h>
#include <sparse_traits.h>            // for sparse::zero, sparse::volatile_zero
#include <tiny_value_traits.h> 

#include <algorithm>               // for std::copy, std::fill, std::equal
#include <cassert>

namespace prox
{
  
  namespace detail
  {
    
    template <typename B>
    class MassBlockAccessor
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
        
        static int idx_into(size_t const i, size_t const j, block_type const& src)
        {
          return src.safe_idx_into(i,j);
        }
        
        static void copy(block_type const& orig, block_type& src)
        {
          src.safe_copy(orig);
        }
      };
    
  }//namespace detail
  
  /**
   * 6x6 sparse and symmetric matrix:
   * m
   *  m
   *   m
   *    abc
   *    bde
   *    cef
   *
   * represented in a length 7 array: m_data
   *
   * m_data[0] = m, m_data[1] = a, m_data[2] = b, m_data[3] = c, m_data[4] = d, ...
   */
  template <typename T>
  class MassBlock   
    {
      
    public:
      
      typedef MassBlock<T>          block_type;
      typedef T                     value_type;
      typedef T&                    reference;
      typedef T const&              const_reference;
      typedef T*                    pointer;
      typedef T const*              const_pointer;
      typedef T*                    iterator;
      typedef T const*              const_iterator;
      
      typedef tiny::ValueTraits<T>   value_traits;
      
      typedef detail::MassBlockAccessor<block_type>   accessor; 
      
    private:
      
      friend class detail::MassBlockAccessor<block_type>;
    
    protected:
      
      typedef T*                data_container_type;
      
    protected:
      
      value_type m_data[7];
      
    protected:
      
      void safe_copy(block_type const& orig)
      {
        std::copy( orig.m_data, orig.m_data + size(), this->m_data);
      }
      
      int safe_idx_into(size_t i, size_t j) const
      {
        // if return = 0-6 then m_data[return_value]
        // if return < 0   then zero
        
        assert(i < nrows() && j < ncols()  || !"safe_idx_into(): invalid arguments");
        if (i < j)
        {
          std::swap(i, j);
        }
        if (i == j)
        {
          if (i < 3)
          {
            return 0;
          }
          else if (i == 3)
          {
            return 1;
          }
          else
          {
            return (i << 1)-4;
          }
        }
        else if ( j > 2 )
        {
          if ( j == 4 )
          {
            return 5;
          }
          else
          {
            return i - j + 1;
          }
        }
        return -1;
      }
      
    public:
      
      MassBlock() {}
      
      ~MassBlock() {}
      
      MassBlock(const_reference v)
      {
        *this = v;
      }
      
      MassBlock(block_type const& orig)
      {
        this->safe_copy(orig);
      }
      
      MassBlock & operator=(MassBlock const& rhs)
      {
        if (&rhs != this)
        {
          this->safe_copy(rhs);
        }
        return *this;
      }
      
      MassBlock & operator=(const_reference rhs)
      {
        std::fill(this->m_data, this->m_data + size(), rhs);
        return *this;
      }
      
      bool operator==(MassBlock const& rhs) const
      {
        return std::equal(this->begin(), this->end(), rhs.begin());
      }
      
      bool operator!=(MassBlock const & rhs) const
      {
        return !operator==(rhs);
      }
      
      value_type & operator[](size_t const i)
      {
        assert(i < size() || !"[]: invalid argument");
        return this->m_data[i];
      }
      
      value_type const& operator[](size_t const i) const
      {
        assert(i < size() || !"[]: invalid argument");
        return this->m_data[i];
      }
      
      value_type& operator()(size_t const i, size_t const j)
      {
        int idx = this->safe_idx_into(i, j);
        assert(idx >= 0  || !"(): not allowed to modify structural invariant");
        return this->m_data[idx];
      }
      
      value_type const & operator()(size_t const i, size_t const j) const
      {
        int idx = this->safe_idx_into(i, j);
        if (idx < 0)
        {
          return value_traits::zero();
        }
        return this->m_data[idx];
      }
      
      iterator       begin()       { return this->m_data;          }
      const_iterator begin() const { return this->m_data;          }
      iterator       end()         { return this->m_data + size(); }
      const_iterator end()   const { return this->m_data + size(); }
      
      static size_t nrows() { return 6u;      }
      static size_t ncols() { return nrows(); }
      static size_t size()  { return 7u;      }
      
    };
  
  /**
   * @warning precondition: mb is assumed non-singular
   */
  template<typename T>
	inline void inverse(MassBlock<T> & mb)
	{
    typedef typename MassBlock<T>::value_type     value_type;
    typedef typename MassBlock<T>::value_traits   value_traits;
    
		if (mb[0] == value_traits::infinity()) 
		{
			std::fill(mb.begin(), mb.end(), value_traits::zero());
		}
		else if (mb[0] == value_traits::zero() )   // 2009-08-04 Kenny: floating point comparison with zero?
		{
			std::fill(mb.begin(), mb.end(), value_traits::infinity()); 
		}
		else
		{
      // 2009-08-04 Kenny: This seems to be Cramer's rule specialized for a symmetric 3x3 matrix.
      
			mb[0] = value_traits::one() / mb[0];  
			// d = determinant of 3x3 lower right matrix in mb
			value_type d = ( mb[1] * (mb[4]  *mb[6] - (mb[5]*mb[5]))
                  + mb[3] * (2*mb[2]*mb[5] -  mb[3]*mb[4] )
                  - mb[2] *  mb[2]  *mb[6]                );
      
      d = value_traits::one() / d;
      // Cramer's rule - optimized to exploit symmetry of Mass_block
      value_type it3 =  (mb[2]*mb[5] - mb[3]*mb[4]) * d;
      value_type it4 =  (mb[1]*mb[6] - mb[3]*mb[3]) * d;
      value_type it5 =  (mb[1]*mb[5] - mb[2]*mb[3]) * d;
      value_type it6 =  (mb[1]*mb[4] - mb[2]*mb[2]) * d;
      
      mb[1] =  (mb[4]*mb[6] - mb[5]*mb[5]) * d;
      mb[2] = -(mb[2]*mb[6] - mb[3]*mb[5]) * d;
      mb[3] =  it3;
      mb[4] =  it4;
      mb[5] = -it5;
      mb[6] =  it6;
		}
	}

 template<typename T>
 inline void prod(MassBlock<T> const & lhs, sparse::Block<6,1,T> const & rhs, sparse::Block<6,1,T> & res)
 {
   res[0] += lhs[0] * rhs[0];
   res[1] += lhs[0] * rhs[1];
   res[2] += lhs[0] * rhs[2];
   res[3] += lhs[1]*rhs[3] + lhs[2]*rhs[4] + lhs[3]*rhs[5];
   res[4] += lhs[2]*rhs[3] + lhs[4]*rhs[4] + lhs[5]*rhs[5];
   res[5] += lhs[3]*rhs[3] + lhs[5]*rhs[4] + lhs[6]*rhs[5];
  }
  
 template<typename T>
 inline void prod(MassBlock<T> const& lhs, sparse::Block<6,4,T> const& rhs, sparse::Block<6,4,T>& res)
 {
   //checked using MatLab
   res[0] += lhs[0] * rhs[0];
   res[1] += lhs[0] * rhs[1];
   res[2] += lhs[0] * rhs[2];
   res[3] += lhs[0] * rhs[3];
   res[4] += lhs[0] * rhs[4];
   res[5] += lhs[0] * rhs[5];
   res[6] += lhs[0] * rhs[6];
   res[7] += lhs[0] * rhs[7];
   res[8] += lhs[0] * rhs[8];
   res[9] += lhs[0] * rhs[9];
   res[10] += lhs[0] * rhs[10];
   res[11] += lhs[0] * rhs[11];
   res[12] += lhs[1] * rhs[12] + lhs[2] * rhs[16] + lhs[3] * rhs[20];
   res[13] += lhs[1] * rhs[13] + lhs[2] * rhs[17] + lhs[3] * rhs[21];
   res[14] += lhs[1] * rhs[14] + lhs[2] * rhs[18] + lhs[3] * rhs[22];
   res[15] += lhs[1] * rhs[15] + lhs[2] * rhs[19] + lhs[3] * rhs[23];
   res[16] += lhs[2] * rhs[12] + lhs[4] * rhs[16] + lhs[5] * rhs[20];
   res[17] += lhs[2] * rhs[13] + lhs[4] * rhs[17] + lhs[5] * rhs[21];
   res[18] += lhs[2] * rhs[14] + lhs[4] * rhs[18] + lhs[5] * rhs[22];
   res[19] += lhs[2] * rhs[15] + lhs[4] * rhs[19] + lhs[5] * rhs[23];
   res[20] += lhs[3] * rhs[12] + lhs[5] * rhs[16] + lhs[6] * rhs[20];
   res[21] += lhs[3] * rhs[13] + lhs[5] * rhs[17] + lhs[6] * rhs[21];
   res[22] += lhs[3] * rhs[14] + lhs[5] * rhs[18] + lhs[6] * rhs[22];
   res[23] += lhs[3] * rhs[15] + lhs[5] * rhs[19] + lhs[6] * rhs[23];
  }

 template<typename T>
 inline void prod(sparse::Block<4,6,T> const& lhs, MassBlock<T> const& rhs, sparse::Block<4,6,T>& res)
 {   
   //checked using MatLab
   res(0,0) += rhs[0] * lhs(0,0);
   res(1,0) += rhs[0] * lhs(1,0);
   res(2,0) += rhs[0] * lhs(2,0);
   res(3,0) += rhs[0] * lhs(3,0);
   
   res(0,1) += rhs[0] * lhs(0,1);
   res(1,1) += rhs[0] * lhs(1,1);
   res(2,1) += rhs[0] * lhs(2,1);
   res(3,1) += rhs[0] * lhs(3,1);
   
   res(0,2) += rhs[0] * lhs(0,2);
   res(1,2) += rhs[0] * lhs(1,2);
   res(2,2) += rhs[0] * lhs(2,2);
   res(3,2) += rhs[0] * lhs(3,2);

   res(0,3) += rhs[1] * lhs(0,3) + rhs[2] * lhs(0,4) + rhs[3] * lhs(0,5);
   res(1,3) += rhs[1] * lhs(1,3) + rhs[2] * lhs(1,4) + rhs[3] * lhs(1,5);
   res(2,3) += rhs[1] * lhs(2,3) + rhs[2] * lhs(2,4) + rhs[3] * lhs(2,5);
   res(3,3) += rhs[1] * lhs(3,3) + rhs[2] * lhs(3,4) + rhs[3] * lhs(3,5);
   
   res(0,4) += rhs[2] * lhs(0,3) + rhs[4] * lhs(0,4) + rhs[5] * lhs(0,5);
   res(1,4) += rhs[2] * lhs(1,3) + rhs[4] * lhs(1,4) + rhs[5] * lhs(1,5);
   res(2,4) += rhs[2] * lhs(2,3) + rhs[4] * lhs(2,4) + rhs[5] * lhs(2,5);
   res(3,4) += rhs[2] * lhs(3,3) + rhs[4] * lhs(3,4) + rhs[5] * lhs(3,5);
   
   res(0,5) += rhs[3] * lhs(0,3) + rhs[5] * lhs(0,4) + rhs[6] * lhs(0,5);
   res(1,5) += rhs[3] * lhs(1,3) + rhs[5] * lhs(1,4) + rhs[6] * lhs(1,5);
   res(2,5) += rhs[3] * lhs(2,3) + rhs[5] * lhs(2,4) + rhs[6] * lhs(2,5);
   res(3,5) += rhs[3] * lhs(3,3) + rhs[5] * lhs(3,4) + rhs[6] * lhs(3,5);
  }
} // namespace prox

// PROX_MASS_BLOCK_H
#endif 
