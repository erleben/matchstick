#ifndef GL3_TEXTURE_3D_H
#define GL3_TEXTURE_3D_H

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
   * A 3D Texture convenience Class.
   * This class encapsylates all openGL technicalities in dealing with a 3D texture.
   */
  class Texture3D
  {
  protected:

    GLuint       m_texture_ID;          ///< The ID of the texture object.
    int          m_internal_format;     ///< The internal format of the texture, i.e. how many color compoents (and their size).

    int          m_width;               ///< The number of pixels in a image row.
    int          m_height;              ///< The number of rows in a image.
    int          m_depth;               ///< The number of images in volume.

    unsigned int m_format;              ///< External pixel format, i.e how many components.
    unsigned int m_type;                ///< External pixel type, i.e. unsigned char, unsigned short etc..
    unsigned int m_texture_target;      ///< The texture target.

  public:

    unsigned int get_texture_ID()      const { return m_texture_ID;      }
    unsigned int get_texture_target()  const { return m_texture_target;  }
    int          width()               const { return m_width;  }
    int          height()              const { return m_height;  }
    int          depth()               const { return m_depth;  }
    int          get_internal_format() const { return m_internal_format; }

  protected:

    void create()
    {
      assert(m_texture_ID==0 || !"Texture3D::create(): Texture was already created");

      glGenTextures(1, &m_texture_ID);
      gl3::check_errors("create: glGenTextures");

      if(m_texture_ID == 0)
      {
        util::Log logging;

        logging<< "Texture3D::create(): Could not create texture ID" << util::Log::newline();
      }
    }

    /**
     * Check Texture Size
     * This method test if the desired texture can be loaded into texture memory. Note
     * it does not test whether the texture would actually be resident in texture memory. It
     * only tests if there is available resources!!!
     *
     * @param internal_format  The number of color components in the texture.
     *                         Must be 1, 2, 3, or 4, or one of the following
     *                         symbolic constants: GL_ALPHA, GL_ALPHA4, GL_ALPHA8,
     *                         GL_ALPHA12, GL_ALPHA16, GL_LUMINANCE, GL_LUMINANCE4,
     *                         GL_LUMINANCE8, GL_LUMINANCE12, GL_LUMINANCE16,
     *                         GL_LUMINANCE_ALPHA, GL_LUMINANCE4_ALPHA4, GL_LUMINANCE6_ALPHA2,
     *                         GL_LUMINANCE8_ALPHA8, GL_LUMINANCE12_ALPHA4, GL_LUMINANCE12_ALPHA12,
     *                         GL_LUMINANCE16_ALPHA16, GL_INTENSITY, GL_INTENSITY4, GL_INTENSITY8,
     *                         GL_INTENSITY12, GL_INTENSITY16, GL_R3_G3_B2, GL_RGB, GL_RGB4, GL_RGB5,
     *                         GL_RGB8, GL_RGB10, GL_RGB12, GL_RGB16, GL_RGBA, GL_RGBA2, GL_RGBA4,
     *                         GL_RGB5_A1, GL_RGBA8, GL_RGB10_A2, GL_RGBA12, or GL_RGBA16.
     * @param width            The width of the texture image.
     * @param height           The height of the texture image.
     * @param depth            The depth of the texture image.
     * @param external_format  The format of the pixel data. It can assume one of the symbolic values:
     *                         GL_COLOR_INDEX, GL_RED, GL_GREEN,GL_BLUE,GL_ALPHA,GL_RGB,GL_RGBA,GL_BGR_EXT,GL_BGR_EXT
     *                         GL_BGRA_EXT,GL_LUMINANCE,GL_LUMINANCE_ALPHA
     * @param external_type    The data type of the pixel data. The following symbolic values are accepted:
     *                         GL_UNSIGNED_BYTE, GL_BYTE, GL_BITMAP, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT,
     *                         GL_INT, and GL_FLOAT.
     *
     * @return                  The texture size in bytes, or zero if the texture can not be loaded into texture memory.
     */
    int check_texture_size(
                           int internal_format
                           , int width
                           , int height
                           , int depth
                           , int external_format
                           , int external_type
                           )
    {
      using std::ceil;

      //--- Test texture dimensions against lower bounds on 3D texture size!
      GLint max_tex_size;
      GLint max_tex_size_ext;

      glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &max_tex_size);
      glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE_EXT, &max_tex_size_ext);
      gl3::check_errors("check_texture_size: glGetInteger");

      if(width>max_tex_size ||height>max_tex_size || depth>max_tex_size )
        return 0;

      if(width>max_tex_size_ext ||height>max_tex_size_ext || depth>max_tex_size_ext )
        return 0;

      //--- see if texture is capable of being loaded into texture memory
      //---
      //--- If successfull this does not imply that texture will be resident in texture memory!!!
      unsigned int proxy_texture_target = GL_PROXY_TEXTURE_3D;

      glTexImage3D( proxy_texture_target, 0, internal_format, width, height, depth, 0, external_format, external_type, 0 );
      gl3::check_errors("check_texture_size: glTexImage3D");

      GLint tex_size[ 3 ] = { 0, 0, 0 };
      GLint channel_size[ 6 ] = { 0, 0, 0, 0, 0, 0};
      glGetTexLevelParameteriv( proxy_texture_target, 0, GL_TEXTURE_WIDTH, &( tex_size[ 0 ] )     );
      gl3::check_errors("check_texture_size: glGetTexLEvelParameter 1");

      if(tex_size[0]==0)
        return 0;

      glGetTexLevelParameteriv( proxy_texture_target, 0, GL_TEXTURE_HEIGHT        , &( tex_size[ 1 ] )     );
      gl3::check_errors("check_texture_size: glGetTexLevelParameter 2");
      glGetTexLevelParameteriv( proxy_texture_target, 0, GL_TEXTURE_DEPTH         , &( tex_size[ 2 ] )     );
      gl3::check_errors("check_texture_size: glGetTexLevelParameter 3");
      glGetTexLevelParameteriv( proxy_texture_target, 0, GL_TEXTURE_RED_SIZE      , &( channel_size[ 0 ] ) );
      gl3::check_errors("check_texture_size: glGetTexLevelParameter 4");
      glGetTexLevelParameteriv( proxy_texture_target, 0, GL_TEXTURE_GREEN_SIZE    , &( channel_size[ 1 ] ) );
      gl3::check_errors("check_texture_size: glGetTexLevelParameter 5");
      glGetTexLevelParameteriv( proxy_texture_target, 0, GL_TEXTURE_BLUE_SIZE     , &( channel_size[ 2 ] ) );
      gl3::check_errors("check_texture_size: glGetTexLevelParameter 6");
      glGetTexLevelParameteriv( proxy_texture_target, 0, GL_TEXTURE_ALPHA_SIZE    , &( channel_size[ 3 ] ) );
      gl3::check_errors("check_texture_size: glGetTexLevelParameter 7");

      // 2014-19-09 Kenny code review: legacy pixel formats? seems to generate errors in opengl core profile
      //glGetTexLevelParameteriv( proxy_texture_target, 0, GL_TEXTURE_LUMINANCE_SIZE, &( channel_size[ 4 ] ) );
      //gl3::check_errors("check_texture_size: glGetTexLevelParameter 8");
      //glGetTexLevelParameteriv( proxy_texture_target, 0, GL_TEXTURE_INTENSITY_SIZE, &( channel_size[ 5 ] ) );
      //gl3::check_errors("check_texture_size: glGetTexLevelParameter 9");

      int total_bits = channel_size[ 0 ] + channel_size[ 1 ] + channel_size[ 2 ] + channel_size[ 3 ] + channel_size[ 4 ] + channel_size[ 5 ];

      float bytes = static_cast< float >( ceil( total_bits / 8.0 ));

      int memory_size = static_cast<int>(tex_size[ 0 ] * tex_size[ 1 ] * tex_size[ 2 ] * bytes);

      return memory_size;
    }

  public:

    Texture3D()
    : m_texture_ID(0)
    , m_internal_format(0)
    , m_width( 0  )
    , m_height( 0 )
    , m_depth( 0  )
    , m_format(0)
    , m_type(0)
    , m_texture_target( 0 )
    {}

    /**
     * Texture3D Constructor.
     *
     * @param internal_format  The number of color components in the texture.
     *                         Must be 1, 2, 3, or 4, or one of the following
     *                         symbolic constants: GL_ALPHA, GL_ALPHA4, GL_ALPHA8,
     *                         GL_ALPHA12, GL_ALPHA16, GL_LUMINANCE, GL_LUMINANCE4,
     *                         GL_LUMINANCE8, GL_LUMINANCE12, GL_LUMINANCE16,
     *                         GL_LUMINANCE_ALPHA, GL_LUMINANCE4_ALPHA4, GL_LUMINANCE6_ALPHA2,
     *                         GL_LUMINANCE8_ALPHA8, GL_LUMINANCE12_ALPHA4, GL_LUMINANCE12_ALPHA12,
     *                         GL_LUMINANCE16_ALPHA16, GL_INTENSITY, GL_INTENSITY4, GL_INTENSITY8,
     *                         GL_INTENSITY12, GL_INTENSITY16, GL_R3_G3_B2, GL_RGB, GL_RGB4, GL_RGB5,
     *                         GL_RGB8, GL_RGB10, GL_RGB12, GL_RGB16, GL_RGBA, GL_RGBA2, GL_RGBA4,
     *                         GL_RGB5_A1, GL_RGBA8, GL_RGB10_A2, GL_RGBA12, or GL_RGBA16.
     * @param width            The width of the image.
     * @param height           The height of the image.
     * @param depth            The depth of the image.
     * @param format           The format of the pixel data. It can assume one of the symbolic values:
     *                         GL_COLOR_INDEX, GL_RED, GL_GREEN,GL_BLUE,GL_ALPHA,GL_RGB,GL_RGBA,GL_BGR_EXT,GL_BGR_EXT
     *                         GL_BGRA_EXT,GL_LUMINANCE,GL_LUMINANCE_ALPHA
     * @param type             The data type of the pixel data. The following symbolic values are accepted:
     *                         GL_UNSIGNED_BYTE, GL_BYTE, GL_BITMAP, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT,
     *                         GL_INT, and GL_FLOAT.
     * @param pixels           A pointer to the image data in memory.
     */
    Texture3D(
              int internal_format
              , int width
              , int height
              , int depth
              , int format
              , int type
              , const void * pixels
              )
    : m_texture_ID(0)
    , m_internal_format(internal_format)
    , m_width( width  )
    , m_height( height )
    , m_depth( depth  )
    , m_format(format)
    , m_type(type)
    , m_texture_target( GL_TEXTURE_3D )
    {
      gl3::check_errors("Texture3D");

      assert( tiny::is_power2( m_width ) || !"Texture3D(): Texture width was not a power of two"  );
      assert( tiny::is_power2( m_height) || !"Texture3D(): Texture height was not a power of two" );
      assert( tiny::is_power2( m_depth ) || !"Texture3D(): Texture depth was not a power of two"  );

      int const memory_size = check_texture_size(internal_format,m_width,m_height,m_depth,format,type);

      assert(memory_size>0 || !"Texture3D():  insuficient memory could not load 3D texture");

      {
        util::Log logging;

        logging << "Texture3D():  memory = " << memory_size << " bytes" << util::Log::newline();
      }

      create();
      gl3::check_errors("Texture3D: create");

      // 2014-19-09 Kenny code review: legacy profile? seems to generate errors in opengl core profile
      // glEnable(m_texture_target);
      // gl3::check_errors("Texture3D: glEnable");

      glBindTexture(m_texture_target, m_texture_ID);
      gl3::check_errors("Texture3D: glBindTexture");

      //--- Setup texture parameters
      glTexParameteri( m_texture_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
      gl3::check_errors("Texture3D: glTexParameter 1");
      glTexParameteri( m_texture_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
      gl3::check_errors("Texture3D: glTexParameter 2");
      glTexParameteri( m_texture_target, GL_TEXTURE_WRAP_R, GL_REPEAT);
      gl3::check_errors("Texture3D: glTexParameter 3");
      glTexParameteri( m_texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      gl3::check_errors("Texture3D: glTexParameter 4");
      glTexParameteri( m_texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      gl3::check_errors("Texture3D: glTexParameter 5");

      //--- Create Texture Object
      glTexImage3D(
                   m_texture_target
                   , 0
                   , internal_format
                   , m_width
                   , m_height
                   , m_depth
                   , 0
                   , format
                   , type
                   , pixels
                   );

      // 2014-19-09 Kenny code review: Hmm, not sure one always wants to use mip-mapping?
      glGenerateMipmap(GL_TEXTURE_3D);

      gl3::check_errors("Texture3D: glTexImage3D");
    }

    ~Texture3D()
    {
    }

  public:

    void clear()
    {
      if (m_texture_ID!=0)
      {
        glDeleteTextures(1, &m_texture_ID);
        gl3::check_errors("Texture3D: delete texture");
      }
      m_texture_ID = 0;
    }

    /**
     * Bind Texture.
     * This method makes sure that the texture target is enabled and the
     * texture object is bound.
     */
    void bind() const
    {
      // 2014-19-09 Kenny code review: legacy profile? seems to generate errors in opengl core profile
      //if( !glIsEnabled(m_texture_target) )
      //  glEnable(m_texture_target);
      glActiveTexture( GL_TEXTURE0 );


      glBindTexture(m_texture_target, m_texture_ID);
      gl3::check_errors("Texture3D::bind()  glBindTexture");
    }

    void unbind() const
    {
      glBindTexture(m_texture_target, 0);
    }

    /**
     * Load Texture From Memory.
     *
     * @param pixels   A contiguos memory location where texture
     *                 data should be loaded from.
     */
    template <typename T>
    void load(std::vector<T> const & pixels, unsigned int channels)
    {
      assert(pixels != 0 || !"Texture3D::load() - pixels are null");

      bind();

      m_format = external_format( channels );
      m_type   = external_type<T>();

      glTexSubImage3D(
                      m_texture_target         //GLenum target
                      , 0                      //GLint level
                      , 0                      //GLint x offset
                      , 0                      //GLint y offset
                      , 0                      //GLint z offset
                      , m_width                //GLsizei width
                      , m_height               //GLsizei height
                      , m_depth                //GLsizei depth
                      , m_format               //GLenum format
                      , m_type                 //GLenum type
                      , &pixels[0]             //const GLvoid *pixels
                      );
    }

    /**
     * Set Texture Wrapping Mode.
     *
     * @param flag   If true texture wrapping is set to GL_REPEAT otherwise it is set to GL_CLAMP.
     */
    void set_repeating(bool const & flag) const
    {
      bind();

      if(flag)
      {
        glTexParameteri( m_texture_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri( m_texture_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri( m_texture_target, GL_TEXTURE_WRAP_R, GL_REPEAT);
      }
      else
      {
        glTexParameteri( m_texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri( m_texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri( m_texture_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
      }
    }

  };

}//namespace gl3

//GL3_TEXTURE_3D_H
#endif
