#ifndef PROCEDURAL_H
#define PROCEDURAL_H

#include <procedural_noise.h>
#include <procedural_types.h>
#include <procedural_factory.h>
#include <procedural_make_obj_packing.h>
#include <procedural_make_grain_packing.h>
#include <procedural_make_box_on_inclined_plane.h>

#include <content.h>
#include <mesh_array.h>

#include <vector>

namespace procedural
{
	
  template<typename MT>
	void make_arch(  content::API * engine
                 , typename MT::vector3_type const& position
                 , typename MT::quaternion_type const & orientation
                 , typename MT::real_type const & r_outer
                 , typename MT::real_type const & r_inner
                 , typename MT::real_type const & pillar_height
                 , typename MT::real_type const & stone_depth
                 , size_t const & arch_slices
                 , size_t const & pillar_segments
                 , MaterialInfo<typename MT::real_type> mat_info
                 );
	
  template<typename MT>
	void make_cannonball(  content::API * engine
                         , typename MT::real_type         const & radius
                         , typename MT::vector3_type      const & position
                         , typename MT::quaternion_type   const & orientation
                         , typename MT::vector3_type      const & direction
                         , MaterialInfo<typename MT::real_type> mat_info
                         );
	
  template<typename MT>
	GeometryHandle<MT> make_cannonball_geometry(  content::API * physics
                                              , typename MT::real_type const & radius
                                              );
	
  template<typename MT>
	size_t make_cannonball_rigid_body(  content::API * physics
                                    , GeometryHandle<MT> const & ball
                                    , typename MT::vector3_type const & position
                                    , typename MT::quaternion_type const & orientation
                                    , typename MT::vector3_type const & direction
                                    , MaterialInfo<typename MT::real_type> mat_info
                                    );
	
  template<typename MT>
	void make_colosseum(  content::API * engine
                      , typename MT::vector3_type const & position
                      , typename MT::quaternion_type const & orientation
                      , typename MT::real_type const & r_outer
                      , typename MT::real_type const & r_inner
                      , size_t const & slices
                      , size_t const & segments
                      , MaterialInfo<typename MT::real_type> mat_info
                      );
	
  template<typename MT>
  void make_slide(  content::API * engine
                  , typename MT::vector3_type const & position
                  , typename MT::quaternion_type const & orientation
                  , typename MT::real_type const & pi_frac
                  , MaterialInfo<typename MT::real_type> mat_info
                  );

  template<typename MT>
  void make_cylinder(  content::API * engine
                  , typename MT::vector3_type const & position
                  , typename MT::quaternion_type const & orientation
                  , typename MT::real_type const & radius
                  , typename MT::real_type const & length
                  , typename MT::real_type const & Sa
                  , typename MT::real_type const & Sb
                  , MaterialInfo<typename MT::real_type> mat_info
                  );


  template<typename MT>
	void make_temple(  content::API * engine
                   , typename MT::vector3_type const & position
                   , typename MT::quaternion_type const & orientation
                   , typename MT::real_type const & temple_height
                   , typename MT::real_type const & pillar_width
                   , size_t const & num_pillars_x
                   , size_t const & num_pillars_z
                   , MaterialInfo<typename MT::real_type> mat_info
                   );
	
  template<typename MT>
	void make_dome(  content::API * engine
                 , typename MT::vector3_type const & position
                 , typename MT::quaternion_type const & orientation
                 , typename MT::real_type const & r_outer
                 , typename MT::real_type const & r_inner
                 , size_t const & slices
                 , size_t const & segments
                 , MaterialInfo<typename MT::real_type> mat_info
                 );
	
  template<typename MT>
	void make_ground(  content::API *  engine
                   , typename MT::vector3_type const & position
                   , typename MT::quaternion_type const & orientation
                   , MaterialInfo<typename MT::real_type> mat_info
                   , typename MT::real_type const & width
                   , typename MT::real_type const & height
                   , typename MT::real_type const & depth
                   );
	
  template<typename MT>
 	void make_pantheon(  content::API * engine
                     , typename MT::vector3_type const & position
                     , typename MT::quaternion_type const & orientation
                     , typename MT::real_type const & r_outer
                     , typename MT::real_type const & r_inner
                     , typename MT::real_type const & height
                     , size_t const & slices
                     , size_t const & segments
                     , MaterialInfo<typename MT::real_type> mat_info
                     );
	
  template<typename MT>
	void make_tower(  content::API * engine
                  , typename MT::vector3_type const & position
                  , typename MT::quaternion_type const & orientation
                  , typename MT::real_type const & r_outer
                  , typename MT::real_type const & r_inner
                  , typename MT::real_type const & height
                  , size_t const & slices
                  , size_t const & segments
                  , MaterialInfo<typename MT::real_type> mat_info
                  , bool const & use_cubes
                  );

  template<typename MT>
	void make_wall(  content::API * engine
                 , typename MT::vector3_type const& position
                 , typename MT::quaternion_type const & orientation
                 , typename MT::real_type const & width
                 , typename MT::real_type const & height
                 , typename MT::real_type const & depth
                 , size_t const & layers
                 , size_t const & span
                 , MaterialInfo<typename MT::real_type> mat_info
                 );
	
  template<typename MT>
	void make_greek_pillar(  content::API *  engine
                         , typename MT::vector3_type const & position
                         , typename MT::quaternion_type const & orientation
                         , typename MT::real_type const & pillar_width
                         , typename MT::real_type const & pillar_height
                         , typename MT::real_type const & pillar_depth
                         , size_t const & pillar_segments
                         , size_t const & pillar_slices
                         , MaterialInfo<typename MT::real_type> mat_info
                         , mesh_array::TetGenSettings tetset  = mesh_array::tetgen_default_settings()
                         );
  
  template<typename MT>
	void make_sphere_cube(  content::API *  engine
                        , typename MT::vector3_type const & position
                        , typename MT::quaternion_type const & orientation
                        , typename MT::real_type const & sphere_radius
                        , size_t const & spheres
                        , MaterialInfo<typename MT::real_type> mat_info
                        );
  
  template<typename MT>
	void make_tetrahedral_stack(  content::API *  engine
                              , typename MT::vector3_type const & position
                              , typename MT::quaternion_type const & orientation
                              , typename MT::real_type const & cube_width
                              , typename MT::real_type const & cube_height
                              , typename MT::real_type const & cube_depth
                              , size_t const & sub_divisions
                              , MaterialInfo<typename MT::real_type> mat_info
                              );
  template<typename MT>
  void make_stack(
                  content::API * engine
                  , typename MT::vector3_type const& position
                  , typename MT::quaternion_type const & orientation
                  , typename MT::real_type const & stone_dim
                  , size_t const & layers
                  , MaterialInfo<typename MT::real_type> mat_info
                  );
  
  template<typename MT>
  void make_heavy_sphere_light_sphere(
                                      content::API *  engine
                                      , typename MT::vector3_type const & position
                                      , typename MT::quaternion_type const & orientation
                                      , typename MT::real_type const & sphere_radius
                                      , size_t const & scale
                                      , MaterialInfo<typename MT::real_type> mat_info
                                      );
  
  template<typename MT>
  void make_dropping_spheres(
                             content::API *  engine
                             , typename MT::vector3_type const & position
                             , typename MT::quaternion_type const & orientation
                             , typename MT::real_type const & sphere_radius
                             , size_t const & w
                             , size_t const & h
                             , size_t const & d
                             , MaterialInfo<typename MT::real_type> mat_info
                             );

  template<typename MT>
  void make_dropping_capsules(
                             content::API *  engine
                             , typename MT::vector3_type const & position
                             , typename MT::quaternion_type const & orientation
                             , typename MT::real_type const & cylinder_radius
                             , typename MT::real_type const & cylinder_length
                             , typename MT::real_type const & spacing
                             , unsigned int const structure_dir
                             , size_t const & w
                             , size_t const & h
                             , size_t const & d
                             , MaterialInfo<typename MT::real_type> mat_info
                             );
  
  template<typename MT>
	void make_spheres(
                    content::API * engine
                    , typename MT::vector3_type const& position
                    , typename MT::quaternion_type const & orientation
                    , typename MT::real_type const & radius
                    , MaterialInfo<typename MT::real_type> mat_info
                    );
  
  template<typename MT>
  void make_tetrahedron(
                content::API * engine
                , typename MT::vector3_type const& one
                , typename MT::vector3_type const& two
                , typename MT::vector3_type const& three
                , typename MT::vector3_type const& four
                , typename MT::vector3_type const & position
                , typename MT::quaternion_type const & orientation
                , MaterialInfo<typename MT::real_type> mat_info
                , bool const fixed
                );
  
  template<typename MT>
	void make_tetrahedron(
                        content::API * engine
                        , typename MT::vector3_type const& position
                        , typename MT::quaternion_type const & orientation
                        , MaterialInfo<typename MT::real_type> mat_info
                        , bool const fixed = false
                        );
  
  template<typename MT>
	void make_obj(
                content::API * engine
                , std::string const & name
                , typename MT::real_type const & scale
                , typename MT::vector3_type const & position
                , typename MT::quaternion_type const & orientation
                , MaterialInfo<typename MT::real_type> mat_info
                , bool const fixed = false
                , std::string const & material = "Stone"
                , mesh_array::TetGenSettings = mesh_array::tetgen_default_settings()
                );

  template<typename MT>
  void make_box(
                content::API *  engine
                , typename MT::vector3_type const & position
                , typename MT::quaternion_type const & orientation
                , typename MT::real_type const & width
                , typename MT::real_type const & height
                , typename MT::real_type const & depth
                , MaterialInfo<typename MT::real_type> mat_info
                , bool const fixed = false
                );
  
  template<typename MT>
  void make_twisted_stack(
                          content::API * engine
                          , typename MT::vector3_type const& position
                          , typename MT::quaternion_type const & orientation
                          , typename MT::real_type const & stone_dim
                          , size_t const & layers
                          , typename MT::real_type const & twist
                          , MaterialInfo<typename MT::real_type> mat_info
                          );
  template<typename MT>
  void make_sphere_layer(
                         content::API *  engine
                         , typename MT::vector3_type const & position
                         , typename MT::quaternion_type const & orientation
                         , typename MT::real_type const & sphere_radius
                         , size_t const & spheres_width
                         , size_t const & spheres_length
                         , MaterialInfo<typename MT::real_type> mat_info
                         );

  template<typename MT>
	void make_box_container(  content::API *  engine
                          , typename MT::vector3_type const & position
                          , typename MT::quaternion_type const & orientation
                          , typename MT::real_type const & width
                          , typename MT::real_type const & height
                          , typename MT::real_type const & depth
                          , typename MT::real_type const & wall_thickness
                          , MaterialInfo<typename MT::real_type> mat_info
                          );

  template<typename MT>
	void make_sphere_packing(
                           content::API *  engine
                           , typename MT::vector3_type const & position
                           , typename MT::quaternion_type const & orientation
                           , typename MT::real_type const & min_radius
                           , typename MT::real_type const & max_radius
                           , typename MT::real_type const & width
                           , typename MT::real_type const & height
                           , typename MT::real_type const & depth
                           , size_t const & number_of_spheres
                           , MaterialInfo<typename MT::real_type> mat_info
                           );

  template<typename MT>
  void make_sliding_point(
                          content::API *  engine
                          , typename MT::vector3_type const & position
                          , typename MT::quaternion_type const & orientation
                          , typename MT::real_type const & scene_size
                          , MaterialInfo<typename MT::real_type> const & mat_info
                          , bool const & use_spike
                          , bool const & use_wedge
                          , mesh_array::TetGenSettings tetset
                          );

  template<typename MT>
  void make_two_points(
                       content::API *  engine
                       , typename MT::vector3_type const & position
                       , typename MT::quaternion_type const & orientation
                       , typename MT::real_type const & scene_size
                       , MaterialInfo<typename MT::real_type> const & mat_info
                       , bool const & use_spike
                       , bool const & use_wedge
                       , bool const & use_spike_and_wedge
                       , mesh_array::TetGenSettings tetset
                       );

  template<typename MT>
  void make_point_in_crack(
                          content::API *  engine
                          , typename MT::vector3_type const & position
                          , typename MT::quaternion_type const & orientation
                          , typename MT::real_type const & scene_size
                          , MaterialInfo<typename MT::real_type> const & mat_info
                          , mesh_array::TetGenSettings tetset
                          );

  template<typename MT>
  void make_cliff_edge(
                       content::API *  engine
                       , typename MT::vector3_type const & position
                       , typename MT::quaternion_type const & orientation
                       , typename MT::real_type const & scene_size
                       , MaterialInfo<typename MT::real_type> const & mat_info
                       , mesh_array::TetGenSettings tetset
                       );

  template<typename MT>
  void make_internal_edge(
                          content::API *  engine
                          , typename MT::vector3_type const & position
                          , typename MT::quaternion_type const & orientation
                          , typename MT::real_type const & scene_size
                          , MaterialInfo<typename MT::real_type> const & mat_info
                          , mesh_array::TetGenSettings tetset
                          );

  template<typename MT>
  MaterialInfo<typename MT::real_type> create_material_info( content::API * engine );
  
  
} //namespace procedural

#include <procedural_make_scene.h>

// PROCEDURAL_H
#endif
