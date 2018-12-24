#ifndef CONTENT_IO_MOTION_H
#define CONTENT_IO_MOTION_H

namespace content
{
  namespace details
  {    
    /**
     * A motion data structure.
     * This is used for bundling arguments so code becomes more readable.
     */
    struct Motion
    {
      float m_vx;        ///< x-component of linear velocity.
      float m_vy;        ///< y-component of linear velocity.
      float m_vz;        ///< z-component of linear velocity.
      float m_wx;        ///< x-component of angular velocity.
      float m_wy;        ///< y-component of angular velocity.
      float m_wz;        ///< z-component of angular velocity.
      
      Motion()
      : m_vx(0.0f)
      , m_vy(0.0f)
      , m_vz(0.0f)
      , m_wx(0.0f)
      , m_wy(0.0f)
      , m_wz(0.0f)
      {}
      
    };
    
  }// namespace details  
}// namespace content

// CONTENT_IO_MOTION_H
#endif
