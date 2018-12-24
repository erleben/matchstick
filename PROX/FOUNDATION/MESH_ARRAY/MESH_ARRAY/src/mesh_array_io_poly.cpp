#include <mesh_array_io_poly.h>

#include <boost/filesystem.hpp>

#include <string>
#include <fstream>

namespace mesh_array
{
  
  template<typename T>
  void write_poly(
                  std::string const & filename
                  , T3Mesh const & mesh
                  , VertexAttribute<T,T3Mesh> const & X
                  , VertexAttribute<T,T3Mesh> const & Y
                  , VertexAttribute<T,T3Mesh> const & Z
                  )
  {
    boost::filesystem::path  mypath(filename);
    
    std::string const ext = boost::filesystem::extension( mypath );
    
    assert( ext == ".poly" || !"read_poly(): illegal file name extension");
    
    /*
     #Node Section
     nCnt 3 0  0             #Number of nodes, dimension, number of attributes, boundary marker
     nIdx xval yval zval     #Node index, followed by x,y and z coordinates
     */
    std::ofstream file(filename.c_str());
    
    file.precision(30);
    
    file << mesh.vertex_size() << " " << 3u << std::endl;
    
    size_t const cntV = mesh.vertex_size();
    
    for(size_t idx = 0u; idx < cntV; ++idx)
    {
      Vertex const & vertex = mesh.vertex(idx);
      
      file << idx << " " << X(vertex) << " " << Y(vertex)  << " " << Z(vertex)  << std::endl;
    }
    file << std::endl;
    
    /*
     #Face Section
     fCnt 0          #Number of faces, boundary markers
     1               #Number of polygons
     3 n1Idx n2Idx ........ nNIdx         #Number of vertices, followed by node indices
     */
    file << mesh.triangle_size() << " 0" << std::endl;
    
    for(size_t idx = 0u; idx < mesh.triangle_size(); ++idx)
    {
      Triangle const & triangle = mesh.triangle(idx);
      
      file << 1u << std::endl;
      
      file << 3u 
        << " "
        << triangle.i()
        << " "
        << triangle.j()
        << " "
        << triangle.k()
        << std::endl;        
    }
    
    /*
     #Hole Section
     0                     #Number of holes
     */
    file << 0u << std::endl;
    /*
     #Region Section
     rCnt                     #Number of regions
     rIdx xval yval zval rNo rAtt             #region index, coordinates of point inside region, region number, region attribute
     */
    file << 0u << std::endl;
    
    file.flush();
    file.close();
  }
  
  template
  void write_poly<float>(
                         std::string const & filename
                         , T3Mesh const & mesh
                         , VertexAttribute<float,T3Mesh> const & X
                         , VertexAttribute<float,T3Mesh> const & Y
                         , VertexAttribute<float,T3Mesh> const & Z
                         );
  
  template
  void write_poly<double>(
                          std::string const & filename
                          , T3Mesh const & mesh
                          , VertexAttribute<double,T3Mesh> const & X
                          , VertexAttribute<double,T3Mesh> const & Y
                          , VertexAttribute<double,T3Mesh> const & Z
                          );
    
} // namespace mesh_array
