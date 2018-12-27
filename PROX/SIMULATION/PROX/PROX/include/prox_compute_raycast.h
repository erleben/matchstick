#ifndef PROX_COMPUTE_RAYCAST_H
#define PROX_COMPUTE_RAYCAST_H

#include <prox_rigid_body.h>
#include <geometry.h>
#include <narrow.h>

#include <vector>
#include <cassert>

namespace prox
{

  template< typename M>
  inline bool compute_raycast(
                              geometry::Ray<typename M::vector3_type> const & ray
                              , std::vector< RigidBody< M > > & bodies
                              , narrow::System<typename M::tiny_types> & narrow_system
                              , size_t & body_index
                              , typename M::vector3_type & point
                              , typename M::real_type & distance
                              )
  {
    typedef typename M::tiny_types                                  tiny_types;
    typedef typename M::vector3_type                                V;
    typedef typename M::value_traits                                VT;
    typedef typename M::real_type                                   T;
    typedef typename prox::RigidBody<M>                             body_type;
    typedef          std::vector<body_type>                         body_container;
    typedef typename body_container::iterator                       body_iterator;
    typedef typename narrow::Geometry<tiny_types>                   geometry_type;
    //typedef typename narrow::System<tiny_types>::geometry_iterator  geometry_iterator;

    //--- Initialization -------------------------------------------------------
    point        = V::zero();
    distance     = VT::infinity();
    bool did_hit = false;

    //--- Preprocessing geometries so they reflect current state ---------------
    std::vector< narrow::UpdateWorkItem< tiny_types > > kdop_bvh_update_work_pool;

    kdop_bvh_update_work_pool.reserve( bodies.size() ); // Make sure all space we may need is pre-allocated.

    for(body_iterator body = bodies.begin(); body != bodies.end(); ++body)
    {
      geometry_type const & geometry = narrow_system.get_geometry( body->get_geometry_idx() );

      if(geometry.has_shape() )
      {
        narrow::UpdateWorkItem<tiny_types> work_item = narrow::UpdateWorkItem<tiny_types>(
                                                                                                        *body
                                                                                                        , geometry
                                                                                                        , body->get_position()
                                                                                                        , body->get_orientation()
                                                                                                        );
        kdop_bvh_update_work_pool.push_back( work_item );
      }
    }

    narrow::update_kdop_bvh( kdop_bvh_update_work_pool );

    //--- Test each body for intersection and find the "closest" one -----------
    size_t idx = 0u;
    for(body_iterator body = bodies.begin(); body != bodies.end(); ++body,++idx)
    {
      T min_x = VT::zero();
      T min_y = VT::zero();
      T min_z = VT::zero();
      T max_x = VT::zero();
      T max_y = VT::zero();
      T max_z = VT::zero();

      body->get_box( min_x, min_y, min_z, max_x, max_y, max_z );

      V                 const min_coord = V::make( min_x, min_y, min_z );
      V                 const max_coord = V::make( max_x, max_y, max_z );
      geometry::AABB<V> const aabb      = geometry::make_aabb( min_coord, max_coord );

      T       aabb_distance  = VT::infinity();
      V       aabb_point     = V::zero();

      if ( geometry::compute_raycast_aabb( ray, aabb, aabb_point, aabb_distance) )
      {
        T       body_distance  = VT::infinity();
        V       body_point     = V::zero();

        geometry_type const & geometry = narrow_system.get_geometry( body->get_geometry_idx() );

        bool body_hit = narrow::raycast(
                                   ray
                                   , *body
                                   , geometry
                                   , body->get_position()
                                   , body->get_orientation()
                                   , body_point
                                   , body_distance
                                   );

        if( body_hit && (body_distance < distance) )
        {
          body_index    = idx;
          point    = body_point;
          distance = body_distance;
          did_hit  = true;
        }

      }

    }
    
    return did_hit;
  }

} // namespace prox

// PROX_COMPUTE_RAYCAST_H
#endif
