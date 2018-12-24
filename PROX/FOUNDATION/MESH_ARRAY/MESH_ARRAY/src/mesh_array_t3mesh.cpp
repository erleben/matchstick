#include <mesh_array_t3mesh.h>

#include <cassert>

namespace mesh_array
{
  T3Mesh::T3Mesh()
  : m_next_free_vertex(0)
  , m_next_free_triangle(0)
  {
  }
  
  T3Mesh::~T3Mesh()
  {
    this->clear();
  }
  
  T3Mesh::T3Mesh(T3Mesh const & o) 
  {
    (*this) = o; 
  }
    
  T3Mesh & T3Mesh::operator=(T3Mesh const & o)
  { 
    if (this != &o)
    {
      this->m_vertices           = o.m_vertices;
      this->m_triangles          = o.m_triangles;
      this->m_next_free_triangle = o.m_next_free_triangle;
      this->m_next_free_vertex   = o.m_next_free_vertex;
    }
    return *this; 
  }
  
  void T3Mesh::clear()
  {
    this->m_vertices.clear();
    this->m_triangles.clear();
    this->m_next_free_vertex   = 0u;
    this->m_next_free_triangle = 0u;
  }
  
  void T3Mesh::set_capacity(size_t const & V, size_t const & T)
  {
    this->m_vertices.resize(V);
    this->m_triangles.resize(T);
    this->m_next_free_vertex   = 0u;
    this->m_next_free_triangle = 0u;
  }
  
  Vertex & T3Mesh::push_vertex()
  {
    assert( this->m_next_free_vertex < this->m_vertices.size() || !"No space for new vertex" );
    
    size_t const idx = this->m_next_free_vertex;    
    ++(this->m_next_free_vertex);
    
    this->m_vertices[idx] = Vertex(idx);

    return this->m_vertices[idx];
  }
  
  Triangle & T3Mesh::push_triangle(Vertex const & i, Vertex const & j, Vertex const & k)
  {
    assert( this->m_next_free_triangle < this->m_triangles.size() || !"No space for new triangle" );
    
    assert(i.idx() < this->m_next_free_vertex || !"Illegal vertex index");
    assert(j.idx() < this->m_next_free_vertex || !"Illegal vertex index");
    assert(k.idx() < this->m_next_free_vertex || !"Illegal vertex index");
    assert(i.idx() != j.idx()                 || !"i was equal to j");
    assert(i.idx() != k.idx()                 || !"i was equal to k");
    assert(j.idx() != k.idx()                 || !"j was equal to k");
    
    size_t const idx = this->m_next_free_triangle;    
    ++(this->m_next_free_triangle);
    
    this->m_triangles[idx] = Triangle(i.idx(), j.idx(), k.idx(), idx);
    
    return this->m_triangles[idx];
  }
  
  Vertex & T3Mesh::vertex(size_t const & idx)
  {
    assert( idx < this->m_next_free_vertex || !"illegal index" );
    
    return this->m_vertices[idx]; 
  }
  
  Vertex const & T3Mesh::vertex(size_t const & idx) const
  {
    assert( idx < this->m_next_free_vertex || !"illegal index" );
    
    return this->m_vertices[idx]; 
  }
  
  Triangle & T3Mesh::triangle(size_t const & idx)
  {
    assert( idx < this->m_next_free_triangle || !"illegal index" );
    
    return this->m_triangles[idx]; 
  }
  
  Triangle const & T3Mesh::triangle(size_t const & idx) const 
  {
    assert( idx < this->m_next_free_triangle || !"illegal index" );
    
    return this->m_triangles[idx]; 
  }
  
  size_t T3Mesh::vertex_size() const 
  {
    return this->m_next_free_vertex; 
  }
  
  size_t T3Mesh::triangle_size() const 
  {
    return this->m_next_free_triangle; 
  }

  size_t T3Mesh::vertex_capacity() const 
  {
    return this->m_vertices.size(); 
  }
  
  size_t T3Mesh::triangle_capacity() const 
  {
    return this->m_triangles.size(); 
  }
  
} // end namespace mesh_array
