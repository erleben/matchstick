#include <mesh_array_indexed_vertex_attribute.h>

#include <factory/mesh_array_make_box.h>
#include <factory/mesh_array_make_cuboid.h>

#include <tiny_math_types.h>



namespace mesh_array
{
  
  template<typename MT>
  void make_box(
                     typename MT::real_type const & width
                     , typename MT::real_type const & height
                     , typename MT::real_type const & depth
                     , T3Mesh & mesh
                     , VertexAttribute<typename MT::real_type,T3Mesh> & X
                     , VertexAttribute<typename MT::real_type,T3Mesh> & Y
                     , VertexAttribute<typename MT::real_type,T3Mesh> & Z
                     )
  {
    typedef typename MT::value_traits    VT;
    typedef typename MT::vector3_type    V;

		V const v0 = V::make( - width, -height, depth )*VT::half();
		V const v1 = V::make(   width, -height, depth )*VT::half();
		V const v2 = V::make(   width,  height, depth )*VT::half();
		V const v3 = V::make( - width,  height, depth )*VT::half();
		V const v4 =  v0 - V::make( VT::zero(), VT::zero(), depth );
		V const v5 =  v1 - V::make( VT::zero(), VT::zero(), depth );
		V const v6 =  v2 - V::make( VT::zero(), VT::zero(), depth );
		V const v7 =  v3 - V::make( VT::zero(), VT::zero(), depth );
		
		make_cuboid<MT>( v0, v1, v2, v3, v4, v5, v6, v7, mesh, X, Y, Z );
	}
  
  template<typename MT>
  void make_box(
                typename MT::real_type const & width
                , typename MT::real_type const & height
                , typename MT::real_type const & depth
                , T3Mesh & mesh
                , VertexAttribute<typename MT::vector3_type,T3Mesh> & coords
                )
  {
    typedef typename MT::vector3_type    V;

    IndexedVertexAttribute<V,T3Mesh> X = IndexedVertexAttribute<V,T3Mesh>(0, coords);
    IndexedVertexAttribute<V,T3Mesh> Y = IndexedVertexAttribute<V,T3Mesh>(1, coords);
    IndexedVertexAttribute<V,T3Mesh> Z = IndexedVertexAttribute<V,T3Mesh>(2, coords);
    
    make_box<MT>(width,height,depth,mesh,X,Y,Z);
  }
    
  typedef tiny::MathTypes<float> MTf;
  typedef tiny::MathTypes<double> MTd;
  
  template
  void make_box<MTf>(
                MTf::real_type const & width
                , MTf::real_type const & height
                , MTf::real_type const & depth
                , T3Mesh & mesh
                , VertexAttribute<MTf::real_type,T3Mesh> & X
                , VertexAttribute<MTf::real_type,T3Mesh> & Y
                , VertexAttribute<MTf::real_type,T3Mesh> & Z
                );

  template
  void make_box<MTd>(
                MTd::real_type const & width
                , MTd::real_type const & height
                , MTd::real_type const & depth
                , T3Mesh & mesh
                , VertexAttribute<MTd::real_type,T3Mesh> & X
                , VertexAttribute<MTd::real_type,T3Mesh> & Y
                , VertexAttribute<MTd::real_type,T3Mesh> & Z
                );
  
  template
  void make_box<MTf>(
                     MTf::real_type const & width
                     , MTf::real_type const & height
                     , MTf::real_type const & depth
                     , T3Mesh & mesh
                     , VertexAttribute<MTf::vector3_type,T3Mesh> & coords
                     );
  
  template
  void make_box<MTd>(
                     MTd::real_type const & width
                     , MTd::real_type const & height
                     , MTd::real_type const & depth
                     , T3Mesh & mesh
                     , VertexAttribute<MTd::vector3_type,T3Mesh> & coords
                     );

} //namespace mesh_array
