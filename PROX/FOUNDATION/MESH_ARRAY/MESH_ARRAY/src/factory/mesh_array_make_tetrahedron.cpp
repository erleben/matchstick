#include <factory/mesh_array_make_tetrahedron.h>

#include <tiny_math_types.h>

namespace mesh_array
{
  
  template<typename MT>
  void make_tetrahedron(
                   typename MT::vector3_type const & p0
                   , typename MT::vector3_type const & p1
                   , typename MT::vector3_type const & p2
                   , typename MT::vector3_type const & p3
                   , T3Mesh & mesh
                   , VertexAttribute<typename MT::real_type,T3Mesh> & X
                   , VertexAttribute<typename MT::real_type,T3Mesh> & Y
                   , VertexAttribute<typename MT::real_type,T3Mesh> & Z
                   )
  {
    mesh.clear();
    
    mesh.set_capacity( 4u, 4u );
 
    X.bind(mesh);
    Y.bind(mesh);
    Z.bind(mesh);
    
    Vertex const vi = mesh.push_vertex();
    Vertex const vj = mesh.push_vertex();
    Vertex const vk = mesh.push_vertex();
    Vertex const vm = mesh.push_vertex();
    
    X(vi) = p0(0);    Y(vi) = p0(1);    Z(vi) = p0(2);
    X(vj) = p1(0);    Y(vj) = p1(1);    Z(vj) = p1(2);
    X(vk) = p2(0);    Y(vk) = p2(1);    Z(vk) = p2(2);
    X(vm) = p3(0);    Y(vm) = p3(1);    Z(vm) = p3(2);
    
		mesh.push_triangle( vi, vk, vj );
		mesh.push_triangle( vi, vj, vm );
		mesh.push_triangle( vj, vk, vm );
		mesh.push_triangle( vk, vi, vm );
	}

  typedef tiny::MathTypes<float> MTf;
  typedef tiny::MathTypes<double> MTd;

  template
  void make_tetrahedron<MTf>(
                        MTf::vector3_type const & p0
                        , MTf::vector3_type const & p1
                        , MTf::vector3_type const & p2
                        , MTf::vector3_type const & p3
                        , T3Mesh & mesh
                        , VertexAttribute<MTf::real_type,T3Mesh> & X
                        , VertexAttribute<MTf::real_type,T3Mesh> & Y
                        , VertexAttribute<MTf::real_type,T3Mesh> & Z
                        );
  template
  void make_tetrahedron<MTd>(
                         MTd::vector3_type const & p0
                        , MTd::vector3_type const & p1
                        , MTd::vector3_type const & p2
                        , MTd::vector3_type const & p3
                        , T3Mesh & mesh
                        , VertexAttribute<MTd::real_type,T3Mesh> & X
                        , VertexAttribute<MTd::real_type,T3Mesh> & Y
                        , VertexAttribute<MTd::real_type,T3Mesh> & Z
                        );

} //namespace mesh_array
