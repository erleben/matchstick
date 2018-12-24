#ifndef CONTENT_IO_TRANSFORM_H
#define CONTENT_IO_TRANSFORM_H

namespace content
{
    namespace details
    {    
        /**
         * A coordinate transform data structure.
         * This is used for bundling arguments so code becomes more readable.
         */
        struct Transform
        {
            float m_x;     ///< Translation along x-axis.
            float m_y;     ///< Translation along y-axis.
            float m_z;     ///< Translation along z-axis.
            float m_qs;    ///< First Euler angle,  first rotation around Z
            float m_qx;    ///< Second Euler angle, second rotation around Y
            float m_qy;    ///< Third Euler angle,  third rotation around Z
            float m_qz;    ///< Third Euler angle,  third rotation around Z
            
            Transform()
            : m_x(0.0f)
            , m_y(0.0f)
            , m_z(0.0f)
            , m_qs(0.0f)
            , m_qx(0.0f)
            , m_qy(0.0f)
            , m_qz(0.0f)
            {}
            
        };
        
    }// namespace details  
}// namespace content

// CONTENT_IO_TRANSFORM_H
#endif
