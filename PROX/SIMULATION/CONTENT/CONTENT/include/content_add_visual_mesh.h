#ifndef CONTENT_ADD_VISUAL_MESH_H
#define CONTENT_ADD_VISUAL_MESH_H

#include <content.h>

#include <mesh_array.h>

#include <tiny.h>

namespace content
{

  inline void add_visual_mesh_of_tetrameshes(
                                             size_t const & gid
                                             , content::API * engine
                                             , mesh_array::T3Mesh & mesh
                                             , mesh_array::VertexAttribute<float, mesh_array::T3Mesh> & X
                                             , mesh_array::VertexAttribute<float, mesh_array::T3Mesh> & Y
                                             , mesh_array::VertexAttribute<float, mesh_array::T3Mesh> & Z
                                             )
  {
    // Get raw data from engine
    size_t N = 0u; ///< Number of vertices
    size_t K = 0u; ///< Number of tetrahedra

    engine->get_tetramesh_shape(  gid, N, K);

    if(N==0 || K==0)  // Test if empty mesh and skip if this is the case
      return;

    std::vector<size_t> vertices(N);
    std::vector<size_t> tetrahedra(K*4);
    std::vector<float>  coordinates(3*N);

    engine->get_tetramesh_shape(  gid
                                , &vertices[0]
                                , &tetrahedra[0]
                                , &coordinates[0]
                                );


    // Convert from raw data to a T4Mesh
    mesh_array::T4Mesh tetmesh;
    mesh_array::VertexAttribute<float, mesh_array::T4Mesh> tetX;
    mesh_array::VertexAttribute<float, mesh_array::T4Mesh> tetY;
    mesh_array::VertexAttribute<float, mesh_array::T4Mesh> tetZ;

    mesh_array::convert(N, K, &vertices[0], &tetrahedra[0], &coordinates[0], tetmesh, tetX, tetY, tetZ);

    mesh_array::make_t3mesh( tetmesh, tetX, tetY, tetZ, mesh, X, Y, Z );
  }
  
}//namespace content

// CONTENT_ADD_VISUAL_MESH_H
#endif
