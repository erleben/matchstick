#ifndef TINY_POWER2_H
#define TINY_POWER2_H

namespace tiny
{
  
  /**
   * This method test whether there exist some positive integer, n, such that
   *
   *  2^n == val
   *
   * In which case it returns true otherwise it returns false.
   */
  template< class T>
  inline bool is_power2( T val )
  {
    T next = 1u;
    for ( unsigned int i = 0u; i < 32u; ++i )
    {
      if ( val == next )
        return true;
      next = next << 1u;
    }
    return false;
  }
  
  /**
   * This function finds the smallest positive integer, n, such that
   *
   *   val <= 2^n
   *
   * and returns the value 2^n.
   */
  template<class T>
  inline T upper_power2( T val )
  {
    T next = 1u;
    for ( unsigned int i = 0u; i < 32u; ++i )
    {
      if ( next >= val )
        return next;
      next = next << 1u;
    }
    return 0u;
  }

  /**
   * This function finds the largest positive integer, n, such that
   *
   *   2^n <= val
   *
   * and returns the value 2^n.
   */
  template<class T>
  inline T lower_power2( T val )
  {
    // 2009-07-05 Kenny: Used to be  31u. However, for signed types the last bit is used for the sign, so the mehtod would fails for int but succede for unsigned int.
    T next = 1u << 30u;  
    for ( unsigned int i = 0u; i < 32u; ++i )
    {
      if ( next <= val )
        return next;
      next = next >> 1u;
    }
    return 0u;
  }
  
  
} // namespace tiny

//TINY_POWER2_H
#endif
