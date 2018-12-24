#ifndef MESH_ARRAY_MAKE_T3MESH_H
#define MESH_ARRAY_MAKE_T3MESH_H

#include <mesh_array_t3mesh.h>
#include <mesh_array_t4mesh.h>
#include <mesh_array_constants.h>
#include <mesh_array_adjacency_info_t4mesh.h>
#include <mesh_array_vertex_ring_t4mesh.h>
#include <mesh_array_vertex_attribute.h>
#include <mesh_array_tetrahedron_attribute_t4mesh.h>

namespace mesh_array
{

  template<typename T>
  inline void make_t3mesh(
                          T4Mesh const & tetmesh
                          , VertexAttribute<T,T4Mesh> const & tetX
                          , VertexAttribute<T,T4Mesh> const & tetY
                          , VertexAttribute<T,T4Mesh> const & tetZ
                          , T3Mesh  & mesh
                          , VertexAttribute<T,T3Mesh>  & X
                          , VertexAttribute<T,T3Mesh>  & Y
                          , VertexAttribute<T,T3Mesh>  & Z
                          )
  {
    X.release();
    Y.release();
    Z.release();
    mesh.clear();

    VertexRing<T4Mesh>    ring(tetmesh);
    AdjacencyInfo<T4Mesh> A(ring);

    size_t const N = tetmesh.vertex_size();
    size_t const K = tetmesh.tetrahedron_size();

    size_t const EMPTY = UNASSIGNED();

    std::vector<bool>  lut;
    lut.resize(N);
    std::fill(lut.begin(), lut.end(), false);

    size_t surface_K = 0u;

    for ( size_t e = 0u; e < K; ++e)
    {
      size_t const opposite_i  = A.i( e );
      size_t const opposite_j  = A.j( e );
      size_t const opposite_k  = A.k( e );
      size_t const opposite_m  = A.m( e );

      Tetrahedron const tet = tetmesh.tetrahedron( e );

      size_t const i  = tet.i();
      size_t const j  = tet.j();
      size_t const k  = tet.k();
      size_t const m  = tet.m();

      if ( opposite_i == EMPTY )
      {
        lut[j] = true;
        lut[k] = true;
        lut[m] = true;

        ++surface_K;
      }
      if ( opposite_j == EMPTY )
      {
        lut[i] = true;
        lut[k] = true;
        lut[m] = true;

        ++surface_K;
      }
      if ( opposite_k == EMPTY )
      {
        lut[i] = true;
        lut[j] = true;
        lut[m] = true;

        ++surface_K;
      }
      if ( opposite_m == EMPTY )
      {
        lut[i] = true;
        lut[j] = true;
        lut[k] = true;

        ++surface_K;
      }
    }

    size_t surface_N = 0u;
    for ( size_t e = 0u; e < N; ++e)
    {
      surface_N = lut[e] ? surface_N + 1 : surface_N;
    }

    mesh.set_capacity(surface_N, surface_K);

    X.bind(mesh);
    Y.bind(mesh);
    Z.bind(mesh);

    std::vector<Vertex>  vertices;  // maps from T4 vertex index to a T3Mesh vertex handle
    vertices.resize(N);

    for ( size_t e = 0u; e < K; ++e)
    {
      size_t const opposite_i  = A.i( e );
      size_t const opposite_j  = A.j( e );
      size_t const opposite_k  = A.k( e );
      size_t const opposite_m  = A.m( e );
      
      Tetrahedron const tet = tetmesh.tetrahedron( e );
      
      size_t const i  = tet.i();
      size_t const j  = tet.j();
      size_t const k  = tet.k();
      size_t const m  = tet.m();
      
      if ( opposite_i == EMPTY )
      {
        if(vertices[j].idx() == EMPTY)
          vertices[j] = mesh.push_vertex();

        if(vertices[k].idx() == EMPTY)
          vertices[k] = mesh.push_vertex();

        if(vertices[m].idx() == EMPTY)
          vertices[m] = mesh.push_vertex();

        mesh.push_triangle(vertices[j], vertices[k], vertices[m] );
      }
      
      if ( opposite_j == EMPTY )
      {
        if(vertices[i].idx() == EMPTY)
          vertices[i] = mesh.push_vertex();

        if(vertices[m].idx() == EMPTY)
          vertices[m] = mesh.push_vertex();

        if(vertices[k].idx() == EMPTY)
          vertices[k] = mesh.push_vertex();

        mesh.push_triangle(vertices[i], vertices[m], vertices[k] );
      }

      if ( opposite_k == EMPTY )
      {
        if(vertices[i].idx() == EMPTY)
          vertices[i] = mesh.push_vertex();

        if(vertices[j].idx() == EMPTY)
          vertices[j] = mesh.push_vertex();

        if(vertices[m].idx() == EMPTY)
          vertices[m] = mesh.push_vertex();

        mesh.push_triangle(vertices[i], vertices[j], vertices[m] );
      }

      if ( opposite_m == EMPTY )
      {
        if(vertices[j].idx() == EMPTY)
          vertices[j] = mesh.push_vertex();

        if(vertices[i].idx() == EMPTY)
          vertices[i] = mesh.push_vertex();

        if(vertices[k].idx() == EMPTY)
          vertices[k] = mesh.push_vertex();

        mesh.push_triangle(vertices[j], vertices[i], vertices[k] );
      }

    }

    for ( size_t e = 0u; e < N; ++e)
    {
      if(lut[e])
      {
        Vertex const v_in  = tetmesh.vertex(e);
        Vertex const v_out = vertices[e];

        X(v_out) = tetX(v_in);
        Y(v_out) = tetY(v_in);
        Z(v_out) = tetZ(v_in);
      }
    }
    
  }
}// namespace mesh_array
  
  // MESH_ARRAY_MAKE_T3MESH_H
#endif
