#include <kdop.h>
#include <mesh_array.h>
#include <geometry.h>
#include <tiny.h>


#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

typedef tiny::MathTypes<float> MT;
typedef MT::vector3_type       V;
typedef MT::real_type          T;
typedef MT::value_traits       VT;


class GeometryInfo
{
public:

  kdop::Tree<T,6>                                   m_tree;
  mesh_array::T4Mesh                                m_mesh;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> m_X;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> m_Y;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> m_Z;

  mesh_array::TetrahedronAttribute<mesh_array::TetrahedronSurfaceInfo,mesh_array::T4Mesh> m_surface_map;

};


void make_geometry( GeometryInfo & info )
{
  mesh_array::T3Mesh surface;
  mesh_array::VertexAttribute<T,mesh_array::T3Mesh> sX;
  mesh_array::VertexAttribute<T,mesh_array::T3Mesh> sY;
  mesh_array::VertexAttribute<T,mesh_array::T3Mesh> sZ;

  mesh_array::make_box<MT>( 2.0f, 2.0f, 2.0f, surface, sX, sY, sZ);

  mesh_array::T4Mesh mesh_in;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> X_in;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Y_in;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Z_in;

  mesh_array::tetgen(surface, sX, sY, sZ, mesh_in, X_in, Y_in, Z_in);
  
  kdop::mesh_reorder( mesh_in, X_in, Y_in, Z_in, info.m_mesh, info.m_X, info.m_Y, info.m_Z );

  mesh_array::compute_surface_map( info.m_mesh, info.m_X, info.m_Y, info.m_Z, info.m_surface_map );

  info.m_tree = kdop::make_tree<V,6,T>( 32000, info.m_mesh, info.m_X, info.m_Y, info.m_Z );

  BOOST_CHECK( info.m_surface_map.size()>0u );

}


bool compute_raycast(geometry::Ray<V> const & ray, GeometryInfo const & object, V & hit_point, T & length)
{
  return kdop::raycast(
                       ray
                       , object.m_tree
                       , object.m_mesh
                       , object.m_X
                       , object.m_Y
                       , object.m_Z
                       , object.m_surface_map
                       , hit_point
                       , length
                       );
}

BOOST_AUTO_TEST_SUITE(kdop);

BOOST_AUTO_TEST_CASE(raycast_test)
{

  GeometryInfo object;

  make_geometry(object);

  // Hit straigth on
  {
    V const r      = V::make( 0.0, 0.0, 1.0);
    V const p      = V::make( 0.0, 0.0,-3.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool const hit = compute_raycast(ray, object, q, length);

    BOOST_CHECK( hit );

    BOOST_CHECK_CLOSE( length, 2.0, 0.01);
    BOOST_CHECK_CLOSE( q(0),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(1),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(2), -1.0, 0.01);
  }

  // Hit straigth on corner
  {
    V const r      = V::make( 0.0, 0.0, 1.0);
    V const p      = V::make( -1.0, -1.0,-3.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool const hit = compute_raycast(ray, object, q, length);

    BOOST_CHECK( hit );

    BOOST_CHECK_CLOSE( length, 2.0, 0.01);
    BOOST_CHECK_CLOSE( q(0), -1.0, 0.01);
    BOOST_CHECK_CLOSE( q(1), -1.0, 0.01);
    BOOST_CHECK_CLOSE( q(2), -1.0, 0.01);
  }


  // Hit straigth on edge
  {
    V const r      = V::make( 0.0, 0.0, 1.0);
    V const p      = V::make( 0.0, -1.0,-3.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool const hit = compute_raycast(ray, object, q, length);

    BOOST_CHECK( hit );

    BOOST_CHECK_CLOSE( length, 2.0, 0.01);
    BOOST_CHECK_CLOSE( q(0),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(1), -1.0, 0.01);
    BOOST_CHECK_CLOSE( q(2), -1.0, 0.01);
  }


  // Aligned ray no hitting
  {
    V const r      = V::make( 0.0, 0.0, 1.0);
    V const p      = V::make( -2.0, 0.0,-3.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool const hit = compute_raycast(ray, object, q, length);

    BOOST_CHECK( !hit );

    BOOST_CHECK_CLOSE( length, VT::infinity(), 0.01);
    BOOST_CHECK_CLOSE( q(0),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(1),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(2),  0.0, 0.01);
  }


  // Obligue ray no hitting
  {
    V const r      = V::make( 1.0, 1.0, 1.0);
    V const p      = V::make( 0.0, 0.0,-10.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool const hit = compute_raycast(ray, object, q, length);

    BOOST_CHECK( !hit );

    BOOST_CHECK_CLOSE( length, VT::infinity(), 0.01);
    BOOST_CHECK_CLOSE( q(0),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(1),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(2),  0.0, 0.01);
  }


  // Obligue ray central hit
  {
    V const r      = V::make( 1.0,  1.0,  1.0);
    V const p      = V::make(-2.0, -2.0, -3.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool const hit = compute_raycast(ray, object, q, length);

    BOOST_CHECK( hit );

    BOOST_CHECK_CLOSE( length, 2*1.732050807568877, 0.01);
    BOOST_CHECK_CLOSE( q(0),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(1),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(2), -1.0, 0.01);
  }


  // Obligue ray corner hit
  {
    V const r      = V::make( 1.0,  1.0,  1.0);
    V const p      = V::make( 0.0,  0.0, -2.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool const hit = compute_raycast(ray, object, q, length);

    BOOST_CHECK( hit );

    BOOST_CHECK_CLOSE( length, 1.732050807568877, 0.01);
    BOOST_CHECK_CLOSE( q(0),  1.0, 0.01);
    BOOST_CHECK_CLOSE( q(1),  1.0, 0.01);
    BOOST_CHECK_CLOSE( q(2), -1.0, 0.01);
  }


  // Obligue ray edge hit
  {
    V const r      = V::make( 1.0,  1.0,  1.0);
    V const p      = V::make( 0.0, -1.0, -2.0);

    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool const hit = compute_raycast(ray, object, q, length);

    BOOST_CHECK( hit );

    BOOST_CHECK_CLOSE( length, 1.732050807568877, 0.01);
    BOOST_CHECK_CLOSE( q(0),  1.0, 0.01);
    BOOST_CHECK_CLOSE( q(1),  0.0, 0.01);
    BOOST_CHECK_CLOSE( q(2), -1.0, 0.01);
  }

}

BOOST_AUTO_TEST_SUITE_END();
