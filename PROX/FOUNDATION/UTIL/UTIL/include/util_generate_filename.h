#ifndef UTIL_GENERATE_FILENAME_H
#define UTIL_GENERATE_FILENAME_H

#include <sstream>
#include <string>
#include <iomanip>

namespace util
{

  /**
   * This function is a helper for generating nicely formatted file names.
   */
  inline std::string generate_filename( std::string const & prefix, unsigned int const & frame, std::string const & extension)
  {
    std::ostringstream filename;

    filename << prefix
             << "_"
             << std::setw(6)
             << std::setfill('0')
             << frame
             << "."
             << extension;

    return filename.str();
  }

  
} // namespace util

// UTIL_GENERATE_FILENAME_H
#endif
