#ifndef MESH_ARRAY_COMPUTE_VERTEX_NORMALS_H
#define MESH_ARRAY_COMPUTE_VERTEX_NORMALS_H

#include <mesh_array_t3mesh.h>
#include <mesh_array_vertex_attribute.h>

#include <cmath>
#include <vector>


namespace mesh_array
{

  template <typename MT>
  inline void compute_vertex_normals(
                                     T3Mesh const & mesh
                                     , mesh_array::VertexAttribute<typename MT::real_type, T3Mesh> const & X
                                     , mesh_array::VertexAttribute<typename MT::real_type, T3Mesh> const & Y
                                     , mesh_array::VertexAttribute<typename MT::real_type, T3Mesh> const & Z
                                     , std::vector<typename MT::vector3_type> & normals
                                     )
  {
    using std::min;
    using std::max;
    using std::acos;

    typedef typename MT::value_traits    VT;
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;

    unsigned int const cntV = mesh.vertex_size();
    unsigned int const cntT = mesh.triangle_size();

    normals.resize( cntV );

    for(unsigned int v = 0u; v < cntV; ++v)
    {
      normals[v] = V::zero();
    }

    for(unsigned int t = 0u; t < cntT; ++t)
    {
      mesh_array::Triangle const & tri = mesh.triangle( t );

      mesh_array::Vertex const & i = mesh.vertex( tri.i() );
      mesh_array::Vertex const & j = mesh.vertex( tri.j() );
      mesh_array::Vertex const & k = mesh.vertex( tri.k() );

      V const pi = V::make( X(i), Y(i), Z(i) );
      V const pj = V::make( X(j), Y(j), Z(j) );
      V const pk = V::make( X(k), Y(k), Z(k) );

      V const a_i      = unit(pj - pi);
      V const b_i      = unit(pk - pi);
      T const dot_i    = min( VT::one(), max( -VT::one(), inner_prod(a_i,b_i) ) );
      T const alpha_i  = acos( dot_i );

      V const a_j      = unit(pk - pj);
      V const b_j      = unit(pi - pj);
      T const dot_j    = min( VT::one(), max( -VT::one(), inner_prod(a_j,b_j) ) );
      T const alpha_j  = acos( dot_j );

      V const a_k      = unit(pi - pk);
      V const b_k      = unit(pj - pk);
      T const dot_k    = min( VT::one(), max( -VT::one(), inner_prod(a_k,b_k) ) );
      T const alpha_k  = acos( dot_k );

      V const a = pj - pi;
      V const b = pk - pi;
      V const n = unit( cross(a,b) );

      normals[ tri.i() ] += n*alpha_i;
      normals[ tri.j() ] += n*alpha_j;
      normals[ tri.k() ] += n*alpha_k;
    }
    for(unsigned int v = 0u; v < cntV; ++v)
    {
      normals[v] = unit( normals[v] );
    }
  }

} // namespace mesh_array

// MESH_ARRAY_COMPUTE_VERTEX_NORMALS_H
#endif
