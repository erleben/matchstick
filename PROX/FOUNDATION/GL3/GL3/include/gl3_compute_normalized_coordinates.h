#ifndef GL3_NORMALIZED_COORDINATES_H
#define GL3_NORMALIZED_COORDINATES_H

#include <cmath>

namespace gl3
{

  inline void compute_normalized_coordinates( float const & sx, float const & sy, float & nx, float & ny)
  {
    using std::min;
    using std::max;

    GLfloat viewport[4];
    glGetFloatv(GL_VIEWPORT,viewport);

    nx =     (2.0 * sx) / viewport[2] - 1.0;
    ny = - ( (2.0 * sy) / viewport[3] - 1.0 );

    nx = max( -1.0f, min( 1.0f, nx ) );
    ny = max( -1.0f, min( 1.0f, ny ) );

  }


} // namespace gl3

// GL3_NORMALIZED_COORDINATES_H
#endif
