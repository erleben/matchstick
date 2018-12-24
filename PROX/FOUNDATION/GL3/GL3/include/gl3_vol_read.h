#ifndef GL3_VOL_READ_H
#define GL3_VOL_READ_H

#include <util_log.h>

#include <cstdio>
#include <cassert>
#include <vector>

namespace gl3
{

  /**
   *
   * The first 4096 bytes of the .vol files contain the header and zero padding to fill the 4096 bytes up. Then follows an array of volSize*volSize*volSize*numChannels*bytesPerChannel bytes containing the actual volume data.
   *
   * The header elements are:
   * magic: should be "VOLU"
   * version: should be 4
   * texName: the name of the texture
   * wrap: is the texture tileable?
   * volSize: side length of the teture
   * numChannels: e.g. 3 for RGB
   * bytesPerChannel: 1 for bytes, 4 for float
   */
  struct VolumeHeader
  {
    char magic[4];
    int  version;
    char texName[256];
    bool wrap;
    int  volSize;
    int  numChannels;
    int  bytesPerChannel;
  };

  inline bool vol_read(std::string const & filename, VolumeHeader & header, std::vector<unsigned char> & data)
  {
    FILE *stream;
    if((stream = fopen( filename.c_str(), "rb" )) == NULL )
    {
      util::Log logging;

      logging << "vol_read(): Unable to open file" << filename << util::Log::newline();
      return false;
    }

    char buffer[4096];
    fread(buffer, 4096, 1, stream);

    header = *((VolumeHeader *) buffer);

    if ((header.magic[0] != 'V') || (header.magic[1] != 'O') || (header.magic[2] != 'L') || (header.magic[3] != 'U'))
    {
      util::Log logging;

      logging << "vol_read(): bad header: invalid magic in "<< filename << util::Log::newline();

      return false;
    }
    if (header.version != 4)
    {
      util::Log logging;

      logging << "vol_read():bad header: version != 4 in "<< filename << util::Log::newline();

      return false;
    }
    if (header.bytesPerChannel != 1)
    {
      util::Log logging;

      logging << "vol_read():bad header: only byte textures supported in "<< filename << util::Log::newline();

      return false;
    }

    int const bytes = header.volSize*header.volSize*header.volSize*header.numChannels;

    assert(bytes>0 || !"vol_read() bad byte count -- internal error");
    data.resize( bytes );

    fread( &data[0], bytes, 1, stream);

    fclose(stream);

    return true;
  }

}// namespace gl3

// GL3_VOL_READ_H
#endif
