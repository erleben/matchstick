#include <util_string_helper.h>

#include <cassert>
#include <sstream>
#include <cstdlib>

namespace util
{
  
  template<typename T>
  T to_value(std::string const & txt)
  {
    assert( txt.size() > 0u || !"to_value(): empty string");
    
    std::istringstream str_stream( txt );
    
    T value;
    str_stream >> value;
    
    return value;
  }
  
  template<typename T>
  std::string to_string(T const & value)
  {
    std::ostringstream str_stream;
    
    str_stream.precision(30);
    
    str_stream << value;
    
    return str_stream.str();
  }

  template<>
  bool to_value<bool>(std::string const & txt)
  {
    if(txt.empty())
      return false;

    if (txt.compare("true") == 0)
      return true;

    if (txt.compare("TRUE") == 0)
      return true;

    if (txt.compare("1") == 0)
      return true;

    if (txt.compare("yes") == 0)
      return true;

    if (txt.compare("Yes") == 0)
      return true;

    return false;
  }

  template
  float to_value<float>(std::string const & txt);
  
  template
  double to_value<double>(std::string const & txt);
  
  template
  size_t to_value<size_t>(std::string const & txt);

  template
  unsigned int to_value<unsigned int>(std::string const & txt);

  template
  int to_value<int>(std::string const & txt);

  template<>
  std::string to_string<bool>(bool const & value)
  {
    return (value ? "true" : "false");
  }

  template
  std::string to_string<float>(float const & value);
  
  template
  std::string to_string<double>(double const & value);
  
  template
  std::string to_string<size_t>(size_t const & value);

  template
  std::string to_string<unsigned int>(unsigned int const & value);

  template
  std::string to_string<int>(int const & value);

} // namespace util
