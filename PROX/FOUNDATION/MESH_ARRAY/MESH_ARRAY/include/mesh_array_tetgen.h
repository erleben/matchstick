#ifndef MESH_ARRAY_TETGEN_H
#define MESH_ARRAY_TETGEN_H

#include <mesh_array_t3mesh.h>
#include <mesh_array_t4mesh.h>
#include <mesh_array_vertex_attribute.h>

#include <string>

namespace mesh_array
{
  
  struct TetGenSettings
  {
    
    double       m_quality_ratio;      ///< quality t4mesh is issued if > 0. A minimum radius-edge ratio may be specifyed (default 2.0).
    double       m_maximum_volume;     ///< max volume constraints on t4mesh if > 0.
    std::string  m_filename;           ///< use intermediate files to/fro tetget if name specified.
    bool         m_quiet_output;       ///< keep output spam as silent as possible, great for RELEASE.
    bool         m_verify_input;       ///< DEBUG: detects plc intersections, i.e. verify "bad" input mesh.
    bool         m_suppress_splitting; ///< suppresses splitting of boundary facets/segments
    
    TetGenSettings();
    
    std::string cmdline() const;
  };

  
  TetGenSettings tetgen_default_settings();
  
  TetGenSettings tetgen_cdt_settings();

  TetGenSettings tetgen_quality_settings();
  

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
              , TetGenSettings const & settings = TetGenSettings()
              );
  
} // namespace mesh_array

//MESH_ARRAY_TETGEN_H
#endif
