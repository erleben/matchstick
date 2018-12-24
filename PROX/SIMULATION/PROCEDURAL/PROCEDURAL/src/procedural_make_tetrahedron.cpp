#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{

  template<typename MT>
  void make_tetrahedron(
                content::API * engine
                , typename MT::vector3_type const& one
                , typename MT::vector3_type const& two
                , typename MT::vector3_type const& three
                , typename MT::vector3_type const& four
                , typename MT::vector3_type const& position
                , typename MT::quaternion_type const & orientation
                , MaterialInfo<typename MT::real_type> mat_info
                , bool const fixed = false)
  {
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type Q;
    typedef typename MT::value_traits    VT;

    T const stone_density = get_material_density<MT>(mat_info, "Stone");
    size_t const mid      = get_material_id<MT>(mat_info, "Stone");

    GeometryHandle<MT> tet_handle = create_geometry_handle_tetrahedron<MT>( engine, one, two, three, four);

    V const T_b2m = tet_handle.Tb2m();
    Q const Q_b2m = tet_handle.Qb2m();

    V const T_m2l = V::make( VT::zero(), VT::zero(), VT::zero() );
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
                          , tet_handle
                          , mid
                          , stone_density
                          , fixed
                          );



  }

  typedef tiny::MathTypes<float> MTf;

  template
  void make_tetrahedron<MTf>(
                     content::API * engine
                     , typename MTf::vector3_type const& one
                     , typename MTf::vector3_type const& two
                     , typename MTf::vector3_type const& three
                     , typename MTf::vector3_type const& four
                     , MTf::vector3_type const & position
                     , MTf::quaternion_type const & orientation
                     , MaterialInfo<MTf::real_type> mat_info
                     , bool const fixed);

  template<typename MT>
	void make_tetrahedron(
                  content::API * engine
                  , typename MT::vector3_type const& position
                  , typename MT::quaternion_type const & orientation
                  , MaterialInfo<typename MT::real_type> mat_info
                  , bool const fixed = false)
	{
    typedef typename MT::vector3_type    V;

		V one   = V::make(0.0, 0.0, 0.0);
		V two   = V::make(1.0, 0.0, 0.0);
		V three = V::make(0.0, 0.0, 1.0);
		V four  = V::make(0.0, 1.0, 0.0);

    make_tetrahedron<MT>(engine, one, two, three, four, position, orientation, mat_info);
}

template
void make_tetrahedron<MTf>(
                     content::API * engine
                     , MTf::vector3_type const & position
                     , MTf::quaternion_type const & orientation
                     , MaterialInfo<MTf::real_type> mat_info
                     , bool const fixed);


} //namespace procedural
