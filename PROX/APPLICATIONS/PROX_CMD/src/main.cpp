#include <tiny.h>
#include <mass.h>
#include <geometry.h>
#include <mesh_array.h>
#include <prox_engine.h>
#include <procedural.h>

#include <util_config_file.h>
#include <util_string_helper.h>
#include <util_log.h>
#include <util_timestamp.h>

#include <boost/algorithm/string.hpp>

#include <sstream>
#include <string>
#include <cmath>


namespace prox_cmd
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
    
  protected:
    
    T             m_time;
    T             m_time_step;
    T             m_total_time;
    T             m_fps;
    bool          m_profiling;
    bool          m_xml_record;
    bool          m_save_contact_data;
    
    std::string   m_matlab_file;
    std::string   m_procedural_scene;
    std::string   m_xml_save_scene_file;
    std::string   m_xml_save_channel_file;
    
    std::string                       m_obj_path;
    std::string                       m_output_path;
    std::string                       m_working_directory;
    
    util::ConfigFile                    m_config_file;
    procedural::MaterialInfo<T>         m_mat_info;
    prox::Engine                        m_engine;
    content::ChannelStorage             m_channel_storage;
    size_t                              m_key_idx;
    size_t                              m_max_keys;
    
  protected:
    
    void clear()
    {
      m_obj_path                 = "";
      m_output_path              = "";
      m_working_directory        = "";
      
      m_time                     = VT::zero();
      m_time_step                = VT::numeric_cast(0.01f);
      m_fps                      = VT::numeric_cast(25.0f);
      
      m_profiling                = false;
      m_xml_record               = false;
      m_save_contact_data        = false;
      
      m_matlab_file              = "out.m";
      m_procedural_scene         = "wall";
      m_xml_save_channel_file    = "out_channels.xml";
      m_xml_save_scene_file      = "out_scene.xml";
      
      m_engine.clear();
      m_config_file.clear();
    }
    
  public:
    
    void save_xml_file()
    {
      content::xml_write(m_output_path + m_xml_save_scene_file, &m_engine);
      content::xml_write(m_output_path + m_xml_save_channel_file, m_channel_storage);
    }
    
    void load_config_file(std::string const & cfg_file)
    {
      clear();
      
      {
        util::Log logging;
        
        logging << "loading cfg: " << cfg_file << util::Log::newline();
      }
      
      bool cfg_loaded = m_config_file.load(cfg_file);
      
      assert(cfg_loaded || !"load_config_file(): cfg file could not be loaded");
      
      util::LogInfo::on()            = util::to_value<bool>(m_config_file.get_value("logging","true"));
      util::LogInfo::console()       = util::to_value<bool>(m_config_file.get_value("console","true"));
      util::LogInfo::filename()      = m_config_file.get_value("log_file","log.txt");
      
      {
        util::Log logging;
        
        logging << "### " << util::timestamp() << util::Log::newline();
      }
      
      m_obj_path               = m_config_file.get_value( "obj_path",                ""                   );
      m_working_directory      = m_config_file.get_value( "working_directory",       ""                   );
      m_output_path            = m_config_file.get_value( "output_path",             ""                   );
      
      m_matlab_file            = m_config_file.get_value( "matlab_file",             "output.m"           );
      m_procedural_scene       = m_config_file.get_value( "procedural_scene",        "wall"               );
      m_xml_save_scene_file    = m_config_file.get_value( "xml_save_scene_file",     "out_scene.xml"      );
      m_xml_save_channel_file  = m_config_file.get_value( "xml_save_channel_file",   "out_channels.xml"   );
      
      m_xml_record             = util::to_value<bool>(  m_config_file.get_value("xml_record", "false"   ) );
      m_total_time             = util::to_value<float>( m_config_file.get_value("total_time",  "3.0"    ) );
      m_time_step              = util::to_value<float>( m_config_file.get_value("time_step",   "0.01"   ) );
      m_fps                    = util::to_value<float>( m_config_file.get_value("fps",         "25.0"   ) );
      m_profiling              = util::to_value<bool>(  m_config_file.get_value("profiling",   "false"  ) );
      m_save_contact_data      = util::to_value<bool>(  m_config_file.get_value("save_contact_data", "false"   ));
      
      assert( m_time_step*m_fps < VT::one() || !"Application::run(): Invalid fps & timestep setting");
      
      m_engine.set_parameters_from_config_file( cfg_file );
      
      procedural::make_scene<MT>(m_procedural_scene, m_obj_path, &m_engine, m_config_file);
      
      if(m_xml_record)
      {
        // Prepare xml channel storage for recording
        std::vector<size_t> rids;
        size_t const N = m_engine.get_number_of_rigid_bodies();
        rids.resize(N);
        m_engine.get_rigid_body_indices( &rids[0] );
        
        m_channel_storage.clear();
        
        for(size_t i = 0u; i < N; ++i)
        {
          size_t const id = rids[i];
          
          std::string const name = m_engine.get_rigid_body_name(id);
          
          m_channel_storage.create_channel( id, name );
        }
      }
    }
    
    void save_contact_data()
    {
      static int counter = 0u;
      
      std::stringstream filename;
      
      // compute output width for filename
      static int width = std::ceil( std::log10( std::ceil( m_total_time / m_time_step ) ) ) + 1;
      
      filename << m_output_path
      << m_procedural_scene
      << "contact_data_"
      << std::setw(width)
      << std::setfill('0')
      << counter
      << ".m";
      
      m_engine.write_matlab_contact_data(filename.str(), counter);
      
      ++counter;
    }
    
    int run()
    {
      using std::min;
      
      T const frame_time = 1.0 / m_fps;   // The time between two frames
      
      
      while (m_time < m_total_time)
      {
        T const time_left = m_total_time - m_time;
        
        {
          util::Log logging;
          
          logging << "Application::run(): Time : " << m_time << " / " << m_total_time << util::Log::newline();
          logging << "Application::run(): Time left : " << time_left<< util::Log::newline();
          logging.flush();
        }
        
        T const time_to_next_frame    = min(time_left, frame_time);
        T       time_left_to_simulate = time_to_next_frame;
        
        while(time_left_to_simulate > VT::zero())
        {
          T const safe_time_step = min(m_time_step, time_left_to_simulate);
          
          m_engine.simulate(safe_time_step);
          time_left_to_simulate = time_left_to_simulate - safe_time_step;
        }
        
        m_time += time_to_next_frame;
        
        if (m_xml_record)
        {
          //--- record xml motion channel data -----------------------------
          
          float const time = m_engine.get_time();
          
          for(size_t channel_idx = 0u; channel_idx < m_channel_storage.get_number_of_channels(); ++channel_idx )
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
        
        if (m_save_contact_data)
        {
          save_contact_data();
        }
        
      }
      
      {
        util::Log logging;
        
        logging << "Application::run(): Total time reached "
        << m_total_time
        << util::Log::newline();
      }
      
      if(m_profiling)
      {
        m_engine.write_matlab_profiling_data( m_output_path + m_matlab_file );
      }
      
      if(m_xml_record)
      {
        save_xml_file();
      }
      
      return 0;
    }
    
  public:
    
    Application()
    {
    }
    
    virtual ~Application()
    {
      clear();
    }
    
  };
  
}// end of namespace prox_cmd

int main(int argc, char **argv)
{
  prox_cmd::Application app;
  
  std::string cfg_file = "";
  
  if (argv[1])
    cfg_file = argv[1];
  else
    cfg_file = "default.cfg";
  
  app.load_config_file(cfg_file);
  
  return app.run();
}
