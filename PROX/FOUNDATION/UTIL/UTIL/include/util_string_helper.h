#ifndef UTIL_STRING_HELPER_H
#define UTIL_STRING_HELPER_H

#include <string>

/// (C) by Kenny Erleben 2014

namespace util
{
  template<typename T>
  T to_value(std::string const & txt);
  
  template<typename T>
  std::string to_string(T const & value);

  /**
   * Convenience tool for quickly testing if text contains subtext or not.
   */
  inline bool contains( std::string const & text, std::string const & subtext)
  {
    return text.find(subtext) != std::string::npos;
  }
  
} // namespace util

// UTIL_STRING_HELPER_H
#endif
