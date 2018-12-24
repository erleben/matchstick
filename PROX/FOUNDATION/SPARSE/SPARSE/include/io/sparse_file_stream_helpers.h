#ifndef SPARSE_FILE_STREAM_HELPERS_H
#define SPARSE_FILE_STREAM_HELPERS_H

#include <stdexcept>  // for std::logic_error

#include <fstream>

namespace sparse
{
  namespace io
  {

    /**
     * @tparam FS   The file stream type.
     */
    template <typename FS>
    inline void open_file_stream(std::string const & file_path, FS & file)
    {

      file.open( file_path.c_str() );
      if (!file.is_open() || file.bad())
      {
        // 2009-07-01 Kenny: Why an exception?
        throw std::logic_error("could not open the specified file");
      }
    }
    
    /**
     * @tparam FS   The file stream type.
     */
    template <typename FS>
    inline void close_file_stream(FS & file)
    {
      file.flush();
      file.close();
    }
    
  } // namespace io
}   // namespace sparse

// SPARSE_FILE_STREAM_HELPERS_H
#endif 
