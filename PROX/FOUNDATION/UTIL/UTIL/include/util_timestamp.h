#ifndef UTIL_TIMESTAMP_H
#define UTIL_TIMESTAMP_H

#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>

namespace util
{

  inline std::string timestamp()
  {

    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );

    std::stringstream ss;

    ss << (now->tm_year + 1900)
       << "-"
       << std::setw(2)
       << std::setfill('0')
       << (now->tm_mon + 1)
       << "-"
       <<  now->tm_mday
       << " "
       << std::setw(2)
       << std::setfill('0')
       << (now->tm_hour)
       << ":"
       << std::setw(2)
       << std::setfill('0')
       << (now->tm_min )
       << ":"
       << std::setw(2)
       << std::setfill('0')
       << (now->tm_sec);

    return ss.str();
  }

} //namespace util

// UTIL_TIMESTAMP_H
#endif
