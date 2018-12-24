#ifndef PROCEDURAL_MAKE_GRAIN_PACKING_H
#define PROCEDURAL_MAKE_GRAIN_PACKING_H

#include <procedural_factory.h>

#include <tiny_math_types.h>
#include <tiny_vector_functions.h>

#include <util_log.h>

#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>

namespace procedural
{

  namespace details
  {
    /**
     *
     * @grain_scale                   This parameter can be used to scale all the
     *                                input data. Data is often given in units of
     *                                voxel size, ie. 25nm or 50nm. The grains often
     *                                vary in size from a few voxels and upto the order
     *                                of 40-50 voxels. Hence if voxel size is 50nm and
     *                                one desires the maximum grain to be of size of
     *                                order 1. Then scale should be
     *
     *                                     1/(50*50e-9) = 4e5 = 400000.
     *
     *                                If voxel_size is 25nm then scale should
     *                                be 800000 to make largest grain unit-size.
     *
     *
     *
     *
     * @param max_number_of_grains    The maximum number of grains to parse
     *                                from the given data file. This is helpful
     *                                in order not to read all data if one need
     *                                less grains. If file have less grains that
     *                                this number then the parser only reads the
     *                                number of grains in the data file.
     */
    template<typename MT>
    void parese_grain_data_file(
                                content::API *  engine
                                , std::string                          const & filename
                                , typename MT::real_type               const & voxel_size
                                , typename MT::real_type               const & grain_scale
                                , size_t                               const & max_number_of_grains
                                , std::vector< GeometryHandle<MT> >          & grains
                                , typename MT::real_type                     & grain_size
                                )
    {
      using std::floor;
      using std::ceil;

      typedef typename MT::vector3_type     V;
      typedef typename MT::value_traits     VT;

      std::ifstream file;
      file.open(filename.c_str());

      if (!file.good())
      {
        util::Log logging;

        logging << "parese_grain_data_file(): could not open file = " << filename << util::Log::newline();
        
        return;
      }

      grains.clear();

      unsigned int i = 0u;  // counter for how many grains have been read so far

      V max_coord = V::make( VT::lowest(),VT::lowest(),VT::lowest()    );
      V min_coord = V::make( VT::highest(),VT::highest(),VT::highest() );

      while (!file.eof() && i < max_number_of_grains)
      {
        std::vector<V> vertices;

        std::string line;

        std::getline(file, line);

        if(line.empty())
          continue;

        std::stringstream stream;

        stream.str(line);

        if (!stream.eof())
        {
          unsigned int grain_type = 0u;

          stream >> grain_type;

          char separatur = ' ';

          stream >> separatur;
        }

        while (!stream.eof())
        {
          V p;

          stream >> p;

          p = p * (voxel_size * grain_scale);

          max_coord = tiny::max( p, max_coord);
          min_coord = tiny::min( p, min_coord);

          if (!stream.eof())
          {
            char dummy;
            stream >> dummy;
          }

          vertices.push_back(p);
        }

        GeometryHandle<MT> const handle = create_geometry_handle_convex<MT>(engine, vertices);

        grains.push_back( handle );

        ++i;
      }

      file.close();

      V const grain_bounding_box = max_coord - min_coord;

      grain_size = tiny::max( grain_bounding_box );

      {
        util::Log logging;

        logging << "parese_grain_data_file(): Grain size = " << grain_size << util::Log::newline();
      }
    }

  }// end namespace details

  template<typename MT>
  void make_grain_packing(
                          content::API *  engine
                          , typename MT::vector3_type            const & position
                          , typename MT::quaternion_type         const & orientation
                          , std::string                          const & filename
                          , typename MT::real_type               const & voxel_size
                          , typename MT::real_type               const & grain_scale
                          , typename MT::real_type               const & number_of_grains_in_x
                          , typename MT::real_type               const & number_of_grains_in_z
                          , size_t                               const & total_number_of_grains
                          , typename MT::real_type               const & grain_density
                          , MaterialInfo<typename MT::real_type> const & mat_info
                          , typename MT::real_type                     & grain_size
                          )
  {
    using std::floor;
    using std::ceil;

    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type  Q;
    typedef typename MT::value_traits    VT;

    std::vector< GeometryHandle<MT> > grains;

    details::parese_grain_data_file(  engine
                                    , filename
                                    , voxel_size
                                    , grain_scale
                                    , total_number_of_grains
                                    , grains
                                    , grain_size
                                    );


    size_t const mid = get_material_id<MT>(mat_info, "Stone");

    unsigned int const I = number_of_grains_in_x;
    unsigned int const K = number_of_grains_in_z;
    unsigned int const J = std::ceil( 1.0f*total_number_of_grains/(I*K)  );

    unsigned int grain_count = 0u;

    for(unsigned int j=0u; j < J; ++j)
      for(unsigned int k=0u; k < K; ++k)
        for(unsigned int i=0u; i < I; ++i)
        {
          if(grain_count > total_number_of_grains)
            return;

          T const x = (-number_of_grains_in_x*grain_size + grain_size)*VT::half()  + i*grain_size;
          T const z = (-number_of_grains_in_z*grain_size + grain_size)*VT::half()  + k*grain_size;
          T const y =  grain_size*VT::half() + j*grain_size;

          int choice = grain_count % grains.size();

          V const T_b2m = grains[choice].Tb2m();
          Q const Q_b2m = grains[choice].Qb2m();

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

          create_rigid_body<MT>(  engine
                                , T_b2w
                                , Q_b2w
                                , grains[choice]
                                , mid
                                , grain_density
                                );
          ++grain_count;
        }
  }

} //namespace procedural

// PROCEDURAL_MAKE_GRAIN_PACKING_H
#endif
