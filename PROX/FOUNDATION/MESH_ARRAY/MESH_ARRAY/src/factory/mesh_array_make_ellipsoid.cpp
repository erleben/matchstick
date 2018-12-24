#include <factory/mesh_array_make_ellipsoid.h>
#include <factory/mesh_array_make_sphere.h>
#include <factory/mesh_array_scale.h>

#include <tiny_math_types.h>

namespace mesh_array
{
  
  template<typename MT>
  void make_ellipsoid(
                     typename MT::real_type const & a
                     , typename MT::real_type const & b
                     , typename MT::real_type const & c
                     , size_t const & slices
                     , size_t const & segments
                     , T3Mesh & mesh
                     , VertexAttribute<typename MT::real_type,T3Mesh> & X
                     , VertexAttribute<typename MT::real_type,T3Mesh> & Y
                     , VertexAttribute<typename MT::real_type,T3Mesh> & Z
                     )
  {
    typedef typename MT::value_traits    VT;
    
		make_sphere<MT>(VT::one(), slices, segments, mesh, X, Y, Z);
		scale<MT>(a,b,c, mesh, X, Y, Z);
	}

  typedef tiny::MathTypes<float> MTf;
  typedef tiny::MathTypes<double> MTd;

  template
  void make_ellipsoid<MTf>(
                      MTf::real_type const & a
                      , MTf::real_type const & b
                      , MTf::real_type const & c
                      , size_t const & slices
                      , size_t const & segments
                      , T3Mesh & mesh
                      , VertexAttribute<MTf::real_type,T3Mesh> & X
                      , VertexAttribute<MTf::real_type,T3Mesh> & Y
                      , VertexAttribute<MTf::real_type,T3Mesh> & Z
                      );

  template
  void make_ellipsoid<MTd>(
                      MTd::real_type const & a
                      , MTd::real_type const & b
                      , MTd::real_type const & c
                      , size_t const & slices
                      , size_t const & segments
                      , T3Mesh & mesh
                      , VertexAttribute<MTd::real_type,T3Mesh> & X
                      , VertexAttribute<MTd::real_type,T3Mesh> & Y
                      , VertexAttribute<MTd::real_type,T3Mesh> & Z
                      );

} //namespace mesh_array
