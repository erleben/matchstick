#ifndef CONTENT_IO_VERTEX_H
#define CONTENT_IO_VERTEX_H

namespace content
{
  namespace details
  {
    /**
     * A simple vertex data structure for passing vertex information.
     */
    struct Vertex
    {
      size_t m_idx; ///< Index of the vertex
      float m_x;    ///< Coordinates of the vertex
      float m_y;
      float m_z; 
      
      Vertex()
      : m_idx(0u)
      , m_x(0.0f)
      , m_y(0.0f)
      , m_z(0.0f)
      {}
    };
        
  }// namespace details  
}// namespace content

// CONTENT_IO_VERTEX_H
#endif
