#ifndef UTIL_FILE_LOCATOR_H
#define UTIL_FILE_LOCATOR_H

#include <util_log.h>

#include <boost/filesystem.hpp>

#include <string>
#include <vector>
#include <stdexcept>

namespace util
{

  class FileLocator
  {
  protected:


    std::vector<std::string>  m_search_paths;
    std::string                        m_filename;

  public:

    void add_search_path(std::string const & search_path)
    {
      m_search_paths.push_back(search_path);
    }

    void set_filename(std::string const & filename)
    {
      m_filename = filename;
    }

  public:

    FileLocator()
    : m_search_paths()
    , m_filename("")
    {}

    FileLocator(std::string const & filename)
    {
      set_filename(filename);
    }

    FileLocator(std::string const & search_path,std::string const & filename)
    {
      add_search_path(search_path);
      set_filename(filename);
    }


  public:

    std::string get_filename() const
    {
      util::Log log;

      {
        std::string                    filename      = m_filename;
        boost::filesystem::path        filename_path = boost::filesystem::path( filename );

        if( boost::filesystem::exists( filename_path ) && boost::filesystem::is_regular_file( filename_path )   )
          return filename;

      }

      std::vector<std::string>::const_iterator search_path = m_search_paths.begin();
      std::vector<std::string>::const_iterator end         = m_search_paths.end();

      for (; search_path != end; ++search_path)
      {
        boost::filesystem::path parent_path   = boost::filesystem::path( (*search_path) );
        boost::filesystem::path filename_path = boost::filesystem::path( m_filename );
        boost::filesystem::path full_path     = parent_path / filename_path;

        if( boost::filesystem::exists( full_path ) && boost::filesystem::is_regular_file( full_path )   )
          return full_path.string();
      }

      log << "FileLocator::get_filename(): \"" << m_filename << "\" could not be located" << Log::newline();

      throw std::logic_error ("FileLocator::get_filename(): \"" + m_filename + "\" could not be located" );

      return "";
    }

  };

  inline std::string get_full_filename( std::string const & parent_folder, std::string const & filename )
  {
    return FileLocator( parent_folder, filename ).get_filename();
  }

}//namespace util

// UTIL_FILE_LOCATOR_H
#endif
