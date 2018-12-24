#ifndef GL3_FRUSTUM_H
#define GL3_FRUSTUM_H

#include <gl3_glm.h>

namespace gl3
{

  class Plane
  {
  protected:

    glm::vec3 m_n;
    float     m_w;

  public:

    glm::vec3       & n()       { return m_n; }
    glm::vec3 const & n() const { return m_n; }

    float       & w()       { return m_w; }
    float const & w() const { return m_w; }

  public:

    Plane()
    : m_n(0.0f,0.0f,0.0f)
    , m_w(0.0f)
    {}

  };

  class Frustum
  {
  public:

    static unsigned int const RIGHT  = 0u; ///< The RIGHT side of the frustum
    static unsigned int const LEFT   = 1u; ///< The LEFT	 side of the frustum
    static unsigned int const BOTTOM = 2u; ///< The BOTTOM side of the frustum
    static unsigned int const TOP    = 3u; ///< The TOP side of the frustum
    static unsigned int const BACK   = 4u; ///< The BACK	side of the frustum (near plane)
    static unsigned int const FRONT  = 5u; ///< The FRONT side of the frustum (far plane)

  protected:

    Plane  m_planes[6];  ///< The planes of the frustum. Normals are pointing inwards!!!

  public:

    Plane & plane(unsigned int const & idx)
    {
      assert( idx < 6u || !"illegal index value");

      return m_planes[idx];
    }

    Plane const & plane(unsigned int const & idx) const
    {
      assert( idx < 6u || !"illegal index value");

      return m_planes[idx];
    }

    /**
     * Computes the current clip planes in model space.
     *
     * @param MVP    The model view projection matrix. That is MVP = projection * view * model.
     */
    void update(glm::mat4 const & MVP)
    {
      using std::sqrt;

      //--- let x be the homegeneous point in model space and y the correspoding
      //--- canonical view volume point, then we have the
      //---
      //---            y = P V M x
      //---
      //--- where M is the model-view transform, and P the projection transform. Now
      //--- define the C matrix as:
      //---
      //---     C = P*V*M
      //---
      //--- Here C is in column major form,
      //---
      //---      | c0 c4 c8   c12 |
      //--- C =  | c1 c5 c9   c13 |
      //---      | c2 c6 c10  c14 |
      //---      | c3 c7 c11  c15 |
      //---
      //--- We define the plane equation as
      //---
      //---    n*p - w = 0,
      //---
      //--- where given a point p0 on the plane we have
      //---
      //---    w = n*p0
      //---
      //--- We can represent such a plane by a 4 dimensional vector,
      //---
      //---      [ n^T -w ]^T
      //---
      //--- In the canonical view volume we have 6 such vectors
      //---
      //---   right : [-1  0  0  1]
      //---   left  : [ 1  0  0  1]
      //---   bottom: [ 0  1  0  1]
      //---   top   : [ 0 -1  0  1]
      //---   back  : [ 0  0 -1  1]
      //---   front : [ 0  0  1  1]
      //---
      //--- We want to transform these back into model space, such that we can do
      //--- model space clipping.
      //---
      //---  Let a plane given by [ n^T -w ]^T be first rotated by R and then translated by T
      //---
      //---     p0' = R p0 + T
      //---      n' = R n
      //---
      //--- So the rotated plane equation is
      //---
      //---    R n *p -  R n *( R p0 + T ) = 0
      //---
      //---    R n *p -  R n * R p0 -  R n * T  = 0
      //---    R n *p -    n * p0   -  n * R^T T  = 0
      //---
      //--- Or using homegeneous coordinates
      //---
      //---               |R       0 |  | n|
      //---   | p^T  1 |  |-R^T T  1 |  |-w|   = 0
      //---
      //--- But how do we transform a plane with C?....
      //---
      float const * C = glm::value_ptr(MVP);

      m_planes[RIGHT].n() = glm::vec3( C[ 3] - C[0], C[ 7] - C[4], C[11] - C[8] );
      m_planes[RIGHT].w()     =  -(C[15] - C[12]);

      m_planes[LEFT].n() = glm::vec3( C[ 3] + C[ 0], C[ 7] + C[ 4], C[11] + C[ 8] );
      m_planes[LEFT].w()      =  -(C[15] + C[12]);

      m_planes[BOTTOM].n() = glm::vec3( C[ 3] + C[ 1], C[ 7] + C[ 5], C[11] + C[ 9] );
      m_planes[BOTTOM].w()    =  -(C[15] + C[13]);

      m_planes[TOP].n() = glm::vec3( C[ 3] - C[ 1], C[ 7] - C[ 5], C[11] - C[ 9] );
      m_planes[TOP].w()       =  -(C[15] - C[13]);

      m_planes[BACK].n() = glm::vec3( C[ 3] - C[ 2], C[ 7] - C[ 6], C[11] - C[10] );
      m_planes[BACK].w()      =  -(C[15] - C[14]);

      m_planes[FRONT].n() = glm::vec3( C[ 3] + C[ 2], C[ 7] + C[ 6], C[11] + C[10] );
      m_planes[FRONT].w()     =  -(C[15] + C[14]);

      //--- Normalize plane normals
      for(unsigned int i=0;i<6;++i)
      {
        float const tmp = glm::length(  m_planes[i].n() );
        if(tmp)
        {
          m_planes[i].n() /= tmp;
          m_planes[i].w() /= tmp;
        }
      }
    }

  };

} // namespace gl3

// GL3_FRUSTUM_H
#endif
