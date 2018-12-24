#ifndef MESH_ARRAY_T4MESH_H
#define MESH_ARRAY_T4MESH_H

#include <mesh_array_vertex.h>
#include <mesh_array_tetrahedron.h>

#include <vector>

namespace mesh_array
{
    
  /**
   * Tetrahedron mesh array data structure specialized for a static mesh.
   */
  class T4Mesh
  {
  protected:
    
    std::vector<Vertex>       m_vertices;
    std::vector<Tetrahedron>  m_tetrahedra;
    
    size_t                    m_next_free_vertex;
    size_t                    m_next_free_tetrahedron;
    
    public :
    
    T4Mesh();
    
    ~T4Mesh();
    
    T4Mesh(T4Mesh const & o);
    
    T4Mesh & operator=(T4Mesh const& o);
    
  public: 
    
    void clear();
    
    void set_capacity(size_t const & V, size_t const & T);
    
    Vertex & push_vertex();
    
    Tetrahedron & push_tetrahedron(Vertex const & i, Vertex const & j, Vertex const & k, Vertex const & m);
    
    Vertex const & vertex(size_t const & idx) const;
    Vertex       & vertex(size_t const & idx);
    
    Tetrahedron const & tetrahedron(size_t const & idx) const;
    Tetrahedron       & tetrahedron(size_t const & idx);
    
    size_t vertex_size() const;
    
    size_t tetrahedron_size() const;

    size_t vertex_capacity() const;
    
    size_t tetrahedron_capacity() const;
    
  };

} // end namespace mesh_array

// MESH_ARRAY_T4MESH_H
#endif 
