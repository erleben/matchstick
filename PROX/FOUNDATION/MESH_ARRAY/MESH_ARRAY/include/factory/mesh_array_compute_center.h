#ifndef MESH_ARRAY_COMPUTE_CENTER_H
#define MESH_ARRAY_COMPUTE_CENTER_H

#include <mesh_array_t3mesh.h>
#include <mesh_array_t4mesh.h>
#include <mesh_array_vertex_attribute.h>

namespace mesh_array
{

  template<typename MT>
  inline typename MT::vector3_type compute_center(
                                           T3Mesh const & mesh
                                           , VertexAttribute<typename MT::real_type,T3Mesh> const & X
                                           , VertexAttribute<typename MT::real_type,T3Mesh> const & Y
                                           , VertexAttribute<typename MT::real_type,T3Mesh> const & Z
                                           )
  {
    typedef typename MT::vector3_type    V;

    V center = V::zero();

    for(size_t i=0u; i < mesh.vertex_size(); ++i)
    {
      Vertex const v = mesh.vertex(i);
      center += V::make( X(v), Y(v), Z(v) );
    }
    
    center /= mesh.vertex_size();
    
    return center;
  }

  template<typename MT>
  inline typename MT::vector3_type compute_center(
                                                  T4Mesh const & mesh
                                                  , VertexAttribute<typename MT::real_type,T4Mesh> const & X
                                                  , VertexAttribute<typename MT::real_type,T4Mesh> const & Y
                                                  , VertexAttribute<typename MT::real_type,T4Mesh> const & Z
                                                  )
  {
    typedef typename MT::vector3_type    V;

    V center = V::zero();

    for(size_t i=0u; i < mesh.vertex_size(); ++i)
    {
      Vertex const v = mesh.vertex(i);
      center += V::make( X(v), Y(v), Z(v) );
    }

    center /= mesh.vertex_size();
    
    return center;
  }




} //namespace mesh_array

// MESH_ARRAY_COMPUTE_CENTER_H
#endif 
