#ifndef MESH_ARRAY_TETRAHEDRON_CIRCULATOR
#define MESH_ARRAY_TETRAHEDRON_CIRCULATOR

#include <mesh_array_vertex_ring_t4mesh.h>

namespace mesh_array
{
  
  template<typename M>
  class TetrahedronCirculator
  {};
  
  template<>
  class TetrahedronCirculator<T4Mesh>
  {
  protected:
    
    VertexRing<T4Mesh> const *    m_owner;
    size_t                        m_cur;
    size_t                        m_end;
    Tetrahedron                   m_undefined;
    
  public:
    
    TetrahedronCirculator();
    
    ~TetrahedronCirculator();
    
    TetrahedronCirculator( VertexRing<T4Mesh> const & VR, Vertex const & V);
    
    TetrahedronCirculator(TetrahedronCirculator const & o);
        
    TetrahedronCirculator const & operator=(TetrahedronCirculator const& o);
    
    bool operator==(TetrahedronCirculator const & o) const;
    
  public:
    
    Tetrahedron const * operator->() const;
    
    Tetrahedron       * operator->();
    
    Tetrahedron const & operator*() const;
    
    Tetrahedron       & operator*();
    
    TetrahedronCirculator &  operator++();
    
    bool operator()() const;
    
  };
  
} // end namespace mesh_array

// MESH_ARRAY_TETRAHEDRON_CIRCULATOR
#endif 
