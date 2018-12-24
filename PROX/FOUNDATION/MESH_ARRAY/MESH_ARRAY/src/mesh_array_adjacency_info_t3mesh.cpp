#include <mesh_array_adjacency_info_t3mesh.h>
#include <mesh_array_triangle_circulator.h>

namespace mesh_array 
{  
  
  AdjacencyInfo<T3Mesh>::AdjacencyInfo()
  {
  }
  
  AdjacencyInfo<T3Mesh>::~AdjacencyInfo()
  {
    this->release();
  }
  
  AdjacencyInfo<T3Mesh>::AdjacencyInfo(AdjacencyInfo<T3Mesh> const & A)
  {
    *this = A;
  }
  
  AdjacencyInfo<T3Mesh> AdjacencyInfo<T3Mesh>::operator=(AdjacencyInfo<T3Mesh> const & A)
  {
    if(this != &A)
    {
      this->m_owner = A.m_owner;
      this->m_info  = A.m_info;
    }
    return *this;
  }
  
  AdjacencyInfo<T3Mesh>::AdjacencyInfo(VertexRing<T3Mesh  > const & VR)
  {
    this->bind(VR);
  }
  
  void AdjacencyInfo<T3Mesh>::release()
  {
    this->m_owner = 0;
    this->m_info.clear();
  }
  
  void AdjacencyInfo<T3Mesh>::bind(VertexRing<T3Mesh> const & VR)
  {
    typedef T3Mesh                           mesh_type;
    //typedef VertexRing< mesh_type >          vertex_ring_type;
    typedef TriangleCirculator<mesh_type> circulator_type; 
    
    this->release();
    
    this->m_owner = &VR;
    
    mesh_type const * M = this->m_owner->mesh();
    
    size_t const T = M->triangle_size();
    
    if (T==0u)
      return;
    
    this->m_info.resize(T, NeighborInfo() );
    
    for(size_t t = 0u; t < T; ++t)
    {
      Triangle triangle = M->triangle(t);
      
      size_t const idx = triangle.idx();
      size_t const i   = triangle.i();
      size_t const j   = triangle.j();
      size_t const k   = triangle.k();
      
      circulator_type ci( *(this->m_owner), i);         
      while(ci())
      {
        if(ci->idx() != idx)
        {
          // Opposite k -- shared face is ij
          if ( ci->has_vertex(j) )
          {
            this->m_info[idx].m_k = ci->idx();
          }
          
          // Opposite j -- shared face is ik
          if ( ci->has_vertex(k) )
          {
            this->m_info[idx].m_j = ci->idx();
          }
          
        }
        ++ci;
      }
      
      circulator_type cj( *(this->m_owner), j);         
      while(cj())
      {
        if(cj->idx() != idx)
        {
          // Opposite i -- shared face is jk
          if ( cj->has_vertex(k) )
          {
            this->m_info[idx].m_i = cj->idx();
          }
        }
        ++cj;
      }
      
    }
  }
    
  size_t AdjacencyInfo<T3Mesh>::i(size_t const & idx) const
  {
    return this->m_info[idx].m_i;    
  }

  size_t AdjacencyInfo<T3Mesh>::j(size_t const & idx) const
  {
    return this->m_info[idx].m_j;    
  }
 
  size_t AdjacencyInfo<T3Mesh>::k(size_t const & idx) const
  {
    return this->m_info[idx].m_k;    
  }
  
  
} // end namespace mesh_array
