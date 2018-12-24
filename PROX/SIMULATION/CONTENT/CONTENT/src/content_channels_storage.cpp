#include <content_channels.h>

namespace content
{  
  //--------------------------------------------------------------------------------		
  void ChannelStorage::clear()
  {
    m_channels.clear();
  }
  //--------------------------------------------------------------------------------						
  size_t ChannelStorage::create_channel( size_t const & id, std::string const & name )
  {
    Channel C;
    
    C.m_id   = id;
    C.m_name = name;
    
    m_channels.push_back(C);
    return m_channels.size()-1u;
  }
  //--------------------------------------------------------------------------------				
  size_t ChannelStorage::create_key( size_t const & channel_idx, float const & time )
  {
    KeyTick K;
    K.m_time = time;
    m_channels[channel_idx].m_keys.push_back(K);
    return m_channels[channel_idx].m_keys.size() - 1u;
  }
  //--------------------------------------------------------------------------------						
  void ChannelStorage::set_key_position( size_t const & channel_idx, size_t const & key_idx, float const & x, float const & y, float const & z )
  {
    m_channels[channel_idx].m_keys[key_idx].m_x = x;
    m_channels[channel_idx].m_keys[key_idx].m_y = y;
    m_channels[channel_idx].m_keys[key_idx].m_z = z;
  }
  //--------------------------------------------------------------------------------				
  void ChannelStorage::set_key_orientation( size_t const & channel_idx, size_t const & key_idx,  float const & qs, float const & qx, float const & qy, float const & qz )
  {
    m_channels[channel_idx].m_keys[key_idx].m_qs = qs;
    m_channels[channel_idx].m_keys[key_idx].m_qx = qx;
    m_channels[channel_idx].m_keys[key_idx].m_qy = qy;
    m_channels[channel_idx].m_keys[key_idx].m_qz = qz;
  }
  //--------------------------------------------------------------------------------		
  size_t ChannelStorage::get_number_of_channels() const
  {
    return m_channels.size();
  }
  //--------------------------------------------------------------------------------		
  size_t ChannelStorage::get_channel_id( size_t const & channel_index ) const
  {
    return m_channels[channel_index].m_id;
  }
  //--------------------------------------------------------------------------------		
  std::string const & ChannelStorage::get_channel_name( size_t const & channel_index ) const
  {
    return m_channels[channel_index].m_name;
  }
  //--------------------------------------------------------------------------------		
  size_t ChannelStorage::get_number_of_keys(size_t const & channel_index) const
  {
    return m_channels[channel_index].m_keys.size();
  }
  //--------------------------------------------------------------------------------		
  float ChannelStorage::get_key_time( size_t const & channel_index, size_t const & key_index ) const
  {
    return m_channels[channel_index].m_keys[key_index].m_time;
  }
  //--------------------------------------------------------------------------------				
  void ChannelStorage::get_key_position( size_t const &  channel_index
                                        , size_t const & key_index
                                        , float & x
                                        , float & y
                                        , float & z 
                                        ) const
  {
    x = m_channels[channel_index].m_keys[key_index].m_x;
    y = m_channels[channel_index].m_keys[key_index].m_y;
    z = m_channels[channel_index].m_keys[key_index].m_z;
  }
  //--------------------------------------------------------------------------------		
  void ChannelStorage::get_key_orientation( size_t const &  channel_index
                                           , size_t const & key_index
                                           , float & qs
                                           , float & qx
                                           , float & qy 
                                           , float & qz 
                                           ) const
  {
    qs = m_channels[channel_index].m_keys[key_index].m_qs;
    qx = m_channels[channel_index].m_keys[key_index].m_qx;
    qy = m_channels[channel_index].m_keys[key_index].m_qy;
    qz = m_channels[channel_index].m_keys[key_index].m_qz;
  }
  //--------------------------------------------------------------------------------		
  
}// namespace content
