#include <mesh_array_io_matlab.h>

#include <tiny.h>

#include <util_string_helper.h>

#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>

#include <cassert>
#include <fstream>

namespace mesh_array 
{
  
  template<typename T>
  void read_matlab(
                   std::string const & filename
                   , T3Mesh & mesh
                   , VertexAttribute<T,T3Mesh> & X
                   , VertexAttribute<T,T3Mesh> & Y
                   , VertexAttribute<T,T3Mesh> & Z
                   )
  {        
    boost::filesystem::path  mypath(filename);
    
    std::string const ext = boost::filesystem::extension( mypath );
    
    assert( ext == ".m" || !"read_matlab(): illegal file name extension");
    
    bool const tst = boost::filesystem::exists( mypath );
    
    assert( tst|| !"read_matlab(): file did not exist");
    
    typedef boost::tokenizer< boost::char_separator<char> > Tokenizer;
    boost::char_separator<char> sep(" XYZT=[];");
    
    std::ifstream file(filename.c_str());
    
    assert (file.is_open() || !"read_matlab(): unable to open file");
    
    std::vector<T> px;
    std::vector<T> py;
    std::vector<T> pz;
    std::vector<size_t> I;
    std::vector<size_t> J;
    std::vector<size_t> K;
    
    // matlab format is (remeber indexing in matlab is one-based )
    // X = [ x1 x2 ... xN];
    // Y = [ y1 y2 ... yN];
    // Z = [ z1 z2 ... zN];
    // T = [ i1 j1 k1; i2 j2 k2; ... iK jK kK ];
        
    std::string line;
    
    assert( !file.eof() || !"read_matlab(): EOF");
    getline(file,line);
    {
      Tokenizer tokens(line, sep);
      for (Tokenizer::iterator tok = tokens.begin(); tok != tokens.end(); ++tok)
      {
        T const x = util::to_value<T>( *tok );
        
        assert( is_number(x) || !"read_matlab(): nan");
        assert( is_finite(x) || !"read_matlab(): inf");
  
        px.push_back( x );      
      }
    }
    assert( !file.eof() || !"read_matlab(): EOF");
    getline(file,line);
    {
      Tokenizer tokens(line, sep);
      for (Tokenizer::iterator tok = tokens.begin(); tok != tokens.end(); ++tok)
      {
        T const y = util::to_value<T>( *tok );
        
        assert( is_number(y) || !"read_matlab(): nan");
        assert( is_finite(y) || !"read_matlab(): inf");
        
        py.push_back( y );      
      }
    }
    assert( !file.eof() || !"read_matlab(): EOF");
    getline(file,line);
    {
      Tokenizer tokens(line, sep);
      for (Tokenizer::iterator tok = tokens.begin(); tok != tokens.end(); ++tok)
      {
        T const z = util::to_value<T>( *tok );
        
        assert( is_number(z) || !"read_matlab(): nan");
        assert( is_finite(z) || !"read_matlab(): inf");
        
        pz.push_back( z );      
      }
    }
    assert( !file.eof() || !"read_matlab(): EOF");
    getline(file,line);
    {      
      Tokenizer tokens(line, sep);
      for (Tokenizer::iterator tok = tokens.begin(); tok != tokens.end();)
      {
        size_t const i = util::to_value<size_t>( *tok ) - 1u;
        ++tok;
        size_t const j = util::to_value<size_t>( *tok ) - 1u;
        ++tok;
        size_t const k = util::to_value<size_t>( *tok ) - 1u;
        ++tok;
        
        I.push_back(i);
        J.push_back(j);
        K.push_back(k);
      }
    }
    
    file.close();

    size_t const cntV = px.size();
    size_t const cntT = I.size();
      
    mesh.clear();
    
    mesh.set_capacity(cntV,cntT);
    
    X.bind(mesh);
    Y.bind(mesh);
    Z.bind(mesh);
        
    for(size_t idx=0u; idx < cntV; ++idx)
    {
      Vertex v = mesh.push_vertex();
      X(v) = px[idx];
      Y(v) = py[idx];
      Z(v) = pz[idx];
    }    
    for(size_t idx=0u; idx< cntT; ++idx)
    {
      Vertex const & vi = mesh.vertex( I[idx] );
      Vertex const & vj = mesh.vertex( J[idx] );
      Vertex const & vk = mesh.vertex( K[idx] );
      
      mesh.push_triangle(vi, vj, vk);
    }
  }
  
  template
  void read_matlab<float>(
                   std::string const & filename
                   , T3Mesh & mesh
                   , VertexAttribute<float,T3Mesh> & X
                   , VertexAttribute<float,T3Mesh> & Y
                   , VertexAttribute<float,T3Mesh> & Z
                   );

  template
  void read_matlab<double>(
                   std::string const & filename
                   , T3Mesh & mesh
                   , VertexAttribute<double,T3Mesh> & X
                   , VertexAttribute<double,T3Mesh> & Y
                   , VertexAttribute<double,T3Mesh> & Z
                   );
  
  template<typename T>
  void read_matlab(
                   std::string const & filename
                   , T4Mesh & mesh
                   , VertexAttribute<T,T4Mesh> & X
                   , VertexAttribute<T,T4Mesh> & Y
                   , VertexAttribute<T,T4Mesh> & Z
                   )
  {        
    boost::filesystem::path  mypath(filename);
    
    std::string const ext = boost::filesystem::extension( mypath );
    
    assert( ext == ".m" || !"read_matlab(): illegal file name extension");
    
    bool const tst = boost::filesystem::exists( mypath );
    
    assert( tst|| !"read_matlab(): file did not exist");
    
    typedef boost::tokenizer< boost::char_separator<char> > Tokenizer;
    boost::char_separator<char> sep(" XYZT=[];");
    
    std::ifstream file(filename.c_str());
    
    assert (file.is_open() || !"read_matlab(): unable to open file");
    
    std::vector<T> px;
    std::vector<T> py;
    std::vector<T> pz;
    std::vector<size_t> I;
    std::vector<size_t> J;
    std::vector<size_t> K;
    std::vector<size_t> M;
    
    // matlab format is (remeber indexing in matlab is one-based )
    // X = [ x1 x2 ... xN];
    // Y = [ y1 y2 ... yN];
    // Z = [ z1 z2 ... zN];
    // T = [ i1 j1 k1 m1; i2 j2 k2 m2; ... iK jK kK mK ];
    
    std::string line;
    
    assert( !file.eof() || !"read_matlab(): EOF");
    getline(file,line);
    {
      Tokenizer tokens(line, sep);
      for (Tokenizer::iterator tok = tokens.begin(); tok != tokens.end(); ++tok)
      {
        T const x = util::to_value<T>( *tok );
        
        assert( is_number(x) || !"read_matlab(): nan");
        assert( is_finite(x) || !"read_matlab(): inf");
        
        px.push_back( x );      
      }
    }
    assert( !file.eof() || !"read_matlab(): EOF");
    getline(file,line);
    {
      Tokenizer tokens(line, sep);
      for (Tokenizer::iterator tok = tokens.begin(); tok != tokens.end(); ++tok)
      {
        T const y = util::to_value<T>( *tok );
        
        assert( is_number(y) || !"read_matlab(): nan");
        assert( is_finite(y) || !"read_matlab(): inf");
        
        py.push_back( y );      
      }
    }
    assert( !file.eof() || !"read_matlab(): EOF");
    getline(file,line);
    {
      Tokenizer tokens(line, sep);
      for (Tokenizer::iterator tok = tokens.begin(); tok != tokens.end(); ++tok)
      {
        T const z = util::to_value<T>( *tok );
        
        assert( is_number(z) || !"read_matlab(): nan");
        assert( is_finite(z) || !"read_matlab(): inf");
        
        pz.push_back( z );      
      }
    }
    assert( !file.eof() || !"read_matlab(): EOF");
    getline(file,line);
    {      
      Tokenizer tokens(line, sep);
      for (Tokenizer::iterator tok = tokens.begin(); tok != tokens.end();)
      {
        size_t const i = util::to_value<size_t>( *tok ) - 1u;
        ++tok;
        size_t const j = util::to_value<size_t>( *tok ) - 1u;
        ++tok;
        size_t const k = util::to_value<size_t>( *tok ) - 1u;
        ++tok;
        size_t const m = util::to_value<size_t>( *tok ) - 1u;
        ++tok;
        
        I.push_back(i);
        J.push_back(j);
        K.push_back(k);
        M.push_back(m);
      }
    }
    
    file.close();
    
    size_t const cntV = px.size();
    size_t const cntT = I.size();
    
    mesh.clear();
    
    mesh.set_capacity(cntV,cntT);
    
    X.bind(mesh);
    Y.bind(mesh);
    Z.bind(mesh);
    
    for(size_t idx=0u; idx < cntV; ++idx)
    {
      Vertex v = mesh.push_vertex();
      X(v) = px[idx];
      Y(v) = py[idx];
      Z(v) = pz[idx];
    }    
    for(size_t idx=0u; idx< cntT; ++idx)
    {
      Vertex const & vi = mesh.vertex( I[idx] );
      Vertex const & vj = mesh.vertex( J[idx] );
      Vertex const & vk = mesh.vertex( K[idx] );
      Vertex const & vm = mesh.vertex( M[idx] );
      
      mesh.push_tetrahedron(vi, vj, vk, vm);
    }
  }
  
  template
  void read_matlab<float>(
                   std::string const & filename
                   , T4Mesh & mesh
                   , VertexAttribute<float,T4Mesh> & X
                   , VertexAttribute<float,T4Mesh> & Y
                   , VertexAttribute<float,T4Mesh> & Z
                   );

  template
  void read_matlab<double>(
                   std::string const & filename
                   , T4Mesh & mesh
                   , VertexAttribute<double,T4Mesh> & X
                   , VertexAttribute<double,T4Mesh> & Y
                   , VertexAttribute<double,T4Mesh> & Z
                   );
  
  template<typename T>
  void write_matlab(
                    std::string const & filename
                    , T3Mesh const & mesh
                    , VertexAttribute<T,T3Mesh> const & X
                    , VertexAttribute<T,T3Mesh> const & Y
                    , VertexAttribute<T,T3Mesh> const & Z
                    )
  {    
    boost::filesystem::path  mypath(filename);
    
    std::string const ext = boost::filesystem::extension( mypath );
    
    assert( ext == ".m" || !"write_matlab(): illegal file name extension");
    
    std::ofstream file (filename.c_str());
    
    assert( file.is_open() || !"write_matlab(): could not open file");
    
    file.precision(30);
    
    size_t const cntV = mesh.vertex_size();
    
    file << "X = [";
    for(size_t idx = 0u; idx < cntV; ++idx)
    {
      Vertex const & vertex = mesh.vertex(idx);
      file << " " << X(vertex);
    }
    file << " ];" << std::endl;

    file << "Y = [";
    for(size_t idx = 0u; idx < cntV; ++idx)
    {
      Vertex const & vertex = mesh.vertex(idx);
      file << " " << Y(vertex);
    }
    file << " ];" << std::endl;
    
    file << "Z = [";
    for(size_t idx = 0u; idx < cntV; ++idx)
    {
      Vertex const & vertex = mesh.vertex(idx);
      file << " " << Z(vertex);
    }
    file << " ];" << std::endl;
        
    size_t const cntT = mesh.triangle_size();
    
    file << "T = [";
    for(size_t idx = 0u; idx < cntT; ++idx)
    {
      Triangle const & triangle = mesh.triangle(idx);
      size_t const i = triangle.i() + 1u;
      size_t const j = triangle.j() + 1u;
      size_t const k = triangle.k() + 1u;
      file << " " << i << " " << j << " " << k << ";";
    }
    file << " ];" << std::endl;

    file.flush();
    file.close();
  }
  
  template
  void write_matlab<float>(
                    std::string const & filename
                    , T3Mesh const & mesh
                    , VertexAttribute<float,T3Mesh> const & X
                    , VertexAttribute<float,T3Mesh> const & Y
                    , VertexAttribute<float,T3Mesh> const & Z
                    );

  template
  void write_matlab<double>(
                    std::string const & filename
                    , T3Mesh const & mesh
                    , VertexAttribute<double,T3Mesh> const & X
                    , VertexAttribute<double,T3Mesh> const & Y
                    , VertexAttribute<double,T3Mesh> const & Z
                    );
  
  template<typename T>
  void write_matlab(
                    std::string const & filename
                    , T4Mesh const & mesh
                    , VertexAttribute<T,T4Mesh> const & X
                    , VertexAttribute<T,T4Mesh> const & Y
                    , VertexAttribute<T,T4Mesh> const & Z
                    )
  {
    boost::filesystem::path  mypath(filename);
    
    std::string const ext = boost::filesystem::extension( mypath );
    
    assert( ext == ".m" || !"write_matlab(): illegal file name extension");
    
    std::ofstream file (filename.c_str());
    
    assert( file.is_open() || !"write_matlab(): could not open file");
    
    file.precision(30);
    
    size_t const cntV = mesh.vertex_size();
    
    file << "X = [";
    for(size_t idx = 0u; idx < cntV; ++idx)
    {
      Vertex const & vertex = mesh.vertex(idx);
      file << " " << X(vertex);
    }
    file << " ];" << std::endl;
    
    file << "Y = [";
    for(size_t idx = 0u; idx < cntV; ++idx)
    {
      Vertex const & vertex = mesh.vertex(idx);
      file << " " << Y(vertex);
    }
    file << " ];" << std::endl;
    
    file << "Z = [";
    for(size_t idx = 0u; idx < cntV; ++idx)
    {
      Vertex const & vertex = mesh.vertex(idx);
      file << " " << Z(vertex);
    }
    file << " ];" << std::endl;
    
    size_t const cntT = mesh.tetrahedron_size();
    
    file << "T = [";
    for(size_t idx = 0u; idx < cntT; ++idx)
    {
      Tetrahedron const & tetrahedron = mesh.tetrahedron(idx);
      size_t const i = tetrahedron.i() + 1u;
      size_t const j = tetrahedron.j() + 1u;
      size_t const k = tetrahedron.k() + 1u;
      size_t const m = tetrahedron.m() + 1u;
      file << " " << i << " " << j << " " << k << " " << m << ";";
    }
    file << " ];" << std::endl;
    
    file.flush();
    file.close();
  }
  
  template
  void write_matlab<float>(
                    std::string const & filename
                    , T4Mesh const & mesh
                    , VertexAttribute<float,T4Mesh> const & X
                    , VertexAttribute<float,T4Mesh> const & Y
                    , VertexAttribute<float,T4Mesh> const & Z
                    );

  template
  void write_matlab<double>(
                    std::string const & filename
                    , T4Mesh const & mesh
                    , VertexAttribute<double,T4Mesh> const & X
                    , VertexAttribute<double,T4Mesh> const & Y
                    , VertexAttribute<double,T4Mesh> const & Z
                    );
  
} // end namespace mesh_array

