#include <content_io_utils.h>
#include <content_io_base.h>

#include <sstream>
#include <stdexcept>  // needed for std::runtime_error

namespace content
{
  namespace details
  {
    
    bool read_collection (TiXmlElement const * tag, std::string const & item, bool (*dispatch)(TiXmlElement const *, Cache &), Cache & data )
    {
      TiXmlElement const * child = tag->FirstChildElement ( item );
      for ( ;child; child = child->NextSiblingElement ( item ) )
      {
        if ( !(*dispatch)( child, data ) )
          return false;
      }
      return true;
    }
    
    bool read_bool(TiXmlElement const * element, std::string const & attrib_name, bool const & default_value, bool const & mandatory)
    {
      char const * rawptr = element->Attribute ( attrib_name );
      std::string text = rawptr ? rawptr : "";
      if (text.size()  == 0 )
      {
        if(mandatory)
          throw std::runtime_error( attrib_name + " on " + element->Value() + " did not exist" );
        return default_value;
      }
      
      if(! (text.compare("true")==0 || text.compare("false")==0) )
      {
        if(mandatory)
          throw std::runtime_error( attrib_name + " on " + element->Value() + " was  set to " + text );
        return default_value;
      }
      if(text.compare("true")==0)
        return true;
      return false;      
    }
    
    Transform read_transform(TiXmlElement const * element, Cache & data, Transform const & default_value)
    {
      Transform value = default_value;
      
      std::string ref = read_string(element,"ref", false);
      if(ref.size() != 0)
      {
        value = data.get_transform_from_name(ref);
      }
      
      value.m_x  = read_float(element, "x",  value.m_x,  false );
      value.m_y  = read_float(element, "y",  value.m_y,  false );
      value.m_z  = read_float(element, "z",  value.m_z,  false );
      value.m_qs = read_float(element, "qs", value.m_qs, false );
      value.m_qx = read_float(element, "qx", value.m_qx, false );
      value.m_qy = read_float(element, "qy", value.m_qy, false );
      value.m_qy = read_float(element, "qz", value.m_qz, false );
      return value;      
    }
    
    Motion read_motion(TiXmlElement const * element, Cache & data, Motion const & default_value)
    {
      Motion value = default_value;
      
      std::string ref = read_string(element,"ref", false);
      if(ref.size() != 0)
      {
        value = data.get_motion_from_name(ref);
      }
      
      value.m_vx = read_float(element, "vx", value.m_vx, false );
      value.m_vy = read_float(element, "vy", value.m_vy, false );
      value.m_vz = read_float(element, "vz", value.m_vz, false );
      value.m_wx = read_float(element, "wx", value.m_wx, false );
      value.m_wy = read_float(element, "wy", value.m_wy, false );
      value.m_wy = read_float(element, "wz", value.m_wz, false );
      return value;      
    }
    
    
  } // namespace details
  
}// namespace content

