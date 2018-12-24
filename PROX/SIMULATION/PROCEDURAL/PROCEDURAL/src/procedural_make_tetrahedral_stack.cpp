#include <procedural_factory.h>

#include <tiny_math_types.h>

namespace procedural
{
	
  template<typename MT>
	void make_tetrahedral_stack(
                       content::API *  engine
                       , typename MT::vector3_type const & position
                       , typename MT::quaternion_type const & orientation
                       , typename MT::real_type const & cube_width
                       , typename MT::real_type const & cube_height
                       , typename MT::real_type const & cube_depth
                       , size_t const & sub_divisions
                       , MaterialInfo<typename MT::real_type> mat_info
                       )
	{
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type  Q;
    typedef typename MT::value_traits    VT;
    
		T      const stone_density	= get_material_density<MT>(mat_info, "Stone");
		size_t const mid	      		= get_material_id<MT>(mat_info, "Stone");
		
		//--- Find the size of each of the boxes
		T const box_width  = cube_width/sub_divisions;
		T const box_height = cube_height/sub_divisions;
		T const box_depth  = cube_depth/sub_divisions;
    
		//--- The dimensions of the box is in place now we will make the individual tetras that make up the box
		V one   = V::make( -box_width/2.0, -box_height/2.0, -box_depth/2.0 );
		V two   = V::make(  box_width/2.0, -box_height/2.0, -box_depth/2.0 );
		V three = V::make(  box_width/2.0,  box_height/2.0, -box_depth/2.0 );
		V four  = V::make( -box_width/2.0,  box_height/2.0, -box_depth/2.0 );

		V five  = V::make( -box_width/2.0, -box_height/2.0,  box_depth/2.0 );
		V six   = V::make(  box_width/2.0, -box_height/2.0,  box_depth/2.0 );
		V seven = V::make(  box_width/2.0,  box_height/2.0,  box_depth/2.0 );
		V eight = V::make( -box_width/2.0,  box_height/2.0,  box_depth/2.0 );

    GeometryHandle<MT> tetra_handle1 = create_geometry_handle_tetrahedron<MT>( engine, one,  two,   three, six   );
    GeometryHandle<MT> tetra_handle2 = create_geometry_handle_tetrahedron<MT>( engine, one,  three, four,  eight );
    GeometryHandle<MT> tetra_handle3 = create_geometry_handle_tetrahedron<MT>( engine, five, eight, six,   one   );
    GeometryHandle<MT> tetra_handle4 = create_geometry_handle_tetrahedron<MT>( engine, six,  eight, seven, three );
    GeometryHandle<MT> tetra_handle5 = create_geometry_handle_tetrahedron<MT>( engine, one,  three, eight, six   );

		T x = VT::one();
    T y = VT::one();
    T z = VT::one();
    
		for (size_t i = 0; i < sub_divisions; ++i)
    {
      x = i*box_width - (cube_width/VT::two()) + (box_width/VT::two());
      for (size_t j = 0; j < sub_divisions; ++j)
      {
        y =  (box_height/VT::two())+j*box_height;
        for (size_t k = 0; k < sub_divisions; ++k)
        {
          z = k*box_depth;
          
          V const Tb = tetra_handle1.Tb2m();
          Q const Qb = tetra_handle1.Qb2m();
					V const Tm = V::make( x, y, z );
          Q const Qm = Q::identity();
          V const Tu = rotate(Qm,Tb) + Tm;
          Q const Qu  = Qm*Qb;
          create_rigid_body<MT>(  engine
                                , Tu
                                , Qu
                                , tetra_handle1
                                , mid
                                , stone_density
                                );
          
					V const Tb2 = tetra_handle2.Tb2m();
          Q const Qb2 = tetra_handle2.Qb2m();
          V const Tu2 = rotate(Qm,Tb2) + Tm;
          Q const Qu2 = Qm*Qb2;
					create_rigid_body<MT>(  engine
                                , Tu2
                                , Qu2
                                , tetra_handle2
                                , mid
                                , stone_density
                                );
          
					V const Tb3 = tetra_handle3.Tb2m();
          Q const Qb3 = tetra_handle3.Qb2m();
          V const Tu3 = rotate(Qm,Tb3) + Tm;
          Q const Qu3 = Qm*Qb3;
					create_rigid_body<MT>(  engine
                                , Tu3
                                , Qu3
                                , tetra_handle3
                                , mid
                                , stone_density
                                );
          
					V const Tb4 = tetra_handle4.Tb2m();
          Q const Qb4 = tetra_handle4.Qb2m();
          V const Tu4 = rotate(Qm,Tb4) + Tm;
          Q const Qu4 = Qm*Qb4;
					create_rigid_body<MT>(  engine
                                , Tu4
                                , Qu4
                                , tetra_handle4
                                , mid
                                , stone_density
                                );
          
					V const Tb5 = tetra_handle5.Tb2m();
          Q const Qb5 = tetra_handle5.Qb2m();
          V const Tu5 = rotate(Qm,Tb5) + Tm;
          Q const Qu5 = Qm*Qb5;
					create_rigid_body<MT>(  engine
                                , Tu5
                                , Qu5
                                , tetra_handle5
                                , mid
                                , stone_density
                                );
        }
      }
    }
  }
  
  typedef tiny::MathTypes<float> MTf;
  
  template
	void make_tetrahedral_stack<MTf>(
                       content::API *  engine
                       , MTf::vector3_type const & position
                       , MTf::quaternion_type const & orientation
                       , MTf::real_type const & cube_width
                       , MTf::real_type const & cube_height
                       , MTf::real_type const & cube_depth
                       , size_t const & sub_divisions
                       , MaterialInfo<MTf::real_type> mat_info
                       );
  
} //namespace procedural
