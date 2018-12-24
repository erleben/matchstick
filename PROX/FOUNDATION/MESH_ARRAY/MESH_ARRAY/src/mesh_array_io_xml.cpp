#include <mesh_array_io_xml.h>
#include <util_string_helper.h>

#include <tiny.h>

#include <tinyxml.h>

#include <boost/filesystem.hpp>

#include <cassert>

namespace mesh_array
{
  
  template<typename T>
  void read_xml(
                std::string const & filename
                , T4Mesh & mesh
                , VertexAttribute<T,T4Mesh> & X
                , VertexAttribute<T,T4Mesh> & Y
                , VertexAttribute<T,T4Mesh> & Z
                )
  {
    boost::filesystem::path  mypath(filename);
    
    std::string const ext = boost::filesystem::extension( mypath );
    
    assert( ext == ".xml" || !"read_xml(): illegal file name extension");
        
#ifdef TIXML_USE_STL
    TiXmlDocument xml_document(filename);
#else
    TiXmlDocument xml_document(filename.c_str());
#endif
    
    assert( xml_document.LoadFile() || !"read_xml(): file not found");
    
    TiXmlHandle document_handle( &xml_document );
    
    TiXmlElement * xml_t4mesh = document_handle.FirstChild( "T4MESH" ).Element();
    
    assert( xml_t4mesh                          || !"read_xml(): missing T4MESH tag");
    assert( xml_t4mesh->Attribute("nodes")      || !"read_xml(): missing nodes attribute");
    assert( xml_t4mesh->Attribute("tetrahedra") || !"read_xml(): missing tetrahedra attribute");
    
    size_t const cntV = util::to_value<size_t>(  xml_t4mesh->Attribute("nodes")  );
    size_t const cntT = util::to_value<size_t>(  xml_t4mesh->Attribute("tetrahedra")  );
    
    mesh.clear();
    
    mesh.set_capacity( cntV, cntT);
    
    for(size_t idx=0u; idx < cntV; ++idx)
      mesh.push_vertex();
    
    TiXmlElement * xml_tetrahedron = document_handle.FirstChild( "T4MESH" ).FirstChild( "TETRAHEDRON" ).Element();
    
    for( ; xml_tetrahedron; xml_tetrahedron=xml_tetrahedron->NextSiblingElement("TETRAHEDRON") )
    {
      assert( xml_tetrahedron->Attribute("i") || !"read_xml(): missing i index");
      assert( xml_tetrahedron->Attribute("j") || !"read_xml(): missing j index");
      assert( xml_tetrahedron->Attribute("k") || !"read_xml(): missing k index");
      assert( xml_tetrahedron->Attribute("m") || !"read_xml(): missing m index");
            
      size_t const i = util::to_value<size_t>( xml_tetrahedron->Attribute("i") );
      size_t const j = util::to_value<size_t>( xml_tetrahedron->Attribute("j") );
      size_t const k = util::to_value<size_t>( xml_tetrahedron->Attribute("k") );
      size_t const m = util::to_value<size_t>( xml_tetrahedron->Attribute("m") );
      
      assert( i < cntV || !"read_xml(): i was out of range");
      assert( j < cntV || !"read_xml(): j was out of range");
      assert( k < cntV || !"read_xml(): k was out of range");
      assert( m < cntV || !"read_xml(): m was out of range");
      
      Vertex const & vi = mesh.vertex(i);
      Vertex const & vj = mesh.vertex(j);
      Vertex const & vk = mesh.vertex(k);
      Vertex const & vm = mesh.vertex(m);
      
      mesh.push_tetrahedron(vi,vj,vk,vm);
    }
    
    X.bind(mesh);
    Y.bind(mesh);
    Z.bind(mesh);
    
    TiXmlElement * xml_point = document_handle.FirstChild( "T4MESH" ).FirstChild( "NODE" ).Element();
    
    for( ; xml_point; xml_point=xml_point->NextSiblingElement("NODE") )
    {
      assert( xml_point->Attribute("idx") || !"read_xml(): missing index on point");
      assert( xml_point->Attribute("x")   || !"read_xml(): missing x coordinate");
      assert( xml_point->Attribute("y")   || !"read_xml(): missing y coordinate");
      assert( xml_point->Attribute("z")   || !"read_xml(): missing z coordinate");
      
      size_t const idx = util::to_value<size_t>( xml_point->Attribute("idx") ) ;
      
      assert( idx < cntV || !"read_xml(): index out of range");
      
      T const x = util::to_value<T>(xml_point->Attribute("x"));
      T const y = util::to_value<T>(xml_point->Attribute("y"));
      T const z = util::to_value<T>(xml_point->Attribute("z"));
      
      assert(is_number(x) || !"read_xml(): x coordinate was not a number");
      assert(is_finite(x) || !"read_xml(): x coordinate was not finite");
      assert(is_number(y) || !"read_xml(): y coordinate was not a number");
      assert(is_finite(y) || !"read_xml(): y coordinate was not finite");
      assert(is_number(z) || !"read_xml(): z coordinate was not a number");
      assert(is_finite(z) || !"read_xml(): z coordinate was not finite");
      
      Vertex const & vertex = mesh.vertex(idx);
      
      X( vertex ) = x;
      Y( vertex ) = y;
      Z( vertex ) = z;
    }
    
    xml_document.Clear();
  }
  
  template
  void read_xml<float>(
                std::string const & filename
                , T4Mesh & mesh
                , VertexAttribute<float,T4Mesh> & X
                , VertexAttribute<float,T4Mesh> & Y
                , VertexAttribute<float,T4Mesh> & Z
                );

  template
  void read_xml<double>(
                std::string const & filename
                , T4Mesh & mesh
                , VertexAttribute<double,T4Mesh> & X
                , VertexAttribute<double,T4Mesh> & Y
                , VertexAttribute<double,T4Mesh> & Z
                );

  template<typename T>
  void write_xml(
                 std::string const & filename
                 , T4Mesh const & mesh
                 , VertexAttribute<T,T4Mesh> const & X
                 , VertexAttribute<T,T4Mesh> const & Y
                 , VertexAttribute<T,T4Mesh> const & Z
                 )
  {
    assert(X.size()==mesh.vertex_size() || !"write_xml(): X coordinates and vertex size mismatch");
    assert(Y.size()==mesh.vertex_size() || !"write_xml(): Y coordinates and vertex size mismatch");
    assert(Z.size()==mesh.vertex_size() || !"write_xml(): Z coordinates and vertex size mismatch");
    
    boost::filesystem::path  mypath(filename);
    
    std::string const ext = boost::filesystem::extension( mypath );
    
    assert( ext == ".xml" || !"write_xml(): illegal file name extension");
    
    
    // build document
    TiXmlDocument doc;
    
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    TiXmlElement * meshelem = new TiXmlElement( "T4MESH" );
    
    meshelem->SetAttribute( "nodes", mesh.vertex_size() );
    meshelem->SetAttribute( "tetrahedra", mesh.tetrahedron_size() );
    
    doc.LinkEndChild(decl);
    doc.LinkEndChild(meshelem);
        
    for(size_t idx = 0u; idx < mesh.tetrahedron_size(); ++idx)
    {
      TiXmlElement * elem = new TiXmlElement( "TETRAHEDRON" );
      
      Tetrahedron const & tetrahedron = mesh.tetrahedron( idx );
      
      elem->SetAttribute( "i", util::to_string( tetrahedron.i() )  );
      elem->SetAttribute( "j", util::to_string( tetrahedron.j() )  );
      elem->SetAttribute( "k", util::to_string( tetrahedron.k() )  );
      elem->SetAttribute( "m", util::to_string( tetrahedron.m() )  );
            
      meshelem->LinkEndChild( elem );
    }
    
    for(size_t idx = 0u; idx < mesh.vertex_size(); ++idx)
    {
      TiXmlElement * elem = new TiXmlElement( "NODE" );
     
      Vertex const & vertex = mesh.vertex(idx);
      
      elem->SetAttribute( "idx", util::to_string( idx )  );
      elem->SetAttribute( "x", util::to_string( X(vertex) ) );
      elem->SetAttribute( "y", util::to_string( Y(vertex) ) );
      elem->SetAttribute( "z", util::to_string( Z(vertex) ) );
      
      meshelem->LinkEndChild( elem );
    }
    
    // write the document
#ifdef TIXML_USE_STL
    doc.SaveFile(filename);
#else
    doc.SaveFile(filename.c_str());
#endif
  }

  template
  void write_xml<float>(
                 std::string const & filename
                 , T4Mesh const & mesh
                 , VertexAttribute<float,T4Mesh> const & X
                 , VertexAttribute<float,T4Mesh> const & Y
                 , VertexAttribute<float,T4Mesh> const & Z
                 );

  template
  void write_xml<double>(
                 std::string const & filename
                 , T4Mesh const & mesh
                 , VertexAttribute<double,T4Mesh> const & X
                 , VertexAttribute<double,T4Mesh> const & Y
                 , VertexAttribute<double,T4Mesh> const & Z
                 );
  
} // namespace mesh_array
