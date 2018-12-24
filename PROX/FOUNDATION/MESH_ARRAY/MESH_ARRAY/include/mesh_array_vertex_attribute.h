#ifndef MESH_ARRAY_VERTEX_ATTRIBUTE_H
#define MESH_ARRAY_VERTEX_ATTRIBUTE_H

#include <mesh_array_vertex.h>
#include <mesh_array_vertex_attribute.h>

#include <mesh_array_t3mesh.h>
#include <mesh_array_t4mesh.h>

#include <vector>

namespace mesh_array 
{
  
  /**
   * Vertex Attribute Class.
   *
   * @tparam T    The attribute type to be stored for each vertex
   * @tparam M    The mesh type of the vertices.
   */
  template<typename T, typename M> 
  class VertexAttribute 
  {
  public:

    typedef typename std::vector<T>::reference       reference;
		typedef typename std::vector<T>::const_reference const_reference;

  protected:

    M      const  * m_mesh;
    std::vector<T>  m_data;

  public:

    VertexAttribute()
    : m_mesh(0)
    , m_data()
    {}

    virtual ~VertexAttribute()
    {
      this->release();
    }

    VertexAttribute(M const & m)
    {
      this->bind( m );
    }

    VertexAttribute(VertexAttribute<T,M> const & o)
    {
      (*this) = o;
    }

    VertexAttribute<T,M> const & operator=(VertexAttribute<T,M> const & o)
    {
      if( this != &o)
      {
        this->m_mesh = o.m_mesh;
        this->m_data = o.m_data;
      }
      return *this;
    }

  public:

    virtual M const * mesh() const
    {
      return this->m_mesh;
    }

    virtual void bind(M const & m)
    {
      this->m_data.resize(m.vertex_capacity());
      this->m_mesh = &m;
    }

    virtual void release()
    {
      this->m_mesh = 0;
      this->m_data.clear();
    }

    virtual size_t size() const
    {
      return this->m_data.size();
    }

    virtual const_reference operator()(Vertex const & v) const
    {
      return this->m_data[v.idx()];
    }

    virtual reference operator()(Vertex const & v)
    {
      return this->m_data[v.idx()];
    }

    virtual const_reference operator[](Vertex const & v) const
    {
      return this->operator()(v);
    }

    virtual reference operator[](Vertex const & v)
    {
      return this->operator()(v);
    }

  };

  typedef VertexAttribute<bool,   T3Mesh>   t3_vertex_bool_attribute;
  typedef VertexAttribute<float,  T3Mesh>   t3_vertex_float_attribute;
  typedef VertexAttribute<int,    T3Mesh>   t3_vertex_int_attribute;
  typedef VertexAttribute<size_t, T3Mesh>   t3_vertex_uint_attribute;

  typedef VertexAttribute<bool,   T4Mesh> t4_vertex_bool_attribute;
  typedef VertexAttribute<float,  T4Mesh> t4_vertex_float_attribute;
  typedef VertexAttribute<double, T4Mesh> t4_vertex_double_attribute;
  typedef VertexAttribute<int,    T4Mesh> t4_vertex_int_attribute;
  typedef VertexAttribute<size_t, T4Mesh> t4_vertex_uint_attribute;

} // end namespace mesh_array

// MESH_ARRAY_VERTEX_ATTRIBUTE_H
#endif 
