#include <mesh_array_vertex_ring_t4mesh.h>
#include <mesh_array_constants.h>

#include <algorithm>

namespace mesh_array
{
   VertexRing<T4Mesh>::VertexRing()
  {
  }
  
   VertexRing<T4Mesh>::~VertexRing()
  {
    this->release();
  }
  
  VertexRing<T4Mesh>::VertexRing( T4Mesh  const & m) 
  { 
    this->bind(m); 
  }

  VertexRing<T4Mesh>::VertexRing( VertexRing< T4Mesh  > const & o) 
  {
    (*this) = o; 
  }

   VertexRing<T4Mesh> &  VertexRing< T4Mesh  >::operator=( VertexRing< T4Mesh  > const & o)
  { 
    if (this != &o)
    {
      this->m_mesh   = o.m_mesh;
      this->m_V2T    = o.m_V2T;
      this->m_offset = o.m_offset;
    }
    return *this; 
  }

  T4Mesh const * VertexRing<T4Mesh>::mesh() const
  { 
    return this->m_mesh;
  }

  void VertexRing<T4Mesh>::bind(T4Mesh const & m)
  { 
    this->release();
    
    this->m_mesh = &m;
    
    size_t const T = m.tetrahedron_size();
    size_t const N = 4u * T;
    size_t const V = m.vertex_size();
    
    if(V==0)
      return;

    if(T==0)
      return;
    
    this->m_V2T.resize( N, std::pair<size_t,size_t>( UNASSIGNED(), UNASSIGNED()  ) );
    this->m_offset.resize( V + 1u, UNASSIGNED() );
    
    for(size_t t = 0u; t < T; ++t)
    {
      Tetrahedron tetrahedron = m.tetrahedron( t );
      
      size_t const e0 = 4u * t;
      size_t const e1 = e0 + 1u;
      size_t const e2 = e1 + 1u;
      size_t const e3 = e2 + 1u;
      
      this->m_V2T[ e0 ].first = tetrahedron.i();
      this->m_V2T[ e1 ].first = tetrahedron.j();
      this->m_V2T[ e2 ].first = tetrahedron.k();
      this->m_V2T[ e3 ].first = tetrahedron.m();
      
      this->m_V2T[ e0 ].second = tetrahedron.idx();
      this->m_V2T[ e1 ].second = tetrahedron.idx();
      this->m_V2T[ e2 ].second = tetrahedron.idx();
      this->m_V2T[ e3 ].second = tetrahedron.idx();
    }
    
    std::sort( this->m_V2T.begin(), this->m_V2T.end() );
    
    this->m_offset[ this->m_V2T[ 0 ].first ] = 0;
    this->m_offset[ V ] = N;
    
    for(size_t n = 1u; n < N; ++n)
    {
      size_t const cur  = this->m_V2T[ n ].first;
      size_t const prev = this->m_V2T[ n - 1u ].first;
      if (prev != cur)
      {
        this->m_offset[cur] = n;
      }
    }
    
    for(size_t v = 1u; v<V+1u; ++v)
    {
      size_t const cur = this->m_offset[v];
      if (cur == UNASSIGNED() )
        continue;
      
      size_t scan = v-1u;
      size_t left = this->m_offset[scan];
      while(left == UNASSIGNED() )
      {
        this->m_offset[scan] = cur;
        --scan;
        left =  this->m_offset[scan];
      }
    }
    
  }
  
  void VertexRing< T4Mesh  >::release()
  {
    this->m_mesh = 0;
    this->m_V2T.clear();
    this->m_offset.clear();
  }

  std::vector< size_t > const & VertexRing<T4Mesh>::offset() const
  {
    return this->m_offset;
  }
  
  std::vector< std::pair<size_t, size_t > > const & VertexRing<T4Mesh>::V2T() const
  {
    return this->m_V2T;
  }
  
} // end namespace mesh_array
