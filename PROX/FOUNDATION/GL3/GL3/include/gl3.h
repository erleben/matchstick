#ifndef GL3_H
#define GL3_H

#ifdef WIN32
#  define WIN32_LEAN_AND_MEAN
#  define NOMINMAX
#  include <windows.h>
#  undef WIN32_LEAN_AND_MEAN
#  undef NOMINMAX
#endif

#include <GL/glew.h>

#if defined(__APPLE__) && !defined (VMDMESA)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <string>

inline std::string glErrorString(GLenum const & error_code)
{
  static const struct
  {
    GLenum      code;
    std::string string;
  } errors[] =
  {
    /* GL */
    {GL_NO_ERROR, "no error"},
    {GL_INVALID_ENUM, "invalid enumerant"},
    {GL_INVALID_VALUE, "invalid value"},
    {GL_INVALID_OPERATION, "invalid operation"},
    {GL_STACK_OVERFLOW, "stack overflow"},
    {GL_STACK_UNDERFLOW, "stack underflow"},
    {GL_OUT_OF_MEMORY, "out of memory"},
    {0, "unknown error" }
  };
  for (unsigned int i=0u; errors[i].code; ++i)
  {
    if (errors[i].code == error_code)
      return errors[i].string;
  }
  return "unknown error";
}

//GL3_H
#endif
