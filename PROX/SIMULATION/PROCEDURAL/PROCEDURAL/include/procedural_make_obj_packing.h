#ifndef PROCEDURAL_MAKE_OBJ_PACKING_H
#define PROCEDURAL_MAKE_OBJ_PACKING_H

#include <procedural_factory.h>

#include <mesh_array.h>

#include <tiny_math_types.h>

#include <vector>

namespace procedural
{

  template<typename MT>
  inline void make_obj_packing(
                               content::API *  engine
                               , typename MT::vector3_type const & position
                               , typename MT::quaternion_type const & orientation
                               , std::vector< std::string> const & obj_names
                               , unsigned int const & number_of_objects_in_x
                               , unsigned int const & number_of_objects_in_y
                               , unsigned int const & number_of_objects_in_z
                               , typename MT::real_type const & object_size
                               , typename MT::real_type const & spacing
                               , MaterialInfo<typename MT::real_type> const & mat_info
                               , mesh_array::TetGenSettings tetset = mesh_array::tetgen_default_settings()
                               )
  {
    using std::floor;
    using std::ceil;

    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type Q;
    typedef typename MT::value_traits    VT;

    T      const stone_density	 = get_material_density<MT>(mat_info, "Stone");
    size_t const mid		      	 = get_material_id<MT>(mat_info, "Stone");
    size_t const number_of_names = obj_names.size();

    if(number_of_names==0)
      return;

    std::vector< GeometryHandle<MT> > obj_handle;
    obj_handle.resize(number_of_names);

    for (unsigned int c=0;c< number_of_names; ++c)
    {
      obj_handle[c] = create_geometry_handle_obj<MT>( engine, obj_names[c], object_size, false, tetset);
    }

    T const width  = number_of_objects_in_x*object_size + (number_of_objects_in_x-1u)*spacing;
    T const height = number_of_objects_in_y*object_size + (number_of_objects_in_y-1u)*spacing;
    T const depth  = number_of_objects_in_z*object_size + (number_of_objects_in_z-1u)*spacing;

    T const min_x = (-width  + object_size)*VT::half();
    T const min_y = (-height + object_size)*VT::half();
    T const min_z = (-depth  + object_size)*VT::half();

    unsigned int const I = number_of_objects_in_x;
    unsigned int const J = number_of_objects_in_y;
    unsigned int const K = number_of_objects_in_z;

    unsigned int obj_count = 0u;

    for(unsigned int j=0u; j < J; ++j)
      for(unsigned int k=0u; k < K; ++k)
        for(unsigned int i=0u; i < I; ++i)
        {
          T const x = (min_x + i*(object_size+spacing));
          T const y = (min_y + j*(object_size+spacing));
          T const z = (min_z + k*(object_size+spacing));

          int choice = obj_count % number_of_names;

          V const T_b2m = obj_handle[choice].Tb2m();
          Q const Q_b2m = obj_handle[choice].Qb2m();

          V const T_m2l = V::make( x, y, z );
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

          create_rigid_body<MT>(
                                engine
                                , T_b2w
                                , Q_b2w
                                , obj_handle[choice]
                                , mid
                                , stone_density
                                );
          ++obj_count;
        }
  }
  
} //namespace procedural

// PROCEDURAL_MAKE_OBJ_PACKING_H
#endif