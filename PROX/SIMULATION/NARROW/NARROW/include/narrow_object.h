#ifndef NARROW_OBJECT_H
#define NARROW_OBJECT_H

#include <narrow_geometry.h>
#include <narrow_params.h>

#include <kdop_make_tree.h>
#include <kdop_tree.h>

#include <mesh_array.h>

#include <limits>

namespace narrow
{

  /**
   * Narrow Phase Object Interface.
   * This class defines a common interface for all narrow phase collision objects.
   *
   * @tparam M    The math types
   */
  template<typename M>
  class Object
  {
  protected:

    typedef typename M::real_type                         T;
    typedef typename M::vector3_type                      V;
    typedef typename M::quaternion_type                   Q;
    typedef typename M::value_traits                      VT;
    typedef typename M::coordsys_type                     C;

  public:

    template<typename MP>
    friend void make_kdop_bvh(Params<MP> const & params, Object<MP> & object, Geometry<MP> const & geometry);

    mesh_array::VertexAttribute<T,mesh_array::T4Mesh> m_X;      ///< Deformed (spatial) x-coordinate
    mesh_array::VertexAttribute<T,mesh_array::T4Mesh> m_Y;      ///< Deformed (spatial) y-coordinate
    mesh_array::VertexAttribute<T,mesh_array::T4Mesh> m_Z;      ///< Deformed (spatial) z-coordinate
    mesh_array::VertexAttribute<V,mesh_array::T4Mesh> m_structure_map; ///< More complicated solution for friction using a mesh array

    kdop::Tree<T,8> m_tree;                                     ///< kDOP BVH tree used for collision detection of deformed (spatial) mesh

    T m_dynamic_radius;                                         ///< The current updated radius for the current deformed shape.

  protected:
    
    size_t           m_geometry_idx;  ///< A geometry index.

  public:

    void set_geometry_idx( size_t const & idx )
    {
      this->m_geometry_idx = idx;
    }

    size_t const & get_geometry_idx() const
    {
      return this->m_geometry_idx;
    }

    void set_structure_map(mesh_array::VertexAttribute<V,mesh_array::T4Mesh> & structure_map)
    {
        this->m_structure_map = structure_map;
    }

    T const & get_dynamic_radius() const
    {
      return this->m_dynamic_radius;
    }

    void set_dynamic_radius(T const & value)
    {
      assert(value >= VT::zero() || !"set_dynamic_radius(): illegal radius value");

      this->m_dynamic_radius = value;
    }

  public:

    Object()
    : m_X()
    , m_Y()
    , m_Z()
    , m_tree()
    , m_dynamic_radius( VT::zero() )
    , m_geometry_idx( 0u )
    {}

    ~ Object(){}

    Object(Object const & obj)
    {
      *this = obj;
    }

    Object& operator=( Object const & obj)
    {
      if( this != &obj)
      {
        this->m_X                   = obj.m_X;
        this->m_Y                   = obj.m_Y;
        this->m_Z                   = obj.m_Z;
        this->m_tree                = obj.m_tree;
        this->m_geometry_idx        = obj.m_geometry_idx;
        this->m_structure_map       = obj.m_structure_map;
        this->m_dynamic_radius      = obj.m_dynamic_radius;
      }
      return *this;
    }

  };

  template<typename M>
  inline void make_kdop_bvh(Params<M> const & params, Object<M> & object, Geometry<M> const & geometry)
  {
    typedef typename M::real_type                         T;
    typedef typename M::vector3_type                      V;

    if( geometry.has_shape() )
    {
      size_t mem_bytes = params.get_chunk_bytes();

      object.m_tree = kdop::make_tree<V,8,T>(
                                             mem_bytes
                                             , geometry.m_mesh
                                             , geometry.m_X0
                                             , geometry.m_Y0
                                             , geometry.m_Z0
                                             );

      object.m_X.bind(geometry.m_mesh);
      object.m_Y.bind(geometry.m_mesh);
      object.m_Z.bind(geometry.m_mesh);

      object.m_dynamic_radius = geometry.m_static_radius;
    }
  }
  
} //namespace narrow

// NARROW_OBJECT_H
#endif
