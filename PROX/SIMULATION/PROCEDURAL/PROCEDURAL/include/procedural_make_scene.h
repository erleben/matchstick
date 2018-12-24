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
    tetset.m_quality_ratio = util::to_value<double>(params.get_value("tetgen_quality_ratio", "2.0"));
    tetset.m_maximum_volume = util::to_value<double>(params.get_value("tetgen_maximum_volume", "0.0"));
    tetset.m_quiet_output = util::to_value<bool>(params.get_value("tetgen_quiet_output", "true"));
    tetset.m_suppress_splitting = util::to_value<bool>(params.get_value("tetgen_suppress_splitting", "true"));

    procedural::MaterialInfo<T> mat_info = procedural::create_material_info<MT>(engine);

    if (scene.compare("arch") == 0)
    {
      procedural::make_ground<MT>(
                                  engine, V::zero(), Q::identity(), mat_info, 10.0f, 1.0f, 10.0f
                                  );

      procedural::make_arch<MT>(
                                engine, V::zero(), Q::identity()
                                , 2      // r outer
                                , 1.5    // r inner
                                , 2      // pillar height
                                , 2.0    // stone depth
                                , 7      // arch slices
                                , 3      // pillar segments
                                , mat_info
                                );

    }
    if (scene.compare("stack") == 0)
    {
      procedural::make_ground<MT>(
                                  engine, V::zero(), Q::identity(), mat_info, 10.0f, 1.0f, 10.0f
                                  );

      unsigned int const layers = util::to_value<unsigned int>(params.get_value("procedural_param_1", "5"));

      procedural::make_stack<MT>(
                                 engine, V::make(VT::zero(), VT::zero(), VT::zero()), Q::identity(), VT::one(), layers, mat_info
                                 );
    }
    if (scene.compare("box_on_inclined_plane") == 0)
    {
      // Angle of the slide
      float const degree = util::to_value<float>(params.get_value("procedural_param_1", "20.0"));
      // Angle of the down sliding thingy
      float const angle = util::to_value<float>(params.get_value("procedural_param_2", "20.0"));


      procedural::make_ground<MT>(
                                  engine, V::zero(), Q::identity(), mat_info, 35, 1.0f, 10
                                  );

      procedural::make_slide_angle<MT>(
                                       engine, V::zero(), Q::identity(), degree, angle, mat_info
                                       );
    }
    if (scene.compare("rolling_cylinder") == 0)
    {
      // Angle of the slide
      float const radius = util::to_value<float>(params.get_value("procedural_param_1", "2.0"));
      // Angle of the down sliding thingy
      float const length = util::to_value<float>(params.get_value("procedural_param_2", "3.0"));

      // S vector of ramp
      float const Sa = util::to_value<float>(params.get_value("procedural_param_3", "0.0"));
      // S vector of cylinder
      float const Sb = util::to_value<float>(params.get_value("procedural_param_4", "0.0"));

      procedural::make_ground<MT>(
                                  engine, V::zero(), Q::identity(), mat_info, 32.0f, 1.0f, 20.0f
                                  );

      procedural::make_cylinder<MT>(
                                    engine, V::zero(), Q::identity(), radius, length, Sa, Sb, mat_info
                                    );

    }
    if (scene.compare("capsule_hopper") == 0)
    {
      float const funnel_size = util::to_value<float>(params.get_value("procedural_param_1", "10.0"));
      float const funnel_height = util::to_value<float>(params.get_value("procedural_param_2", "5.0"));
      float const ground_size = util::to_value<float>(params.get_value("procedural_param_3", "10.0"));
      float const spacing = util::to_value<float>(params.get_value("procedural_param_4", "0.1"));
      float const width_x = util::to_value<float>(params.get_value("procedural_param_5", "10.0"));
      float const height_y = util::to_value<float>(params.get_value("procedural_param_6", "10.0"));
      float const depth_z = util::to_value<float>(params.get_value("procedural_param_7", "10.0"));
      float const cylinder_radius = util::to_value<float>(params.get_value("procedural_param_8", "0.5"));
      float const cylinder_length = util::to_value<float>(params.get_value("procedural_param_9", "1"));
      unsigned int const structure_direction = util::to_value<float>(params.get_value("procedural_param_10", "1"));

      procedural::make_ground<MT>(
                                  engine, V::zero(), Q::identity(), mat_info, ground_size, 1.0f, ground_size
                                  );

      if (structure_direction == 1)
      {
        procedural::make_obj<MT>(
                                 engine, obj_path + "funnel.obj", funnel_size, V::make(0, funnel_height, 0), V::make(0, 1, 0),
                                 Q::Rz(VT::pi()), mat_info,
                                 true, false, "Stone", tetset
                                 );
      }
      else
      {
        procedural::make_obj<MT>(
                                 engine, obj_path + "funnel.obj", funnel_size, V::make(0, funnel_height, 0), V::make(1, 0, 0),
                                 Q::Rz(VT::pi()), mat_info,
                                 true, false, "Stone", tetset
                                 );
      }


      procedural::make_dropping_capsules<MT>(
                                             engine
                                             , V::make(0, funnel_height, 0)
                                             , Q::identity()
                                             , cylinder_radius
                                             , cylinder_length
                                             , spacing
                                             , structure_direction
                                             , width_x, height_y, depth_z
                                             , mat_info
                                             );

    }

  }
}//namespace procedural


// PROCEDURAL_MAKE_SCENE_H
#endif