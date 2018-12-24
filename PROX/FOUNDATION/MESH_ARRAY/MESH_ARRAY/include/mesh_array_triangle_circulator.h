#ifndef MESH_ARRAY_TRIANGLE_CIRCULATOR
#define MESH_ARRAY_TRIANGLE_CIRCULATOR

#include <mesh_array_vertex_ring_t3mesh.h>

namespace mesh_array
{
  
  template<typename M>
  class TriangleCirculator
  {};
  
  template<>
  class TriangleCirculator<T3Mesh>
  {
  protected:
    
    VertexRing<T3Mesh> const * m_owner;
    size_t                     m_cur;
    size_t                     m_end;
    Triangle                   m_undefined;
    
  public:
    
    TriangleCirculator();
    
    ~TriangleCirculator();
  
    TriangleCirculator(VertexRing<T3Mesh> const & VR, Vertex const & v);
    
    TriangleCirculator(TriangleCirculator const & o);
        
    TriangleCirculator const & operator=(TriangleCirculator const & o);
    
    bool operator==(TriangleCirculator const & o) const;
    
  public:
    
    Triangle const * operator->() const;
    
    Triangle       * operator->();
    
    Triangle const & operator*() const;
    
    Triangle       & operator*();
    
    TriangleCirculator &  operator++();

    bool operator()() const;
    
  };
  
} // end namespace mesh_array

// MESH_ARRAY_TRIANGLE_CIRCULATOR
#endif 
