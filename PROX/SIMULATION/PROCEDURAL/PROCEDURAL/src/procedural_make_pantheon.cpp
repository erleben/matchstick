#include <procedural.h>
#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{

  template<typename MT>
  void make_pantheon(
                     content::API * engine
                     , typename MT::vector3_type const & position
                     , typename MT::quaternion_type const & orientation
                     , typename MT::real_type const & r_outer
                     , typename MT::real_type const & r_inner
                     , typename MT::real_type const & height
                     , size_t const & slices
                     , size_t const & segments
                     , MaterialInfo<typename MT::real_type> mat_info
                     )
  {
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type Q;
    typedef typename MT::value_traits    VT;

    Q const Qw = orientation;
    V       pw = rotate(Qw,position);

    make_tower<MT>(
                   engine
                   , pw
                   , Qw
                   , r_outer
                   , r_inner
                   , height
                   , slices*2
                   , segments
                   , mat_info
                   , false
                   );

    pw = pw + rotate(Qw , V::make( VT::zero(), VT::zero(), height ) );

    make_dome<MT>(  engine
                  , pw
                  , Qw
                  , r_outer
                  , r_inner
                  , slices
                  , segments
                  , mat_info
                  );
  }

  typedef tiny::MathTypes<float> MTf;

  template
  void make_pantheon<MTf>(
                          content::API * engine
                          , MTf::vector3_type const & position
                          , MTf::quaternion_type const & orientation
                          , MTf::real_type const & r_outer
                          , MTf::real_type const & r_inner
                          , MTf::real_type const & height
                          , size_t const & slices
                          , size_t const & segments
                          , MaterialInfo<MTf::real_type> mat_info
                          );
  
} //namespace procedural
