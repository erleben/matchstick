#ifndef MESH_ARRAY_DISPLACE_VERTICES_H
#define MESH_ARRAY_DISPLACE_VERTICES_H

#include <mesh_array_t3mesh.h>
#include <mesh_array_vertex_attribute.h>

#include <vector>

namespace mesh_array
{

  template <typename MT>
  inline void displace_vertices(
                                T3Mesh const & mesh
                                , mesh_array::VertexAttribute<typename MT::real_type, T3Mesh> & X
                                , mesh_array::VertexAttribute<typename MT::real_type, T3Mesh> & Y
                                , mesh_array::VertexAttribute<typename MT::real_type, T3Mesh> & Z
                                , typename MT::real_type const & distance
                                , std::vector<typename MT::vector3_type> & directions
                                )
  {
    using std::min;
    using std::max;
    using std::acos;

    typedef typename MT::vector3_type    V;

    unsigned int const cntV = mesh.vertex_size();

    for(unsigned int i = 0u; i < cntV; ++i)
    {
      mesh_array::Vertex const & v = mesh.vertex( i );

      V const x_old = V::make( X(v), Y(v), Z(v) );
      V const x_new = x_old + directions[i]*distance;

      X(v) = x_new(0);
      Y(v) = x_new(1);
      Z(v) = x_new(2);
    }

  }

} // namespace mesh_array

// MESH_ARRAY_DISPLACE_VERTICES_H
#endif
