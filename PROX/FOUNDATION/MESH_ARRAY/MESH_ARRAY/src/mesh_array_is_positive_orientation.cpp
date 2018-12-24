#include <mesh_array_is_positive_orientation.h>

#include <tiny.h>

#include <cassert>

namespace mesh_array
{
  
  template<typename T>
  bool is_positive_orientation(
               T4Mesh & mesh
              , VertexAttribute<T,T4Mesh> & X
              , VertexAttribute<T,T4Mesh> & Y
              , VertexAttribute<T,T4Mesh> & Z
              )
  {
  
    typedef          tiny::MathTypes<T> M;
    typedef typename M::vector3_type    V;
    typedef typename M::value_traits    VT;
  
    size_t const cntT = mesh.tetrahedron_size();
    
    for( size_t idx = 0u; idx < cntT; ++idx)
    {
      Tetrahedron const tetrahedron = mesh.tetrahedron( idx );
      
      Vertex const vi = mesh.vertex( tetrahedron.i() );
      Vertex const vj = mesh.vertex( tetrahedron.j() );
      Vertex const vk = mesh.vertex( tetrahedron.k() );
      Vertex const vm = mesh.vertex( tetrahedron.m() );
      
      V const pi = V::make(   X(vi), Y(vi), Z(vi) );
      V const pj = V::make(   X(vj), Y(vj), Z(vj) );
      V const pk = V::make(   X(vk), Y(vk), Z(vk) );
      V const pm = V::make(   X(vm), Y(vm), Z(vm) );
    
      V const e_ji = pj - pi;
      V const e_ki = pk - pi;
      V const e_mi = pm - pi;
      
      T const volume =  ( e_mi * cross( e_ji, e_ki) ) / VT::numeric_cast(6.0) ;
            
      if( volume <= VT::zero() )
        return false;
    }
    return true;
  }
  

  template
  bool is_positive_orientation<float>(
                                T4Mesh & mesh
                               , VertexAttribute<float,T4Mesh> & X
                               , VertexAttribute<float,T4Mesh> & Y
                               , VertexAttribute<float,T4Mesh> & Z
                               );

  
  template
  bool is_positive_orientation<double>(
                                T4Mesh & mesh
                               , VertexAttribute<double,T4Mesh> & X
                               , VertexAttribute<double,T4Mesh> & Y
                               , VertexAttribute<double,T4Mesh> & Z
                               );
  
  
} // namespace mesh_array
