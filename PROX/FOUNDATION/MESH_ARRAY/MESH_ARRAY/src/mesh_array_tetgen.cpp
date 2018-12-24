#include <mesh_array_tetgen.h>

#include <mesh_array_io_tetgen.h>
#include <mesh_array_io_poly.h>


#include <tetgen.h>

#include <string>
#include <sstream>

namespace mesh_array
{
  
  TetGenSettings::TetGenSettings()
  : m_quality_ratio(2.0)
  , m_maximum_volume(0.0)
  , m_filename("temporary")
  , m_quiet_output(false)
  , m_verify_input(false)
  , m_suppress_splitting(false)
  {
  }
  
  std::string TetGenSettings::cmdline() const
  {
    std::stringstream input;
    
    input << "p";  // piecewise linear complex (always)

    if (m_suppress_splitting) {
      input << "Y";
    }
    
      //input << "i";
    
    if (m_verify_input)
    {
      input << "d";
    }
    else 
    {
      if (m_quality_ratio > 0)
        input << "q" << m_quality_ratio;  // quality
      
      if (m_maximum_volume > 0)
        input << "a" << m_maximum_volume;  // max volume
      
      if (m_quiet_output)
        input << "Q";  // keep quiet :)
    }
    return input.str();
  }

  TetGenSettings tetgen_default_settings()
  {
    return TetGenSettings();
  }
  
  TetGenSettings tetgen_cdt_settings()
  {
    TetGenSettings settings;
    
    settings.m_quality_ratio     = 0.0;
    settings.m_maximum_volume    = 0.0;
    settings.m_quiet_output      = true;
        
    return settings;
  }

  TetGenSettings tetgen_quality_settings()
  {
    TetGenSettings settings;
    
    settings.m_quality_ratio     = 2.0;
    settings.m_maximum_volume    = 0.05;
    settings.m_quiet_output      = true;
    
    return settings;
  }
  

  template<typename T>
  void tetgen(
              T3Mesh const & surface
              , VertexAttribute<T,T3Mesh> const & inX
              , VertexAttribute<T,T3Mesh> const & inY
              , VertexAttribute<T,T3Mesh> const & inZ
              , T4Mesh & volume
              , VertexAttribute<T,T4Mesh> & outX
              , VertexAttribute<T,T4Mesh> & outY
              , VertexAttribute<T,T4Mesh> & outZ
              , TetGenSettings const & settings
              )
  {
    assert(!settings.m_filename.empty() || !"tetgen(): intermediate filename is missing");
    
    write_poly( settings.m_filename + ".poly", surface, inX, inY, inZ );
    

    tetgenbehavior config;
    
    std::string const cmdline = settings.cmdline();
    
    char * raw_cmdline = const_cast<char*>( cmdline.c_str() );
    
    bool parse_success = config.parse_commandline( raw_cmdline );
    
    assert(parse_success || !"tetgen() parse_commandline failed");
    
    tetgenio in;
    tetgenio out;
    
    char * raw_filename = const_cast<char*>( settings.m_filename.c_str() );    
    
    bool load_success = in.load_poly(raw_filename);
    
    assert(load_success || !"tetgen() load_poly failed");
    
    tetrahedralize(&config, &in, &out);
    
    assert(out.numberoftetrahedra > 0u || !"tetgen(): tetrahedralize failed");
    
    out.save_elements(raw_filename);
    out.save_nodes(raw_filename);
        
    read_tetgen( settings.m_filename, volume, outX, outY, outZ );
  }

  template
  void tetgen<float>(
              T3Mesh const & surface
              , VertexAttribute<float,T3Mesh> const & inX
              , VertexAttribute<float,T3Mesh> const & inY
              , VertexAttribute<float,T3Mesh> const & inZ
              , T4Mesh & volume
              , VertexAttribute<float,T4Mesh> & outX
              , VertexAttribute<float,T4Mesh> & outY
              , VertexAttribute<float,T4Mesh> & outZ
              , TetGenSettings const & settings = TetGenSettings()
              );
  
  template
  void tetgen<double>(
              T3Mesh const & surface
              , VertexAttribute<double,T3Mesh> const & inX
              , VertexAttribute<double,T3Mesh> const & inY
              , VertexAttribute<double,T3Mesh> const & inZ
              , T4Mesh & volume
              , VertexAttribute<double,T4Mesh> & outX
              , VertexAttribute<double,T4Mesh> & outY
              , VertexAttribute<double,T4Mesh> & outZ
              , TetGenSettings const & settings = TetGenSettings()
              );
 
} // namespace mesh_array
