#include <factory/mesh_array_make_conical.h>
#include <factory/mesh_array_profile_sweep.h>

#include <tiny_math_types.h>

#include <vector>

namespace mesh_array
{
  
  template<typename MT>
  void make_conical(
                 typename MT::real_type const & bottom_radius
                 , typename MT::real_type const & top_radius
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
		
		profile[ 0 ] = V::make(   VT::zero(), VT::zero(), VT::zero() );
		profile[ 1 ] = V::make(bottom_radius, VT::zero(), VT::zero() );
		profile[ 2 ] = V::make(   top_radius,     height, VT::zero() );
		profile[ 3 ] = V::make(   VT::zero(),     height, VT::zero() );
		
		profile_sweep<MT>( profile, slices, mesh, X, Y, Z  );
	}
  
  typedef tiny::MathTypes<float> MTf;
  typedef tiny::MathTypes<double> MTd;

  template
  void make_conical<MTf>(
                    MTf::real_type const & bottom_radius
                    , MTf::real_type const & top_radius
                    , MTf::real_type const & height
                    , size_t const & slices
                    , T3Mesh & mesh
                    , VertexAttribute<MTf::real_type,T3Mesh> & X
                    , VertexAttribute<MTf::real_type,T3Mesh> & Y
                    , VertexAttribute<MTf::real_type,T3Mesh> & Z
                    );

  template
  void make_conical<MTd>(
                    MTd::real_type const & bottom_radius
                    , MTd::real_type const & top_radius
                    , MTd::real_type const & height
                    , size_t const & slices
                    , T3Mesh & mesh
                    , VertexAttribute<MTd::real_type,T3Mesh> & X
                    , VertexAttribute<MTd::real_type,T3Mesh> & Y
                    , VertexAttribute<MTd::real_type,T3Mesh> & Z
                    );
  
} //namespace mesh_array
