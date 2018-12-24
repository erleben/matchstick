#include <gl3_check_errors.h>

#include <gl3.h>

#include <util_log.h>

namespace gl3
{

#ifndef NDEBUG

  void check_errors( std::string const & location )
  {
    util::Log logging;

    GLuint error_code;

    while ( (error_code = glGetError()) != 0 )
    {
      logging << glErrorString(error_code);

      if(!location.empty())
        logging << " at " << location;
      logging << util::Log::newline();
    }
    return;
  }

#else

  void check_errors(  )
  {}

  void check_errors( std::string const & )
  {}

#endif

} // namespace gl3

