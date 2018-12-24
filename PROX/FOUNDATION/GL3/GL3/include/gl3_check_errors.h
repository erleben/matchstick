#ifndef GL3_CHECK_ERRORS_H
#define GL3_CHECK_ERRORS_H

#include <string>

namespace gl3
{

#ifndef NDEBUG

  /**
   * A simple OpenGL error checking routine.
   * This compiles away to a no-op method if the NDEBUG preprocessor symbol
   * is defined during compilation.
   *
   * @param location  Optional. A string that can be used to indicate the location
   *                  where the error check occurs.
   */
  void check_errors( std::string const & location = "" );

#else

  void check_errors(  );

  void check_errors( std::string const & );

  void check_errors( std::string const &, std::ostream&  );

#endif


} // namespace gl3

//GL3_CHECK_ERRORS_H
#endif
