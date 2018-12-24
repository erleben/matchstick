#include <mesh_array_vertex_ring_t3mesh.h>

#include <mesh_array_constants.h>

#include <algorithm>

namespace mesh_array
{
   VertexRing<T3Mesh>::VertexRing()
  {
  }
  
   VertexRing<T3Mesh>::~VertexRing()
  {
    this->release();
  }
  
  VertexRing<T3Mesh>::VertexRing(T3Mesh const & m) 
  {
    this->bind(m); 
  }

  VertexRing<T3Mesh>::VertexRing(VertexRing<T3Mesh> const & o) 
  {
    (*this) = o; 
  }

   VertexRing<T3Mesh> & VertexRing<T3Mesh>::operator=(VertexRing<T3Mesh> const & o)
  { 
    if (this != &o)
    {
      this->m_mesh   = o.m_mesh;
      this->m_V2T    = o.m_V2T;
      this->m_offset = o.m_offset;
    }
    return *this; 
  }

  T3Mesh const * VertexRing<T3Mesh>::mesh() const
  { 
    return this->m_mesh;
  }

  void VertexRing<T3Mesh>::bind(T3Mesh const & m)
  { 
    this->release();
    
    this->m_mesh = &m;
    
    size_t const T = m.triangle_size();
    size_t const N = 3u * T;
    size_t const V = m.vertex_size();
    
    if(V==0)
      return;
    
    if(T==0)
      return;
    
    this->m_V2T.resize( N, std::pair<size_t,size_t>( UNASSIGNED(), UNASSIGNED()  ) );
    this->m_offset.resize( V + 1, UNASSIGNED() );

    for(size_t t = 0u; t < T; ++t)
    {
      Triangle triangle = m.triangle( t );
      
      size_t const e0 = 3u * t;
      size_t const e1 = e0 + 1u;
      size_t const e2 = e1 + 1u;
      
      this->m_V2T[ e0 ].first = triangle.i();
      this->m_V2T[ e1 ].first = triangle.j();
      this->m_V2T[ e2 ].first = triangle.k();

      this->m_V2T[ e0 ].second = triangle.idx();
      this->m_V2T[ e1 ].second = triangle.idx();
      this->m_V2T[ e2 ].second = triangle.idx();
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
  
  void VertexRing<T3Mesh>::release()
  {
    this->m_mesh = 0;
    this->m_V2T.clear();
    this->m_offset.clear();
  }

  std::vector< size_t > const & VertexRing<T3Mesh>::offset() const
  {
    return this->m_offset;
  }
  
  std::vector< std::pair<size_t, size_t > > const & VertexRing<T3Mesh>::V2T() const
  {
    return this->m_V2T;
  }
  
  
} // end namespace mesh_array
