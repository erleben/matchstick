#ifndef UTIL_LOG_H
#define UTIL_LOG_H

#include <string>
#include <iostream>
#include <fstream>

namespace util
{

  class LogInfo
  {
  public:

    static bool & console()
    {
      static bool value = true;
      return value;
    }

    static bool & on()
    {
      static bool value = true;
      return value;
    }

    static std::ofstream & stream()
    {
      static std::ofstream value;
      return value;
    }

    static std::string & filename()
    {
      static std::string value = "log.txt";
      return value;
    }

    static bool & first_time()
    {
      static bool value = true;
      return value;
    }

  };


  class Log
  {
  public:

    static std::string tab()     { return "\t"; };
    static std::string newline() { return "\n"; };

    void flush()
    {
      if(LogInfo::on())
      {
        if(LogInfo::console())
        {
          std::cout.flush();
        }
        else
        {
          LogInfo::stream().flush();
        }
      }
    }
  };

  template<typename T>
  inline Log & operator<<(Log & log, T const & data)
  {
    if(LogInfo::on())
    {
      if(LogInfo::console())
      {
        std::cout << data;
      }
      else
      {
        if(LogInfo::first_time())
        {
          LogInfo::stream().open(LogInfo::filename().c_str(), std::ofstream::out );
          LogInfo::first_time() = false;
        }
        else
        {
          LogInfo::stream().open(LogInfo::filename().c_str(), std::ofstream::out | std::ofstream::app);
        }
        LogInfo::stream() << data;
        LogInfo::stream().flush();
        LogInfo::stream().close();
      }
    }
    
    return log;
  }
  
} //namespace util

// UTIL_LOG_H
#endif
