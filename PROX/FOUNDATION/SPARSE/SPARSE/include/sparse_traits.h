#ifndef SPARSE_TRAITS_H
#define SPARSE_TRAITS_H

namespace sparse
{
  template <typename B>
  inline B const& zero_block()
  {
    // 2009-07-14 Kenny: MSVC: warning C4640 construction of local static object is not thread safe
    // 2009-07-01 Kenny: Prober value traits for value_type
    static B z(typename B::value_type(0));
    return z;
  }
  
  template <typename B>
  inline B & volatile_zero_block()
  {
    // 2009-07-14 Kenny: MSVC: warning C4640 construction of local static object is not thread safe
    // 2009-07-01 Kenny: Prober value traits for value_type
    static B z(typename B::value_type(0));
    return z;
  }
  
} // namespace sparse

// SPARSE_TRAITS_H
#endif 
