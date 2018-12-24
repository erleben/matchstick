#include <mesh_array_t4mesh.h>

#include <cassert>

namespace mesh_array
{
  
  T4Mesh::T4Mesh()
  : m_next_free_vertex(0)
  , m_next_free_tetrahedron(0)
  {
  }
  
  T4Mesh::~T4Mesh()
  {
    this->clear();
  }
  
  T4Mesh::T4Mesh(T4Mesh const & o) 
  {
    (*this) = o; 
  }
  
  T4Mesh & T4Mesh::operator=(T4Mesh const & o)
  { 
    if (this != &o)
    {
      this->m_vertices              = o.m_vertices;
      this->m_tetrahedra            = o.m_tetrahedra;
      this->m_next_free_tetrahedron = o.m_next_free_tetrahedron;
      this->m_next_free_vertex      = o.m_next_free_vertex;
    }
    return *this; 
  }
  
  void T4Mesh::clear()
  {
    this->m_vertices.clear();
    this->m_tetrahedra.clear();
    this->m_next_free_vertex      = 0u;
    this->m_next_free_tetrahedron = 0u;
  }
  
  void T4Mesh::set_capacity(size_t const & V, size_t const & T)
  {
    this->m_vertices.resize(V);
    this->m_tetrahedra.resize(T);
    this->m_next_free_vertex      = 0u;
    this->m_next_free_tetrahedron = 0u;
  }
  
  Vertex & T4Mesh::push_vertex()
  {
    assert( this->m_next_free_vertex < this->m_vertices.size() || !"No space for new vertex" );
    
    size_t const idx = this->m_next_free_vertex;    
    ++(this->m_next_free_vertex);

    this->m_vertices[idx] = Vertex(idx);
    
    return this->m_vertices[idx];
  }
  
  Tetrahedron & T4Mesh::push_tetrahedron(Vertex const & i, Vertex const & j, Vertex const & k, Vertex const & m)
  {
    assert( this->m_next_free_tetrahedron < this->m_tetrahedra.size() || !"No space for new tetrahedron" );
    
    assert(i.idx() < this->m_next_free_vertex || !"Illegal vertex index");
    assert(j.idx() < this->m_next_free_vertex || !"Illegal vertex index");
    assert(k.idx() < this->m_next_free_vertex || !"Illegal vertex index");
    assert(m.idx() < this->m_next_free_vertex || !"Illegal vertex index");
    assert(i.idx() != j.idx()                 || !"i was equal to j"    );
    assert(i.idx() != k.idx()                 || !"i was equal to k"    );
    assert(i.idx() != m.idx()                 || !"i was equal to m"    );
    assert(j.idx() != k.idx()                 || !"j was equal to k"    );
    assert(j.idx() != m.idx()                 || !"j was equal to m"    );
    assert(k.idx() != m.idx()                 || !"k was equal to m"    );
    
    size_t const idx = this->m_next_free_tetrahedron;
    
    ++(this->m_next_free_tetrahedron);
    
    this->m_tetrahedra[idx] = Tetrahedron(i.idx(), j.idx(), k.idx(), m.idx(), idx);
        
    return this->m_tetrahedra[idx];
  }
  
  Vertex & T4Mesh::vertex(size_t const & idx)       
  {
    assert( idx < this->m_next_free_vertex || !"illegal index" );
    
    return this->m_vertices[idx]; 
  }
  
  Vertex const & T4Mesh::vertex(size_t const & idx) const 
  {
    assert( idx < this->m_next_free_vertex || !"illegal index" );
    
    return this->m_vertices[idx]; 
  }
  
  Tetrahedron & T4Mesh::tetrahedron(size_t const & idx)
  {
    assert( idx < this->m_next_free_tetrahedron || !"illegal index" );
    
    return this->m_tetrahedra[idx]; 
  }
  
  Tetrahedron const & T4Mesh::tetrahedron(size_t const & idx) const
  {
    assert( idx < this->m_next_free_tetrahedron || !"illegal index" );
    
    return this->m_tetrahedra[idx]; 
  }
  
  size_t T4Mesh::vertex_size() const 
  {
    return this->m_next_free_vertex; 
  }
  
  size_t T4Mesh::tetrahedron_size() const 
  {
    return this->m_next_free_tetrahedron; 
  }

  size_t T4Mesh::vertex_capacity() const 
  {
    return this->m_vertices.size(); 
  }
  
  size_t T4Mesh::tetrahedron_capacity() const 
  {
    return this->m_tetrahedra.size(); 
  }
    
} //end namespace mesh_array
