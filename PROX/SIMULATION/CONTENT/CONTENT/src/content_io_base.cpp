#include <content_io_base.h>

#include <tiny_is_finite.h>
#include <tiny_is_number.h>

#include <cassert>
#include <stdexcept>  // needed for std::runtime_error
#include <sstream>

namespace content
{
  namespace details
  {
    
    std::string read_string(TiXmlElement const * element, std::string const & attrib_name, bool const & mandatory )
    {
      char const * rawptr = element->Attribute ( attrib_name );
      std::string text = rawptr ? rawptr : "";
      if (text.size() == 0 && mandatory)
      {
        throw std::runtime_error( attrib_name + " on " + element->Value() + " did not exist" );
      }
      return text;
    }
    
    float read_float(TiXmlElement const * element, std::string const & attrib_name, float const & default_value, bool const & mandatory)
    {
      float value = default_value;
      char const * rawptr = element->Attribute ( attrib_name );
      std::string text = rawptr ? rawptr : "";
      if (text.size()  == 0 )
      {
        if(mandatory)
          throw std::runtime_error( attrib_name + " on " + element->Value() + " did not exist" );
        return default_value;
      }
      std::istringstream ss ( text );
      ss >> value;
      
      assert( is_number( value ) || !"read_float: nan encountered");          
      assert( is_finite( value ) || !"read_float: inf encountered");          
      
      return value;      
    }
    
    int read_integer(TiXmlElement const * element, std::string const & attrib_name, int const & default_value, bool const & mandatory)
    {
      int value = default_value;
      char const * rawptr = element->Attribute ( attrib_name );
      std::string text = rawptr ? rawptr : "";
      if (text.size()  == 0 )
      {
        if(mandatory)
          throw std::runtime_error( attrib_name + " on " + element->Value() + " did not exist" );
        return default_value;
      }
      std::istringstream ss ( text );
      ss >> value;
      
      assert( is_number( value ) || !"read_integer: nan encountered");          
      assert( is_finite( value ) || !"read_integer: inf encountered");          
      
      return value;      
    }
    
    TiXmlElement const * get_singleton_child( TiXmlNode const * parent, std::string const & name, bool const & mandatory)
    {
      TiXmlElement const * child = parent->FirstChildElement ( name );
      
      if(!child)
      {
        if(mandatory)
          throw std::runtime_error( name + " on " + parent->Value() + " did not exist" );
        return 0;
      }
      if ( child->NextSiblingElement ( name ) )
      {
        throw std::runtime_error( name + " on " + parent->Value() + " was not a singleton" );
        return 0;
      }
      // Everything okay, child existed and it was the only child
      return child;
    }
    
    
  } // namespace details
  
  
}// namespace content
