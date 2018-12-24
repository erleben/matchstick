#ifndef MESH_ARRAY_CONVERT_H
#define MESH_ARRAY_CONVERT_H

#include <mesh_array_vertex_attribute.h>
#include <mesh_array_t4mesh.h>

#include <assert.h>
#include <vector>

namespace mesh_array
{

  template<typename T>
  inline void convert(
                       size_t const & N
                     , size_t const & K
                     , size_t const * vertices
                     , size_t const * tetrahedra
                     , float  const * coordinates
                     , T4Mesh & mesh
                     , VertexAttribute<T,T4Mesh> & X
                     , VertexAttribute<T,T4Mesh> & Y
                     , VertexAttribute<T,T4Mesh> & Z
                     )
  {

    //--- create mesh
    mesh.clear();
    X.release();
    Y.release();
    Z.release();

    mesh.set_capacity( N, K);
    X.bind(mesh);
    Y.bind(mesh);
    Z.bind(mesh);

    // fill in vertices
    for (size_t i = 0u; i < N ; ++i)
    {
      assert(i == vertices[i] || !"convert(): oups not supported behaviour");

      Vertex const v = mesh.push_vertex();

      X(v) = coordinates[3u*i+0u];
      Y(v) = coordinates[3u*i+1u];
      Z(v) = coordinates[3u*i+2u];
    }

    // fill in tetrahedra
    for (size_t i = 0u; i < K ; ++i)
    {
      Vertex const vi = mesh.vertex(tetrahedra[4u*i+0u]);
      Vertex const vj = mesh.vertex(tetrahedra[4u*i+1u]);
      Vertex const vk = mesh.vertex(tetrahedra[4u*i+2u]);
      Vertex const vm = mesh.vertex(tetrahedra[4u*i+3u]);

      mesh.push_tetrahedron( vi, vj, vk, vm);
    }

  }

} // end namespace mesh_array

// MESH_ARRAY_CONVERT_H
#endif 
