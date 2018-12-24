#include <mesh_array_io_tetgen.h>

#include <tiny.h>

#include <boost/filesystem.hpp>

#include <cassert>
#include <fstream>

namespace mesh_array 
{
  
  template<typename T>
  void read_tetgen(
                   std::string const & filename
                   , T3Mesh & mesh
                   , VertexAttribute<T,T3Mesh> & X
                   , VertexAttribute<T,T3Mesh> & Y
                   , VertexAttribute<T,T3Mesh> & Z
                   )
  {    
    boost::filesystem::path  mypath(filename);
    
    std::string const ext = boost::filesystem::extension( mypath );
    
    assert( ext == "" || !"read_tetgen(): illegal file name extension");
        
    std::string const filename1 = filename + ".node";
    std::string const filename2 = filename + ".face";

    std::ifstream file1(filename1.c_str());
    std::ifstream file2(filename2.c_str()); 
    
    assert (file1.is_open() || !"read_tetgen(): unable ot open file");
    assert (file2.is_open() || !"read_tetgen(): unable ot open file");
    
    size_t cnt_vertices;    // Total number of vertices
    size_t dimension;       // The dimension of the coordinate space
    size_t cnt_attributes;  // The number of attributes for each vertex
    size_t cnt_triangles;   // Total number of triangles
    size_t cnt_points;      // The number of points per triangle (must be 3)
    bool has_boundary;      // Boundary marker (if used)
    
    file1 >> cnt_vertices  >> dimension  >> cnt_attributes >> has_boundary;
    file2 >> cnt_triangles >> cnt_points >> cnt_attributes;
    
    assert (cnt_vertices > 0u    || !"read_tetgen(): unsupported format");
    assert (dimension == 3u      || !"read_tetgen(): unsupported format");
    assert (cnt_attributes == 0u || !"read_tetgen(): unsupported format");
    assert (cnt_triangles > 0u   || !"read_tetgen(): unsupported format");
    assert (cnt_points == 3u     || !"read_tetgen(): unsupported format");
    
    mesh.clear();
    
    mesh.set_capacity(cnt_vertices, cnt_triangles);
    
    X.bind(mesh);
    Y.bind(mesh);
    Z.bind(mesh);
    
    std::vector<Vertex> vertices (cnt_vertices);
    
    for(size_t idx=0u; idx<cnt_vertices; ++idx)
    {
      size_t external_idx; 
      
      T x;
      T y;
      T z;
      
      file1 >> external_idx >> x >> y >> z;
      
      assert( external_idx < cnt_vertices || !"read_tetgen(): input error");
      
      assert( is_number(x) || !"read_tetgen(): x was nan");
      assert( is_finite(x) || !"read_tetgen(): x was infinity");
      assert( is_number(y) || !"read_tetgen(): y was nan");
      assert( is_finite(y) || !"read_tetgen(): y was infinity");
      assert( is_number(z) || !"read_tetgen(): z was nan");
      assert( is_finite(z) || !"read_tetgen(): z was infinity");
      
      Vertex v = mesh.push_vertex();
      
      X(v) = x;
      Y(v) = y;
      Z(v) = z;
      
      vertices[external_idx] = v;
    }
    
    for(size_t idx=0u; idx<cnt_triangles; ++idx)
    {
      size_t external_idx;
      
      size_t i;   // external index of node i
      size_t j;   // external index of node j
      size_t k;   // external index of node k
      
      file2 >> external_idx >> i >> j >> k;
      
      assert( external_idx < cnt_triangles || !"read_tetgen(): input error");
      assert( i < cnt_vertices             || !"read_tetgen(): input error");
      assert( j < cnt_vertices             || !"read_tetgen(): input error");
      assert( k < cnt_vertices             || !"read_tetgen(): input error");
      
      mesh.push_triangle(
                         vertices[i]
                         , vertices[j]
                         , vertices[k]
                         );
    }
    
    file1.close();
    file2.close();
  }
  
  template
  void read_tetgen<float>(
                          std::string const & filename
                          , T3Mesh & mesh
                          , VertexAttribute<float,T3Mesh> & X
                          , VertexAttribute<float,T3Mesh> & Y
                          , VertexAttribute<float,T3Mesh> & Z
                          );
  
  template
  void read_tetgen<double>(
                           std::string const & filename
                           , T3Mesh & mesh
                           , VertexAttribute<double,T3Mesh> & X
                           , VertexAttribute<double,T3Mesh> & Y
                           , VertexAttribute<double,T3Mesh> & Z
                           );
  
  template<typename T>
  void read_tetgen(
                   std::string const & filename
                   , T4Mesh & mesh
                   , VertexAttribute<T,T4Mesh> & X
                   , VertexAttribute<T,T4Mesh> & Y
                   , VertexAttribute<T,T4Mesh> & Z
                   )
  {    
    boost::filesystem::path  mypath(filename);
    
    std::string const ext = boost::filesystem::extension( mypath );
    
    assert( ext == "" || !"read_tetgen(): illegal file name extension");
    
    std::string const filename1 = filename + ".node";
    std::string const filename2 = filename + ".ele";
    
    std::ifstream file1(filename1.c_str());
    std::ifstream file2(filename2.c_str());
    
    assert (file1.is_open() || !"read_tetgen(): unable to open file");
    assert (file2.is_open() || !"read_tetgen(): unable to open file");
    
    size_t cnt_vertices;     // Total number of vertices 
    size_t dimension;        // The number of coordinates per vertex
    size_t cnt_attributes;   // The number of attributes per vertex
    size_t cnt_tetrahedra;   // Total number of tetrahedra
    size_t cnt_points;       // The number of vertices per tetrahedra (always 4)
    bool has_boundary;       // Boundary marker
    
    file1 >> cnt_vertices   >> dimension  >> cnt_attributes >> has_boundary;    
    file2 >> cnt_tetrahedra >> cnt_points >> cnt_attributes;
    
    assert (cnt_vertices > 0u    || !"read_tetgen(): unsupported format");
    assert (dimension == 3u      || !"read_tetgen(): unsupported format");
    assert (cnt_attributes == 0u || !"read_tetgen(): unsupported format");
    assert (cnt_tetrahedra > 0u  || !"read_tetgen(): unsupported format");
    assert (cnt_points == 4u     || !"read_tetgen(): unsupported format");
    
    mesh.clear();
    
    mesh.set_capacity(cnt_vertices, cnt_tetrahedra);
    
    X.bind(mesh);
    Y.bind(mesh);
    Z.bind(mesh);
    
    std::vector<Vertex> vertices (cnt_vertices);
    
    for(size_t idx=0u; idx<cnt_vertices; ++idx)
    {
      size_t external_idx;
      
      T x;
      T y;
      T z;
      
      file1 >> external_idx >> x >> y >> z;      
      
      assert( external_idx < cnt_vertices || !"read_tetgen(): input error");
      
      assert( is_number(x) || !"read_tetgen(): x was nan");
      assert( is_finite(x) || !"read_tetgen(): x was infinity");
      assert( is_number(y) || !"read_tetgen(): y was nan");
      assert( is_finite(y) || !"read_tetgen(): y was infinity");
      assert( is_number(z) || !"read_tetgen(): z was nan");
      assert( is_finite(z) || !"read_tetgen(): z was infinity");
      
      
      Vertex v = mesh.push_vertex();
      
      X(v) = x;
      Y(v) = y;
      Z(v) = z;
      
      vertices[external_idx] = v;
    }
    
    for(size_t idx=0u; idx<cnt_tetrahedra; ++idx)
    {
      size_t external_idx;
      
      size_t i;  // external index of node i
      size_t j;  // external index of node j
      size_t k;  // external index of node k
      size_t m;  // external index of node m
      
      file2 >> external_idx >> i >> j >> k >> m;
      
      assert( external_idx < cnt_tetrahedra || !"read_tetgen(): input error");
      assert( i < cnt_vertices              || !"read_tetgen(): input error");
      assert( j < cnt_vertices              || !"read_tetgen(): input error");
      assert( k < cnt_vertices              || !"read_tetgen(): input error");
      assert( m < cnt_vertices              || !"read_tetgen(): input error");
      
      mesh.push_tetrahedron(
                            vertices[i]
                            , vertices[j]
                            , vertices[k]
                            , vertices[m]
                            );
    }
    
    file1.close();
    file2.close();
  }
  
  template
  void read_tetgen<float>(
                          std::string const & filename
                          , T4Mesh & mesh
                          , VertexAttribute<float,T4Mesh> & X
                          , VertexAttribute<float,T4Mesh> & Y
                          , VertexAttribute<float,T4Mesh> & Z
                          );
  
  template
  void read_tetgen<double>(
                           std::string const & filename
                           , T4Mesh & mesh
                           , VertexAttribute<double,T4Mesh> & X
                           , VertexAttribute<double,T4Mesh> & Y
                           , VertexAttribute<double,T4Mesh> & Z
                           );
  
  template<typename T>
  void write_tetgen(
                    std::string const & filename
                    , T3Mesh const & mesh
                    , VertexAttribute<T,T3Mesh> const & X
                    , VertexAttribute<T,T3Mesh> const & Y
                    , VertexAttribute<T,T3Mesh> const & Z
                    )
  {
    boost::filesystem::path  mypath(filename);
    
    std::string const ext = boost::filesystem::extension( mypath );
    
    assert( ext == "" || !"write_tetgen(): illegal file name extension");
    
    std::string const filename1 = filename + ".node";
    std::string const filename2 = filename + ".face";
    
    std::ofstream file1(filename1.c_str());
    std::ofstream file2(filename2.c_str());
    
    assert( file1.is_open() || !"write_tetgen(): could not open file");
    assert( file2.is_open() || !"write_tetgen(): could not open file");
    
    file1.precision(30);
    file2.precision(30);
    
    size_t const cnt_nodes     = mesh.vertex_size();
    size_t const cnt_triangles = mesh.triangle_size();
    
    file1 << cnt_nodes 
    << " " 
    << 3u 
    << " " 
    << 0u 
    << " " 
    << 0u 
    << std::endl;
    
    file2 << cnt_triangles 
    << " " 
    << 3u
    << " "
    << 0u 
    << std::endl;
    
    for(size_t idx=0u; idx<cnt_nodes; ++idx)
    {
      Vertex v = mesh.vertex(idx);
      
      file1 << "  " 
      << v.idx() 
      << "  " 
      << X(v) 
      << " " 
      << Y(v) 
      << " " 
      << Z(v) 
      << std::endl;
    }
    
    for(size_t idx=0u; idx<cnt_triangles; ++idx)
    {
      Triangle t = mesh.triangle(idx);
      
      file2 << t.idx()
      << "  "
      << t.i() 
      << " " 
      << t.j() 
      << " " 
      << t.k() 
      << std::endl;
    }
    
    file1.flush();
    file2.flush();
    file1.close();
    file2.close();
  }
  
  template
  void write_tetgen<float>(
                           std::string const & filename
                           , T3Mesh const & mesh
                           , VertexAttribute<float,T3Mesh> const & X
                           , VertexAttribute<float,T3Mesh> const & Y
                           , VertexAttribute<float,T3Mesh> const & Z
                           );
  
  template
  void write_tetgen<double>(
                            std::string const & filename
                            , T3Mesh const & mesh
                            , VertexAttribute<double,T3Mesh> const & X
                            , VertexAttribute<double,T3Mesh> const & Y
                            , VertexAttribute<double,T3Mesh> const & Z
                            );
  
  template<typename T>
  void write_tetgen(
                    std::string const & filename
                    , T4Mesh const & mesh
                    , VertexAttribute<T,T4Mesh> const & X
                    , VertexAttribute<T,T4Mesh> const & Y
                    , VertexAttribute<T,T4Mesh> const & Z
                    )
  {
    boost::filesystem::path  mypath(filename);
    
    std::string const ext = boost::filesystem::extension( mypath );
    
    assert( ext == "" || !"write_tetgen(): illegal file name extension");
    
    std::string const filename1 = filename + ".node";
    std::string const filename2 = filename + ".ele";
    
    std::ofstream file1(filename1.c_str());
    std::ofstream file2(filename2.c_str());
    
    assert(file1.is_open() || !"write_tetgen(): could not open file");
    assert(file2.is_open() || !"write_tetgen(): could not open file");
    
    file1.precision(30);
    file2.precision(30);
    
    size_t const cnt_nodes       = mesh.vertex_size();
    size_t const cnt_tetrahedron = mesh.tetrahedron_size();
    
    file1 << cnt_nodes
    << " " 
    << 3u 
    << " " 
    << 0u 
    << " " 
    << 0u 
    << std::endl;
    
    file2 << cnt_tetrahedron 
    << " " 
    << 4u 
    << " "
    << 0u 
    << std::endl;
    
    for(size_t idx=0u; idx<cnt_nodes; ++idx)
    {
      Vertex v = mesh.vertex(idx);
      
      file1 << "  " 
      << v.idx()
      << "  " 
      << X(v) 
      << " " 
      << Y(v) 
      << " " 
      << Z(v) 
      << std::endl;
    }
    
    for(size_t idx=0u; idx<cnt_tetrahedron; ++idx)
    {
      Tetrahedron t = mesh.tetrahedron(idx);
      
      file2 << t.idx()
      << "  " 
      << t.i() 
      << " " 
      << t.j() 
      << " " 
      << t.k() 
      << " " 
      << t.m() 
      << std::endl;
    }
    
    file1.flush();
    file2.flush();
    file1.close();
    file2.close();
  }
  
  template
  void write_tetgen<float>(
                           std::string const & filename
                           , T4Mesh const & mesh
                           , VertexAttribute<float,T4Mesh> const & X
                           , VertexAttribute<float,T4Mesh> const & Y
                           , VertexAttribute<float,T4Mesh> const & Z
                           );
  
  template
  void write_tetgen<double>(
                            std::string const & filename
                            , T4Mesh const & mesh
                            , VertexAttribute<double,T4Mesh> const & X
                            , VertexAttribute<double,T4Mesh> const & Y
                            , VertexAttribute<double,T4Mesh> const & Z
                            );
  
} // end namespace mesh_array
