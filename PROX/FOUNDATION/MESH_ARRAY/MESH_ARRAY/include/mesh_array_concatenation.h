#ifndef MESH_ARRAY_CONCATENATION_H
#define MESH_ARRAY_CONCATENATION_H

#include <mesh_array_t3mesh.h>
#include <mesh_array_vertex_attribute.h>

#include <vector>

namespace mesh_array
{

  /**
   * Concatenates mesh A and B into mesh C.
   *
   * The meshes are not welded/fuzed together. This is simply a raw copy of two meshes into one mesh.
   */
  template<typename MT>
  inline void concatenation(
                              T3Mesh const & A
                            , VertexAttribute<typename MT::real_type,T3Mesh> const & Ax
                            , VertexAttribute<typename MT::real_type,T3Mesh> const & Ay
                            , VertexAttribute<typename MT::real_type,T3Mesh> const & Az
                            , T3Mesh const & B
                            , VertexAttribute<typename MT::real_type,T3Mesh> const & Bx
                            , VertexAttribute<typename MT::real_type,T3Mesh> const & By
                            , VertexAttribute<typename MT::real_type,T3Mesh> const & Bz
                            , T3Mesh & C
                            , VertexAttribute<typename MT::real_type,T3Mesh> & Cx
                            , VertexAttribute<typename MT::real_type,T3Mesh> & Cy
                            , VertexAttribute<typename MT::real_type,T3Mesh> & Cz
                            )
  {

    // 2014-09-08 Kenny code review: It would be nice if T3Mesh could support
    // resize operation. That way we could concatenate incrementally in
    // a "+=" fashion. This is however not possible with current T3Mesh implementation.
    C.clear();

    C.set_capacity(A.vertex_size()+B.vertex_size(), A.triangle_size()+B.triangle_size());

    Cx.release();
    Cy.release();
    Cz.release();

    std::vector<Vertex> a2c;
    a2c.resize( A.vertex_size() );
    for(unsigned int v = 0u; v < A.vertex_size(); ++v)
    {
      Vertex const & a = A.vertex(v);

      a2c[a.idx()]   =  C.push_vertex();
    }

    std::vector<Vertex> b2c;
    b2c.resize( B.vertex_size() );
    for(unsigned int v = 0u; v < B.vertex_size(); ++v)
    {
      Vertex const & b = B.vertex(v);

      b2c[b.idx()]   =  C.push_vertex();
    }

    for(unsigned int e = 0u; e < A.triangle_size(); ++e)
    {
      Triangle const & tri = A.triangle(e);

      Vertex const & ai = A.vertex( tri.i() );
      Vertex const & aj = A.vertex( tri.j() );
      Vertex const & ak = A.vertex( tri.k() );

      Vertex const & ci = a2c[ai.idx()];
      Vertex const & cj = a2c[aj.idx()];
      Vertex const & ck = a2c[ak.idx()];

      C.push_triangle(ci,cj,ck);
    }

    for(unsigned int e = 0u; e < B.triangle_size(); ++e)
    {
      Triangle const & tri = B.triangle(e);

      Vertex const & bi  = B.vertex( tri.i() );
      Vertex const & bj  = B.vertex( tri.j() );
      Vertex const & bk  = B.vertex( tri.k() );

      Vertex const & ci  = b2c[bi.idx()];
      Vertex const & cj  = b2c[bj.idx()];
      Vertex const & ck  = b2c[bk.idx()];

      C.push_triangle(ci,cj,ck);
    }

    Cx.bind(C);
    Cy.bind(C);
    Cz.bind(C);

    for(unsigned int v = 0u; v < A.vertex_size(); ++v)
    {
      Vertex const & a = A.vertex(v);
      Vertex const & c = a2c[a.idx()];

      Cx(c) = Ax(a);
      Cy(c) = Ay(a);
      Cz(c) = Az(a);
    }

    for(unsigned int v = 0u; v < B.vertex_size(); ++v)
    {
      Vertex const & b = B.vertex(v);
      Vertex const & c = b2c[b.idx()];
      
      Cx(c) = Bx(b);
      Cy(c) = By(b);
      Cz(c) = Bz(b);
    }
  
  }

  template<typename MT>
  inline void concatenation(
                            T3Mesh const & A
                            , VertexAttribute<typename MT::real_type,T3Mesh> const & Ax
                            , VertexAttribute<typename MT::real_type,T3Mesh> const & Ay
                            , VertexAttribute<typename MT::real_type,T3Mesh> const & Az
                            , T3Mesh & C
                            , VertexAttribute<typename MT::real_type,T3Mesh> & Cx
                            , VertexAttribute<typename MT::real_type,T3Mesh> & Cy
                            , VertexAttribute<typename MT::real_type,T3Mesh> & Cz
                            )
  {
    T3Mesh const B = C;

    VertexAttribute<typename MT::real_type,T3Mesh> const Bx = Cx;
    VertexAttribute<typename MT::real_type,T3Mesh> const By = Cy;
    VertexAttribute<typename MT::real_type,T3Mesh> const Bz = Cz;

    concatenation<MT>(A,Ax,Ay,Az,B,Bx,By,Bz,C,Cx,Cy,Cz);
  }


} //namespace mesh_array

// MESH_ARRAY_CONCATENATION_H
#endif
