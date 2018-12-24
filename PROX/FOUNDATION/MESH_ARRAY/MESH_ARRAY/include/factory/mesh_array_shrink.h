#ifndef MESH_ARRAY_SHRINK_H
#define MESH_ARRAY_SHRINK_H

#include <mesh_array_t3mesh.h>
#include <mesh_array_t4mesh.h>
#include <mesh_array_vertex_attribute.h>

#include <factory/mesh_array_translate.h>
#include <factory/mesh_array_scale.h>
#include <factory/mesh_array_compute_center.h>

#include <tiny_math_types.h>

namespace mesh_array
{

  template<typename MT>
  inline void shrink(
              typename MT::real_type const & factor
              , T3Mesh const & mesh
              , VertexAttribute<typename MT::real_type,T3Mesh> & X
              , VertexAttribute<typename MT::real_type,T3Mesh> & Y
              , VertexAttribute<typename MT::real_type,T3Mesh> & Z
              )
  {
    typedef typename MT::vector3_type    V;

    V const d = compute_center<MT>(mesh,X,Y,Z);

    translate<MT>(-d,mesh,X,Y,Z);
    scale<MT>(factor,factor,factor,mesh,X,Y,Z);
    translate<MT>(d,mesh,X,Y,Z);
  }


  template<typename MT>
  inline void shrink(
                     typename MT::real_type const & factor
                     , T4Mesh const & mesh
                     , VertexAttribute<typename MT::real_type,T4Mesh> & X
                     , VertexAttribute<typename MT::real_type,T4Mesh> & Y
                     , VertexAttribute<typename MT::real_type,T4Mesh> & Z
                     )
  {
    typedef typename MT::vector3_type    V;

    V const d = compute_center<MT>(mesh,X,Y,Z);

    translate<MT>(-d,mesh,X,Y,Z);
    scale<MT>(factor,factor,factor,mesh,X,Y,Z);
    translate<MT>(d,mesh,X,Y,Z);
  }

    
} //namespace mesh_array

// MESH_ARRAY_SHRINK_H
#endif 
