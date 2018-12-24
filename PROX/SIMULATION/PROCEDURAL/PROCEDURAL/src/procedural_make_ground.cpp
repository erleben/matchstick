#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{
  
  template<typename MT>
  void make_ground(  content::API *  engine
                   , typename MT::vector3_type const & position
                   , typename MT::quaternion_type const & orientation
                   , MaterialInfo<typename MT::real_type> mat_info
                   , typename MT::real_type const & width
                   , typename MT::real_type const & height
                   , typename MT::real_type const & depth
                   )
  {
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type Q;
    typedef typename MT::value_traits    VT;
    
    size_t const mid = get_material_id<MT>(mat_info, "Ground");
    
    GeometryHandle<MT> ground = create_geometry_handle_box<MT>(  engine, width, height, depth);
    
    V const Pw = rotate(
                        orientation
                        , V::make( VT::zero(), -height*VT::half(), VT::zero())
                        ) + position;
    Q const Qw = orientation;
    
    size_t rid = create_rigid_body<MT>(  engine
                                       , Pw
                                       , Qw
                                       , ground
                                       , mid
                                       , VT::one()
                                       , true
                                       , "Visualizer/ground"
                                       );
    
    engine->set_material_structure_map(
                                       rid
                                       , 0.0
                                       , 0.0
                                       , 1.0
                                       );
  }
  
  typedef tiny::MathTypes<float> MTf;
  
  template
  void make_ground<MTf>(  content::API *  engine
                        , MTf::vector3_type const & position
                        , MTf::quaternion_type const & orientation
                        , MaterialInfo<MTf::real_type> mat_info
                        , MTf::real_type const & width
                        , MTf::real_type const & height
                        , MTf::real_type const & depth
                        );
  
} //namespace procedural
