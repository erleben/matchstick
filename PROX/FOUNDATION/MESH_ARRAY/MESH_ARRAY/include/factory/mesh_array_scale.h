#ifndef MESH_ARRAY_SCALE_H
#define MESH_ARRAY_SCALE_H

#include <mesh_array_t3mesh.h>
#include <mesh_array_t4mesh.h>
#include <mesh_array_vertex_attribute.h>

namespace mesh_array
{
  template<typename MT>
  inline void scale(
             typename MT::real_type const & sx
             , typename MT::real_type const & sy
             , typename MT::real_type const & sz
             , T3Mesh const & mesh
             , VertexAttribute<typename MT::real_type,T3Mesh> & X
             , VertexAttribute<typename MT::real_type,T3Mesh> & Y
             , VertexAttribute<typename MT::real_type,T3Mesh> & Z
             )
  {
    typedef typename MT::vector3_type    V;

    for(size_t i=0u; i < mesh.vertex_size(); ++i)
    {
      Vertex const v = mesh.vertex(i);

      V const p = V::make( sx*X(v), sy*Y(v), sz*Z(v) );

      X(v) = p(0);
      Y(v) = p(1);
      Z(v) = p(2);
    }
  }


  template<typename MT>
  inline void scale(
                    typename MT::real_type const & sx
                    , typename MT::real_type const & sy
                    , typename MT::real_type const & sz
                    , T4Mesh const & mesh
                    , VertexAttribute<typename MT::real_type,T4Mesh> & X
                    , VertexAttribute<typename MT::real_type,T4Mesh> & Y
                    , VertexAttribute<typename MT::real_type,T4Mesh> & Z
                    )
  {
    typedef typename MT::vector3_type    V;

    for(size_t i=0u; i < mesh.vertex_size(); ++i)
    {
      Vertex const v = mesh.vertex(i);

      V const p = V::make( sx*X(v), sy*Y(v), sz*Z(v) );

      X(v) = p(0);
      Y(v) = p(1);
      Z(v) = p(2);
    }
  }


} //namespace mesh_array

// MESH_ARRAY_SCALE_H
#endif 
