#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{
	
  template<typename MT>
	void make_box_container(  content::API *  engine
                   , typename MT::vector3_type const & position
                   , typename MT::quaternion_type const & orientation
                   , typename MT::real_type const & width
                   , typename MT::real_type const & height
                   , typename MT::real_type const & depth
                   , typename MT::real_type const & wall_thickness
                   , MaterialInfo<typename MT::real_type> mat_info
                   )
	{
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type Q;
    typedef typename MT::value_traits    VT;
		
    size_t const mid = get_material_id<MT>(mat_info, "Ground");

    GeometryHandle<MT> bottom = create_geometry_handle_box<MT>(
                                                               engine
                                                               ,  width
                                                               ,  wall_thickness
                                                               ,  depth
                                                               );

    GeometryHandle<MT> left   = create_geometry_handle_box<MT>(
                                                               engine
                                                               , wall_thickness
                                                               , height+wall_thickness
                                                               , depth+VT::two()*wall_thickness
                                                               );

    GeometryHandle<MT> right  = create_geometry_handle_box<MT>(
                                                               engine
                                                               , wall_thickness
                                                               , height+wall_thickness
                                                               , depth+VT::two()*wall_thickness
                                                               );

    GeometryHandle<MT> front  = create_geometry_handle_box<MT>(
                                                               engine
                                                               ,     width
                                                               ,    height+wall_thickness
                                                               ,     wall_thickness
                                                               );

    GeometryHandle<MT> back   = create_geometry_handle_box<MT>(
                                                               engine
                                                               ,     width
                                                               ,    height+wall_thickness
                                                               ,     wall_thickness
                                                               );

    //--- Bottom ------------------------------------------
    {
      V const Pw = rotate(
                          orientation
                        , V::make( VT::zero(), -height*VT::half() - wall_thickness*VT::half()  , VT::zero())
                          ) + position;

      Q const Qw = orientation;
    
      create_rigid_body<MT>(  engine
                            , Pw
                            , Qw
                            , bottom
                            , mid
                            , VT::one()
                            , true
                            , "Visualizer/ground"
                            );
    }
    //--- Left ------------------------------------------
    {
      V const Pw = rotate(
                          orientation
                          , V::make( -width*VT::half()-wall_thickness*VT::half(), -wall_thickness*VT::half(), VT::zero())
                          ) + position;

      Q const Qw = orientation;

      create_rigid_body<MT>(  engine
                            , Pw
                            , Qw
                            , left
                            , mid
                            , VT::one()
                            , true
                            , "Visualizer/ground"
                            );
    }
    //--- Right ------------------------------------------
    {
      V const Pw = rotate(
                          orientation
                          , V::make( width*VT::half()+wall_thickness*VT::half(), -wall_thickness*VT::half(), VT::zero())
                          ) + position;

      Q const Qw = orientation;

      create_rigid_body<MT>(  engine
                            , Pw
                            , Qw
                            , right
                            , mid
                            , VT::one()
                            , true
                            , "Visualizer/ground"
                            );
    }
    //--- Front ------------------------------------------
    {
      V const Pw = rotate(
                          orientation
                          , V::make( VT::zero(), -wall_thickness*VT::half(), depth*VT::half() + VT::half()*wall_thickness)
                          ) + position;

      Q const Qw = orientation;

      create_rigid_body<MT>(  engine
                            , Pw
                            , Qw
                            , front
                            , mid
                            , VT::one()
                            , true
                            , "Visualizer/ground"
                            );
    }
    //--- Back ------------------------------------------
    {
      V const Pw = rotate(
                          orientation
                          , V::make( VT::zero(), -wall_thickness*VT::half(), - depth*VT::half() - VT::half()*wall_thickness)
                          ) + position;

      Q const Qw = orientation;

      create_rigid_body<MT>(  engine
                            , Pw
                            , Qw
                            , back
                            , mid
                            , VT::one()
                            , true
                            , "Visualizer/ground"
                            );
    }
	}
  
	typedef tiny::MathTypes<float> MTf;
  
  template
	void make_box_container<MTf>(  content::API *  engine
                        , MTf::vector3_type const & position
                        , MTf::quaternion_type const & orientation
                        , MTf::real_type const & width
                        , MTf::real_type const & height
                        , MTf::real_type const & depth
                        , MTf::real_type const & wall_thickness
                        , MaterialInfo<MTf::real_type> mat_info
                        );
  
} //namespace procedural
