#ifndef UTIL_WRITE_MATLAB_MATRIX_H
#define UTIL_WRITE_MATLAB_MATRIX_H

#include <util_write_matlab_vector.h>

#include <sstream>
#include <string>
#include <vector>

namespace util
{

  template<typename T, typename I>
  inline std::string write_matlab_matrix(
                                         std::vector<I> const & rows
                                         , std::vector<I> const & columns
                                         , std::vector<T> const & values
                                         , std::size_t const & nzeros
                                         , std::size_t const & m
                                         , std::size_t const & n
                                         )
  {
    std::stringstream output;

    output << "sparse(";
    output << write_matlab_vector( rows, nzeros, true );
    output << ",";
    output << write_matlab_vector( columns, nzeros, true );
    output << ",";
    output << write_matlab_vector( values, nzeros );
    output << ",";
    output << m;
    output << ",";
    output << n;
    output << ",";
    output << nzeros;
    output << ")";

    output.flush();

    return output.str();
  }

  template<typename T, typename I>
  inline std::string write_matlab_matrix(
                                           std::string const & name
                                         , std::vector<I> const & rows
                                         , std::vector<I> const & columns
                                         , std::vector<T> const & values
                                         , std::size_t const & nzeros
                                         , std::size_t const & m
                                         , std::size_t const & n
                                         )
  {
    std::stringstream output;

    output << name << " = " << write_matlab_matrix(rows,columns, values, nzeros, m ,n) << ";";

    output.flush();

    return output.str();
  }
  
}

// UTIL_WRITE_MATLAB_MATRIX_H
#endif
