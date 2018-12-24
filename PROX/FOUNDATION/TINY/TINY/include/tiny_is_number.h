#ifndef TINY_IS_NUMBER_H
#define TINY_IS_NUMBER_H

#ifdef WIN32
#include <float.h>
#else
#include <cmath>
#endif

namespace tiny
{
  
#ifdef WIN32
#define is_number(val) (_isnan(val)==0)   ///< Is a number test
#else 
#if (__APPLE__)
#define is_number(val) (std::isnan(val)==0)   ///< Is a number test
#else
#ifdef __linux__
#define is_number(val) (std::isnan(val)==false)   ///< Is a number test
#else
#define is_number(val) (isnan(val)==0)   ///< Is a number test
#endif
#endif
#endif
  
} // namespace tiny

//TINY_IS_NUMBER_H
#endif
