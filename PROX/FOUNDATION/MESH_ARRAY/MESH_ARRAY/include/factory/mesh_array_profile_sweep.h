#ifndef MESH_ARRAY_PROFILE_SWEEP_H
#define MESH_ARRAY_PROFILE_SWEEP_H

#include <mesh_array_t3mesh.h>
#include <mesh_array_vertex_attribute.h>

namespace mesh_array
{
  
  /**
   * Sweep a profile around the y-axis to create a full closed mesh surface.
   */
  template<typename MT>
  void profile_sweep(
                     std::vector<typename MT::vector3_type> const & profile
                     , size_t const & slices
                     , T3Mesh & mesh
                     , VertexAttribute<typename MT::real_type,T3Mesh> & X
                     , VertexAttribute<typename MT::real_type,T3Mesh> & Y
                     , VertexAttribute<typename MT::real_type,T3Mesh> & Z
                     );
  
} //namespace mesh_array

// MESH_ARRAY_PROFILE_SWEEP_H
#endif 
