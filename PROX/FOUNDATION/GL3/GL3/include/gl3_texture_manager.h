#ifndef GL3_TEXTURE_MANAGER_H
#define GL3_TEXTURE_MANAGER_H

#include <gl3.h>
#include <gl3_texture_3d.h>
#include <gl3_make_vol_texture.h>

#include <vector>

namespace gl3
{
  class TextureManager
  {
  protected:

    std::vector<Texture3D> m_textures;

  public:

    TextureManager(){}
    ~TextureManager(){}

  public:

    void clear()
    {
      for(unsigned int i = 0u; i < m_textures.size(); ++i)
      {
        m_textures[i].clear();
      }
      m_textures.clear();
    }

    void load(std::string const & texture_filename)
    {
      gl3::Texture3D texture = make_vol_texture( texture_filename );

      m_textures.push_back( texture );

    }

    Texture3D & get(unsigned int const & idx)
    {
      return m_textures[idx];
    }

    Texture3D const & get(unsigned int const & idx) const
    {
      return m_textures[idx];
    }

  };

}//namespace gl3

//GL3_TEXTURE_MANAGER_H
#endif
