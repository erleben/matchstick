#ifndef UTIL_GET_ENVIRONMENT_H
#define	UTIL_GET_ENVIRONMENT_H

#include <util_string_helper.h>

#include <cstdlib>
#include <string>

namespace util
{
    
  template<typename T>
  inline T get_environment(std::string const & key, T const & default_value)
  {
    char * value = std::getenv( key.c_str() );

    return value ? to_value<T>(std::string(value)) : default_value;
  }
    
} // namespace util

// UTIL_GET_ENVIRONMENT_H
#endif

