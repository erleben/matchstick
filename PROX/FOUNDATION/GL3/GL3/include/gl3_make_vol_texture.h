#ifndef GL3_MAKE_VOL_TEXTURE_H
#define GL3_MAKE_VOL_TEXTURE_H

#include <gl3.h>

#include <gl3_texture_types.h>
#include <gl3_texture_3d.h>
#include <gl3_vol_read.h>

#include <util_log.h>

#include <string>
#include <vector>

namespace gl3
{
  Texture3D make_vol_texture(std::string const & filename)
  {
    VolumeHeader header;
    std::vector<unsigned char> data;

    bool const read = vol_read(filename, header, data);

    if(!read)
    {
      util::Log logging;

      logging << "make_vol_texture(): could not read " << filename << util::Log::newline();

      return Texture3D();
    }

    return Texture3D(
                     internal_format<unsigned char>(3)
                     , header.volSize
                     , header.volSize
                     , header.volSize
                     , external_format(3)
                     , external_type<unsigned char>()
                     , &data[0]
                     );
  }
}

// GL3_MAKE_VOL_TEXTURE_H
#endif
