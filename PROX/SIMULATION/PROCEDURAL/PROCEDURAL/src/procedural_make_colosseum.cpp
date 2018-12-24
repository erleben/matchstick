#include <procedural.h>
#include <procedural_factory.h>

#include <tiny_math_types.h>

#include <cmath>

namespace procedural
{
  template<typename MT>
  void make_colosseum(  content::API * engine
                      , typename MT::vector3_type    const & position
                      , typename MT::quaternion_type const & orientation
                      , typename MT::real_type       const & r_outer
                      , typename MT::real_type       const & r_inner
                      , size_t const & slices
                      , size_t const & segments
                      , MaterialInfo<typename MT::real_type> mat_info
                      )
  {
    using std::cos;
    using std::sin;

    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type Q;
    typedef typename MT::value_traits    VT;

    //10-30-14 Sarah: clean up "magic" constants
    T      const r_center        = (r_outer-r_inner)*.5 + r_inner;
    T      const delta_theta     = VT::two()*VT::pi()/slices;
    T      const r_arch_outer    = r_inner*sin( delta_theta*VT::half() );
    T      const r_arch_inner    = r_arch_outer*VT::numeric_cast( 0.6f );
    T      const pillar_height   = VT::one()*r_arch_outer;
    size_t const arch_slices     = 7u;
    size_t const pillar_segments = 5u;
    T      const depth           = r_outer - r_inner;

    T      const arch_height     = r_arch_outer + pillar_height;
    T      const tower_height    = VT::numeric_cast(0.2f)*arch_height;
    T            y               = VT::zero();

    for(size_t i = 0u; i < segments; ++i)
    {
      for(size_t j = 0u; j < slices; ++j)
      {
        T const theta = delta_theta*j;
        T const x     = r_center * cos(  theta  );
        T const z     = r_center * sin(  theta  );

        V const arch_position    = rotate(orientation, (V::make(x,y,z))) +  position;
        Q const arch_orientation = orientation*Q::Ry(VT::pi_half())*Q::Ry( -theta );

        make_arch<MT>(  engine
                      , arch_position
                      , arch_orientation
                      , r_arch_outer
                      , r_arch_inner
                      , pillar_height
                      , depth*0.8f
                      , arch_slices
                      , pillar_segments
                      , mat_info
                      );

      }

      y += arch_height;

      V const tower_position    = rotate( orientation, (V::make(VT::zero(),y,VT::zero())))+ position;

      Q const tower_orientation = orientation*Q::Ry( delta_theta/2.0 )*Q::Rx(-VT::pi_half());

      make_tower<MT>(
                     engine
                     , tower_position
                     , tower_orientation
                     , r_outer
                     , r_inner
                     , tower_height
                     , slices
                     , 1
                     , mat_info
                     , false
                     );

      y += tower_height;

    }
  }

  typedef tiny::MathTypes<float> MTf;

  template
  void make_colosseum<MTf>(  content::API * engine
                           , MTf::vector3_type    const & position
                           , MTf::quaternion_type const & orientation
                           , MTf::real_type       const & r_outer
                           , MTf::real_type       const & r_inner
                           , size_t const & slices
                           , size_t const & segments
                           , MaterialInfo<MTf::real_type> mat_info
                           );

} //namespace procedural
