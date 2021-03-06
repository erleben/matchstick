#ifndef PROCEDURAL_MAKE_SCENE_H
#define PROCEDURAL_MAKE_SCENE_H

#include <content.h>
#include <procedural.h>
#include <procedural_factory.h>

#include <mesh_array.h>

#include <util_config_file.h>

#include <cstdlib>

namespace procedural
{
  
  /**
   * This function can be used to quickly generate known test scenes.
   *
   * @param scene        The named scene to create.
   * @param obj_path     The folder location to find obj-files.
   * @param engine       A pointer to the engine that upon return will have added the named scene to it.
   * @param params       Procedural parameter settings for controlling "some" of the name scenes. Parameters
   *                     could be size of scene or number of object, or object types etc..
   */
  template<typename MT>
  inline void make_scene(
                         std::string const & scene
                         , std::string const & obj_path
                         , content::API * engine
                         , util::ConfigFile const & params
                         ) {
    typedef typename MT::real_type T;
    typedef typename MT::value_traits VT;
    typedef typename MT::vector3_type V;
    typedef typename MT::quaternion_type Q;
    
    mesh_array::TetGenSettings tetset = mesh_array::tetgen_default_settings();
    tetset.m_quality_ratio      = util::to_value<double>( params.get_value("tetgen_quality_ratio", "2.0")       );
    tetset.m_maximum_volume     = util::to_value<double>( params.get_value("tetgen_maximum_volume", "0.0")      );
    tetset.m_quiet_output       = util::to_value<bool>(   params.get_value("tetgen_quiet_output", "true")       );
    tetset.m_suppress_splitting = util::to_value<bool>(   params.get_value("tetgen_suppress_splitting", "true") );
    
    procedural::MaterialInfo<T> mat_info = procedural::create_material_info<MT>(engine);
    
    if (scene.compare("box_on_inclined_plane") == 0)
    {
      // Angle of inclined plane in degrees
      float const angle = util::to_value<float>(params.get_value("procedural_param_1", "20.0"));
      
      // Length of plane slide
      float const length = util::to_value<float>(params.get_value("procedural_param_2", "32.0"));
      
      float const structure_field_plane_x = util::to_value<float>(params.get_value("procedural_param_3", "1.0"));
      float const structure_field_plane_y = util::to_value<float>(params.get_value("procedural_param_4", "0.0"));
      float const structure_field_plane_z = util::to_value<float>(params.get_value("procedural_param_5", "0.0"));
      float const structure_field_box_x   = util::to_value<float>(params.get_value("procedural_param_6", "1.0"));
      float const structure_field_box_y   = util::to_value<float>(params.get_value("procedural_param_7", "0.0"));
      float const structure_field_box_z   = util::to_value<float>(params.get_value("procedural_param_8", "0.0"));
      float const mu_iso                  = util::to_value<float>(params.get_value("procedural_param_9", "1.0"));
      float const mu_ani_x                = util::to_value<float>(params.get_value("procedural_param_10", "1.0"));
      float const mu_ani_y                = util::to_value<float>(params.get_value("procedural_param_11", "1.0"));
      float const mu_ani_z                = util::to_value<float>(params.get_value("procedural_param_12", "1.0"));
      
      {
        size_t const mid = get_material_id<MT>(mat_info, "Stone");
        
        engine->set_coefficient_of_isotropic_friction( mid
                                                      , mid
                                                      , mu_iso
                                                      );
        
        engine->set_coefficients_of_anisotropic_friction( mid
                                                         , mid
                                                         , mu_ani_x
                                                         , mu_ani_y
                                                         , mu_ani_z
                                                         );
      }

      procedural::make_ground<MT>(
                                  engine
                                  , V::zero()
                                  , Q::identity()
                                  , mat_info
                                  , length*1.2f
                                  , 1.0f
                                  , length/3.0f
                                  , 0.0f, 0.0f, 1.0f   // structure direction of ground
                                  );
      
      procedural::make_box_on_inclined_plane<MT>(
                                                 engine
                                                 , V::zero()
                                                 , Q::identity()
                                                 , angle
                                                 , length
                                                 , V::make(structure_field_plane_x, structure_field_plane_y, structure_field_plane_z)
                                                 , V::make(structure_field_box_x, structure_field_box_y, structure_field_box_z )
                                                 , mat_info
                                                 );
    }
    if (scene.compare("rolling_cylinder") == 0)
    {
      // Angle of inclined plane in degrees
      float const angle = util::to_value<float>(params.get_value("procedural_param_1", "20.0"));
      
      // Length of plane slide
      float const length = util::to_value<float>(params.get_value("procedural_param_2", "32.0"));
      
      float const structure_field_plane_x = util::to_value<float>(params.get_value("procedural_param_3", "1.0"));
      float const structure_field_plane_y = util::to_value<float>(params.get_value("procedural_param_4", "0.0"));
      float const structure_field_plane_z = util::to_value<float>(params.get_value("procedural_param_5", "0.0"));
      float const structure_field_box_x   = util::to_value<float>(params.get_value("procedural_param_6", "1.0"));
      float const structure_field_box_y   = util::to_value<float>(params.get_value("procedural_param_7", "0.0"));
      float const structure_field_box_z   = util::to_value<float>(params.get_value("procedural_param_8", "0.0"));
      float const mu_iso                  = util::to_value<float>(params.get_value("procedural_param_9", "1.0"));
      float const mu_ani_x                = util::to_value<float>(params.get_value("procedural_param_10", "1.0"));
      float const mu_ani_y                = util::to_value<float>(params.get_value("procedural_param_11", "1.0"));
      float const mu_ani_z                = util::to_value<float>(params.get_value("procedural_param_12", "1.0"));
      
      {
        size_t const mid = get_material_id<MT>(mat_info, "Stone");
        
        engine->set_coefficient_of_isotropic_friction( mid
                                                      , mid
                                                      , mu_iso
                                                      );
        
        engine->set_coefficients_of_anisotropic_friction( mid
                                                         , mid
                                                         , mu_ani_x
                                                         , mu_ani_y
                                                         , mu_ani_z
                                                         );
      }

      procedural::make_ground<MT>(
                                  engine
                                  , V::zero()
                                  , Q::identity()
                                  , mat_info
                                  , length*1.2f
                                  , 1.0f
                                  , length/3.0f
                                  , 0.0f, 0.0f, 1.0f   // Structure direction of ground
                                  );
      
      procedural::make_cylinder_on_inclined_plane<MT>(
                                                      engine
                                                      , V::zero()
                                                      , Q::identity()
                                                      , angle
                                                      , length
                                                      , V::make(structure_field_plane_x, structure_field_plane_y, structure_field_plane_z)
                                                      , V::make(structure_field_box_x, structure_field_box_y, structure_field_box_z )
                                                      , mat_info
                                                      );
      
    }
    if (scene.compare("arch") == 0)
    {
      float const r_outer           = util::to_value<float>(params.get_value("procedural_param_1", "2.0"));
      float const r_inner           = util::to_value<float>(params.get_value("procedural_param_2", "1.5"));
      float const pillar_height     = util::to_value<float>(params.get_value("procedural_param_3", "2.0"));
      float const stone_depth       = util::to_value<float>(params.get_value("procedural_param_4", "2.0"));
      size_t const arch_slices      = util::to_value<size_t>(params.get_value("procedural_param_5", "7"));
      size_t const pillar_segments  = util::to_value<size_t>(params.get_value("procedural_param_6", "3"));
      float const structure_field_x = util::to_value<float>(params.get_value("procedural_param_7", "1.0"));
      float const structure_field_y = util::to_value<float>(params.get_value("procedural_param_8", "0.0"));
      float const structure_field_z = util::to_value<float>(params.get_value("procedural_param_9", "0.0"));
      float const mu_iso            = util::to_value<float>(params.get_value("procedural_param_10", "1.0"));
      float const mu_ani_x          = util::to_value<float>(params.get_value("procedural_param_11", "1.0"));
      float const mu_ani_y          = util::to_value<float>(params.get_value("procedural_param_12", "1.0"));
      float const mu_ani_z          = util::to_value<float>(params.get_value("procedural_param_13", "1.0"));
      
      {
        size_t const mid = get_material_id<MT>(mat_info, "Stone");
        
        engine->set_coefficient_of_isotropic_friction( mid
                                                      , mid
                                                      , mu_iso
                                                      );
        
        engine->set_coefficients_of_anisotropic_friction( mid
                                                         , mid
                                                         , mu_ani_x
                                                         , mu_ani_y
                                                         , mu_ani_z
                                                         );
      }

      procedural::make_ground<MT>(
                                  engine
                                  , V::zero()
                                  , Q::identity()
                                  , mat_info
                                  , 10.0f, 1.0f, 10.0f
                                  , 0.0, 0.0, 1.0      // Has structural field in z-direction
                                  );
      
      procedural::make_arch<MT>(
                                engine, V::zero(), Q::identity()
                                , r_outer
                                , r_inner
                                , pillar_height
                                , stone_depth
                                , arch_slices
                                , pillar_segments
                                , structure_field_x
                                , structure_field_y
                                , structure_field_z
                                , mat_info
                                );
      
    }
    if (scene.compare("stack") == 0)
    {
      
      unsigned int const layers            = util::to_value<unsigned int>(params.get_value("procedural_param_1", "5"));
      float        const stone_width       = util::to_value<float>(       params.get_value("procedural_param_2", "1.0"));
      float        const structure_field_x = util::to_value<float>(       params.get_value("procedural_param_3", "1.0"));
      float        const structure_field_y = util::to_value<float>(       params.get_value("procedural_param_4", "0.0"));
      float        const structure_field_z = util::to_value<float>(       params.get_value("procedural_param_5", "0.0"));
      float        const mu_iso            = util::to_value<float>(       params.get_value("procedural_param_6", "1.0"));
      float        const mu_ani_x          = util::to_value<float>(       params.get_value("procedural_param_7", "1.0"));
      float        const mu_ani_y          = util::to_value<float>(       params.get_value("procedural_param_8", "1.0"));
      float        const mu_ani_z          = util::to_value<float>(       params.get_value("procedural_param_9", "1.0"));
      float        const px                = util::to_value<float>(       params.get_value("procedural_param_10", "5.0"));
      float        const py                = util::to_value<float>(       params.get_value("procedural_param_11", "10.0"));
      float        const pz                = util::to_value<float>(       params.get_value("procedural_param_12", "0.0"));
      float        const vx                = util::to_value<float>(       params.get_value("procedural_param_13", "-10.0"));
      float        const vy                = util::to_value<float>(       params.get_value("procedural_param_14", "0.0"));
      float        const vz                = util::to_value<float>(       params.get_value("procedural_param_15", "0.0"));
      
      {
        size_t const mid = get_material_id<MT>(mat_info, "Stone");
        
        engine->set_coefficient_of_isotropic_friction( mid
                                                      , mid
                                                      , mu_iso
                                                      );
        
        engine->set_coefficients_of_anisotropic_friction( mid
                                                         , mid
                                                         , mu_ani_x
                                                         , mu_ani_y
                                                         , mu_ani_z
                                                         );
      }

      procedural::make_ground<MT>(
                                  engine
                                  , V::zero()
                                  , Q::identity()
                                  , mat_info
                                  , 10.0f, 1.0f, 10.0f
                                  , 0.0f, 0.0f, 1.0f   // structure direction of ground
                                  );
      
      procedural::make_stack<MT>(
                                 engine
                                 , V::make(VT::zero(), VT::zero(), VT::zero())
                                 , Q::identity()
                                 , stone_width
                                 , layers
                                 , structure_field_x
                                 , structure_field_y
                                 , structure_field_z
                                 , mat_info
                                 );
      
      procedural::make_ball<MT>(engine
                                , stone_width/2.0f  // radius
                                , V::make(px,py,pz) // position
                                , Q::identity()
                                , V::make(vx,vy,vz) // velocity
                                , mat_info
                                );
    }
    if (scene.compare("capsule_hopper") == 0)
    {
      float const funnel_size               = util::to_value<float>(params.get_value("procedural_param_1",  "10.0"));
      float const funnel_height             = util::to_value<float>(params.get_value("procedural_param_2",  "5.0" ));
      float const ground_size               = util::to_value<float>(params.get_value("procedural_param_3",  "10.0"));
      float const spacing                   = util::to_value<float>(params.get_value("procedural_param_4",  "0.1" ));
      size_t const number_in_x              = util::to_value<size_t>(params.get_value("procedural_param_5",  "3"));
      size_t const number_in_y              = util::to_value<size_t>(params.get_value("procedural_param_6",  "3"));
      size_t const number_in_z              = util::to_value<size_t>(params.get_value("procedural_param_7",  "3"));
      float const capsule_radius            = util::to_value<float>(params.get_value("procedural_param_8",  "0.5" ));
      float const capsule_length            = util::to_value<float>(params.get_value("procedural_param_9",  "1.0" ));
      float const structure_field_funnel_x  = util::to_value<float>(params.get_value("procedural_param_10", "1.0" ));
      float const structure_field_funnel_y  = util::to_value<float>(params.get_value("procedural_param_11", "0.0" ));
      float const structure_field_funnel_z  = util::to_value<float>(params.get_value("procedural_param_12", "0.0" ));
      float const structure_field_capsule_x = util::to_value<float>(params.get_value("procedural_param_13", "1.0" ));
      float const structure_field_capsule_y = util::to_value<float>(params.get_value("procedural_param_14", "0.0" ));
      float const structure_field_capsule_z = util::to_value<float>(params.get_value("procedural_param_15", "0.0" ));
      float const mu_iso                    = util::to_value<float>(params.get_value("procedural_param_16", "1.0" ));
      float const mu_ani_x                  = util::to_value<float>(params.get_value("procedural_param_17", "1.0" ));
      float const mu_ani_y                  = util::to_value<float>(params.get_value("procedural_param_18", "1.0" ));
      float const mu_ani_z                  = util::to_value<float>(params.get_value("procedural_param_19", "1.0" ));
      
      {
        size_t const mid = get_material_id<MT>(mat_info, "Stone");
        
        engine->set_coefficient_of_isotropic_friction( mid
                                                      , mid
                                                      , mu_iso
                                                      );
        
        engine->set_coefficients_of_anisotropic_friction( mid
                                                         , mid
                                                         , mu_ani_x
                                                         , mu_ani_y
                                                         , mu_ani_z
                                                         );
      }

      procedural::make_ground<MT>(
                                  engine
                                  , V::zero()
                                  , Q::identity()
                                  , mat_info
                                  , ground_size
                                  , 1.0f
                                  , ground_size
                                  , 0.0f, 0.0f, 1.0f   // structure direction of ground
                                  );
      
      procedural::make_obj<MT>(
                               engine
                               , obj_path + "funnel.obj"        // filename to read
                               , funnel_size                    // scale of object to be created
                               , V::make(0, funnel_height, 0)   // position of object
                               , Q::identity()                  // orientation of object
                               , structure_field_funnel_x
                               , structure_field_funnel_y
                               , structure_field_funnel_z
                               , mat_info         // material models info
                               , true            // fixed object
                               , "Stone"         // material name
                               , tetset          // tetgen settings
                               );
      
      procedural::make_dropping_capsules<MT>(
                                             engine
                                             , V::make(0, 2.0*funnel_height, 0)
                                             , Q::identity()
                                             , capsule_radius
                                             , capsule_length
                                             , spacing
                                             , structure_field_capsule_x
                                             , structure_field_capsule_y
                                             , structure_field_capsule_z
                                             , number_in_x
                                             , number_in_y
                                             , number_in_z
                                             , mat_info
                                             );
      
    }
    
  }
}//namespace procedural


// PROCEDURAL_MAKE_SCENE_H
#endif
