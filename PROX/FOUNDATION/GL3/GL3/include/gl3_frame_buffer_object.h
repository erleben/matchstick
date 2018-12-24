#ifndef GL3_FRAME_BUFFER_OBJECT_H
#define GL3_FRAME_BUFFER_OBJECT_H

#include <gl3.h>
#include <gl3_check_errors.h>
#include <gl3_texture_2d.h>
#include <gl3_texture_3d.h>
#include <gl3_render_buffer.h>

#include <tiny_power2.h>

#include <util_log.h>

#include <vector>
#include <string>
#include <cmath>
#include <cassert>

namespace gl3
{

  /**
   * Frame Buffer Object.
   * This class encapsulates the frame buffer object (FBO) OpenGL spec. See
   * the official spec at:
   * http://oss.sgi.com/projects/ogl-sample/registry/EXT/framebuffer_object.txt
   * for details.
   *
   * A frame buffer object (FBO) is conceptually a structure containing pointers
   * to GPU memory. The memory pointed to is either an OpenGL texture or an
   * OpenGL RenderBuffer. FBOs can be used to render to one or more textures,
   * share depth buffers between multiple sets of color buffers/textures and
   * are a complete replacement for pbuffers.
   *
   * Performance Notes:
   *
   * 1) It is more efficient (but not required) to call bind()
   * on an FBO before making multiple method calls.
   *
   * 2) Use FramebufferObject::disable() sparingly.
   *
   * 3) Binding an FBO is usually much faster than enabling/disabling
   *    a pbuffer, but is still a costly operation. When switching
   *    between multiple FBOs and a visible OpenGL framebuffer.
   *
   * This implementation was inspired by the ideas in the FBO class from
   * GPUGP by Aaron Lefohn.
   */
  class FrameBufferObject
  {
  protected:

    GLuint m_fbo_id;         ///< The Frame buffer Identifier of this FBO.
    GLuint m_saved_fbo_id;   ///< The frame buffer identifier of any previously bound FBO. This is used to test whether the FBO is bound before doing anything to it. See guarded_bind and guarded_unbind.

  protected:

    /**
     * Create Frame Buffer Object Identifier.
     *
     * @return       A new unused identifier for a frame buffer object.
     */
    static GLuint create_fbo_id()
    {
      GLuint id = 0;
      glGenFramebuffers(1, &id);
      return id;
    }

  public:

    FrameBufferObject()
    : m_fbo_id( 0 )
    , m_saved_fbo_id(0)
    {
      //--- Bind this FBO so that it actually gets created now
      guarded_bind();
      guarded_unbind();
    }

    ~FrameBufferObject()
    {
    }

  public:

    void create()
    {
      if(m_fbo_id==0)
        m_fbo_id = create_fbo_id();
    }

    void clear()
    {
      glDeleteFramebuffers(1, &m_fbo_id);
      m_fbo_id = 0;
    }

  protected:

    /**
     * Guarded Bind.
     * This method tests whether this FBO is already bound. If not it will save
     * the currently bound FBO before binding this FBO.
     */
    void guarded_bind()
    {
      //--- Only binds if m_fbo_id is different than the currently bound FBO
      GLint tmp = 0;
      glGetIntegerv( GL_FRAMEBUFFER_BINDING, &tmp );
      m_saved_fbo_id = static_cast<GLuint>(tmp);

      if (m_fbo_id != m_saved_fbo_id)
      {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_id);
      }
    }

    /**
     * Guarded Unbind.
     * In case another FBO was bound prior to invoking guarded_bind(), this
     * method will restore that FBO.
     */
    void guarded_unbind()
    {
      //--- Returns FBO binding to the previously enabled FBO
      if (m_saved_fbo_id != m_fbo_id)
      {
        glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)m_saved_fbo_id);
      }
    }

  public:

    /**
     * Bind FBO.
     * This method binds this FBO as current render target.
     */
    void bind()
    {
      glBindFramebuffer(GL_FRAMEBUFFER, m_fbo_id);
    }

    void set_no_color_buffer()
    {
      guarded_bind();
      glDrawBuffer(GL_NONE);
      guarded_unbind();
    }


    void set_draw_buffer( GLenum attachment )
    {
      guarded_bind();
      glDrawBuffer(attachment);
      guarded_unbind();
    }

    void set_draw_buffers( GLenum attachment1, GLenum attachment2 )
    {
      GLenum attachments[2] = {attachment1, attachment2};
      guarded_bind();
      glDrawBuffers(2, &attachments[0]);
      guarded_unbind();
    }

    void set_draw_buffers( GLenum attachment1, GLenum attachment2, GLenum attachment3 )
    {
      GLenum attachments[3] = {attachment1, attachment2, attachment3};
      guarded_bind();
      glDrawBuffers(3, &attachments[0]);
      guarded_unbind();
    }

    /**
     * Attach Texture.
     * This method binds a texture to the specified "attachment" point of this FBO.
     *
     * @param attachment       The attachment point. For instance the color buffer attachment
     *                         point: GL_COLOR_ATTACHMENT0,...,GL_COLOR_ATTACHMENTn
     * @param texture_target   The texture target: GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_1D,
     *                         GL_TEXTURE_2D, GL_TEXTURE_3D etc..
     * @param texture_id       The texture identifier, i.e the texture name as generated
     *                         by calling the function glGenTextures(...).
     * @param mip_level        The level-of-detail number. Level 0 is the base image
     *                         level. Level n is the nth mipmap reduction image. Default
     *                         is zero.
     * @param z_slice          The z slice. This works as a z-offset into a 3D texture. The
     *                         parameter is only used if texture target is a GL_TEXTURE_3D,
     *                         the default value is zero.
     */
    void attach_texture(
                        GLenum attachment
                        , GLenum texture_target
                        , GLuint texture_id
                        , int mip_level = 0
                        , int z_slice   = 0
                        )
    {
      guarded_bind();

#ifndef NDEBUG
      if( get_attached_id(attachment) != texture_id )
      {
#endif
        //--- decode what type of texture that is being attached and call the
        //--- corresponding FrameBufferTexture?D method.
        if (texture_target == GL_TEXTURE_1D)
        {
          glFramebufferTexture1D(
                                 GL_FRAMEBUFFER
                                 , attachment
                                 , GL_TEXTURE_1D
                                 , texture_id
                                 , mip_level
                                 );
        }
        else if (texture_target == GL_TEXTURE_3D)
        {
          glFramebufferTexture3D(
                                 GL_FRAMEBUFFER
                                 , attachment
                                 , GL_TEXTURE_3D
                                 , texture_id
                                 , mip_level
                                 , z_slice
                                 );
        }
        else
        {
          // Default is GL_TEXTURE_2D, GL_TEXTURE_RECTANGLE_ARB, or cube faces
          glFramebufferTexture2D(
                                 GL_FRAMEBUFFER
                                 , attachment
                                 , texture_target
                                 , texture_id
                                 , mip_level
                                 );
        }
#ifndef NDEBUG
      }
#endif
      guarded_unbind();
    }

    /**
     * Attach 2D Texture.
     *
     * @param attachment      The attachment point. For instance the color buffer
     *                        attachment point: GL_COLOR_ATTACHMENT0,...,GL_COLOR_ATTACHMENTn
     * @param texture         A pointer to Texture2D containing the texture that should be attached.
     */
    void attach_texture(GLenum attachment, Texture2D const & texture)
    {
      attach_texture(attachment, texture.get_texture_target(), texture.get_texture_ID());
    }

    /**
     * Attach 3D Texture.
     *
     * @param attachment     The attachment point. For instance the color buffer
     *                       attachment point: GL_COLOR_ATTACHMENT0,...,GL_COLOR_ATTACHMENTn
     * @param texture        A pointer to Texture3D containing the texture that should be attached.
     */
    void attach_texture(GLenum attachment, Texture3D const & texture, int z_slice)
    {
      attach_texture(attachment, texture.get_texture_target(), texture.get_texture_ID(), 0, z_slice);
    }

    /**
     * Attach Render Buffer.
     * This method binds a render buffer to the spcified "attachment"
     * point of this FBO.
     *
     * @param attachment       The attachment point. For instance the color buffer attachment
     *                         point: GL_COLOR_ATTACHMENT0,...,GL_COLOR_ATTACHMENTn
     *                         However, this is usually GL_DEPTH_ATTACHMENT or GL_STENCIL_ATTACHMENT for
     *                         depth and stencil buffers.
     * @param buffer_id        The identifier of the render buffer. That is the buffer name
     *                         generated by calling glGenRenderbuffers(...).
     */
    void attach_render_buffer( GLenum attachment, GLuint buffer_id )
    {
      guarded_bind();
#ifndef NDEBUG
      if( get_attached_id(attachment) != buffer_id )
      {
#endif
        glFramebufferRenderbuffer( GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, buffer_id);
#ifndef NDEBUG
      }
#endif
      guarded_unbind();
    }

    /**
     * Attach Render Buffer.
     * This method binds a render buffer to the spcified "attachment"
     * point of this FBO.
     *
     * @param attachment       The attachment point. For instance the color buffer attachment
     *                         point: GL_COLOR_ATTACHMENT0,...,GL_COLOR_ATTACHMENTn.
     *                         However, this is usually GL_DEPTH_ATTACHMENT or GL_STENCIL_ATTACHMENT for
     *                         depth and stencil buffers.
     * @param buffer           A pointer to a glRenderBuffer.
     */
    void attach_render_buffer( GLenum attachment, RenderBuffer const &  buffer )
    {
      attach_render_buffer(attachment, buffer.get_id() );
    }

    /**
     * Unattach Resources.
     * This method frees any resource bound to the specified "attachment" point of this FBO.
     *
     * @param attachment       The attachment point. For instance the color buffer
     *                         attachment point: GL_COLOR_ATTACHMENT0,...,GL_COLOR_ATTACHMENTn
     */
    void unattach( GLenum attachment )
    {
      guarded_bind();
      GLenum type = get_attached_type(attachment);
      switch(type)
      {
        case GL_NONE:
          break;
        case GL_RENDERBUFFER:
          attach_render_buffer( attachment, 0 );
          break;
        case GL_TEXTURE:
          attach_texture( attachment, GL_TEXTURE_2D, 0 );
          break;
        default:
        {
          util::Log logging;
          
          logging << "FramebufferObject::unattach(): Unknown attached resource type" << util::Log::newline();
        }

      }
      guarded_unbind();
    }

    /**
     * Test if Frame Buffer Object is valid.
     *
     * @param output   An output stream where error messages (if any) should be
     *                 written. Default value is the logging stream.
     * @return         If the FBO is valid render target then the return value
     *                 is true otherwise it is false. Notice that in non-debug build
     *                 mode this function always return true.
     */
#ifndef NDEBUG
    bool is_valid( )
    {
      util::Log logging;

      guarded_bind();
      GLenum status;
      status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
      switch(status)
      {
        case GL_FRAMEBUFFER_COMPLETE: // Everything's OK
          break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
          logging << "glFrameBufferObject.is_valid():\n\t" << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n";
          break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
          logging << "glFrameBufferObject.is_valid():\n\t" << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n";
          break;
          // NOTE: henrikd 20060326 - Removed from the spec. Glew 1.3.4 does not define it anymore.
          //       See: http://oss.sgi.com/projects/ogl-sample/registry/EXT/framebuffer_object.txt
          //       See: http://sourceforge.net/tracker/index.php?func=detail&aid=1445198&group_id=67586&atid=523274
          //      case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT:
          //        output << "glFrameBufferObject.is_valid():\n\t" << "GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT\n";
          //        break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
          logging << "glFrameBufferObject.is_valid():\n\t" << "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT\n";
          break;
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
          logging << "glFrameBufferObject.is_valid():\n\t"  << "GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT\n";
          break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
          logging << "glFrameBufferObject.is_valid():\n\t" << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER\n";
          break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
          logging << "glFrameBufferObject.is_valid():\n\t"  << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n";
          break;
        case GL_FRAMEBUFFER_UNSUPPORTED:
          logging << "glFrameBufferObject.is_valid():\n\t"  << "GL_FRAMEBUFFER_UNSUPPORTED\n";
          break;
          // NOTE: henrikd 20060326 - Removed from the spec. Glew 1.3.4 does not define it anymore.
          //       See: http://oss.sgi.com/projects/ogl-sample/registry/EXT/framebuffer_object.txt
          //  case GL_FRAMEBUFFER_STATUS_ERROR:
          //    output << "glFrameBufferObject.is_valid():\n\t" << "GL_FRAMEBUFFER_STATUS_ERROR\n";
          //    break;
        default:
          logging << "glFrameBufferObject.is_valid():\n\t"  << "Unknown ERROR\n";
      }
      guarded_unbind();
      return (status==GL_FRAMEBUFFER_COMPLETE);
    }
#else
    bool is_valid() {       return true;     }
#endif

    /**
     * Get Attachment Type.
     *
     * @param attachment       The attachment point. For instance the color buffer attachment
     *                         point: GL_COLOR_ATTACHMENT0,...,GL_COLOR_ATTACHMENTn
     *
     * @return                 Returns the attached type, that is GL_RENDERBUFFER or GL_TEXTURE?
     */
    GLenum get_attached_type( GLenum attachment )
    {
      // Returns GL_RENDERBUFFER or GL_TEXTURE
      guarded_bind();
      GLint type = 0;
      glGetFramebufferAttachmentParameteriv(
                                            GL_FRAMEBUFFER
                                            , attachment
                                            , GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE
                                            , &type
                                            );
      guarded_unbind();
      return GLenum(type);
    }

    /**
     * Get Attachment Identifier.
     *
     * @param attachment       The attachment point. For instance the color buffer attachment
     *                         point: GL_COLOR_ATTACHMENT0,...,GL_COLOR_ATTACHMENTn
     *
     * @return                 The identifier of the of the  render buffer/texture currently
     *                         attached to specified attachement point.
     */
    GLuint get_attached_id( GLenum attachment )
    {
      guarded_bind();
      GLint id = 0;
      glGetFramebufferAttachmentParameteriv(
                                            GL_FRAMEBUFFER
                                            , attachment
                                            , GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME
                                            , &id
                                            );
      guarded_unbind();
      return GLuint(id);
    }

    /**
     * Get MipMap Level.
     *
     * @param attachment       The attachment point. For instance the color buffer attachment
     *                         point: GL_COLOR_ATTACHMENT0,...,GL_COLOR_ATTACHMENTn
     *
     * @return                 The mipmap level that is currently attached to specified attachement point.
     */
    GLint get_attached_mip_level( GLenum attachment )
    {
      guarded_bind();
      GLint level = 0;
      glGetFramebufferAttachmentParameteriv(
                                            GL_FRAMEBUFFER
                                            , attachment
                                            , GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL
                                            , &level
                                            );
      guarded_unbind();
      return level;
    }

    /**
     * Get Cube Face.
     *
     * @param attachment       The attachment point. For instance the color buffer attachment
     *                         point: GL_COLOR_ATTACHMENT0,...,GL_COLOR_ATTACHMENTn
     *
     * @return                 The cube face that is currently attached to specified attachment point.
     */
    GLint get_attached_cube_face( GLenum attachment )
    {
      guarded_bind();
      GLint level = 0;
      glGetFramebufferAttachmentParameteriv(
                                            GL_FRAMEBUFFER
                                            , attachment
                                            , GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE
                                            , &level
                                            );
      guarded_unbind();
      return level;
    }

    /**
     * Get Z Slice.
     *
     * @param attachment    The attachment point. For instance the color buffer attachment
     *                      point: GL_COLOR_ATTACHMENT0,...,GL_COLOR_ATTACHMENTn
     *
     * @return              The z-slice that is currently attached to specified attachment point.
     */
    GLint get_attached_Z_slice( GLenum attachment )
    {
      guarded_bind();
      GLint slice = 0;
      glGetFramebufferAttachmentParameteriv(
                                            GL_FRAMEBUFFER
                                            , attachment
                                            , GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT
                                            , &slice
                                            );
      guarded_unbind();
      return slice;
    }

  public:

    /**
     * Get Maximum Color Attachments
     * That is if GL_COLOR_ATTACHMENT0,...,GL_COLOR_ATTACHMENT(n-1) are
     * valid attachment points then the number n is returned.
     *
     * @return   The number of color buffer attachments permitted.
     */
    static GLint get_max_color_attachments()
    {
      GLint max_attach = 0;
      glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS, &max_attach );
      return max_attach;
    }
    
    /**
     * Disable FBO.
     * Disable all FBO rendering and return to traditional, windowing-system controlled framebuffer.
     * This is NOT an "unbind" for this specific FBO, but rather disables all FBO rendering.
     */
    static void disable()
    {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
  };
  
}//namespace gl3

//GL3_FRAME_BUFFER_OBJECT_H
#endif
