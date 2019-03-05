#ifndef PROX_GUI_APPLICATION_H
#define PROX_GUI_APPLICATION_H

#include <gl3.h>
#include <gl3_glm.h>
#include <gl3_camera.h>
#include <gl3_frustum.h>
#include <gl3_shader_program_manager.h>

#include <prox_gui_select_tool.h>
#include <prox_gui_ucph_colors.h>
#include <prox_gui_lighting.h>
#include <prox_gui_geometry_manager.h>
#include <prox_gui_scene_manager.h>
#include <prox_gui_make_scene.h>
#include <prox_gui_update_scene.h>
#include <prox_gui_draw_scene.h>
#include <prox_gui_draw_contacts.h>
#include <prox_gui_draw_aabbs.h>
#include <prox_gui_shadowmap_manager.h>
#include <prox_gui_draw_shadowmaps.h>
#include <prox_gui_draw_ssao.h>
#include <prox_gui_draw_structure_fields.h>

#include <tiny.h>
#include <prox_engine.h>
#include <procedural.h>

#include <util_string_helper.h>
#include <util_config_file.h>
#include <util_write_matlab_vector.h>
#include <util_log.h>
#include <util_timestamp.h>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include <prox_gui_widget.h>

#include <cstdio>
#include <iomanip>      // std::setw
#include <sstream>      // std::stringstream
#include <fstream>
#include <string>
#include <cfloat>
#include <vector>
#include <cmath>

namespace prox_gui
{
  class Application
  {
    public:
    
    typedef tiny::MathTypes<float> MT;
    typedef MT::real_type          T;
    typedef MT::vector3_type       V;
    typedef MT::matrix3x3_type     M;
    typedef MT::quaternion_type    Q;
    typedef MT::coordsys_type      C;
    typedef MT::value_traits       VT;
    
    
    Widget * gl_widget;
    
    protected:
    
    float                                 m_fovy;                      ///< Field of view in the y-direction.
    float                                 m_z_near;                    ///< Near clipping plane.
    float                                 m_z_far;                     ///< Far clipping plane.
    float                                 m_dolly_sensitivity;         ///< The dolly mouse sensitivity.
    float	                                m_pan_sensitivity;           ///< The panning mouse sensitivity.
    bool                                  m_dolly_mode;                ///< Boolean flag indicating whether the application is currently doing a dolly operation.
    bool	                                m_pan_mode;                  ///< Boolean flag indicating whether the application is currently doing a pan operation.
    bool	                                m_trackball_mode;            ///< Boolean flag indicating whether the application is currently doing a trackball operation.
    bool	                                m_selection_mode;            ///< Boolean flag indicating whether the application is currently doing a selection operation.
    float	                                m_begin_x;                   ///< The starting x-pixel coordinate when doing a mouse operation.
    float	                                m_begin_y;                   ///< The starting y-pixel coordinate when doing a mouse operation.
    gl3::Camera	                          m_camera;                    ///< Camera class taking care of the view transformation (from worlds-space to camera space).
    gl3::Frustum                          m_frustum;                   ///< Frustum class, can be used for optimizing the rendering process by offering simple frustum culling.
    float                                 m_clear_color[4];            ///< RGBA in 0.0 to 1.0 range
    T                                     m_time;
    T                                     m_total_time;
    T                                     m_time_step;
    bool                                  m_draw_debug;                ///< Flag for fliiping betwenedebug rendering mode or "non-debug" mode
    bool                                  m_draw_contacts;
    bool                                  m_draw_structure_fields;
    bool                                  m_draw_wireframe;
    bool                                  m_draw_aabbs;
    bool                                  m_framegrab;
    bool                                  m_save_contact_data;
    bool                                  m_profiling;
    bool                                  m_xml_load;
    bool                                  m_xml_play;
    bool                                  m_xml_record;
    bool                                  m_xml_auto_save;
    bool                                  m_did_auto_save;
    
    std::string                           m_framegrab_file;
    std::string                           m_matlab_file;
    std::string                           m_xml_save_scene_file;
    std::string                           m_xml_load_scene_file;
    std::string                           m_xml_save_channel_file;
    std::string                           m_xml_load_channel_file;
    util::ConfigFile                      m_config_file;
    
    std::string                           m_obj_path;
    std::string                           m_shader_path;
    std::string                           m_working_directory;
    std::string                           m_output_path;
    
    std::string                           m_procedural_scene;
    SelectTool                            m_select_tool;
    prox::Engine                          m_engine;
    content::ChannelStorage               m_channel_storage;
    size_t                                m_key_idx;
    size_t                                m_max_keys;
    gl3::ShaderProgramManager             m_shader_manager;
    SceneManager                          m_scene_manager;
    GeometryManager                       m_geometry_manager;
    Geometry                              m_contact_geometry;
    std::vector<LightInfo>                m_lights;
    std::vector<MaterialInfo>             m_materials;
    MaterialInfo                          m_point_material;
    MaterialInfo                          m_normal_vector_material;
    MaterialInfo                          m_structure_vector_material;
    ShadowmapManager                      m_shadowmap_manager;
    SSAOBuffer                            m_ssao_buffer;
    
    protected:
    
    static int & frame_counter()
    {
      static int cnt = 0;
      
      return cnt;
    }
    
    
    void framegrab()
    {
      std::stringstream filename;
      
      // compute output width for filename
      static int width = std::ceil( std::log10( std::ceil( m_total_time / m_time_step ) ) ) + 1;
      
      filename << m_output_path << m_framegrab_file << std::setw(width) << std::setfill('0') << ++frame_counter() << ".png";
      
      // create directories if necessary
      boost::filesystem::path framegrab_file( filename.str() );
      if ( framegrab_file.has_parent_path() &&
          ! boost::filesystem::exists( framegrab_file.parent_path() ) &&
          ! boost::filesystem::create_directories( framegrab_file.parent_path() ) )
      {
        util::Log logging;
        
        logging << "Could not create directories for file '"
        << filename.str()
        << "'"
        << util::Log::newline();
      }
      
      if(gl_widget)
      gl_widget->save_screen(QString( filename.str().c_str() ));
      
    }
    
    void reset()
    {
      m_obj_path                 = "";
      m_shader_path              = "";
      m_working_directory                 = "";
      m_output_path              = "";
      
      m_time                     = VT::zero();
      m_time_step                = VT::numeric_cast(0.01f);
      m_draw_debug               = false;
      m_draw_contacts            = false;
      m_draw_structure_fields    = false;
      m_draw_wireframe           = false;
      m_draw_aabbs               = false;
      m_framegrab                = false;
      m_save_contact_data        = false;
      m_xml_load                 = false;
      m_xml_play                 = false;
      m_xml_record               = false;
      m_did_auto_save            = false;
      m_profiling                = false;
      m_framegrab_file           = "screen_";
      m_matlab_file              = "profiling.m";
      m_xml_load_channel_file    = "in_channels.xml";
      m_xml_save_channel_file    = "out_channels.xml";
      m_xml_save_scene_file      = "out_scene.xml";
      m_xml_load_scene_file      = "in_scene.xml";
      m_procedural_scene         = "arch";
      m_engine.clear();
      m_config_file.clear();
      m_select_tool.clear();
      m_channel_storage.clear();
      m_lights.clear();
      m_materials.clear();
      m_geometry_manager.clear();
      m_scene_manager.clear();
      m_shader_manager.clear();
      m_shader_manager.clear();
      
      make_default_lighting(m_lights);
      
      m_dolly_mode               = false;
      m_pan_mode                 = false;
      m_trackball_mode           = false;
      m_selection_mode           = false;
      m_camera.target_locked()   = true;
      
      util::Log logging;
      
      logging << "Rotate : hold down left mouse button"                 << util::Log::newline();
      logging << "Dolly  : hold down middle mouse button"               << util::Log::newline();
      logging << "Pan    : press shift and hold down left mouse button" << util::Log::newline();
      logging << "1      : load scene_01_box_on_inclined_plane.cfg"     << util::Log::newline();
      logging << "2      : load scene_02_rolling_cylinder.cfg"          << util::Log::newline();
      logging << "3      : load scene_03_arch.cfg"                      << util::Log::newline();
      logging << "4      : load scene_04_stack.cfg"                     << util::Log::newline();
      logging << "5      : load scene_05_capsule_hopper.cfg"            << util::Log::newline();
      logging << "W      : Toggle draw wireframe"                       << util::Log::newline();
      logging << "C      : Toggle draw contacts"                        << util::Log::newline();
      logging << "X      : Toggle draw structure fields"                << util::Log::newline();
      logging << "A      : Toggle draw AABB"                            << util::Log::newline();
      logging << "F      : Toggle framegrab"                            << util::Log::newline();
      logging << "D      : Toggle draw debug"                           << util::Log::newline();
      logging << "B      : Toggle save contact data"                    << util::Log::newline();
      logging << "P      : Toggle xml play"                             << util::Log::newline();
      logging << "R      : Toggle xml record"                           << util::Log::newline();
      logging << "M      : Write matlab profiling data"                 << util::Log::newline();
      logging << "L      : Load xml file"                               << util::Log::newline();
      logging << "S      : Save xml file"                               << util::Log::newline();
      logging << "V      : Save contact data"                           << util::Log::newline();
      logging << "+      : Single step simulation"                      << util::Log::newline();
      logging << "O      : Toggle orbit mode"                           << util::Log::newline();
      logging << "I      : Toggle target locked mode"                   << util::Log::newline();
      logging << "space  : Toggle run simulation"                       << util::Log::newline();
      
      glm::vec3 const position   = glm::vec3(0,0,20);
      glm::vec3 const target     = glm::vec3(0,0,0);
      glm::vec3 const up         = glm::vec3(0,1,0);
      
      m_camera.init(position,target,up);
      
      m_clear_color[0] = 0.2f;
      m_clear_color[1] = 0.2f;
      m_clear_color[2] = 0.2f;
      m_clear_color[3] = 1.0f;
      
      frame_counter() = 0;
    }
    
    glm::mat4 compute_view_matrix() const
    {
      glm::mat4 const view_matrix = glm::lookAt(m_camera.position(),m_camera.target(),m_camera.up());
      
      return view_matrix;
    }
    
    glm::mat4 compute_projection_matrix() const
    {
      GLfloat viewport[4];
      glGetFloatv(GL_VIEWPORT,viewport);
      
      float const width  = viewport[2]*1.0f;
      float const height = viewport[3]*1.0f;
      
      float const aspect = ( 1.0f * width ) / height;
      
      glm::mat4 const projection_matrix = glm::perspective( glm::radians(m_fovy), aspect, m_z_near, m_z_far );
      
      return projection_matrix;
    }
    
    void get_ray(
                 unsigned int const & screen_x
                 , unsigned int const & screen_y
                 , float & p_x
                 , float & p_y
                 , float & p_z
                 , float & ray_x
                 , float & ray_y
                 , float & ray_z
                 )
    {
      float const sx = 1.0f*screen_x;
      float const sy = 1.0f*screen_y;
      float       nx = 0.0f;
      float       ny = 0.0f;
      float       nz = 1.0f;
      gl3::compute_normalized_coordinates(sx,sy,nx,ny);
      
      glm::mat4 const view_matrix       = compute_view_matrix();
      glm::mat4 const projection_matrix = compute_projection_matrix();
      
      glm::vec3 ray_nds  = glm::vec3(nx, ny, nz);                         // Normalized device space
      glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);    // Clip space
      glm::vec4 ray_eye  = glm::inverse(projection_matrix) * ray_clip;    // Eye space
      
      ray_eye = glm::vec4 (ray_eye.x, ray_eye.y, -1.0, 0.0);              // Manually set to forward direction vector
      
      glm::vec4 ray_world = (glm::inverse (view_matrix) * ray_eye);
      
      ray_world = glm::normalize(ray_world);
      
      ray_x = ray_world.x;
      ray_y = ray_world.y;
      ray_z = ray_world.z;
      p_x   = m_camera.position().x;
      p_y   = m_camera.position().y;
      p_z   = m_camera.position().z;
    }
    
    void get_ray(
                 unsigned int const & screen_x
                 , unsigned int const & screen_y
                 , V & p
                 , V & r
                 )
    {
      float p_x = 0.0f;
      float p_y = 0.0f;
      float p_z = 0.0f;
      float ray_x = 0.0f;
      float ray_y = 0.0f;
      float ray_z = 0.0f;
      
      get_ray(screen_x, screen_y, p_x, p_y, p_z, ray_x, ray_y, ray_z);
      
      p = V::make(p_x,p_y,p_z);
      r = V::make(ray_x,ray_y,ray_z);
    }
    
    public:
    
    void load_xml_file()
    {
      using std::max;
      
      util::Log logging;
      
      content::xml_read(m_xml_load_scene_file, &m_engine);
      content::xml_read(m_xml_load_channel_file, m_channel_storage);
      
      logging << "Read XML data to " << m_xml_save_scene_file << " and " << m_xml_save_channel_file << util::Log::newline();
      
      
      //--- Determine the maximum number of keys that was loaded and initialize bodies with first key frame
      m_key_idx  = 0u;
      m_max_keys = 0u;
      
      for(size_t channel_idx = 0u; channel_idx < m_channel_storage.get_number_of_channels(); ++ channel_idx )
      {
        size_t const body_idx = m_channel_storage.get_channel_id( channel_idx);
        {
          float x= 0.0f;
          float y= 0.0f;
          float z= 0.0f;
          m_channel_storage.get_key_position( channel_idx, m_key_idx, x, y, z);
          m_engine.set_rigid_body_position( body_idx, x, y, z);
        }
        {
          float qs = 0.0f;
          float qx = 0.0f;
          float qy = 0.0f;
          float qz = 0.0f;
          m_channel_storage.get_key_orientation( channel_idx, m_key_idx, qs, qx, qy, qz);
          m_engine.set_rigid_body_orientation( body_idx, qs, qx, qy, qz);
        }
        
        m_max_keys = max (m_max_keys, m_channel_storage.get_number_of_keys(channel_idx) );
      }
      
      logging << "Maximum number of keys =  " << m_max_keys << util::Log::newline();
      
    }
    
    void save_xml_file()
    {
      util::Log logging;
      
      content::xml_write(m_output_path + m_xml_save_scene_file, &m_engine);
      content::xml_write(m_output_path + m_xml_save_channel_file, m_channel_storage);
      
      logging << "Saved XML data to "
      << (m_output_path + m_xml_save_scene_file)
      << " and " << (m_output_path + m_xml_save_channel_file)
      << util::Log::newline();
      
    }
    
    void load_config_file(std::string const & cfg_file)
    {
      using std::min;
      using std::max;
      
      reset();
      
      m_config_file.load(cfg_file);
      
      util::LogInfo::on()            = util::to_value<bool>(m_config_file.get_value("logging","true"));
      util::LogInfo::console()       = util::to_value<bool>(m_config_file.get_value("console","true"));
      util::LogInfo::filename()      = m_config_file.get_value("log_file","log.txt");
      
      {
        util::Log logging;
        
        logging << "### " << util::timestamp() << util::Log::newline();
      }
      
      m_obj_path               = m_config_file.get_value( "obj_path",                       ""                 );
      m_shader_path            = m_config_file.get_value( "shader_path",                    ""                 );
      m_working_directory      = m_config_file.get_value( "working_directory",              ""                 );
      m_output_path            = m_config_file.get_value( "output_path",                    ""                 );
      
      m_xml_load_scene_file    = m_config_file.get_value( "xml_load_scene_file",            "in_scene.xml"     );
      m_xml_save_scene_file    = m_config_file.get_value( "xml_save_scene_file",            "out_scene.xml"    );
      m_xml_load_channel_file  = m_config_file.get_value( "xml_load_channel_file",          "in_channels.xml"  );
      m_xml_save_channel_file  = m_config_file.get_value( "xml_save_channel_file",          "out_channels.xml" );
      m_framegrab_file         = m_config_file.get_value( "framegrab_file",                 "screen_"          );
      m_matlab_file            = m_config_file.get_value( "matlab_file",                    "output.m"         );
      m_procedural_scene       = m_config_file.get_value( "procedural_scene",               "wall"             );
      
      m_draw_debug             = util::to_value<bool>(  m_config_file.get_value("draw_debug",            "false"   ));
      m_draw_contacts          = util::to_value<bool>(  m_config_file.get_value("draw_contacts",         "false"   ));
      m_draw_structure_fields  = util::to_value<bool>(  m_config_file.get_value("draw_structure_fields", "false"   ));
      m_draw_wireframe         = util::to_value<bool>(  m_config_file.get_value("draw_wireframe",        "false"   ));
      m_draw_aabbs             = util::to_value<bool>(  m_config_file.get_value("draw_aabbs",            "false"   ));
      m_framegrab              = util::to_value<bool>(  m_config_file.get_value("framegrab",             "false"   ));
      m_save_contact_data      = util::to_value<bool>(  m_config_file.get_value("save_contact_data",     "false"   ));
      m_xml_load               = util::to_value<bool>(  m_config_file.get_value("xml_load",              "false"   ));
      m_xml_auto_save          = util::to_value<bool>(  m_config_file.get_value("xml_auto_save",         "false"   ));
      m_xml_play               = util::to_value<bool>(  m_config_file.get_value("xml_play",              "false"   ));
      m_xml_record             = util::to_value<bool>(  m_config_file.get_value("xml_record",            "false"   ));
      m_total_time             = util::to_value<float>( m_config_file.get_value("total_time",            "3.0"     ));
      m_time_step              = util::to_value<float>( m_config_file.get_value("time_step",             "0.01"    ));
      m_profiling              = util::to_value<bool>(  m_config_file.get_value("profiling",             "false"   ));
      
      m_engine.set_parameters_from_config_file( cfg_file );
      
      
      if(m_xml_load)
      {
        load_xml_file();
      }
      else
      {
        procedural::Noise::on()    = util::to_value<bool>(   m_config_file.get_value("procedural_noise_on",    "false") );
        procedural::Noise::scale() = util::to_value<double>( m_config_file.get_value("procedural_noise_scale", "0.001") );
        
        procedural::make_scene<MT>(m_procedural_scene, m_obj_path, &m_engine, m_config_file);
      }
      
      //--- Count how big the configuraiton we are simulating is -------------------
      size_t total_vertices     = 0u;
      size_t total_tetrahedrons = 0u;
      size_t total_bodies       = 0u;
      
      for(; total_bodies < m_engine.get_number_of_rigid_bodies(); ++total_bodies)
      {
        size_t const geom_id = m_engine.get_rigid_body_collision_geometry(total_bodies);
        size_t       N       = 0u;
        size_t       K       = 0u;
        
        m_engine.get_tetramesh_shape(geom_id, N, K);
        
        total_vertices     += N;
        total_tetrahedrons += K;
      }
      
      util::Log logging;
      
      logging << total_bodies
      << " bodies, "
      << total_tetrahedrons
      << " tetrahedrons, "
      << total_vertices
      << " vertices."
      << util::Log::newline();
      
      if(m_xml_record)
      {
        assert(!m_xml_play || !"load_config_file(): xml_play and xml_record can not both be on at same time.");
        
        // Prepare xml channel recording
        std::vector<size_t> rids;
        size_t const N = m_engine.get_number_of_rigid_bodies();
        rids.resize(N);
        m_engine.get_rigid_body_indices( &rids[0] );
        
        m_channel_storage.clear();
        for(size_t i = 0u; i < N; ++i)
        {
          size_t      const rid  = rids[i];
          std::string const name = m_engine.get_rigid_body_name(rid);
          
          m_channel_storage.create_channel( rid, name );
        }
      }
      
      make_scene(
                 m_config_file
                 , &m_engine
                 , m_materials
                 , m_shader_path
                 , m_shader_manager
                 , m_scene_manager
                 , m_geometry_manager
                 );
      
      m_point_material = prox_gui::make_material_from_config_file("point", m_config_file);
      m_normal_vector_material = prox_gui::make_material_from_config_file("normal", m_config_file);
      m_structure_vector_material = prox_gui::make_material_from_config_file("structure", m_config_file);

      make_vector_geometry(m_shader_manager,m_geometry_manager);
      make_point_geometry(m_shader_manager,m_geometry_manager);
      make_aabb_geometry(m_shader_manager,m_geometry_manager);
      
      make_lighting_from_config_file(m_config_file, m_lights);
      
      if(m_lights.empty())
      {
        make_default_lighting(m_lights);
      }
      
      init_shadowmaps( m_lights, m_shadowmap_manager);
      
      float const cam_eye_x    = util::to_value<float>( m_config_file.get_value("camera_position_x", "0.0"));
      float const cam_eye_y    = util::to_value<float>( m_config_file.get_value("camera_position_y", "4.0"));
      float const cam_eye_z    = util::to_value<float>( m_config_file.get_value("camera_position_z", "10.0"));
      
      float const cam_target_x = util::to_value<float>( m_config_file.get_value("camera_target_x", "0.0"));
      float const cam_target_y = util::to_value<float>( m_config_file.get_value("camera_target_y", "0.0"));
      float const cam_target_z = util::to_value<float>( m_config_file.get_value("camera_target_z", "0.0"));
      
      float const cam_up_x     = util::to_value<float>( m_config_file.get_value("camera_up_x", "0.0"));
      float const cam_up_y     = util::to_value<float>( m_config_file.get_value("camera_up_y", "1.0"));
      float const cam_up_z     = util::to_value<float>( m_config_file.get_value("camera_up_z", "0.0"));
      
      glm::vec3 const eye      = glm::vec3( cam_eye_x,    cam_eye_y,    cam_eye_z    );
      glm::vec3 const center   = glm::vec3( cam_target_x, cam_target_y, cam_target_z );
      glm::vec3 const up       = glm::vec3( cam_up_x,     cam_up_y,     cam_up_z     );
      
      m_camera.init(eye,center,up);
      
      m_z_near                 = util::to_value<float>( m_config_file.get_value("z_near", "0.1"));
      m_z_far                  = util::to_value<float>( m_config_file.get_value("z_far", "700.0"));
      
      m_clear_color[0]         = util::to_value<float>( m_config_file.get_value("clear_color_r", "0.2"));
      m_clear_color[1]         = util::to_value<float>( m_config_file.get_value("clear_color_g", "0.2"));
      m_clear_color[2]         = util::to_value<float>( m_config_file.get_value("clear_color_b", "0.2"));
      m_clear_color[3]         = util::to_value<float>( m_config_file.get_value("clear_color_a", "1.0"));
      
      GLfloat viewport[4];
      glGetFloatv(GL_VIEWPORT,viewport);
      
      int const width  = viewport[2]*1.0f;
      int const height = viewport[3]*1.0f;
      
      int   const ssao_kernel_size   = util::to_value<int>( m_config_file.get_value("ssao_kernel_size", "64"));
      int   const ssao_blur_size     = util::to_value<int>( m_config_file.get_value("ssao_blur_size", "4"));
      float const ssao_radius        = util::to_value<float>( m_config_file.get_value("ssao_radius", "1.0"));
      
      m_ssao_buffer.m_kernel_size    = min(16, max( ssao_kernel_size, 128 ) );  // Clamping between 16..128
      m_ssao_buffer.m_blur_size      = ssao_blur_size;
      m_ssao_buffer.m_radius         = ssao_radius;
      m_ssao_buffer.m_noise_scale_x  = 1.0*width/ssao_blur_size;
      m_ssao_buffer.m_noise_scale_y  = 1.0*height/ssao_blur_size;
      m_ssao_buffer.m_width          = width;
      m_ssao_buffer.m_height         = height;
      
      m_ssao_buffer.m_clear_color_red   = m_clear_color[0];
      m_ssao_buffer.m_clear_color_green = m_clear_color[1];
      m_ssao_buffer.m_clear_color_blue  = m_clear_color[2];
      m_ssao_buffer.m_clear_color_alpha = m_clear_color[3];
      
      init_ssao_buffer( m_ssao_buffer );
    }
    
    void resize(int const & width, int const & height)
    {
    }
    
    void save_contact_data()
    {
      std::stringstream filename;
      
      // compute output width for filename
      static int width = std::ceil( std::log10( std::ceil( m_total_time / m_time_step ) ) ) + 1;
      
      filename << m_output_path
      << m_framegrab_file
      << "contact_data_"
      << std::setw(width)
      << std::setfill('0')
      << frame_counter()
      << ".m";
      
      m_engine.write_matlab_contact_data(filename.str(), frame_counter());
    }
    
    public:
    
    Application()
    : m_fovy(30.0f)
    , m_z_near(0.1f)
    , m_z_far(700.0f)
    , m_dolly_sensitivity(0.25f)
    , m_pan_sensitivity(0.25f)
    , m_dolly_mode(false)
    , m_pan_mode(false)
    , m_trackball_mode(false)
    , m_selection_mode(false)
    , m_begin_x(0.0f)
    , m_begin_y(0.0f)
    , m_camera()
    , m_frustum()
    {
      m_clear_color[0] = 0.2f;
      m_clear_color[1] = 0.2f;
      m_clear_color[2] = 0.2f;
      m_clear_color[3] = 1.0f;
    }
    
    virtual ~Application()
    {
      reset();
    }
    
    public:
    
    void paint()
    {
      glClearColor(  m_clear_color[0], m_clear_color[1], m_clear_color[2] // rgb
                   , m_clear_color[3]);                                   // a
      gl3::check_errors("paint(): glClearColor");
      
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      gl3::check_errors("paint(): glClear");
      
      if(m_scene_manager.m_objects.empty())  // nothing to draw
      return;
      
      glm::mat4 const view_matrix       = compute_view_matrix();
      glm::mat4 const projection_matrix = compute_projection_matrix();
      
      draw_shadowmaps(
                      m_shader_manager
                      , m_scene_manager
                      , m_geometry_manager
                      , m_shadowmap_manager
                      );
      
      if (!m_draw_debug)
      {
        draw_ssao(
                  m_shader_path
                  , projection_matrix
                  , view_matrix
                  , m_z_near
                  , m_z_far
                  , m_scene_manager
                  , m_geometry_manager
                  , m_lights
                  , m_materials
                  , m_shadowmap_manager
                  , m_ssao_buffer
                  );
      }
      else
      {
        draw_scene(
                   m_draw_wireframe
                   , projection_matrix
                   , view_matrix
                   , m_lights
                   , m_materials
                   , m_shader_manager
                   , m_scene_manager
                   , m_geometry_manager
                   , m_shadowmap_manager
                   );
        
        if (m_draw_contacts)
        {
          draw_contacts(
                        & m_engine
                        , m_config_file
                        , m_point_material
                        , m_normal_vector_material
                        , m_structure_vector_material
                        , projection_matrix
                        , view_matrix
                        , m_lights
                        , m_shader_manager
                        , m_geometry_manager
                        );
        }
        
        if (m_draw_aabbs)
        {
          draw_aabbs(
                     &m_engine
                     , m_config_file
                     , projection_matrix
                     , view_matrix
                     , m_lights
                     , m_materials
                     , m_shader_manager
                     , m_geometry_manager
                     , m_scene_manager
                     );
        }
        if (m_draw_structure_fields)
        {
          draw_structure_fields(
                                &m_engine
                                , m_config_file
                                , projection_matrix
                                , view_matrix
                                , m_lights
                                , m_structure_vector_material
                                , m_shader_manager
                                , m_scene_manager
                                , m_geometry_manager
                                );
          
        }
      }
    }
    
    void key_down(unsigned char choice)
    {
      util::Log logging;
      
      switch(choice)
      {
        case '1': load_config_file(m_working_directory + "scene_01_box_on_inclined_plane.cfg"); break;
        case '2': load_config_file(m_working_directory + "scene_02_rolling_cylinder.cfg");      break;
        case '3': load_config_file(m_working_directory + "scene_03_arch.cfg");                  break;
        case '4': load_config_file(m_working_directory + "scene_04_stack.cfg");                 break;
        case '5': load_config_file(m_working_directory + "scene_05_capsule_hopper.cfg");        break;
        
        case 'W': m_draw_wireframe        = !m_draw_wireframe;                         break;
        case 'C': m_draw_contacts         = !m_draw_contacts;                          break;
        case 'X': m_draw_structure_fields = !m_draw_structure_fields;                  break;
        case 'A': m_draw_aabbs            = !m_draw_aabbs;                             break;
        case 'F': m_framegrab             = !m_framegrab;                              break;
        case 'D': m_draw_debug            = !m_draw_debug;                             break;
        case 'B': m_save_contact_data     = !m_save_contact_data;                      break;
        
        case 'P':
        
        m_xml_play = !m_xml_play;
        
        if(m_xml_play)
        {
          m_xml_record = false;
          m_time       = VT::zero();
        }
        break;
        
        case 'R':
        
        m_xml_record = !m_xml_record;
        
        if(m_xml_record)
        {
          m_xml_play = false;
        }
        
        break;
        
        case 'M': m_engine.write_matlab_profiling_data( m_output_path + m_matlab_file ); break;
        case 'L': load_xml_file(); break;
        case 'S': save_xml_file(); break;
        case 'V': save_contact_data(); break;
        
        case '+': run(); break;
        
        case 'O':
        this->m_camera.orbit_mode() = ! this->m_camera.orbit_mode();
        if(this->m_camera.orbit_mode())
        logging << "orbit mode on" << util::Log::newline();
        else
        logging << "orbit mode off " << util::Log::newline();
        break;
        
        case 'I':
        this->m_camera.target_locked() = ! this->m_camera.target_locked();
        if(this->m_camera.target_locked())
        logging << "target is locked" << util::Log::newline();
        else
        logging << "target is free " << util::Log::newline();
        break;
      };
    }
    
    void init()
    {
      reset();
    }
    
    /**
     * Runs a simulation step.
     *
     * @return false if the simulation is finished, true otherwise.
     */
    bool run()
    {
      if (m_time >= m_total_time)
      {
        if(! m_did_auto_save)
        {
          if(m_xml_auto_save)
          {
            save_xml_file();
          }
          if(m_profiling)
          {
            m_engine.write_matlab_profiling_data( m_output_path + m_matlab_file );
          }
          
          m_did_auto_save = true;
        }
        
        {
          util::Log logging;
          
          logging << "Application::run(): Total time reached "
          << m_total_time
          << " seconds, no more simulation is done... reload to restart"
          << util::Log::newline();
          logging.flush();
        }
        
        return false;
      }
      
      {
        util::Log logging;
        logging << "Application::run(): time: " << m_time << util::Log::newline();
      }
      
      if (m_framegrab)
      {
        framegrab();
      }
      
      if (m_save_contact_data)
      {
        save_contact_data();
      }
      
      if(m_xml_play)
      {
        // get frame m_time from xml data
        
        if(m_key_idx < m_max_keys)
        {
          for(size_t channel_idx = 0u; channel_idx < m_channel_storage.get_number_of_channels(); ++ channel_idx )
          {
            size_t const body_idx = m_channel_storage.get_channel_id( channel_idx);
            {
              float x= 0.0f;
              float y= 0.0f;
              float z= 0.0f;
              m_channel_storage.get_key_position( channel_idx, m_key_idx, x, y, z);
              m_engine.set_rigid_body_position( body_idx, x, y, z);
            }
            {
              float qs = 0.0f;
              float qx = 0.0f;
              float qy = 0.0f;
              float qz = 0.0f;
              m_channel_storage.get_key_orientation( channel_idx, m_key_idx, qs, qx, qy, qz);
              m_engine.set_rigid_body_orientation( body_idx, qs, qx, qy, qz);
            }
          }
          ++m_key_idx;
          
          m_time = m_time + m_time_step;
        }
      }
      else
      {
        m_engine.simulate(m_time_step);
        
        m_time = m_time + m_time_step;
        
        if (m_xml_record)
        {
          // record xml motion channel data
          float const time = m_engine.get_time();
          for(size_t channel_idx = 0u; channel_idx < m_channel_storage.get_number_of_channels(); ++ channel_idx )
          {
            size_t const body_idx = m_channel_storage.get_channel_id( channel_idx);
            size_t const key_idx  = m_channel_storage.create_key( channel_idx, time);
            {
              float x= 0.0f;
              float y= 0.0f;
              float z= 0.0f;
              m_engine.get_rigid_body_position( body_idx, x, y, z);
              m_channel_storage.set_key_position( channel_idx, key_idx, x, y, z);
            }
            
            {
              float qs = 0.0f;
              float qx = 0.0f;
              float qy = 0.0f;
              float qz = 0.0f;
              m_engine.get_rigid_body_orientation( body_idx, qs, qx, qy, qz);
              m_channel_storage.set_key_orientation( channel_idx, key_idx, qs, qx, qy, qz);
            }
          }
          
          
        }
        
      }
      
      update_scene(m_scene_manager, &m_engine);
      return true;
    }
    
    void mouse_down(double cur_x,double cur_y,bool shift,bool ctrl,bool alt,bool left,bool middle,bool right)
    {
      if (middle || (alt && left))  // 2008-08-13 micky: not all mice have a "normal" middle button!
      m_dolly_mode = true;
      
      if ( shift && left )
      m_pan_mode = true;
      
      if(!middle && !right && !ctrl && !alt && !shift && left)// only left button allowed
      {
        m_camera.mouse_down( cur_x, cur_y );
        m_trackball_mode = true;
      }
      
      m_begin_x = cur_x;
      m_begin_y = cur_y;
      
      if(ctrl)
      {
        V p;
        V r;
        get_ray(cur_x, cur_y,p,r);
        
        m_select_tool.select( p, r, &m_engine );
        m_selection_mode = true;
      }
    }
    
    void mouse_up(double cur_x,double cur_y,bool shift,bool ctrl,bool alt,bool left,bool middle,bool right)
    {
      if (m_dolly_mode )
      {
        m_camera.dolly( m_dolly_sensitivity * (cur_y - m_begin_y) );
        m_dolly_mode = false;
      }
      else if ( m_pan_mode )
      {
        m_camera.pan(
                     m_pan_sensitivity*( m_begin_x - cur_x)
                     , m_pan_sensitivity*(cur_y - m_begin_y)
                     );
        m_pan_mode = false;
      }
      else if (m_trackball_mode )
      {
        m_camera.mouse_up( cur_x, cur_y );
        m_trackball_mode = false;
      }
      
      if(ctrl)
      {
        m_select_tool.deselect();
        m_selection_mode = false;
      }
      
      m_begin_x = cur_x;
      m_begin_y = cur_y;
    }
    
    void mouse_move(double cur_x,double cur_y)
    {
      if (m_dolly_mode )
      {
        m_camera.dolly( m_dolly_sensitivity*(cur_y - m_begin_y) );
      }
      else if ( m_pan_mode )
      {
        m_camera.pan(
                     m_pan_sensitivity*(m_begin_x - cur_x)
                     , m_pan_sensitivity*(cur_y - m_begin_y)
                     );
      }
      else if ( m_trackball_mode )
      {
        m_camera.mouse_move( cur_x, cur_y);
      }
      
      m_begin_x = cur_x;
      m_begin_y = cur_y;
      
      if(m_selection_mode)
      {
        V p;
        V r;
        get_ray(cur_x, cur_y, p,r);
        
        V const dof = V::make( m_camera.dof().x, m_camera.dof().y, m_camera.dof().z );
        
        m_select_tool.move_selection(p, r, dof, &m_engine);
        
        // 2014-10-7 Kenny code review: This is expensive to update all objects
        // when only one object has been manipulated
        update_scene(m_scene_manager, &m_engine);
      }
    }
    
  };
  
  class Instance
  {
    public:
    
    static Application & app()
    {
      static Application my_instance;
      
      return my_instance;
    }
    
  };
  
}// namespace prox_gui


// PROX_GUI_APPLICATION_H
#endif
