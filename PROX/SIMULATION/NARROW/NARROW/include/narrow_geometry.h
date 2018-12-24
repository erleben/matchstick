#ifndef NARROW_GEOMETRY_H
#define NARROW_GEOMETRY_H

#include <mesh_array.h>    // needed for mesh_array data types
#include <kdop.h>

#include <tiny_vector_functions.h>  // needed for tiny::norm
#include <tiny_quaternion_functions.h>
#include <tiny_coordsys_functions.h>

#include <vector>
#include <algorithm>

namespace narrow
{
  
  /**
   * Narrow Phase Geometry Type.
   * The geometry of an object consists of a collection of shapes. The object defines
   * a local coordinate system in which the shapes live. Ie. each shape has an associated
   * transform that specifies how the shape is placed in the local object frame. When an
   * object is simulated then the simulator essientially moves the local object frame
   * to a new placement. However, the shapes rigidly follows the local object frames as it
   * moves around.
   *
   * This class offers an interface for easy manipulation of the shapes that make up the geometry
   * of an object.
   *
   * @tparam M     The Math types used.
   */
  template<typename M>
  class Geometry
  {
  protected:

    typedef typename M::coordsys_type C;
    typedef typename M::real_type     T;
    typedef typename M::vector3_type  V;
    typedef typename M::value_traits  VT;

  public:

    mesh_array::T4Mesh      m_mesh;   ///< Volume mesh of object

    mesh_array::VertexAttribute<T,mesh_array::T4Mesh> m_X0;     ///< Undeformed (material) x-coordinate
    mesh_array::VertexAttribute<T,mesh_array::T4Mesh> m_Y0;     ///< Undeformed (material) y-coordinate
    mesh_array::VertexAttribute<T,mesh_array::T4Mesh> m_Z0;     ///< Undeformed (material) z-coordinate

    mesh_array::TetrahedronAttribute<mesh_array::TetrahedronSurfaceInfo,mesh_array::T4Mesh> m_surface_map;

    T                       m_mesh_radius;
    T                       m_mesh_scale;

  public:

    Geometry()
    : m_mesh()
    , m_X0()
    , m_Y0()
    , m_Z0()
    , m_surface_map()
    , m_mesh_radius( VT::zero() )
    , m_mesh_scale(VT::zero() )
    {
    }

    Geometry( Geometry const & geo){ *this = geo; }

    Geometry& operator=( Geometry const & geo)
    {
      if( this != &geo)
      {
        this->m_mesh = geo.m_mesh;
        this->m_X0 = geo.m_X0;
        this->m_Y0 = geo.m_Y0;
        this->m_Z0 = geo.m_Z0;
        this->m_surface_map  = geo.m_surface_map;
        this->m_mesh_radius = geo.m_mesh_scale;
        this->m_mesh_scale = geo.m_mesh_scale;
      }
      return *this;
    }

    ~Geometry()
    {
      this->clear();
    }

  public:


    T get_scale() const { return m_mesh_scale; }

    void set_shape(
                   mesh_array::T4Mesh const & mesh
                   , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & X
                   , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Y
                   , mesh_array::VertexAttribute<T,mesh_array::T4Mesh> const & Z
                   )
    {
      using tiny::norm;
      using std::max;
      using tiny::min;
      using tiny::max;

      kdop::mesh_reorder( mesh
                         , X
                         , Y
                         , Z
                         , m_mesh
                         , m_X0
                         , m_Y0
                         , m_Z0
                         );

      mesh_array::compute_surface_map( m_mesh
                                      , m_X0
                                      , m_Y0
                                      , m_Z0
                                      , m_surface_map
                                      );

      m_mesh_radius = VT::zero();

      size_t const N = m_mesh.vertex_size();

      V min_coord = V(VT::highest());
      V max_coord = V(VT::lowest());

      for(size_t n = 0u; n < N;++n)
      {
        mesh_array::Vertex const & v = m_mesh.vertex(n);

        V const r0 = V::make(  m_X0(v), m_Y0(v), m_Z0(v) );

        min_coord = min(min_coord,r0);
        max_coord = max(max_coord,r0);

        m_mesh_radius = max( m_mesh_radius, norm(r0) );
      }

      m_mesh_scale = min(  max_coord-min_coord );
    }


    bool has_shape() const
    {
      return ((m_mesh.vertex_size() > 0u) && (m_mesh.tetrahedron_size() > 0u));
    }

    void clear()
    {
      m_X0.release();
      m_Y0.release();
      m_Z0.release();
      m_surface_map.release();

      m_mesh.clear();
      m_mesh_radius = VT::zero();
      m_mesh_scale  = VT::zero();
    }


    T const & get_radius() const
    {
      return this->m_mesh_radius;
    }
    
  };
  
} //namespace narrow

// NARROW_GEOMETRY_H
#endif 
