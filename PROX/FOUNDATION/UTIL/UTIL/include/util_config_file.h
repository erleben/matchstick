#ifndef UTIL_CONFIG_FILE_H
#define UTIL_CONFIG_FILE_H

#include <util_string_helper.h>
#include <util_log.h>

#include <boost/filesystem.hpp>

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <stdexcept>

namespace util
{
  /**
   * Reads in a simple configuration file with following format
   *
   * A single value is given as
   *
   *   name = value
   *
   * A list of values are given as
   *
   *   name = value1 value2 value3
   *
   * Comments starts at # and goes to the end of the line like this
   *
   *  # I am a comment
   *  name = value # the rest of this line is a comment
   *
   * A value can not have white space deliminators. Hence "too big"
   * will be considered 2 values. Whereas "filename.txt" is considered
   * one string.
   *
   * All names and values are case-sensitive.
   */
  class ConfigFile
  {
  protected:

    std::map< std::string, std::vector<std::string> > m_values;

  public:

    std::vector<std::string> get_values(std::string const & name) const
    {
      return m_values.at(name);
    }

    std::string get_value(std::string const & name, std::string const & default_value) const
    {
      if(m_values.find(name) != m_values.end())
        return m_values.at(name)[0];
      return default_value;
    }

    std::string get_value(std::string const & name) const
    {
      return get_value(name,"");
    }

    bool exist(std::string const & name) const
    {
      return !(m_values.find(name) == m_values.end());
    }

  public:

    ConfigFile()
    {
    }

    virtual ~ConfigFile(){}

  public:

    void clear()
    {
      m_values.clear();
    }

    bool load(std::string const &  filename)
    {
      util::Log log;

     log << "ConfigFile::load(" << filename << ")" << Log::newline();

     if( filename.empty() )
      {
        throw std::invalid_argument ("ConfigFile::load() empty filename");

        return false;
      }

      std::ifstream file;
      file.open(filename.c_str());

      if (!file.good())
      {
        log <<  "ConfigFile::load() file not found: " << filename <<  Log::newline();

        throw std::invalid_argument ("ConfigFile::load() file not found: " + filename);
        return false;
      }

      std::string const whitespaces(" \t\f\v\n\r");

      while (!file.eof())
      {
        std::string line;

        std::getline(file, line);

        //--- First remove all comments
        size_t const comment_pos = line.find_first_of("#");
        if(comment_pos != std::string::npos)
        {
          line = line.substr(0,comment_pos);
        }

        if(line.empty())
          continue;

        //--- Strip whitespaces and convert into an array of string tokens
        std::vector<std::string> tokens;

        size_t start_pos  = 0;
        size_t end_pos    = 0;

        while (end_pos != std::string::npos)
        {
          start_pos  = line.find_first_not_of(whitespaces, end_pos);
          end_pos    = line.find_first_of(whitespaces, start_pos);

          if (start_pos != std::string::npos)
          {
            std::string token = line.substr(start_pos, end_pos - start_pos);
            tokens.push_back(token);
          }
        }

        if(tokens.size()>0)
        {
          //--- Examine array of string tokens to make sure they have proper format
          if(tokens.size() < 3)
          {
            log << "ConfigFile::load() incorrect syntax in line : " << line << Log::newline();

            throw std::logic_error ("ConfigFile::load() incorrect syntax in line : " + line);

            return false;
          }
          if (tokens[1].compare("=")!= 0)
          {
            log << "ConfigFile::load() incorrect syntax in line : " << line << Log::newline();

            throw std::logic_error ("ConfigFile::load() incorrect syntax in line : " + line);

            return false;
          }

          //--- test if we got an import of another cfg file
          if( tokens[0].compare("import") == 0 )
          {
            using namespace boost::filesystem;

            std::string import_filename      = tokens[2];
            path        filename_path        = path(        filename );
            path        import_filename_path = path( import_filename );
            path        pwd_path             = current_path();

            // Test if the config file actual exists
            if(! exists( import_filename_path ) )
            {
              log << "ConfigFile::load(): Failed to locate import file = " << import_filename << " from " << line << Log::newline();
              log << "ConfigFile::load(): Current path is = " << pwd_path.string() << line << Log::newline();

              // If config file did not exist then try to locate it using the path of current config file
              import_filename      = filename_path.parent_path().string() + tokens[2];
              import_filename_path = path( import_filename );

              // Test if the config file is located at this location
              if(! exists( import_filename_path ) )
              {
                log << "ConfigFile::load(): Failed to locate import file = " << import_filename << " from " << line << Log::newline();

                throw std::logic_error ("ConfigFile::load(): Failed to locate " + import_filename + " from " + line );

                return false;
              }

            }

            if( ! this->load( import_filename ) )
            {
              log << "ConfigFile::load() could not import  " << import_filename << " from " << line << Log::newline();

              throw std::logic_error ("ConfigFile::load() could not import  " + import_filename + " from " + line );

              return false;
            }
          }

          //--- Now we can store the config setting
          m_values[tokens[0]] = std::vector<std::string>( tokens.begin() + 2, tokens.end() );
        }
      }

      return true;
    }

  };
  
}//namespace util

// UTIL_CONFIG_FILE_H
#endif
