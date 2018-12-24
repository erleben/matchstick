#include <mesh_array_t2mesh.h>

#include <cassert>

namespace mesh_array
{
  T2Mesh::T2Mesh()
  : m_next_free_vertex(0)
  , m_next_free_edge(0)
  {
  }
  
  T2Mesh::~T2Mesh()
  {
    this->clear();
  }
  
  T2Mesh::T2Mesh(T2Mesh const & o)
  {
    (*this) = o; 
  }
    
  T2Mesh & T2Mesh::operator=(T2Mesh const & o)
  { 
    if (this != &o)
    {
      this->m_vertices          = o.m_vertices;
      this->m_edges             = o.m_edges;
      this->m_next_free_edge    = o.m_next_free_edge;
      this->m_next_free_vertex  = o.m_next_free_vertex;
    }
    return *this; 
  }
  
  void T2Mesh::clear()
  {
    this->m_vertices.clear();
    this->m_edges.clear();
    this->m_next_free_vertex = 0u;
    this->m_next_free_edge   = 0u;
  }
  
  void T2Mesh::set_capacity(size_t const & V, size_t const & E)
  {
    this->m_vertices.resize(V);
    this->m_edges.resize(E);
    this->m_next_free_vertex = 0u;
    this->m_next_free_edge = 0u;
  }
  
  Vertex & T2Mesh::push_vertex()
  {
    assert( this->m_next_free_vertex < this->m_vertices.size() || !"No space for new vertex" );
    
    size_t const idx = this->m_next_free_vertex;    
    ++(this->m_next_free_vertex);
    
    this->m_vertices[idx] = Vertex(idx);

    return this->m_vertices[idx];
  }
  
  Edge & T2Mesh::push_edge(Vertex const & i, Vertex const & j)
  {
    assert( this->m_next_free_edge < this->m_edges.size() || !"No space for new edge" );
    
    assert(i.idx() < this->m_next_free_vertex || !"Illegal vertex index");
    assert(j.idx() < this->m_next_free_vertex || !"Illegal vertex index");
    assert(i.idx() != j.idx()                 || !"i was equal to j");

    size_t const idx = this->m_next_free_edge;
    ++(this->m_next_free_edge);
    
    this->m_edges[idx] = Edge(i.idx(), j.idx(), idx);
    
    return this->m_edges[idx];
  }
  
  Vertex & T2Mesh::vertex(size_t const & idx)
  {
    assert( idx < this->m_next_free_vertex || !"illegal index" );
    
    return this->m_vertices[idx]; 
  }
  
  Vertex const & T2Mesh::vertex(size_t const & idx) const
  {
    assert( idx < this->m_next_free_vertex || !"illegal index" );
    
    return this->m_vertices[idx]; 
  }
  
  Edge & T2Mesh::edge(size_t const & idx)
  {
    assert( idx < this->m_next_free_edge || !"illegal index" );
    
    return this->m_edges[idx];
  }
  
  Edge const & T2Mesh::edge(size_t const & idx) const
  {
    assert( idx < this->m_next_free_edge || !"illegal index" );
    
    return this->m_edges[idx];
  }
  
  size_t T2Mesh::vertex_size() const
  {
    return this->m_next_free_vertex; 
  }
  
  size_t T2Mesh::edge_size() const
  {
    return this->m_next_free_edge;
  }

  size_t T2Mesh::vertex_capacity() const
  {
    return this->m_vertices.size(); 
  }
  
  size_t T2Mesh::edge_capacity() const
  {
    return this->m_edges.size();
  }
  
} // end namespace mesh_array
