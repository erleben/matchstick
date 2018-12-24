#ifndef CONTENT_CHANNELS_H
#define CONTENT_CHANNELS_H

#include <string>
#include <vector>

namespace content
{
  
  /**
   * Channel Device Class.
   * This class provides functionality for recording and retrieving motion channel data.
   */
	class ChannelStorage
	{
  protected:
		
    /**
     * A key tick is basically a time sample of a 6 DOF entity.
     */
		class KeyTick
		{
		public:
      
			float m_time;         ///< The time of the key tick.
			float m_x;            ///< The x coordinate.
			float m_y;            ///< The y coordinate.
			float m_z;            ///< The z coordinate.
			float m_qs;           ///< Rotation represented as an unit quaternion.
			float m_qx;        
			float m_qy;        
			float m_qz;        
      
    public:
      
      KeyTick()
      : m_time(0.0f)
      , m_x(0.0f)
      , m_y(0.0f)
      , m_z(0.0f)
      , m_qs(0.0f)
      , m_qx(0.0f)
      , m_qy(0.0f)
      , m_qz(1.0f)
      {}
      
		};
		
    /**
     * A motion channel is a collection of time samples.
     */
		class Channel
		{
		public:
      
			size_t                  m_id;        ///< The identifier of this channel.
      std::string             m_name;      ///< The name of this channel.
			std::vector< KeyTick >  m_keys;      ///< All key ticks of this channel.
      
    public:
      
      Channel()
      : m_id(0)
      , m_name("")
      , m_keys()
      {}
      
		};
		
  protected:
    
		std::vector<Channel> m_channels;   ///< A storage of channel data
    
	public:
		
		ChannelStorage()
    : m_channels()
    {}
    
		~ChannelStorage(){}
		
	public:
		
		void clear();
    
    /**
     * Create a channel with a specified id and name.
     *
     * @param id    A identifier number given by end user.
     * @oaram name  A unique name given by end user.
     *
     * @warning     It is end users responsibility to make sure
     *              that id and name is unique. The channel storage
     *              provides no verfication for this.
     *
     * @return      An unique channel index representing the new channel.
     */
		size_t create_channel( size_t const & id, std::string const & name );
    
    /**
     * Create a new key tick in a channel.
     *
     * @param channel_idx      The index of the channel.
     * @param time             The time-stamp of the key tick.
     *
     * @return                  An unique index value within this
     *                          channel which represents the new key tick.
     */
		size_t create_key( size_t const & channel_idx, float const & time );
    
    /**
     * Set Position of Key Tick.
     *
     * @param channel_idx    The index of the channel.
     * @param key_idx        The index of the key tick.
     * @param x              The x-coordinate.
     * @param y              The y-coordinate.
     * @param z              The z-coordinate.
     */
		void set_key_position(  size_t const & channel_idx
                          , size_t const & key_idx
                          , float const & x
                          , float const & y
                          , float const & z
                          );
		
    /**
     * Set Orientation of Key Tick.
     *
     * @param channel_idx    The index of the channel.
     * @param key_idx        The index of the key tick.
     * @param qs             The scalar component of the unit quaterion.
     * @param qx             The imaginary x component of the unit quaterion.
     * @param qy             The imaginary y component of the unit quaterion.
     * @param qz             The imaginary z component of the unit quaterion.
     */
		void set_key_orientation( size_t const & channel_idx
                             , size_t const & key_idx
                             , float const & qs
                             , float const & qx
                             , float const & qy
                             , float const & qz 
                             );
    
    /**
     * Get Number of Channels.
     * (Motion) channels are used to keep a baked simulation of for
     * instance rigid bodies. That is basically a temporal sampling of the
     * position and orientation of a rigid body. However, channels could
     * be generalized to be anything that is temporal sampled during a
     * simulation.
     *
     * @return The number of channels.
     */
		size_t get_number_of_channels() const;
		
    /**
     * Get the id of the Channel.
     *
     * @param channel_idx        An unique index representing the rigid body
     *                           channel.
     * @return                   The id of the specified channel.
     */
		size_t get_channel_id( size_t const & channel_idx ) const;
    
    /**
     * Get the name of the Channel.
     *
     * @param channel_idx        An unique index representing the rigid body
     *                           channel.
     * @return                   The name of the specified channel.
     */
    std::string const & get_channel_name( size_t const & channel_idx ) const;
		
    /**
     * Get number of key ticks in the channels.
     * A key tick (or simply just a key) is a time sample.
     *
     * @param channel_idx     The index value representing the channel.
     * @return                The number of keys in the channel.
     */
		size_t get_number_of_keys(size_t const & channel_idx) const;
		
    /**
     * Get Key Time.
     * @param channel_idx     The index value representing the channel.
     * @param key_idx         The index value representing the key in the channel.
     * @param float           The time of the key tick.
     */
		float get_key_time( size_t const & channel_idx, size_t const & key_idx ) const;
		
    /**
     * Get Key Position.
     * @param channel_idx     The index value representing the channel.
     * @param key_idx         The index value representing the key in the channel.
     * @param x               Upon return this argument holds the x-coordinate.
     * @param y               Upon return this argument holds the y-coordinate.
     * @param z               Upon return this argument holds the z-coordinate.
     */
		void get_key_position( size_t const &  channel_idx
                          , size_t const & key_idx
                          , float & x
                          , float & y
                          , float & z 
                          ) const;
		
    /**
     * Get Key Orientation.
     * @param channel_idx     The index value representing the channel.
     * @param key_idx         The index value representing the key in the channel.
     * @param qs             The scalar component of the unit quaterion.
     * @param qx             The imaginary x component of the unit quaterion.
     * @param qy             The imaginary y component of the unit quaterion.
     * @param qz             The imaginary z component of the unit quaterion.
     */
		void get_key_orientation( size_t const &  channel_idx
                             , size_t const & key_idx
                             ,  float & qs
                             , float & qx
                             , float & qy 
                             , float & qz 
                             ) const;
		
	};
  
  
  /**
   * Read Motion Channels from XML file.
   */
  bool xml_read( std::string const & filename, ChannelStorage & storage );
  
  
  /**
   * Write Motion Channels to XML file.
   */
  bool xml_write( std::string const & filename, ChannelStorage const & storage );
  
}// namespace content

// CONTENT_CHANNELS_H
#endif
