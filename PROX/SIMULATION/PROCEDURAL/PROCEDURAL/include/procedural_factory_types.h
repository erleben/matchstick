#ifndef PROCEDURAL_FACTORY_TYPES_H
#define PROCEDURAL_FACTORY_TYPES_H

#include <mesh_array.h>
#include <mass.h>

namespace procedural
{

  namespace detail
  {

    template<typename MT>
    class MeshData
    : public mass::FaceCallbackInterface<typename MT::real_type>
    {
    protected:

      typedef typename MT::real_type T;

    public:

      mesh_array::T3Mesh  m_mesh;
      mesh_array::VertexAttribute<T,mesh_array::T3Mesh> m_X;
      mesh_array::VertexAttribute<T,mesh_array::T3Mesh> m_Y;
      mesh_array::VertexAttribute<T,mesh_array::T3Mesh> m_Z;

    public:

      size_t get_number_of_vertices(size_t const & face_no) const { return 3u; }

      void get_x_coords(size_t const & face_no, T * coords) const
      {
        mesh_array::Triangle const t = m_mesh.triangle( face_no );

        mesh_array::Vertex const vi = t.i();
        mesh_array::Vertex const vj = t.j();
        mesh_array::Vertex const vk = t.k();

        coords[0] = m_X(vi);
        coords[1] = m_X(vj);
        coords[2] = m_X(vk);
      }

      void get_y_coords(size_t const & face_no, T * coords) const
      {
        mesh_array::Triangle const t = m_mesh.triangle( face_no );

        mesh_array::Vertex const vi = t.i();
        mesh_array::Vertex const vj = t.j();
        mesh_array::Vertex const vk = t.k();

        coords[0] = m_Y(vi);
        coords[1] = m_Y(vj);
        coords[2] = m_Y(vk);
      }

      void get_z_coords(size_t const & face_no, T * coords) const
      {
        mesh_array::Triangle const t = m_mesh.triangle( face_no );

        mesh_array::Vertex const vi = t.i();
        mesh_array::Vertex const vj = t.j();
        mesh_array::Vertex const vk = t.k();

        coords[0] = m_Z(vi);
        coords[1] = m_Z(vj);
        coords[2] = m_Z(vk);
      }

    };

  }//namespace detail

} // end of namespace procedural

// PROCEDURAL_FACTORY_TYPES_H
#endif
