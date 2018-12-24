#ifndef SPARSE_IF_THEN_ELSE_H
#define SPARSE_IF_THEN_ELSE_H

namespace sparse
{
  namespace detail
  {
    
    /**
     * Meta Programming construct for making if-then-else selections at compile time.
     *
     * @tparam B   Compile tme boolean value used to select between types T or F.
     * @tparam T   The type to be selected if B is true.
     * @tparam F   The type to be selected if B is false.
     */
    template <bool B, typename T, typename F>
    class if_then_else;
    
    template <typename T, typename F>
    class if_then_else<true, T, F>
    {
    public:
      typedef T result;
    };
    
    template <typename T, typename F>
    class if_then_else<false, T, F>
    {
    public:
      typedef F result;
    };
    
  } // namespace detail
  
} // namespace sparse

// SPARSE_IF_THEN_ELSE_H
#endif 
