#include <mesh_array_make_convex.h>

#include <tiny_math_types.h>

#if defined(__cplusplus)
extern "C"
{
#endif
#include <stdio.h>
#include <stdlib.h>
#include <qhull.h>
#include <mem.h>
#include <qset.h>
#include <geom.h>
#include <merge.h>
#include <poly.h>
#include <io.h>
#include <stat.h>
#if defined(__cplusplus)
}
#endif

#include <vector>
#include <cassert>

namespace mesh_array
{
  
  template<typename MT>
  void make_convex(
                   std::vector< typename MT::vector3_type> const & vertices
                   , T3Mesh & mesh
                   , VertexAttribute<typename MT::real_type,T3Mesh> & X
                   , VertexAttribute<typename MT::real_type,T3Mesh> & Y
                   , VertexAttribute<typename MT::real_type,T3Mesh> & Z
                   )
  {    
    size_t const N = vertices.size();
    
    assert(N>0u     || !"make_convex(): Must be a positive number of vertices");
    
    std::vector<coordT> coords;
    coords.resize(3u*N);
    for(size_t i = 0u;i<N;++i)
    {
      coords[i*3u   ] = vertices[i](0);
      coords[i*3u+1u] = vertices[i](1);
      coords[i*3u+2u] = vertices[i](2);
    }
    
    //------------------------------------------------
    
    boolT ismalloc = False;
    char flags[] = "qhull Qx Qt"; //QJ maybe Qt
    FILE *outfile = stdout;    //FILE *outfile = 0;
    FILE *errfile = stderr;    //FILE *errfile = 0;
    
    int        K = N;
    int      dim = 3;
    int exitcode = qh_new_qhull (dim,  K, &coords[0], ismalloc, flags, outfile, errfile);
    
    if(!exitcode)
    {
      //--- Determine how many triangle faces we have in hull
      size_t F = 0u;
      {
        facetT *facet;
        qh visit_id++;
        FORALLfacets
        {
          facet->visitid = qh visit_id;
          ++F;
        }
      }
      
      //--- Allocate space for the convex hull mesh
      mesh.clear();
      
      mesh.set_capacity(N,F);
      
      X.bind(mesh);
      Y.bind(mesh);
      Z.bind(mesh);
      
      //--- Fill in vertex coordinate data
      for(size_t v_idx = 0u;v_idx<N;++v_idx)
      {
        mesh.push_vertex();
        Vertex const v     = mesh.vertex( v_idx );
        
        X(v) = vertices[v_idx](0);
        Y(v) = vertices[v_idx](1);
        Z(v) = vertices[v_idx](2);
      }
      
      //--- Fill in triangle face data
      {
        facetT *facet;
        vertexT *vertex, **vertexp;
        qh visit_id++;
        FORALLfacets
        {
          facet->visitid = qh visit_id;
          
          int i = 0;
          Vertex idx[3];
          
          FOREACHvertex_(facet->vertices)
          {
            size_t const v_idx = qh_pointid(vertex->point);
            Vertex const v     = mesh.vertex( v_idx );
            idx[ i++ ] = v;
          }
          
          if(facet->toporient)
          {
            mesh.push_triangle(idx[2], idx[1], idx[0]);
          }
          else
          {
            mesh.push_triangle(idx[0], idx[1], idx[2]);
          }
          
        }
      }
    }
    
    qh_freeqhull(!qh_ALL);
    
    int curlong;
    int totlong;
    qh_memfreeshort (&curlong, &totlong);
    
    if (curlong || totlong)
    {
      fprintf (errfile, "qhull internal warning (main): did not free %d bytes of long memory (%d pieces)\n", totlong, curlong);
    }
    
  }
  
  typedef tiny::MathTypes<float> MTf;
  typedef tiny::MathTypes<double> MTd;
  
  template
  void make_convex<MTf>(
                        std::vector<MTf::vector3_type> const & vertices
                        , T3Mesh & mesh
                        , VertexAttribute<MTf::real_type,T3Mesh> & X
                        , VertexAttribute<MTf::real_type,T3Mesh> & Y
                        , VertexAttribute<MTf::real_type,T3Mesh> & Z
                        );
  
  template
  void make_convex<MTd>(
                        std::vector< MTd::vector3_type> const & vertices
                        , T3Mesh & mesh
                        , VertexAttribute< MTd::real_type,T3Mesh> & X
                        , VertexAttribute< MTd::real_type,T3Mesh> & Y
                        , VertexAttribute< MTd::real_type,T3Mesh> & Z
                        );
  
} //namespace mesh_array
