#ifndef CONTENT_API_H
#define CONTENT_API_H

#include <content_api_input.h>
#include <content_api_output.h>
#include <content_api_profiling.h>

#include <string>

namespace content
{
  
  /**
   * This unique default value that is used as a return value whenever something is not specified.
   */
  size_t const UNDEFINED = 0xFFFFFFFFu;
  
  /**
   * Physics API Interface Class.
   * The interface defines a generic protocol for communicating
   * with a physics engine and/or visualization front end.
   */
  class API
  : public Input
  , public Output
  , public Profiling
    {
    public:
      
      API(){}
      virtual ~API(){}
      
    public:
      
      /**
       * Simualte forward until requested specified simulated time has been simulated.
       *
       * @param dt    The time to simulate forward in time.
       * @return      If simulation was successful then return value is true otherwise it is false.
       */
      virtual bool simulate( float const & dt ) = 0;

      /**
       * Test geometry of rigid bodies for ray cast intersection. If
       * intersection occurs then the intersection point (hit) is
       * computed as well as the signed distance from ray origin (p)
       * to the hit point and the index of the rigid body that was hit.
       *
       * @param p_x        The x-coordinate of the ray origin.
       * @param p_y        The y-coordinate of the ray origin.
       * @param p_z        The z-coordinate of the ray origin.
       * @param ray_x      The x-coordinate of the ray direction.
       * @param ray_y      The y-coordinate of the ray direction.
       * @param ray_z      The z-coordinate of the ray direction.
       * @param body_idx   Upon return this argument holds the value of the index of the rigid body that was hit.
       * @param hit_x      Upon return this argument holds the value of the x-coordinate of the hit point.
       * @param hit_y      Upon return this argument holds the value of the y-coordinate of the hit point.
       * @param hit_z      Upon return this argument holds the value of the z-coordinate of the hit point.
       * @param distance   Upon return this argument holds the distance from ray origin to hit point.
       *
       * @return    If the ray intersect any rigid body then the return
       *            value is true otherwise it is false.
       */
      virtual bool compute_raycast(
                              float const & p_x
                            , float const & p_y
                            , float const & p_z
                            , float const & ray_x
                            , float const & ray_y
                            , float const & ray_z
                            , size_t & body_idx
                            , float & hit_x
                            , float & hit_y
                            , float & hit_z
                            , float & distance
                            ) = 0;
      
    };
  
}// namespace content

// CONTENT_API_H
#endif
