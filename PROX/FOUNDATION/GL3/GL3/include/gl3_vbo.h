#ifndef GL3_VBO_H
#define GL3_VBO_H

#include <gl3.h>
#include <gl3_check_errors.h>

#include <mesh_array_t3mesh.h>
#include <mesh_array_vertex_attribute.h>

#define BUFFER_OFFSET(i) ((void*)(i))

namespace gl3
{

  struct STATIC_VBO {};
  struct DYNAMIC_VBO {};

  template<typename TAG>
  inline size_t get_gl_buffer_type(TAG const & tag);

  template<>
  inline size_t get_gl_buffer_type<STATIC_VBO>( STATIC_VBO const & /*tag*/ )
  {
    return GL_STATIC_DRAW;
  }

  template<>
  inline size_t get_gl_buffer_type<DYNAMIC_VBO>( DYNAMIC_VBO const & /*tag*/ )
  {
    return GL_DYNAMIC_DRAW;
  }

  class VBO
  {
  public:

    class Vertex
    {
    public:

      float m_x;
      float m_y;
      float m_z;
      float m_nx;
      float m_ny;
      float m_nz;

    public:

      Vertex()
      : m_x(0)
      , m_y(0)
      , m_z(0)
      , m_nx(0)
      , m_ny(0)
      , m_nz(0)
      {}

      Vertex(float const & x,float const & y,float const & z,float const & nx,float const & ny,float const & nz)
      : m_x(x)
      , m_y(y)
      , m_z(z)
      , m_nx(nx)
      , m_ny(ny)
      , m_nz(nz)
      {}

    };

  protected:

    unsigned int m_vbo;
    unsigned int m_ibo;

    unsigned int m_vertex_count;
    unsigned int m_index_count;

    bool         m_is_triangles;

  public:

    unsigned int vertex_count() const { return m_vertex_count; }
    unsigned int index_count()  const { return m_index_count;  }

  public:

    VBO()
    : m_vbo(0)
    , m_ibo(0)
    , m_vertex_count(0)
    , m_index_count(0)
    , m_is_triangles(true)
    {
    }

    virtual ~VBO()
    {
      //clear();
    }

  public:

    void clear()
    {
      gl3::check_errors("VBO::clear() invoked");
      if(m_vbo!=0)
      {
        glDeleteBuffers( GL_ARRAY_BUFFER, &m_vbo );
      }
      if(m_ibo!=0)
      {
        glDeleteBuffers( GL_ELEMENT_ARRAY_BUFFER, &m_ibo );
      }
      m_vbo = 0;
      m_ibo = 0;
      m_vertex_count = 0;
      m_index_count  = 0;
      gl3::check_errors("VBO::clear() finished");
    }

    template<typename VBO_TYPE_TAG>
    void create(
                std::vector<unsigned int> const & indices
                , std::vector<Vertex> const & vertices
                , bool const is_triangles
                , VBO_TYPE_TAG const & tag
                )
    {
      gl3::check_errors("VBO::create() invoked");

      clear();

      m_is_triangles = is_triangles;

      m_vertex_count = vertices.size();
      m_index_count  = indices.size();

      glGenBuffers( 1, &m_vbo );
      glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
      glBufferData(
                   GL_ARRAY_BUFFER
                   , m_vertex_count * sizeof(Vertex)
                   , &vertices[0]
                   , get_gl_buffer_type(tag)
                   );

      glGenBuffers( 1, &m_ibo );
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ibo );
      glBufferData(
                   GL_ELEMENT_ARRAY_BUFFER
                   , m_index_count * sizeof(unsigned int)
                   , &indices[0]
                   , GL_STATIC_DRAW
                   );

      gl3::check_errors("VBO::create() glBufferData -- finished");
    }

    void update(std::vector<Vertex> const & vertices)
    {
      gl3::check_errors("VBO::update() invoked");


      glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
      glBufferData(
                   GL_ARRAY_BUFFER
                   , m_vertex_count * sizeof(Vertex)
                   , &vertices[0]
                   , GL_DYNAMIC_DRAW
                   );

      gl3::check_errors("VBO::update() glBufferData -- finished");
    }

    void bind() const
    {
      gl3::check_errors("VBO::bind() invoked");
      if(m_vbo!=0)
        glBindBuffer( GL_ARRAY_BUFFER, m_vbo );

      if(m_ibo!=0)
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ibo );

      gl3::check_errors("VBO::bind() finished");
    }

    void unbind() const
    {
      gl3::check_errors("VBO::unbind() invoked");
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
      glBindBuffer( GL_ARRAY_BUFFER, 0 );
      gl3::check_errors("VBO::unbind() finished");
    }

    void draw()const
    {
      glDrawElements(
                     m_is_triangles ? GL_TRIANGLES : GL_LINES,  // mode
                     m_index_count,                             // count
                     GL_UNSIGNED_INT,                           // type
                     BUFFER_OFFSET(0)                           // Element array buffer offset
                     );
    }

  };


  template <typename T, typename VBO_TYPE_TAG>
  inline VBO make_vbo(
                      mesh_array::T3Mesh const & mesh
                      , mesh_array::VertexAttribute<T, mesh_array::T3Mesh> const & X
                      , mesh_array::VertexAttribute<T, mesh_array::T3Mesh> const & Y
                      , mesh_array::VertexAttribute<T, mesh_array::T3Mesh> const & Z
                      , VBO_TYPE_TAG const & tag
                      )
  {
    typedef typename VBO::Vertex vertex_type;
    using std::sqrt;

    std::vector<vertex_type>   vertices;
    std::vector<unsigned int>  indices;

    unsigned int const count = mesh.triangle_size();

    for(unsigned int e = 0u; e < count; ++e)
    {
      mesh_array::Triangle const & tri = mesh.triangle( e );

      mesh_array::Vertex const & i = mesh.vertex( tri.i() );
      mesh_array::Vertex const & j = mesh.vertex( tri.j() );
      mesh_array::Vertex const & k = mesh.vertex( tri.k() );

      T const xi = X(i); T const yi = Y(i); T const zi = Z(i);
      T const xj = X(j); T const yj = Y(j); T const zj = Z(j);
      T const xk = X(k); T const yk = Y(k); T const zk = Z(k);

      T const ax = xj-xi;
      T const ay = yj-yi;
      T const az = zj-zi;
      T const bx = xk-xj;
      T const by = yk-yj;
      T const bz = zk-zj;

      T const mx =  ay*bz - by*az;
      T const my = -ax*bz + bx*az;
      T const mz =  ax*by - bx*ay;

      T const l = sqrt(mx*mx + my*my + mz*mz);

      T const nx = mx / l;
      T const ny = my / l;
      T const nz = mz / l;

      //indices.push_back( tri.i() );  // This should be done if triangles shared vertex data
      //indices.push_back( tri.j() );
      //indices.push_back( tri.k() );

      indices.push_back( e*3u + 0u );  // However, we do not want to share normals, so we do it like this
      indices.push_back( e*3u + 1u );
      indices.push_back( e*3u + 2u );

      vertices.push_back( vertex_type( xi, yi, zi, nx, ny, nz) );
      vertices.push_back( vertex_type( xj, yj, zj, nx, ny, nz) );
      vertices.push_back( vertex_type( xk, yk, zk, nx, ny, nz) );
    }

    VBO vbo;

    vbo.create( indices, vertices, true, tag );

    return vbo;
  }

  template <typename T>
  inline void update_vbo(
                         VBO & vbo
                         , mesh_array::T3Mesh const & mesh
                         , mesh_array::VertexAttribute<T, mesh_array::T3Mesh> const & X
                         , mesh_array::VertexAttribute<T, mesh_array::T3Mesh> const & Y
                         , mesh_array::VertexAttribute<T, mesh_array::T3Mesh> const & Z
                         )
  {
    using std::sqrt;

    typedef typename VBO::Vertex vertex_type;

    std::vector<vertex_type>   vertices;

    unsigned int const count = mesh.triangle_size();

    for(unsigned int e = 0u; e < count; ++e)
    {
      mesh_array::Triangle const & tri = mesh.triangle( e );

      mesh_array::Vertex const & i = mesh.vertex( tri.i() );
      mesh_array::Vertex const & j = mesh.vertex( tri.j() );
      mesh_array::Vertex const & k = mesh.vertex( tri.k() );

      T const xi = X(i); T const yi = Y(i); T const zi = Z(i);
      T const xj = X(j); T const yj = Y(j); T const zj = Z(j);
      T const xk = X(k); T const yk = Y(k); T const zk = Z(k);

      T const ax = xj-xi;
      T const ay = yj-yi;
      T const az = zj-zi;
      T const bx = xk-xj;
      T const by = yk-yj;
      T const bz = zk-zj;

      T const mx =  ay*bz - by*az;
      T const my = -ax*bz + bx*az;
      T const mz =  ax*by - bx*ay;

      T const l = sqrt(mx*mx + my*my + mz*mz);

      T const nx = mx / l;
      T const ny = my / l;
      T const nz = mz / l;

      vertices.push_back( vertex_type( xi, yi, zi, nx, ny, nz) );
      vertices.push_back( vertex_type( xj, yj, zj, nx, ny, nz) );
      vertices.push_back( vertex_type( xk, yk, zk, nx, ny, nz) );
    }

    vbo.update( vertices );
  }


  template <typename T>
  inline VBO make_vbo(
                      mesh_array::T3Mesh const & mesh
                      , mesh_array::VertexAttribute<T, mesh_array::T3Mesh> const & X
                      , mesh_array::VertexAttribute<T, mesh_array::T3Mesh> const & Y
                      , mesh_array::VertexAttribute<T, mesh_array::T3Mesh> const & Z
                      )
  {
    return make_vbo( mesh, X, Y, Z, STATIC_VBO() );
  }

  inline VBO make_unit_aabb_vbo()
  {
    typedef float T;
    typedef typename VBO::Vertex vertex_type;
    using std::sqrt;

    std::vector<vertex_type>   vertices;
    std::vector<unsigned int>  indices;

    unsigned int count = 0;

    T const x = 0.5;      T const y = 0.5;      T const z = 0.5;

    //--- front face   -------------------------------------------------------
    {
      T nx = 0.0;      T ny = 0.0;      T nz = 1.0;

      vertices.push_back( vertex_type( -x, -y, -z, nx, ny, nz) );
      vertices.push_back( vertex_type(  x, -y, -z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );

      vertices.push_back( vertex_type(  x, -y, -z, nx, ny, nz) );
      vertices.push_back( vertex_type(  x,  y, -z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );

      vertices.push_back( vertex_type(  x, y, -z, nx, ny, nz) );
      vertices.push_back( vertex_type( -x, y, -z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );

      vertices.push_back( vertex_type( -x,  y, -z, nx, ny, nz) );
      vertices.push_back( vertex_type( -x, -y, -z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );
    }
    //--- back face   -------------------------------------------------------
    {
      T nx = 0.0;      T ny = 0.0;      T nz = -1.0;

      vertices.push_back( vertex_type( -x, -y,  z, nx, ny, nz) );
      vertices.push_back( vertex_type(  x, -y,  z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );

      vertices.push_back( vertex_type(  x, -y,  z, nx, ny, nz) );
      vertices.push_back( vertex_type(  x,  y,  z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );

      vertices.push_back( vertex_type(  x, y,  z, nx, ny, nz) );
      vertices.push_back( vertex_type( -x, y,  z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );

      vertices.push_back( vertex_type( -x,  y,  z, nx, ny, nz) );
      vertices.push_back( vertex_type( -x, -y,  z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );
    }
    //--- left face   -------------------------------------------------------
    {
      T nx = -1.0;      T ny = 0.0;      T nz = 0.0;

      vertices.push_back( vertex_type( -x, -y, -z, nx, ny, nz) );
      vertices.push_back( vertex_type( -x, -y,  z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );

      vertices.push_back( vertex_type( -x, -y,  z, nx, ny, nz) );
      vertices.push_back( vertex_type( -x,  y,  z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );

      vertices.push_back( vertex_type( -x,  y,  z, nx, ny, nz) );
      vertices.push_back( vertex_type( -x,  y,  -z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );

      vertices.push_back( vertex_type( -x,  y,  -z, nx, ny, nz) );
      vertices.push_back( vertex_type( -x, -y,  -z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );


    }
    //--- right face   -------------------------------------------------------
    {
      T nx = 1.0;      T ny = 0.0;      T nz = 0.0;

      vertices.push_back( vertex_type( x, -y, -z, nx, ny, nz) );
      vertices.push_back( vertex_type( x, -y,  z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );

      vertices.push_back( vertex_type( x, -y,  z, nx, ny, nz) );
      vertices.push_back( vertex_type( x,  y,  z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );

      vertices.push_back( vertex_type( x,  y,   z, nx, ny, nz) );
      vertices.push_back( vertex_type( x,  y,  -z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );

      vertices.push_back( vertex_type( x,  y,  -z, nx, ny, nz) );
      vertices.push_back( vertex_type( x, -y,  -z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );
    }
    //--- top face   -------------------------------------------------------
    {
      T nx = 0.0;      T ny = 1.0;      T nz = 0.0;

      vertices.push_back( vertex_type( -x, y,  z, nx, ny, nz) );
      vertices.push_back( vertex_type(  x, y,  z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );

      vertices.push_back( vertex_type(  x, y,  z, nx, ny, nz) );
      vertices.push_back( vertex_type(  x, y, -z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );

      vertices.push_back( vertex_type(  x, y,  -z, nx, ny, nz) );
      vertices.push_back( vertex_type( -x, y,  -z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );

      vertices.push_back( vertex_type( -x,  y,  -z, nx, ny, nz) );
      vertices.push_back( vertex_type( -x, y,    z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );


    }
    //--- bottom face   -------------------------------------------------------
    {
      T nx = 0.0;      T ny =-1.0;      T nz = 0.0;

      vertices.push_back( vertex_type( -x, -y,  z, nx, ny, nz) );
      vertices.push_back( vertex_type(  x, -y,  z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );

      vertices.push_back( vertex_type(  x, -y,  z, nx, ny, nz) );
      vertices.push_back( vertex_type(  x, -y, -z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );

      vertices.push_back( vertex_type(  x, -y,  -z, nx, ny, nz) );
      vertices.push_back( vertex_type( -x, -y,  -z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );

      vertices.push_back( vertex_type( -x, -y,  -z, nx, ny, nz) );
      vertices.push_back( vertex_type( -x, -y,    z, nx, ny, nz) );
      indices.push_back( count++ );
      indices.push_back( count++ );

    }


    VBO vbo;
    bool const is_triangles = false;
    vbo.create( indices, vertices, is_triangles, STATIC_VBO() );

    return vbo;
  }



  inline VBO make_quad_vbo()
  {
    typedef float T;
    typedef typename VBO::Vertex vertex_type;

    std::vector<vertex_type>   vertices;
    std::vector<unsigned int>  indices;

    unsigned int count = 0;

    T const x  = 1.0;      T const y  = 1.0;      T const z  = 0.0;
    T const nx = 0.0;      T const ny = 0.0;      T const nz = 1.0;

    vertices.push_back( vertex_type( -x, -y, z, nx, ny, nz) );
    vertices.push_back( vertex_type(  x, -y, z, nx, ny, nz) );
    vertices.push_back( vertex_type(  x,  y, z, nx, ny, nz) );
    indices.push_back( count++ );
    indices.push_back( count++ );
    indices.push_back( count++ );

    vertices.push_back( vertex_type(  x,  y, z, nx, ny, nz) );
    vertices.push_back( vertex_type(  -x,  y, z, nx, ny, nz) );
    vertices.push_back( vertex_type(  -x,  -y, z, nx, ny, nz) );
    indices.push_back( count++ );
    indices.push_back( count++ );
    indices.push_back( count++ );

    VBO vbo;
    bool const is_triangles = true;
    vbo.create( indices, vertices, is_triangles, STATIC_VBO() );

    return vbo;
  }


} // namespace gl3

// GL3_VBO_H
#endif
