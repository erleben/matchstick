#ifndef TINY_PRECISION_H
#define TINY_PRECISION_H

#include <limits>  // for std::numeric_limits<T>::epsilon()

namespace tiny
{
  /**
   * Machine precision is the smallest number eps such that the difference between 1 and 1 + eps is
   * nonzero, ie., it is the smallest difference between two numbers that the computer recognizes. On
   * a 32 bit computer, single precision is 2^-23 (approximately 10e-7) while double
   * precision is 2^-52 (approximately 10e-16) . 
   */
  template <typename T>
  inline T machine_precision()
  {
    return std::numeric_limits<T>::epsilon();
  }
  
  template <typename T>
  inline T working_precision()
  {
    return std::numeric_limits<T>::epsilon()*10;
  }
  
  template <typename T>
  inline T working_precision(unsigned int scale_factor)
  {
    return std::numeric_limits<T>::epsilon()*scale_factor;
  }
  
} // namespace tiny

//TINY_PRECISION_H
#endif
