#ifndef PROX_COMPUTE_STRUCTURE_MAP_CONSTANT_H
#define PROX_COMPUTE_STRUCTURE_MAP_CONSTANT_H

#include <mesh_array_vertex_attribute.h>

namespace prox
{

  /**
   *  The structure map holds information of the structure direction in the
   *  vertices. This is a simple implementation which assign one global
   *  structure direction to all the vertices.
   *
   */
  template<typename MT>
  inline void compute_structure_map_constant(
                                             mesh_array::T4Mesh const & mesh
                                             , mesh_array::VertexAttribute<typename MT::real_type, mesh_array::T4Mesh> const & X
                                             , mesh_array::VertexAttribute<typename MT::real_type, mesh_array::T4Mesh> const & Y
                                             , mesh_array::VertexAttribute<typename MT::real_type, mesh_array::T4Mesh> const & Z
                                             , typename MT::vector3_type const & structure_direction
                                             , mesh_array::VertexAttribute<typename MT::vector3_type, mesh_array::T4Mesh> & structure_map
                                             )
  {
    structure_map.bind(mesh);
    for ( size_t idx = 0u; idx < mesh.vertex_size(); ++idx)
    {
      mesh_array::Vertex const vertex = mesh.vertex( idx );
      structure_map(vertex) = structure_direction;
    }
  }

}// namespace mesh_array

//PROX_COMPUTE_STRUCTURE_MAP_CONSTANT_H
#endif
