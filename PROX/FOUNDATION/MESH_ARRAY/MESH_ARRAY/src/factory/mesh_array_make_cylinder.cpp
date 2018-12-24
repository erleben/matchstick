#include <factory/mesh_array_make_cylinder.h>
#include <factory/mesh_array_profile_sweep.h>

#include <tiny_math_types.h>

#include <vector>

namespace mesh_array
{
  
  template<typename MT>
  void make_cylinder(
                    typename MT::real_type const & radius
                    , typename MT::real_type const & height
                    , size_t const & slices
                    , T3Mesh & mesh
                    , VertexAttribute<typename MT::real_type,T3Mesh> & X
                    , VertexAttribute<typename MT::real_type,T3Mesh> & Y
                    , VertexAttribute<typename MT::real_type,T3Mesh> & Z
                    )
  {
    typedef typename MT::value_traits    VT;
    typedef typename MT::vector3_type    V;

		std::vector<V> profile;
		
		profile.resize(4u);
		
		profile[ 0 ] = V::make( VT::zero(), -height/VT::two(), VT::zero() );
		profile[ 1 ] = V::make(     radius, -height/VT::two(), VT::zero() );
		profile[ 2 ] = V::make(     radius,  height/VT::two(), VT::zero() );
		profile[ 3 ] = V::make( VT::zero(),  height/VT::two(), VT::zero() );
		
		profile_sweep<MT>( profile, slices, mesh, X, Y, Z  );
	}
  
  typedef tiny::MathTypes<float> MTf;
  typedef tiny::MathTypes<double> MTd;
  
  template
  void make_cylinder<MTf>(
                     MTf::real_type const & radius
                     , MTf::real_type const & height
                     , size_t const & slices
                     , T3Mesh & mesh
                     , VertexAttribute<MTf::real_type,T3Mesh> & X
                     , VertexAttribute<MTf::real_type,T3Mesh> & Y
                     , VertexAttribute<MTf::real_type,T3Mesh> & Z
                     );

  template
  void make_cylinder<MTd>(
                     MTd::real_type const & radius
                     , MTd::real_type const & height
                     , size_t const & slices
                     , T3Mesh & mesh
                     , VertexAttribute<MTd::real_type,T3Mesh> & X
                     , VertexAttribute<MTd::real_type,T3Mesh> & Y
                     , VertexAttribute<MTd::real_type,T3Mesh> & Z
                     );

} //namespace mesh_array
