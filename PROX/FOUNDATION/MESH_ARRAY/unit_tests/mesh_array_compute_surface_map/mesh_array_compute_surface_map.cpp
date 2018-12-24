#include <mesh_array.h>
#include <tiny.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

#include <iostream>   // needed for debugging... This unit-test is not yet perfect (2015-12-11 Kenny)
#include <vector>
#include <cmath>

BOOST_AUTO_TEST_SUITE(mesh_array);

BOOST_AUTO_TEST_CASE(mesh_array_compute_surface_map_sphere)
{

  typedef tiny::MathTypes<float> MT;
  typedef MT::real_type          T;
  typedef MT::vector3_type       V;

  mesh_array::T3Mesh surf;
  mesh_array::VertexAttribute<T,mesh_array::T3Mesh> sX;
  mesh_array::VertexAttribute<T,mesh_array::T3Mesh> sY;
  mesh_array::VertexAttribute<T,mesh_array::T3Mesh> sZ;

  size_t const slices   = 12u;
  size_t const segments = 24u;
  T      const radius   = 12.0f;
  
  mesh_array::make_sphere<MT>(radius, slices, segments, surf, sX, sY, sZ);
  
  mesh_array::T4Mesh mesh;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> X;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Y;
  mesh_array::VertexAttribute<T,mesh_array::T4Mesh> Z;

  mesh_array::tetgen(surf, sX, sY, sZ, mesh, X, Y, Z );
  
  mesh_array::TetrahedronAttribute< mesh_array::TetrahedronSurfaceInfo ,mesh_array::T4Mesh> surface_map;
  
  mesh_array::compute_surface_map( mesh, X, Y, Z, surface_map );
  
  for (size_t idx = 0u; idx < mesh.tetrahedron_size(); ++idx)
  {
    mesh_array::Tetrahedron const tetrahedron = mesh.tetrahedron(idx);

    bool const opposite_i_on_surface = surface_map( tetrahedron ).m_i;
    bool const opposite_j_on_surface = surface_map( tetrahedron ).m_j;
    bool const opposite_k_on_surface = surface_map( tetrahedron ).m_k;
    bool const opposite_m_on_surface = surface_map( tetrahedron ).m_m;

    V const p_i =  V::make( X( tetrahedron.i() ), Y( tetrahedron.i() ), Z( tetrahedron.i() ) );
    V const p_j =  V::make( X( tetrahedron.j() ), Y( tetrahedron.j() ), Z( tetrahedron.j() ) );
    V const p_k =  V::make( X( tetrahedron.k() ), Y( tetrahedron.k() ), Z( tetrahedron.k() ) );
    V const p_m =  V::make( X( tetrahedron.m() ), Y( tetrahedron.m() ), Z( tetrahedron.m() ) );

    T const distance_i = fabs(radius  - tiny::norm(p_i) );
    T const distance_j = fabs(radius  - tiny::norm(p_j) );
    T const distance_k = fabs(radius  - tiny::norm(p_k) );
    T const distance_m = fabs(radius  - tiny::norm(p_m) );

    bool const i_on_surface = distance_i > 10e-5 ? false : true;
    bool const j_on_surface = distance_j > 10e-5 ? false : true;
    bool const k_on_surface = distance_k > 10e-5 ? false : true;
    bool const m_on_surface = distance_m > 10e-5 ? false : true;

    std::cout << "\t" << distance_i
              << " "  << distance_j
              << " "  << distance_k
              << " "  << distance_m
              << " "  << std::endl;

    std::cout << "\t"
              << i_on_surface
              << " "
              << j_on_surface
              << " "
              << k_on_surface
              << " "
              << m_on_surface
              << " "
              << std::endl;

    std::cout << "\t"
              << opposite_i_on_surface
              << " "
              << opposite_j_on_surface
              << " "
              << opposite_k_on_surface
              << " "
              << opposite_m_on_surface
              << " "
              << std::endl;
  }
}

BOOST_AUTO_TEST_SUITE_END();
