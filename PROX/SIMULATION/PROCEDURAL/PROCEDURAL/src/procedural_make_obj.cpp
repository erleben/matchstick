#include <mesh_array.h>

#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{

  template<typename MT>
  void make_obj(
                content::API * engine
                , std::string const & name
                , typename MT::real_type const & scale
                , typename MT::vector3_type const& position
                , typename MT::vector3_type const& structure_direction
                , typename MT::quaternion_type const & orientation
                , MaterialInfo<typename MT::real_type> mat_info
                , bool const fixed = false
                , bool const blind_copy = false
                , std::string const & material = "Stone"
                , mesh_array::TetGenSettings tetset = mesh_array::tetgen_default_settings()
                )
  {
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type Q;
    typedef typename MT::value_traits    VT;

    T const stone_density = get_material_density<MT>(mat_info, material);
    size_t const mid      = get_material_id<MT>(mat_info, material);

    GeometryHandle<MT> obj_handle = create_geometry_handle_obj<MT>(engine, name, scale, blind_copy, tetset);

    V const T_b2m = obj_handle.Tb2m();
    Q const Q_b2m = obj_handle.Qb2m();

    V const T_m2l = V::make( VT::zero(), VT::half(), VT::zero() );
    Q const Q_m2l = Q::identity();

    V const T_l2w = blind_copy ? V::zero()     : position;
    Q const Q_l2w = blind_copy ? Q::identity() : orientation;

    V T_b2w;
    Q Q_b2w;

    compute_body_to_world_transform<MT>(
                                        T_b2m
                                        , Q_b2m
                                        , T_m2l
                                        , Q_m2l
                                        , T_l2w
                                        , Q_l2w
                                        , T_b2w
                                        , Q_b2w
                                        );

    size_t rid = create_rigid_body<MT>(  engine
                                       , T_b2w
                                       , Q_b2w
                                       , obj_handle
                                       , mid
                                       , stone_density
                                       , fixed
                                       );

    engine->set_material_structure_map(
                                       rid
                                       , structure_direction(0)
                                       , structure_direction(1)
                                       , structure_direction(2)
                                       );
  }


  template<typename MT>
  void make_obj(
                content::API * engine
                , std::string const & name
                , typename MT::real_type const & scale
                , typename MT::vector3_type const& position
                , typename MT::quaternion_type const & orientation
                , MaterialInfo<typename MT::real_type> mat_info
                , bool const fixed = false
                , bool const blind_copy = false
                , std::string const & material = "Stone"
                , mesh_array::TetGenSettings tetset = mesh_array::tetgen_default_settings()
                )
  {
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type Q;
    typedef typename MT::value_traits    VT;

    T const stone_density = get_material_density<MT>(mat_info, material);
    size_t const mid      = get_material_id<MT>(mat_info, material);

    GeometryHandle<MT> obj_handle = create_geometry_handle_obj<MT>(engine, name, scale, blind_copy, tetset);

    V const T_b2m = obj_handle.Tb2m();
    Q const Q_b2m = obj_handle.Qb2m();

    V const T_m2l = V::make( VT::zero(), VT::half(), VT::zero() );
    Q const Q_m2l = Q::identity();

    V const T_l2w = blind_copy ? V::zero()     : position;
    Q const Q_l2w = blind_copy ? Q::identity() : orientation;

    V T_b2w;
    Q Q_b2w;

    compute_body_to_world_transform<MT>(
                                        T_b2m
                                        , Q_b2m
                                        , T_m2l
                                        , Q_m2l
                                        , T_l2w
                                        , Q_l2w
                                        , T_b2w
                                        , Q_b2w
                                        );

    size_t rid = create_rigid_body<MT>(  engine
                                       , T_b2w
                                       , Q_b2w
                                       , obj_handle
                                       , mid
                                       , stone_density
                                       , fixed
                                       );

    engine->set_material_structure_map(
                                       rid
                                       , 1.0
                                       , 0.0
                                       ,0.0
                                       );
  }

  typedef tiny::MathTypes<float> MTf;

  template
  void make_obj<MTf>(
                     content::API * engine
                     , std::string const & name
                     , MTf::real_type const & scale
                     , MTf::vector3_type const & position
                     , MTf::quaternion_type const & orientation
                     , MaterialInfo<MTf::real_type> mat_info
                     , bool const fixed
                     , bool const blind_copy
                     , std::string const & material
                     , mesh_array::TetGenSettings tetset
                     );


  template
  void make_obj<MTf>(
                     content::API * engine
                     , std::string const & name
                     , MTf::real_type const & scale
                     , MTf::vector3_type const & position
                     , MTf::vector3_type const & structure_direction
                     , MTf::quaternion_type const & orientation
                     , MaterialInfo<MTf::real_type> mat_info
                     , bool const fixed
                     , bool const blind_copy
                     , std::string const & material
                     , mesh_array::TetGenSettings tetset
                     );


} //namespace procedural
