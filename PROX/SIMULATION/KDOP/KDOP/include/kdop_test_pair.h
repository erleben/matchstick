#ifndef KDOP_TEST_PAIR_H
#define	KDOP_TEST_PAIR_H

#include <kdop_tree.h>

#include <contacts/geometry_contacts_callback.h>

#include <mesh_array.h>
#include <mesh_array_t4mesh.h>
#include <mesh_array_vertex_attribute.h>

namespace kdop
{

  template< typename V, size_t K, typename T >
  class TestPair
  {
  public:

    Tree<T, K> const * m_tree_a;
    Tree<T, K> const * m_tree_b;

    mesh_array::T4Mesh const * m_mesh_a;
    mesh_array::T4Mesh const * m_mesh_b;

    mesh_array::VertexAttribute<T, mesh_array::T4Mesh> const * m_x_a;
    mesh_array::VertexAttribute<T, mesh_array::T4Mesh> const * m_x_b;
    mesh_array::VertexAttribute<T, mesh_array::T4Mesh> const * m_y_a;

    mesh_array::VertexAttribute<T, mesh_array::T4Mesh> const * m_y_b;
    mesh_array::VertexAttribute<T, mesh_array::T4Mesh> const * m_z_a;
    mesh_array::VertexAttribute<T, mesh_array::T4Mesh> const * m_z_b;

    mesh_array::TetrahedronAttribute<mesh_array::TetrahedronSurfaceInfo, mesh_array::T4Mesh> const * m_surface_map_a;
    mesh_array::TetrahedronAttribute<mesh_array::TetrahedronSurfaceInfo, mesh_array::T4Mesh> const * m_surface_map_b;

    mesh_array::VertexAttribute<V, mesh_array::T4Mesh> const * m_Sa;
    mesh_array::VertexAttribute<V, mesh_array::T4Mesh> const * m_Sb;

    geometry::ContactsCallback<V> * m_callback;

  public:

    TestPair()
    : m_tree_a(0)
    , m_tree_b(0)
    , m_mesh_a(0)
    , m_mesh_b(0)
    , m_x_a(0)
    , m_x_b(0)
    , m_y_a(0)
    , m_y_b(0)
    , m_z_a(0)
    , m_z_b(0)
    , m_surface_map_a(0)
    , m_surface_map_b(0)
    , m_Sa(0)
    , m_Sb(0)
    , m_callback(0)
    {}

    TestPair(
               Tree<T, K> const & tree_A
             , Tree<T, K> const & tree_B
             , mesh_array::T4Mesh const & mesh_A
             , mesh_array::T4Mesh const & mesh_B
             , mesh_array::VertexAttribute<T, mesh_array::T4Mesh> const & X_A
             , mesh_array::VertexAttribute<T, mesh_array::T4Mesh> const & X_B
             , mesh_array::VertexAttribute<T, mesh_array::T4Mesh> const & Y_A
             , mesh_array::VertexAttribute<T, mesh_array::T4Mesh> const & Y_B
             , mesh_array::VertexAttribute<T, mesh_array::T4Mesh> const & Z_A
             , mesh_array::VertexAttribute<T, mesh_array::T4Mesh> const & Z_B
             , mesh_array::TetrahedronAttribute<mesh_array::TetrahedronSurfaceInfo, mesh_array::T4Mesh> const & surface_map_a
             , mesh_array::TetrahedronAttribute<mesh_array::TetrahedronSurfaceInfo, mesh_array::T4Mesh> const & surface_map_b
             , mesh_array::VertexAttribute<V, mesh_array::T4Mesh> const & Sa
             , mesh_array::VertexAttribute<V, mesh_array::T4Mesh> const & Sb
             , geometry::ContactsCallback<V> & callback
             )
    : m_tree_a(&tree_A)
    , m_tree_b(&tree_B)
    , m_mesh_a(&mesh_A)
    , m_mesh_b(&mesh_B)
    , m_x_a(&X_A)
    , m_x_b(&X_B)
    , m_y_a(&Y_A)
    , m_y_b(&Y_B)
    , m_z_a(&Z_A)
    , m_z_b(&Z_B)
    , m_surface_map_a(&surface_map_a)
    , m_surface_map_b(&surface_map_b)
    , m_Sa(&Sa)
    , m_Sb(&Sb)
    , m_callback(&callback)
    {}

  };

} // namespace kdop

// KDOP_TEST_PAIR_H
#endif

