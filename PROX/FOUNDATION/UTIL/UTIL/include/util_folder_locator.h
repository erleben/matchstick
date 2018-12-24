#ifndef UTIL_FOLDER_LOCATOR_H
#define UTIL_FOLDER_LOCATOR_H

#include <util_log.h>

#include <boost/filesystem.hpp>

#include <string>
#include <vector>
#include <stdexcept>

namespace util
{

  class FolderLocator
  {
  protected:


    std::vector<std::string>  m_search_paths;
    std::string               m_folder_name;

  public:

    void add_search_path(std::string const & search_path)
    {
      m_search_paths.push_back(search_path);
    }

    void set_folder_name(std::string const & folder_name)
    {
      m_folder_name = folder_name;
    }

  public:

    FolderLocator()
    : m_search_paths()
    , m_folder_name("")
    {}

    FolderLocator(std::string const & folder_name)
    {
      set_folder_name(folder_name);
    }

    FolderLocator(std::string const & search_path,std::string const & folder_name)
    {
      add_search_path(search_path);
      set_folder_name(folder_name);
    }


  public:

    std::string get_folder_name() const
    {
      util::Log log;

      {
        std::string                    folder_name = m_folder_name;
        boost::filesystem::path        folder_path = boost::filesystem::path( folder_name );

        if( boost::filesystem::exists( folder_path ) && boost::filesystem::is_directory( folder_path )   )
          return folder_name;
      }

      std::vector<std::string>::const_iterator search_path = m_search_paths.begin();
      std::vector<std::string>::const_iterator end         = m_search_paths.end();

      for (; search_path != end; ++search_path)
      {
        boost::filesystem::path parent_path   = boost::filesystem::path( (*search_path) );
        boost::filesystem::path folder_path   = boost::filesystem::path( m_folder_name );
        boost::filesystem::path full_path     = parent_path / folder_path;

        if( boost::filesystem::exists( full_path ) && boost::filesystem::is_directory( full_path )   )
          return full_path.string();
      }

      log << "FolderLocator::get_folder_name(): \"" << m_folder_name << "\" could not be located" << Log::newline();

      throw std::logic_error ("FolderLocator::get_folder_name(): \"" + m_folder_name + " \"could not be located" );

      return "";
    }

  };

  inline std::string get_full_folder_name( std::string const & parent_folder, std::string const & folder_name )
  {
    return FolderLocator( parent_folder, folder_name ).get_folder_name();
  }

}//namespace util

// UTIL_FOLDER_LOCATOR_H
#endif
