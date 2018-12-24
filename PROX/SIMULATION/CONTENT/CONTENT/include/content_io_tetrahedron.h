#ifndef CONTENT_IO_TETRAHEDRON_H
#define CONTENT_IO_TETRAHEDRON_H

namespace content
{
  namespace details
  {
    /**
     * A simple point data structure for passing point information.
     */
    struct Tetrahedron
    {
      size_t m_i;  ///< Vertex indices of tetrahedron (assumed in posiive orientation)
      size_t m_j;
      size_t m_k;
      size_t m_m;


      Tetrahedron()
      : m_i(0u)
      , m_j(0u)
      , m_k(0u)
      , m_m(0u)
      {}
    };
        
  }// namespace details  
}// namespace content

// CONTENT_IO_TETRAHEDRON_H
#endif
