#ifndef UTIL_WRITE_MATLAB_VECTOR_H
#define UTIL_WRITE_MATLAB_VECTOR_H

#include <sstream>
#include <string>
#include <vector>

namespace util
{

  /**
   * This version fo the write_vector is usefull for writing indices into
   * matlab scripts. Problem is that in C++ one often use zero-based indexing
   * however in Matlab one would use one-based indexing. Hence by setting
   * add_one to true this function converts from zero-based indexing to
   * one-based indexing.
   */
  template<typename T>
  inline std::string write_matlab_vector(std::vector<T> const & values, size_t const & size, bool const & add_one = false)
  {
    std::stringstream output;

    output << "[";

    if(add_one)
    {
      for(size_t i = 0u; i< size; ++i)
      {
        output <<  values[i] + T(1) << " ";
      }
    }
    else
    {
      for(size_t i = 0u; i< size; ++i)
      {
        output <<  values[i] << " ";
      }
    }

    output << "]";

    output.flush();

    return output.str();
  }

  template<typename T>
  inline std::string write_matlab_vector(std::vector<T> const & values )
  {
    return write_matlab_vector(values, values.size(), false);
  }

  template<typename T>
  inline std::string write_matlab_vector(std::string const & name
                                         , std::vector<T> const & values)
  {
    std::stringstream output;

    output << name << " = [";

    for(size_t i = 0u; i< values.size(); ++i)
    {
      output << values[i] << " ";
    }
    output << "];";
    
    output.flush();
    
    return output.str();
  }
  
}// namespace util

// UTIL_WRITE_MATLAB_VECTOR_H
#endif
