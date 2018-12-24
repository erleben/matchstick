#include <factory/mesh_array_make_cuboid.h>

#include <tiny_math_types.h>

namespace mesh_array
{
  
  template<typename MT>
  void make_cuboid(
                   typename MT::vector3_type const & v0
                   , typename MT::vector3_type const & v1
                   , typename MT::vector3_type const & v2
                   , typename MT::vector3_type const & v3
                   , typename MT::vector3_type const & v4
                   , typename MT::vector3_type const & v5
                   , typename MT::vector3_type const & v6
                   , typename MT::vector3_type const & v7
                   , T3Mesh & mesh
                   , VertexAttribute<typename MT::real_type,T3Mesh> & X
                   , VertexAttribute<typename MT::real_type,T3Mesh> & Y
                   , VertexAttribute<typename MT::real_type,T3Mesh> & Z
                   )
  {    
		mesh.clear();
    X.release();
    Y.release();
    Z.release();

    mesh.set_capacity( 8u, 12u);

    std::vector<Vertex> vertices;
    vertices.resize(8);

    vertices[0] = mesh.push_vertex();
    vertices[1] = mesh.push_vertex();
    vertices[2] = mesh.push_vertex();
    vertices[3] = mesh.push_vertex();
    vertices[4] = mesh.push_vertex();
    vertices[5] = mesh.push_vertex();
    vertices[6] = mesh.push_vertex();
    vertices[7] = mesh.push_vertex();

    unsigned int const quads[6][5] = {
      { 0, 1, 2, 3 },
      { 4, 7, 6, 5 },
      { 4, 0, 3, 7 },
      { 1, 5, 6, 2 },
      { 7, 3, 2, 6 },
      { 5, 1, 0, 4 },
    };

		for(size_t i=0u;i<6u;++i)
		{
			Vertex const vi = mesh.vertex( quads[i][0] );
			Vertex const vj = mesh.vertex( quads[i][1] );
			Vertex const vk = mesh.vertex( quads[i][2] );
			Vertex const vm = mesh.vertex( quads[i][3] );
      
      mesh.push_triangle(vi,vj,vk);
      mesh.push_triangle(vi,vk,vm);
		}

    X.bind(mesh);
    Y.bind(mesh);
    Z.bind(mesh);

    X(vertices[0]) = v0(0);
    X(vertices[1]) = v1(0);
    X(vertices[2]) = v2(0);
    X(vertices[3]) = v3(0);
    X(vertices[4]) = v4(0);
    X(vertices[5]) = v5(0);
    X(vertices[6]) = v6(0);
    X(vertices[7]) = v7(0);

    Y(vertices[0]) = v0(1);
    Y(vertices[1]) = v1(1);
    Y(vertices[2]) = v2(1);
    Y(vertices[3]) = v3(1);
    Y(vertices[4]) = v4(1);
    Y(vertices[5]) = v5(1);
    Y(vertices[6]) = v6(1);
    Y(vertices[7]) = v7(1);

    Z(vertices[0]) = v0(2);
    Z(vertices[1]) = v1(2);
    Z(vertices[2]) = v2(2);
    Z(vertices[3]) = v3(2);
    Z(vertices[4]) = v4(2);
    Z(vertices[5]) = v5(2);
    Z(vertices[6]) = v6(2);
    Z(vertices[7]) = v7(2);
	}
	
  typedef tiny::MathTypes<float> MTf;
  typedef tiny::MathTypes<double> MTd;
  
  template
  void make_cuboid<MTf>(
                        MTf::vector3_type const & v0
                        , MTf::vector3_type const & v1
                        , MTf::vector3_type const & v2
                        , MTf::vector3_type const & v3
                        , MTf::vector3_type const & v4
                        , MTf::vector3_type const & v5
                        , MTf::vector3_type const & v6
                        , MTf::vector3_type const & v7
                        , T3Mesh & mesh
                        , VertexAttribute<MTf::real_type,T3Mesh> & X
                        , VertexAttribute<MTf::real_type,T3Mesh> & Y
                        , VertexAttribute<MTf::real_type,T3Mesh> & Z
                        );
  
  template
  void make_cuboid<MTd>(
                        MTd::vector3_type const & v0
                        , MTd::vector3_type const & v1
                        , MTd::vector3_type const & v2
                        , MTd::vector3_type const & v3
                        , MTd::vector3_type const & v4
                        , MTd::vector3_type const & v5
                        , MTd::vector3_type const & v6
                        , MTd::vector3_type const & v7
                        , T3Mesh & mesh
                        , VertexAttribute<MTd::real_type,T3Mesh> & X
                        , VertexAttribute<MTd::real_type,T3Mesh> & Y
                        , VertexAttribute<MTd::real_type,T3Mesh> & Z
                        );
  
  
} //namespace mesh_array
