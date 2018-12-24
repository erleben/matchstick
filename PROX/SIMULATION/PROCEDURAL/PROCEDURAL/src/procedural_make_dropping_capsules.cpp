//
// Created by ht on 7/10/18.
//
#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{

  template<typename MT>
  void make_dropping_capsules(
                              content::API *  engine
                              , typename MT::vector3_type const & position
                              , typename MT::quaternion_type const & orientation
                              , typename MT::real_type const & capsule_radius
                              , typename MT::real_type const & capsule_length
                              , typename MT::real_type const & spacing
                              , unsigned int const structure_direction
                              , size_t const & w
                              , size_t const & h
                              , size_t const & d
                              , MaterialInfo<typename MT::real_type> mat_info
                              )
  {
    typedef typename MT::real_type          T;
    typedef typename MT::vector3_type       V;
    typedef typename MT::quaternion_type    Q;
    typedef typename MT::value_traits       VT;

    T      const stone_density	= get_material_density<MT>(mat_info, "Stone");
    size_t const mid		    = get_material_id<MT>(mat_info, "Stone");


    GeometryHandle<MT> geometry_handle  = create_geometry_handle_capsule<MT>(  engine
                                                                             , capsule_radius
                                                                             , capsule_length
                                                                             );

    T x = VT::one();
    T y = VT::one();
    T z = VT::one();
    T const offset_x = capsule_radius * w;
    T const offset_z = capsule_radius * d;

    for (size_t i = 0; i < w; ++i)
    {
      x = VT::two()*i*capsule_radius+i*spacing - offset_x;
      for (size_t j = 0; j < h; ++j)
      {
        y = j*(capsule_length+spacing) + VT::four();

        for (size_t k = 0; k < d; ++k)
        {
          z = VT::two()*k*capsule_radius+k*spacing - offset_z;

          V const T_b2m = geometry_handle.Tb2m();
          Q const Q_b2m = geometry_handle.Qb2m();

          V const T_m2l = V::make( x, y, z );
          //Q const Q_m2l = Q::Rx(-VT::pi()/4);
          Q const Q_m2l = Q::identity();

          V const T_l2w = position;
          Q const Q_l2w = orientation;

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
                                             , geometry_handle
                                             , mid
                                             , stone_density
                                             );

          if( structure_direction == 0u)
          {
            // direction along capsule axis
            engine->set_material_structure_map(
                                               rid
                                               , VT::zero(), VT::one(), VT::zero()
                                               , VT::one(), VT::zero(), VT::zero()
                                               , VT::zero(), VT::one(), VT::zero()
                                               );
          }
          else if (structure_direction == 1u)
          {
            // direction across capsule axis
            engine->set_material_structure_map(
                                               rid
                                               , VT::zero(), VT::one(), VT::zero()
                                               , VT::one(), VT::zero(), VT::zero()
                                               , VT::zero(), VT::zero(), -VT::one()
                                               );

          }
          else
          {

          }

        }
      }
    }
  }

  typedef tiny::MathTypes<float> MTf;

  template
  void make_dropping_capsules<MTf>(
                                    content::API *  engine
                                    ,  MTf::vector3_type const & position
                                    ,  MTf::quaternion_type const & orientation
                                    ,  MTf::real_type const & cylinder_radius
                                    ,  MTf::real_type const & cylinder_length
                                    ,  MTf::real_type const & spacing
                                    , unsigned int const structure_dir
                                    , size_t const & w
                                    , size_t const & h
                                    , size_t const & d
                                    , MaterialInfo< MTf::real_type> mat_info
                                    );

} //namespace procedural
