#include <mesh_array_adjacency_info_t4mesh.h>
#include <mesh_array_tetrahedron_circulator.h>

namespace mesh_array
{  
  AdjacencyInfo<T4Mesh>::AdjacencyInfo()
  {
  }
  
  AdjacencyInfo<T4Mesh>::~AdjacencyInfo()
  {
    this->release();
  }
  
  AdjacencyInfo<T4Mesh>::AdjacencyInfo(AdjacencyInfo<T4Mesh> const & A)
  {
    *this = A;
  }
  
  AdjacencyInfo<T4Mesh> AdjacencyInfo<T4Mesh>::operator=(AdjacencyInfo<T4Mesh> const & A)
  {
    if(this != &A)
    {
      this->m_owner = A.m_owner;
      this->m_info  = A.m_info;
    }
    return *this;
  }
  
  AdjacencyInfo<T4Mesh>::AdjacencyInfo(VertexRing<T4Mesh  > const & VR)
  {
    this->bind(VR);
  }
  
  void AdjacencyInfo<T4Mesh>::release()
  {
    this->m_owner = 0;
    this->m_info.clear();
  }
  
  void AdjacencyInfo<T4Mesh>::bind(VertexRing<T4Mesh> const & VR)
  {
    typedef T4Mesh                           mesh_type;
    //typedef VertexRing< mesh_type >          vertex_ring_type;
    typedef TetrahedronCirculator<mesh_type> circulator_type; 
    
    this->release();
    
    this->m_owner = &VR;
    
    mesh_type const * M = this->m_owner->mesh();
    
    size_t const T = M->tetrahedron_size();
    
    if (T==0u)
      return;
    
    this->m_info.resize(T, NeighborInfo() );
    
    for(size_t t = 0u; t < T; ++t)
    {
      Tetrahedron tetrahedron = M->tetrahedron(t);
      
      size_t const idx = tetrahedron.idx();
      size_t const i   = tetrahedron.i();
      size_t const j   = tetrahedron.j();
      size_t const k   = tetrahedron.k();
      size_t const m   = tetrahedron.m();
      
      circulator_type ci( *(this->m_owner), i);         
      while(ci())
      {
        if(ci->idx() != idx)
        {
          // Opposite m -- shared face is ijk
          if ( ci->has_vertex(j) && ci->has_vertex(k))
          {
            this->m_info[idx].m_m = ci->idx();
          }
          // Opposite k -- shared face is ijm
          if ( ci->has_vertex(j) && ci->has_vertex(m))
          {
            this->m_info[idx].m_k = ci->idx();
          }
          // Opposite j -- shared face is ikm
          if ( ci->has_vertex(k) && ci->has_vertex(m))
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
          // Opposite i -- shared face is jkm
          if ( cj->has_vertex(k) && cj->has_vertex(m))
          {
            this->m_info[idx].m_i = cj->idx();
          }
        }
        ++cj;
      }
      
    }
  }

  
  size_t AdjacencyInfo<T4Mesh>::i(size_t const & idx) const
  {
    return this->m_info[idx].m_i;    
  }

  size_t AdjacencyInfo<T4Mesh>::j(size_t const & idx) const
  {
    return this->m_info[idx].m_j;    
  }

  size_t AdjacencyInfo<T4Mesh>::k(size_t const & idx) const
  {
    return this->m_info[idx].m_k;    
  }

  size_t AdjacencyInfo<T4Mesh>::m(size_t const & idx) const
  {
    return this->m_info[idx].m_m;    
  }

  
} // end namespace mesh_array
