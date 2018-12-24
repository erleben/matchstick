#ifndef MESH_ARRAY_T3MESH_H
#define MESH_ARRAY_T3MESH_H

#include <mesh_array_vertex.h>
#include <mesh_array_triangle.h>

#include <vector>

namespace mesh_array
{
    
  /**
   * Triangle mesh array data structure specialized for a static mesh.
   */
  class T3Mesh
  {
  protected:
    
    std::vector<Vertex>       m_vertices;
    std::vector<Triangle>     m_triangles;
    
    size_t                    m_next_free_vertex;
    size_t                    m_next_free_triangle;
    
  public :

    T3Mesh();
    
    ~T3Mesh();
    
    T3Mesh(T3Mesh const & o);
           
    T3Mesh & operator=(T3Mesh const& o);
    
  public: 

    void clear();
    
    void set_capacity(size_t const & V, size_t const & T);
    
    // 2013-03-29 Kenny: Code review: Is is a limitation
    // that one can not allocate everything at once. Instead
    // one should first use set_capacity and then do push_XXX
    // to actually allocate vertices and triangles. This is
    // inherently sequential and would limit parallel usage.
    
    Vertex & push_vertex();
        
    Triangle & push_triangle(Vertex const & i, Vertex const & j, Vertex const & k);
    
    Vertex       & vertex(size_t const & idx);
    Vertex const & vertex(size_t const & idx) const;
    
    Triangle       & triangle(size_t const & idx);
    Triangle const & triangle(size_t const & idx) const;
    
    size_t vertex_size() const;
    
    size_t triangle_size() const;

    size_t vertex_capacity() const;
    
    size_t triangle_capacity() const;
    
  };
  
}//end of namespace mesh_array

// MESH_ARRAY_T3MESH_H
#endif
