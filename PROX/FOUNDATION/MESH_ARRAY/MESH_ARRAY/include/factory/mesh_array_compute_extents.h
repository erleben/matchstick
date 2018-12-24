#ifndef MESH_ARRAY_COMPUTE_EXTENTS_H
#define MESH_ARRAY_COMPUTE_EXTENTS_H

#include <mesh_array_t3mesh.h>
#include <mesh_array_t4mesh.h>
#include <mesh_array_vertex_attribute.h>

namespace mesh_array
{
  
  template<typename MT>
  inline void compute_extents(
                       T3Mesh const & mesh
                       , VertexAttribute<typename MT::real_type,T3Mesh> const & C
                       , typename MT::real_type & low
                       , typename MT::real_type & hi
                       )
  {
    typedef typename MT::real_type       T;
    typedef typename MT::value_traits    VT;

    T h = VT::lowest();
    T l = VT::highest();

    for(size_t i=0u; i < mesh.vertex_size(); ++i)
    {
      Vertex const v = mesh.vertex(i);
      h = std::max(h, C(v));
      l = std::min(l, C(v));
    }
    low = l;
    hi  = h;
  }


  template<typename MT>
  inline void compute_extents(
                              T4Mesh const & mesh
                              , VertexAttribute<typename MT::real_type,T4Mesh> const & C
                              , typename MT::real_type & low
                              , typename MT::real_type & hi
                              )
  {
    typedef typename MT::real_type       T;
    typedef typename MT::value_traits    VT;

    T h = VT::lowest();
    T l = VT::highest();

    for(size_t i=0u; i < mesh.vertex_size(); ++i)
    {
      Vertex const v = mesh.vertex(i);
      h = std::max(h, C(v));
      l = std::min(l, C(v));
    }
    low = l;
    hi  = h;
  }


} //namespace mesh_array

// MESH_ARRAY_COMPUTE_CENTER_H
#endif 
