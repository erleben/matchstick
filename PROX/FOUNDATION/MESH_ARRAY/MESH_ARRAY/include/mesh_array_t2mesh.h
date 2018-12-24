#ifndef MESH_ARRAY_T2MESH_H
#define MESH_ARRAY_T2MESH_H

#include <mesh_array_vertex.h>
#include <mesh_array_edge.h>

#include <vector>

namespace mesh_array
{
    
  /**
   * Edge mesh array data structure specialized for a static mesh.
   */
  class T2Mesh
  {
  protected:
    
    std::vector<Vertex>       m_vertices;
    std::vector<Edge>         m_edges;
    
    size_t                    m_next_free_vertex;
    size_t                    m_next_free_edge;
    
  public :

    T2Mesh();
    
    ~T2Mesh();
    
    T2Mesh(T2Mesh const & o);
           
    T2Mesh & operator=(T2Mesh const& o);
    
  public: 

    void clear();
    
    void set_capacity(size_t const & V, size_t const & E);
    
    // 2013-03-29 Kenny: Code review: Is is a limitation
    // that one can not allocate everything at once. Instead
    // one should first use set_capacity and then do push_XXX
    // to actually allocate vertices and triangles. This is
    // inherently sequential and would limit parallel usage.
    
    Vertex & push_vertex();
        
    Edge & push_edge(Vertex const & i, Vertex const & j);
    
    Vertex       & vertex(size_t const & idx);
    Vertex const & vertex(size_t const & idx) const;
    
    Edge       & edge(size_t const & idx);
    Edge const & edge(size_t const & idx) const;
    
    size_t vertex_size() const;
    
    size_t edge_size() const;

    size_t vertex_capacity() const;
    
    size_t edge_capacity() const;
    
  };
  
}//end of namespace mesh_array

// MESH_ARRAY_T2MESH_H
#endif
