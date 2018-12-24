#ifndef GL3_RENDER_BUFFER_H
#define GL3_RENDER_BUFFER_H

#include <gl3.h>
#include <gl3_check_errors.h>
#include <gl3_texture_types.h>

#include <tiny_power2.h>

#include <util_log.h>

#include <vector>
#include <string>
#include <cmath>
#include <cassert>

namespace gl3
{

  /**
   * Render Buffer.
   * This class encapsulates the render buffer OpenGL object described in the
   * frame buffer object (FBO) OpenGL spec. See the official spec at:
   * http://oss.sgi.com/projects/ogl-sample/registry/EXT/framebuffer_object.txt
   * for complete details.
   *
   * A "render buffer" is a chunk of GPU memory used by frame buffer objects to
   * represent "traditional" framebuffer memory (depth, stencil, and color buffers).
   * By "traditional," we mean that the memory cannot be bound as a texture.
   * With respect to GPU shaders, Render buffer memory is "write-only." Framebuffer
   * operations such as alpha blending, depth test, alpha test, stencil test, etc.
   * read from this memory in post-fragement-shader (ROP) operations.
   *
   * The most common use of render buffers is to create depth and stencil buffers.
   * Note that as of 7/1/05, NVIDIA drivers to do not support stencil Renderbuffers.
   *
   * Usage Notes:
   * 1) "internal_format" can be any of the following: Valid OpenGL internal formats
   * beginning with: RGB, RGBA, DEPTH_COMPONENT
   *
   * or a stencil buffer format (not currently supported  in NVIDIA drivers as of 7/1/05).
   * STENCIL_INDEX1
   * STENCIL_INDEX4
   * STENCIL_INDEX8
   * STENCIL_INDEX16
   *
   * This implementation was inspired by the ideas in the FBO class from
   * GPUGP by Aaron Lefohn.
   */
  class RenderBuffer
  {
  protected:

    GLuint m_buffer_id;  ///<  The render buffer identifier of this render buffer.

  protected:

    /**
     * Create Unused Render Buffer identifier.
     *
     * @return    The unused identifier.
     */
    static GLuint create_buffer_id()
    {
      GLuint id = 0;
      glGenRenderbuffers(1, &id);
      check_errors("RenderBuffer::create_buffer_id(): glGenRenderbuffers");
      return id;
    }

  public:

    RenderBuffer()
    : m_buffer_id( create_buffer_id() )
    {}

    /**
     * Specialized Constructor.
     *
     * @param internal_format   The format of the render buffer: RGB, RGBA, DEPTH_COMPONENT etc..
     * @param width             The number of pixels in a row.
     * @param height            The number of rows in the buffer.
     */
    RenderBuffer(GLenum internal_format, int width, int height)
    : m_buffer_id( create_buffer_id() )
    {
      set(internal_format, width, height);
    }

    ~RenderBuffer()
    {
      glDeleteRenderbuffers(1, &m_buffer_id);
    }

  public:

    void bind()
    {
      glBindRenderbuffer(GL_RENDERBUFFER, m_buffer_id);
      check_errors("RenderBuffer::bind(): glBindRenderbuffer");
    }

    void unbind()
    {
      glBindRenderbuffer(GL_RENDERBUFFER, 0);
      check_errors("RenderBuffer::unbind(): glBindRenderbuffer");
    }

    /**
     * Set Render Buffer Format and Size.
     *
     * @param internal_format   The format of the render buffer: RGB, RGBA, DEPTH_COMPONENT etc..
     * @param width             The number of pixels in a row.
     * @param height            The number of rows in the buffer.
     */
    void set(GLenum internal_format, int width, int height)
    {
      int max_size = RenderBuffer::get_max_size();

      if (width > max_size || height > max_size )
      {
        util::Log logging;

        logging << "Renderbuffer.set(): " << "Size too big (" << width << ", " << height << ")" << util::Log::newline();

        return;
      }

      //--- Guarded bind
      GLint tmp = 0;
      glGetIntegerv( GL_RENDERBUFFER_BINDING, &tmp );
      check_errors("RenderBuffer::set(): glGetIntegerv");
      GLuint saved_id = static_cast<GLuint>(tmp);
      if (saved_id != m_buffer_id)
      {
        bind();
      }

      //--- Allocate memory for renderBuffer
      glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, height );
      check_errors("RenderBuffer::set(): glRenderbufferStorage");

      //--- Guarded unbind
      if (saved_id != m_buffer_id)
      {
        unbind();
      }
    }

    /**
     * Get Identifier.
     *
     * @return    The identifier of the render buffer.
     */
    GLuint get_id() const
    {
      return m_buffer_id;
    }

  public:

    /**
     * Get Maximum Size.
     *
     * @return   The maximum size of a render buffer. I.e. the maximum
     *           number of ``pixels'' in widht and height arguments.
     */
    static GLint get_max_size()
    {
      GLint max_size = 0;
      glGetIntegerv( GL_MAX_RENDERBUFFER_SIZE, &max_size );
      check_errors("RenderBuffer::get_max_size(): glGetIntegerv");
      return max_size;
    }

  };

  /**
   * Create Depth Buffer.
   * This method is a convenience tool, it makes it easier to
   * create a render buffer representing a depth buffer.
   *
   * @param width     The number of pixels in a row of the depth buffer.
   * @param height    The number of rows in the depth buffer.
   */
  inline RenderBuffer create_depth_buffer(unsigned int width, unsigned int height)
  {
    RenderBuffer buffer;
    buffer.set(GL_DEPTH_COMPONENT24,width, height);
    return buffer;
  }

  /**
   * Create Stencil Buffer.
   * This method is a convenience tool, it makes it easier to
   * create a render buffer representing a stencil buffer.
   *
   * @param width     The number of pixels in a row of the stencil buffer.
   * @param height    The number of rows in the stencil buffer.
   */
  inline RenderBuffer create_stencil_buffer(unsigned int width, unsigned int height)
  {
    RenderBuffer buffer;
    buffer.set(GL_STENCIL_INDEX16,width, height);
    return buffer;
  }

  /**
   * Create Stencil Buffer.
   * This method is a convenience tool, it makes it easier to
   * create a render buffer representing a packed depth and stencil buffer.
   *
   * @param width     The number of pixels in a row of the buffer.
   * @param height    The number of rows in the buffer.
   */
  inline RenderBuffer create_packed_depth_and_stencil_buffer(unsigned int width, unsigned int height)
  {
    RenderBuffer buffer;
    buffer.set(GL_DEPTH24_STENCIL8,width, height);
    return buffer;
  }

} // namespace gl3

//GL3_RENDER_BUFFER_H
#endif
