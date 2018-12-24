#include <procedural_factory.h>

#include <tiny_math_types.h>

#include <cmath>

namespace procedural
{
	
  template<typename MT>
	void make_box(
                content::API *  engine
                , typename MT::vector3_type const & position
                , typename MT::quaternion_type const & orientation
                , typename MT::real_type const & width
                , typename MT::real_type const & height
                , typename MT::real_type const & depth
                , MaterialInfo<typename MT::real_type> mat_info
                                , bool const fixed )
	{
    using std::cos;
    using std::sin;
    
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type  Q;
    
		T      const stone_density  = get_material_density<MT>(mat_info, "Stone");
		size_t const mid            = get_material_id<MT>(mat_info, "Stone");
		
		GeometryHandle<MT> box_handle = create_geometry_handle_box<MT>( engine, width, height, depth );
    
    V const T_b2m = box_handle.Tb2m();
    Q const Q_b2m = box_handle.Qb2m();

    V const T_m2l = V::zero();
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

    create_rigid_body<MT>(
                          engine
                          , T_b2w
                          , Q_b2w
                          , box_handle
                          , mid
                          , stone_density
                          , fixed
                          );
    
	}
  
  typedef tiny::MathTypes<float>  MTf;

  template
  void make_box<MTf>(
                content::API *  engine
                , MTf::vector3_type const & position
                , MTf::quaternion_type const & orientation
                , MTf::real_type const & width
                , MTf::real_type const & height
                , MTf::real_type const & depth
                , MaterialInfo<typename MTf::real_type> mat_info
                , bool const fixed);
  
} //namespace procedural
