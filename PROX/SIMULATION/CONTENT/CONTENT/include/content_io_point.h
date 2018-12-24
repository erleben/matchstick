#ifndef CONTENT_IO_POINT_H
#define CONTENT_IO_POINT_H

namespace content
{
  namespace details
  {
    /**
     * A simple point data structure for passing point information.
     */
    struct Point
    {
      float m_x; ///< Coordinates of the point
      float m_y;
      float m_z; 
      
      Point()
      : m_x(0.0f)
      , m_y(0.0f)
      , m_z(0.0f)
      {}
    };
        
  }// namespace details  
}// namespace content

// CONTENT_IO_POINT_H
#endif
