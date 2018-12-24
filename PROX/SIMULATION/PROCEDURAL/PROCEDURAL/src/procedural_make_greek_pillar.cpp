#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{
	
  template<typename MT>
	void make_greek_pillar(
                         content::API *  engine
                         , typename MT::vector3_type const & position
                         , typename MT::quaternion_type const & orientation
                         , typename MT::real_type const & pillar_width
                         , typename MT::real_type const & pillar_height
                         , typename MT::real_type const & pillar_depth
                         , size_t const & pillar_segments
                         , size_t const & pillar_slices
                         , MaterialInfo<typename MT::real_type> mat_info
                         , mesh_array::TetGenSettings tetset
                      )
	{
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type  Q;
    typedef typename MT::value_traits    VT;
    
		T      const stone_density	= get_material_density<MT>(mat_info, "Stone");
		size_t const mid		      	= get_material_id<MT>(mat_info, "Stone");
		
		std::vector<V>  segment_vertices(pillar_slices*2u+2u);
		
		/// pillar element height ratios, must sum to one
		T const b_box_ratio		  = VT::numeric_cast( 0.06f );
		T const b_conical_ratio	= VT::numeric_cast( 0.06f );
		T const c_pillar_ratio	= VT::numeric_cast( 0.80f );
		T const t_conical_ratio	= VT::numeric_cast( 0.05f );
		T const t_box_ratio		  = VT::numeric_cast( 0.03f );
    
    // element heigths
		T const b_box_height		  = pillar_height*b_box_ratio;
		T const b_conical_height	= pillar_height*b_conical_ratio;
		T const c_pillar_height   = pillar_height*c_pillar_ratio;
		T const t_conical_height	= pillar_height*t_conical_ratio;
		T const t_box_height		  = pillar_height*t_box_ratio;
    
    T y = b_box_height*VT::half();
		{
			GeometryHandle<MT> bottom_box = create_geometry_handle_box<MT>(engine
                                                                     , pillar_width
                                                                     , b_box_height
                                                                     , pillar_depth);
      
      V const local_translation = V::make( VT::zero(), y, VT::zero() );
      
      V body_to_world_translation;
      Q body_to_world_orientation;
      
      compute_body_to_world_transform<MT>(
                                            bottom_box.Tb2m()    // body to model
                                          , bottom_box.Qb2m()    // body to model
                                          , local_translation   // model to local
                                          , Q::identity()       // model to local
                                          , position            // local to wolrd
                                          , orientation         // local to world
                                          , body_to_world_translation
                                          , body_to_world_orientation
                                          );
      
      create_rigid_body<MT>(  engine
                            , body_to_world_translation
                            , body_to_world_orientation
                            , bottom_box
                            , mid
                            , stone_density
                            );
		}
		
		y += VT::half()*b_box_height;
    {
			
			GeometryHandle<MT> pillar_segment = create_geometry_handle_pillar_segment<MT>(  engine
                                                                                    , VT::numeric_cast(  0.5f*pillar_width      )
                                                                                    , VT::numeric_cast( (0.5f*pillar_width)*.8f )
                                                                                    , b_conical_height
                                                                                    , pillar_slices
                                                                                    , tetset
                                                                                    );
      
      V const local_translation = V::make( VT::zero(), y, VT::zero() );
      
      V body_to_world_translation;
      Q body_to_world_orientation;
     
      compute_body_to_world_transform<MT>(
                                            pillar_segment.Tb2m()       // body 2 model
                                          , pillar_segment.Qb2m()       // body 2 model
                                          , local_translation           // model 2 local
                                          , Q::identity()               // model 2 local
                                          , position                    // local 2 world
                                          , orientation                 // local 2 world
                                          , body_to_world_translation
                                          , body_to_world_orientation
                                          );
      
      create_rigid_body<MT>(  engine
                            , body_to_world_translation
                            , body_to_world_orientation
                            , pillar_segment
                            , mid
                            , stone_density
                            );
      
		}
    
    y += b_conical_height;
    
		{
			
			T const pillar_segment_height = c_pillar_height / pillar_segments;
			T const s_bottom_radius       = VT::numeric_cast(0.8f)*(pillar_width*VT::half());
			T       top_radius			      = VT::numeric_cast(0.6f)*(pillar_width*VT::half());
			T       bottom_radius         = s_bottom_radius;
			T const alpha                 = (top_radius-bottom_radius)/c_pillar_height;
			
			
			for(size_t i = 0;i<pillar_segments;++i)
			{
				top_radius = s_bottom_radius + alpha * pillar_segment_height*(1+i);
				
        GeometryHandle<MT> c_pillar_segment = create_geometry_handle_pillar_segment<MT>(
                                                                                        engine
                                                                                        , bottom_radius
                                                                                        , top_radius
                                                                                        , pillar_segment_height
                                                                                        , pillar_slices
                                                                                        , tetset
                                                                                        );
				
        V const local_translation = V::make( VT::zero(), y, VT::zero() );
        
        V body_to_world_translation;
        Q body_to_world_orientation;
        
        compute_body_to_world_transform<MT>(
                                              c_pillar_segment.Tb2m()  // body to model
                                            , c_pillar_segment.Qb2m()  // body to model
                                            , local_translation        // model to local
                                            , Q::identity()            // model to local
                                            , position                 // local to world
                                            , orientation              // local to world
                                            , body_to_world_translation
                                            , body_to_world_orientation
                                            );
        
        
				create_rigid_body<MT>(  engine
                              , body_to_world_translation
                              , body_to_world_orientation
                              , c_pillar_segment
                              , mid
                              , stone_density
                              );
				
				bottom_radius = top_radius;
        y += pillar_segment_height;
			}
		}
    
		y += t_conical_height;
    {
			
			GeometryHandle<MT> pillar_segment = create_geometry_handle_pillar_segment<MT>(
                                                                                    engine
                                                                                    , VT::numeric_cast( (0.5f*pillar_width)       )
                                                                                    , VT::numeric_cast( 0.6f*(0.5f*pillar_width)  )
                                                                                    , t_conical_height
                                                                                    , pillar_slices
                                                                                    , tetset
                                                                                    );
      
      V const local_translation = V::make( VT::zero(), y, VT::zero() );
      
      V body_to_world_translation;
      Q body_to_world_orientation;
      
      compute_body_to_world_transform<MT>(
                                            pillar_segment.Tb2m()      // body to model
                                          , pillar_segment.Qb2m()      // body to model
                                          , local_translation          // model to local
                                          , Q::Ru( VT::pi(), V::i() )  // model to local
                                          , position                   // local to world
                                          , orientation                // local to world
                                          , body_to_world_translation
                                          , body_to_world_orientation
                                          );
      
      create_rigid_body<MT>(  engine
                            , body_to_world_translation
                            , body_to_world_orientation
                            , pillar_segment
                            , mid
                            , stone_density
                            );
      

		}
    
    y += VT::half()*t_box_height;
		{
			GeometryHandle<MT> top_box = create_geometry_handle_box<MT>(
                                                                     engine
                                                                     , pillar_width
                                                                     , t_box_height
                                                                     , pillar_depth
                                                                     );
			
      V const local_translation = V::make( VT::zero(), y, VT::zero() );
      
      V body_to_world_translation;
      Q body_to_world_orientation;
      
      compute_body_to_world_transform<MT>(
                                            top_box.Tb2m()              // body to model
                                          , top_box.Qb2m()              // body to model
                                          , local_translation           // model to local
                                          , Q::identity()               // model to local
                                          , position                    // local to world
                                          , orientation                 // local to world
                                          , body_to_world_translation
                                          , body_to_world_orientation
                                          );
      
      create_rigid_body<MT>(  engine
                            , body_to_world_translation
                            , body_to_world_orientation
                            , top_box
                            , mid
                            , stone_density
                            );
      

		}
	}
  
  typedef tiny::MathTypes<float> MTf;
  
  template
	void make_greek_pillar<MTf>(
                              content::API *  engine
                              , MTf::vector3_type const & position
                              , MTf::quaternion_type const & orientation
                              , MTf::real_type const & pillar_width
                              , MTf::real_type const & pillar_height
                              , MTf::real_type const & pillar_depth
                              , size_t const & pillar_segments
                              , size_t const & pillar_slices
                              , MaterialInfo<MTf::real_type> mat_info
                              , mesh_array::TetGenSettings tetset
                            );
	
} //namespace procedural

