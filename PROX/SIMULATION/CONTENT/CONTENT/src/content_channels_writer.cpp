#include <content_channels.h>

#include <tinyxml.h>

namespace content
{
  namespace details
  {
    
    bool make_doc(TiXmlDocument & doc, ChannelStorage const & storage)
    {      
      TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "UTF-8", "" );
      TiXmlElement * channels = new TiXmlElement( "channels" );
      doc.LinkEndChild(decl);
      doc.LinkEndChild(channels);
      
      size_t N = storage.get_number_of_channels();
      
      for(size_t channel_idx = 0;channel_idx < N; ++channel_idx)
      {
        size_t      const   id  = storage.get_channel_id( channel_idx );
        std::string const name  = storage.get_channel_name( channel_idx );
        
        
        TiXmlElement * channel = new TiXmlElement( "channel" );
        channel->SetAttribute("id", id );
        channel->SetAttribute("name", name );
        channels->LinkEndChild(channel);
        
        size_t K = storage.get_number_of_keys(channel_idx);
        
        for(size_t key_idx = 0;key_idx<K;++key_idx)
        {
          TiXmlElement * key = new TiXmlElement( "key" );
          
          float time = storage.get_key_time( channel_idx, key_idx );
          key->SetDoubleAttribute("time", time );
          
          float x = 0.0f;
          float y = 0.0f;
          float z = 0.0f;
          storage.get_key_position( channel_idx, key_idx, x, y, z );
          
          float qs = 0.0f;
          float qx = 0.0f;
          float qy = 0.0f;
          float qz = 1.0f;
          storage.get_key_orientation( channel_idx, key_idx, qs, qx, qy, qz );
          
          key->SetDoubleAttribute("x", x);
          key->SetDoubleAttribute("y", y);
          key->SetDoubleAttribute("z", z);
          key->SetDoubleAttribute("qs", qs);
          key->SetDoubleAttribute("qx", qx);
          key->SetDoubleAttribute("qy", qy);
          key->SetDoubleAttribute("qz", qz);
          
          channel->LinkEndChild(key);
        }            
      }
      return true;
    }
    
  }// namespace details
  
  bool xml_write( std::string const & filename, ChannelStorage const & storage )
  {
    // build document
    TiXmlDocument doc;
    
    if (! content::details::make_doc(doc, storage) )
      return false;
    
    // write the document
#ifdef TIXML_USE_STL
    doc.SaveFile(filename);
#else
    doc.SaveFile(filename.c_str());
#endif
    
    return true;
  }
  
}// namespace content
