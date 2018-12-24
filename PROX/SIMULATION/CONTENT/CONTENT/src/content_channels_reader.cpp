#include <content_io_base.h>
#include <content_channels.h>

#include <tinyxml.h>

#include <cassert>
#include <stdexcept>  // needed for std::runtime_error
#include <sstream>
#include <cmath>      // needed for std::sqrt

namespace content
{
  namespace details
  {
    /**
     * The Data  Cache Class.
     * Used to pass information on when parsing XML document.
     */
    class ChannelCache
    {
    public:
      
      ChannelStorage * m_storage;      
      size_t           m_idx;
      
    public:
      
      ChannelCache()
      : m_storage(0)
      , m_idx(0u)
      {}
      
    };
    
    bool read_collection (TiXmlElement const * tag, std::string const & item, bool (*dispatch)(TiXmlElement const *, ChannelCache &), ChannelCache & data )
    {
      TiXmlElement const * child = tag->FirstChildElement ( item );
      for ( ;child; child = child->NextSiblingElement ( item ) )
      {
        if ( !(*dispatch)( child, data ) )
          return false;
      }
      return true;
    }
    
    bool read_key( TiXmlElement const * tag, ChannelCache & data  )
    {
      using std::sqrt;
      
      assert(tag || !"key tag was missing?");
      
      float const time  = read_float( tag, "time", 0.0f, true  );
      float const x     = read_float( tag, "x",    0.0f, false );
      float const y     = read_float( tag, "y",    0.0f, false );
      float const z     = read_float( tag, "z",    0.0f, false );
      float const qs    = read_float( tag, "qs",   0.0f, false );
      float const qx    = read_float( tag, "qx",   0.0f, false );
      float const qy    = read_float( tag, "qy",   0.0f, false );
      float const qz    = read_float( tag, "qz",   1.0f, false );
      
      // Make sure quaternion is a unit quaternion
      float const qnorm = sqrt(qs*qs + qx*qx + qy*qy+ qz*qz);
      assert(  qnorm > 0.0f || !"read_key: quaternion was bad");
      float const norm_qs = qs / qnorm;
      float const norm_qx = qx / qnorm;
      float const norm_qy = qy / qnorm;
      float const norm_qz = qz / qnorm;
      
      size_t const channel_idx = data.m_idx;
      size_t const key_idx     = data.m_storage->create_key( channel_idx, time);
      
      data.m_storage->set_key_position( channel_idx, key_idx, x, y, z);
      data.m_storage->set_key_orientation( channel_idx, key_idx, norm_qs, norm_qx, norm_qy, norm_qz);
      
      return true;
    }
    
    bool read_channel( TiXmlElement const * tag, ChannelCache & data  )
    {
      if(!tag)
        return true;
      
      std::string const name = read_string(  tag, "name", false );
      size_t      const   id = read_integer( tag,   "id", 0, false );
      
      size_t const channel_idx = data.m_storage->create_channel( id, name );
      data.m_idx = channel_idx;
      
      if( !read_collection( tag, "key", &read_key, data  ) ) 
        return false;
      
      return true;
    }
    
  } // namespace details
  
  
  bool xml_read( std::string const & filename, ChannelStorage &  storage )
  {    
#ifdef TIXML_USE_STL
    TiXmlDocument xml_document ( filename );
#else
    TiXmlDocument xml_document ( filename.c_str() );
#endif
    
    if ( !xml_document.LoadFile() )
    {
      throw std::invalid_argument("xml_read(...): Error " + filename  + " not found!");
      return false;
    }
    TiXmlHandle document_tag ( &xml_document );
    
    details::ChannelCache data;
    data.m_storage = &storage;
    
    TiXmlElement const * channels_tag = details::get_singleton_child( document_tag.Node(), "channels", true );
    if ( !read_collection( channels_tag, "channel", &(details::read_channel), data  ) ) 
      return false;
    
    return true;
  }
  
}// namespace content
