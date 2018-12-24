#include <mesh_array_io_obj.h>

#include <tiny.h>

#include <util_log.h>

#include <boost/algorithm/string.hpp>  //--- needed for split and is_any_of function
#include <boost/lexical_cast.hpp>      //--- needed for lexical cast function
#include <boost/filesystem.hpp>        //--- needed for boost::filesystem::path

#include <cassert>
#include <sstream>
#include <fstream>
#include <vector>
#include <stdexcept>

namespace mesh_array
{
 
  template<typename T>
  void read_obj(
                std::string const & filename
                , T3Mesh & mesh
                , VertexAttribute<T,T3Mesh> & X
                , VertexAttribute<T,T3Mesh> & Y
                , VertexAttribute<T,T3Mesh> & Z
                )
  {    
    boost::filesystem::path  mypath(filename);
    
    std::string const ext = boost::filesystem::extension( mypath );
    
    assert( ext == ".obj" || !"read_obj(): illegal file name extension");

    if( ext != ".obj")
    {
      util::Log logging;

      logging << "mesh_array::read_obj(...): illegal file name extension" << util::Log::newline();

      throw std::logic_error("Filename extension must be obj");

      return;
    }
    
    bool const did_exist = boost::filesystem::exists( mypath );
    
    assert( did_exist || !"read_obj(): file did not exist");

    if (!did_exist)
    {
      util::Log logging;

      logging << "mesh_array::read_obj(...): file did not exist" << util::Log::newline();

      throw std::logic_error("Filename did not exist");

      return;
    }

    typedef std::vector<T>       fcontainer;
    typedef std::vector<size_t>  icontainer;
    
    std::ifstream file(filename.c_str());
    
    assert(file.is_open() || !"read_obj(): could not open file");
    
    fcontainer px;
    fcontainer py;
    fcontainer pz;
    fcontainer nx;
    fcontainer ny;
    fcontainer nz;
    fcontainer tx;
    fcontainer ty;
    fcontainer tz;
    icontainer I;
    icontainer J;
    icontainer K;
    
    while(!file.eof())
    {
      std::string line;
      
      if(! getline(file,line) )
        break;
      
      std::stringstream stream(line);
      
      std::string type;
      stream >> type;
      
      if(line[0] == '#')
      {
        continue;
      }
      else if(type == "g")
      {
        std::string group_name;
        stream >> group_name;
        
        // create new mesh with name=group name!!!
        
        continue;
      }
      else if(type == "usemtl")
      {
        std::string material_name;
        stream >> material_name;
        
        // material = materials[ material_name ];
        
        continue;
      }
      else if(type == "mtllib")
      {
        std::string material_filename;
        stream >> material_filename;
        
        // read materials from material_filename
        
        continue;
      }
      else if(type == "v")
      {
        T value;
        
        stream >> value;        
        assert(is_number(value) || !"read_obj(): nan");
        assert(is_finite(value) || !"read_obj(): inf");
        px.push_back(value);
        
        stream >> value;
        assert(is_number(value) || !"read_obj(): nan");
        assert(is_finite(value) || !"read_obj(): inf");
        py.push_back(value);
        
        stream >> value;
        assert(is_number(value) || !"read_obj(): nan");
        assert(is_finite(value) || !"read_obj(): inf");
        pz.push_back(value);
      }
      else if(type == "vt")
      {
        T value;
        
        stream >> value;
        assert(is_number(value) || !"read_obj(): nan");
        assert(is_finite(value) || !"read_obj(): inf");
        tx.push_back(value);
        
        if(!stream.eof())
        {
          stream >> value;
          assert(is_number(value) || !"read_obj(): nan");
          assert(is_finite(value) || !"read_obj(): inf");
          ty.push_back(value);
        }
        
        if(!stream.eof())
        {
          stream >> value;
          assert(is_number(value) || !"read_obj(): nan");
          assert(is_finite(value) || !"read_obj(): inf");
          tz.push_back(value);
        }          
      }
      else if(type == "vn")
      {
        T value;
        
        stream >> value;
        assert(is_number(value) || !"read_obj(): nan");
        assert(is_finite(value) || !"read_obj(): inf");
        nx.push_back(value);
        
        stream >> value;
        assert(is_number(value) || !"read_obj(): nan");
        assert(is_finite(value) || !"read_obj(): inf");
        ny.push_back(value);
        
        stream >> value;
        assert(is_number(value) || !"read_obj(): nan");
        assert(is_finite(value) || !"read_obj(): inf");
        nz.push_back(value);
      }
      else if(type == "f")
      {
        icontainer face;
        
        //--- parse:
        //---  f v0_idx//vn0_idx//vt0_idx  ...  vN_idx//vnN_idx//vtN_idx
        while(!stream.eof())
        {
          std::string raw;
          stream >> raw;
          
          // NOTE: hd 20060605 - hack to let linux version gracefully read windows-formatted files.
          if (raw.empty())
            break;
          
          std::vector<std::string> split_vector;
          boost::algorithm::split(
                                  split_vector
                                  , raw
                                  , boost::algorithm::is_any_of("/")
                                  , boost::algorithm::token_compress_on
                                  );
          
          assert(!split_vector.empty() || !"obj_read() : Internal error, no indices on face");
          
          //size_t const cnt = split_vector.size();  // if 1 then only coords, 2 coords+normal, 3 coords+normal+texture 
                    
          //std::vector<std::string>::iterator value = split_vector.begin();
          //std::vector<std::string>::iterator end   = split_vector.end();
          //for(;value != end; ++value)
          //  face.push_back(boost::lexical_cast<size_t>(*value));  // if we wanted to extract texture and normal indices too
          
          size_t const idx = boost::lexical_cast<size_t>( split_vector[0] );
          face.push_back( idx ); // we only care about vertex indices

        }
        
        // We assume that a polygonal face is convex so we perform ear clipping algorithm
        // if more than 3 vertices in polygon face
        size_t const triangles =  face.size() - 2u;
        for(size_t t = 0u;t<triangles;++t)
        {
          size_t const i = face[0u]     - 1u;   // obj files uses one-based indexing so we convert to zero-based indexing 
          size_t const j = face[t+1u]   - 1u;
          size_t const k = face[t+2u]   - 1u;
          
          I.push_back(i);
          J.push_back(j);
          K.push_back(k);
        }
        
      }
      else
      {
        util::Log logging;

        logging << "read_obj_materials(): skipping "<< type << util::Log::newline();
      }
    }
    file.close();
    
    
    size_t const cntV = px.size();
    size_t const cntT = I.size();
    
    assert(cntV > 0u || !"read_obj(): no vertices");
    assert(cntT > 0u || !"read_obj(): no triangles");
    
    mesh.clear();
    
    mesh.set_capacity(cntV, cntT);
    
    X.bind(mesh);
    Y.bind(mesh);
    Z.bind(mesh);
    
    std::vector<Vertex> vertices (cntV);
    
    for(size_t idx=0u; idx<cntV; ++idx)
    {
      Vertex v = mesh.push_vertex();
      
      X(v) = px[idx];
      Y(v) = py[idx];
      Z(v) = pz[idx];
      
      vertices[idx] = v;
    }
    
    for(size_t idx=0u; idx<cntT; ++idx)
    {
      size_t const i = I[idx];
      size_t const j = J[idx];
      size_t const k = K[idx];
      
      mesh.push_triangle(vertices[i],vertices[j],vertices[k]);
    }
  }
  
  
  template
  void read_obj<float>(
                std::string const & filename
                , T3Mesh & mesh
                , VertexAttribute<float,T3Mesh> & X
                , VertexAttribute<float,T3Mesh> & Y
                , VertexAttribute<float,T3Mesh> & Z
                );

  template
  void read_obj<double>(
                std::string const & filename
                , T3Mesh & mesh
                , VertexAttribute<double,T3Mesh> & X
                , VertexAttribute<double,T3Mesh> & Y
                , VertexAttribute<double,T3Mesh> & Z
                );
  
  
  template<typename T>
  void write_obj(
                 std::string const & filename
                 , T3Mesh const & mesh
                 , VertexAttribute<T,T3Mesh> const & X
                 , VertexAttribute<T,T3Mesh> const & Y
                 , VertexAttribute<T,T3Mesh> const & Z
                 )
  {    
    boost::filesystem::path  mypath(filename);
    
    std::string const ext = boost::filesystem::extension( mypath );
    
    assert( ext == ".obj" || !"write_obj(): illegal file name extension");
    
    std::ofstream file(filename.c_str());
    
    assert( file.is_open() || !"write_obj(): could not open file");
    
    file << "g group0"  << std::endl;
    
    file.precision(30);
    
    size_t const cntV = mesh.vertex_size();
    
    for(size_t idx = 0u; idx < cntV; ++idx)
    {
      Vertex const & vertex = mesh.vertex( idx );
      
      file << "v " << X(vertex) << ' ' << Y(vertex)  << ' ' << Z(vertex)  << std::endl;
      //file << "vt " << TX(vertex) << ' ' << TY(vertex)  << ' ' << TZ(vertex)  << std::endl;
      //file << "vn " << NX(vertex) << ' ' << NY(vertex)  << ' ' << NZ(vertex)  << std::endl;
    }
    
    size_t const cntT = mesh.triangle_size();
    for(size_t idx = 0u; idx <cntT; ++idx)
    {
      Triangle const & triangle = mesh.triangle(idx);
      
      file << "f";
      
      size_t const i = triangle.i() + 1u; //--- need one-based indices!!!
      size_t const j = triangle.j() + 1u; //--- need one-based indices!!!
      size_t const k = triangle.k() + 1u; //--- need one-based indices!!!
      
      file << " " 
      << i   /*   << "/" << ni << "/" ti */ 
      << " " 
      << j   /*   << "/" << nj << "/" tj */ 
      << " " 
      << k   /*   << "/" << nk << "/" tk */ 
      << std::endl;
    }
    file.flush();
    file.close();
  }

  template
  void write_obj<float>(
                 std::string const & filename
                 , T3Mesh const & mesh
                 , VertexAttribute<float,T3Mesh> const & X
                 , VertexAttribute<float,T3Mesh> const & Y
                 , VertexAttribute<float,T3Mesh> const & Z
                 );

  
  template
  void write_obj<double>(
                 std::string const & filename
                 , T3Mesh const & mesh
                 , VertexAttribute<double,T3Mesh> const & X
                 , VertexAttribute<double,T3Mesh> const & Y
                 , VertexAttribute<double,T3Mesh> const & Z
                 );
  
  
  
} // namespace mesh_array
