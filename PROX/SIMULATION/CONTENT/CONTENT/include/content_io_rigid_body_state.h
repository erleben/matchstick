#ifndef CONTENT_IO_RIGID_BODY_STATE_H
#define CONTENT_IO_RIGID_BODY_STATE_H

#include <content_api.h>

namespace content
{
  namespace details
  {    
    
    /**
     * A Rigid Body State Information.
     * This data structure serves as a wrapper for all data relevant to the state
     * information of a single rigid body.
     */
    struct RigidBodyState
    {
      Transform m_T;   ///< The position and orientation of the center of mass and body frame.
      Motion    m_M;   ///< The linear and angular velocities.
      
      
      float m_mass;    ///< The total mass of the rigid body.
      float m_Ixx;     ///< The model frame inertia product xx.
      float m_Iyy;     ///< The model frame inertia product yy.
      float m_Izz;     ///< The model frame inertia product zz.
      
      bool m_active;   ///< If this flag is set to false then the rigid body should be allocated but not simulated.
      bool m_fixed;    ///< If this flag is set to true then the rigid body should be a fixed body.
      bool m_scripted; ///< If this flag is set to true then the rigid body should be a scripted body.

      size_t m_material_idx;   ///< A index for the material type of the rigid body.
      size_t m_motion_idx;   ///< A index for the scripted motion if the ridid body was scripted.

      RigidBodyState()
      : m_T()
      , m_M()
      , m_mass(1.0f)
      , m_Ixx( 1.0f )
      , m_Iyy( 1.0f )
      , m_Izz( 1.0f )
      , m_active(true)
      , m_fixed(false)
      , m_scripted(false)
      , m_material_idx(UNDEFINED)
      , m_motion_idx(UNDEFINED)
      {}
      
    };
    
  }// namespace details  
}// namespace content

// CONTENT_IO_RIGID_BODY_STATE_H
#endif
