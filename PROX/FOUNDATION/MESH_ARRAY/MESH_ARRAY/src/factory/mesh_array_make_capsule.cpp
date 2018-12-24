#include <factory/mesh_array_make_capsule.h>
#include <factory/mesh_array_profile_sweep.h>

#include <tiny_math_types.h>

#include <vector>

namespace mesh_array
{
  
  template<typename MT>
  void make_capsule(
                    typename MT::real_type const & radius
                    , typename MT::real_type const & height
                    , size_t const & slices
                    , size_t const & segments
                    , T3Mesh & mesh
                    , VertexAttribute<typename MT::real_type,T3Mesh> & X
                    , VertexAttribute<typename MT::real_type,T3Mesh> & Y
                    , VertexAttribute<typename MT::real_type,T3Mesh> & Z
                    )
  {
    typedef typename MT::real_type       T;
    typedef typename MT::value_traits    VT;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type Q;
    
		std::vector<V> profile;
		
		profile.resize(segments);
		
		T const dtheta = VT::pi() / (segments-1);
    
		for(size_t i=0;i < segments; ++i )
		{
			T const theta = dtheta*i;
			
			Q const R = Q::Ru( theta ,V::k() );
			T const dh = (i < (segments/2)) ? -height/2.0f : height/2.0f;
			
			profile[ i ] = rotate( R, V::make(  VT::zero(), -radius, VT::zero() ) ) + V::make(VT::zero(),dh,VT::zero());
		}
		
		profile_sweep<MT>( profile, slices, mesh, X, Y, Z );
	}
  
  typedef tiny::MathTypes<float> MTf;
  typedef tiny::MathTypes<double> MTd;
  
  template
  void make_capsule<MTf>(
                         MTf::real_type const & radius
                         , MTf::real_type const & height
                         , size_t const & slices
                         , size_t const & segments
                         , T3Mesh & mesh
                         , VertexAttribute<MTf::real_type,T3Mesh> & X
                         , VertexAttribute<MTf::real_type,T3Mesh> & Y
                         , VertexAttribute<MTf::real_type,T3Mesh> & Z
                         );
	
  template
  void make_capsule<MTd>(
                         MTd::real_type const & radius
                         , MTd::real_type const & height
                         , size_t const & slices
                         , size_t const & segments
                         , T3Mesh & mesh
                         , VertexAttribute<MTd::real_type,T3Mesh> & X
                         , VertexAttribute<MTd::real_type,T3Mesh> & Y
                         , VertexAttribute<MTd::real_type,T3Mesh> & Z
                         );
  
} //namespace mesh_array
