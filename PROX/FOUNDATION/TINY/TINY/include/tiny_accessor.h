#ifndef TINY_ACCESSOR_H
#define TINY_ACCESSOR_H

#include <cstddef>  // Needed for size_t

namespace tiny
{
  namespace detail
  {
    
    /**
     * Accessor class.
     * This class provides cast operations for type conversions.
     *
     * @tparam M    The math base type that should be accessed.
     */
    template <typename M> 
    class Accessor
      {
      public:
        
        typedef typename M::op_type  op_type;
        
        static op_type & cast (M & m, size_t const & i, size_t const & j)
        {
          return m.get_op_type(i,j);
        } 
        
        static op_type cast (M const & m, size_t const i, size_t const j)
        {
          return m.get_op_type(i,j);
        } 
        
        static size_t stride   ()  { return M::stride;    }
        static size_t padding  ()  { return M::padding;   }
        static size_t J_padded ()  { return M::J_padded;  }
        static size_t allocsize()  { return M::allocsize; }
        
      };
    
  } // namespace detail
} // namespace tiny

// TINY_ACCESSOR_H
#endif 
