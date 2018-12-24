#ifndef PROX_COMPUTE_STRUCTURE_MAP_FROM_ROTATIONAL_SWEEP_H
#define PROX_COMPUTE_STRUCTURE_MAP_FROM_ROTATIONAL_SWEEP_H

#include <mesh_array_vertex_attribute.h>

namespace prox
{
  /**
   *  The structure map holds information of the structure direction in the
   *  vertices. This is a structure map creates a map by sweeping a given
   *  direction at a refernece point around a specified axis.
   *
   * Below is a matlab script that can be used for getting intuition about the mapping.
   *
   *   clear all
   *   close all
   *   clc
   *   N = 20;
   *   x = zeros(N,1); y = zeros(N,1); z = zeros(N,1);
   *   sx = zeros(N,1); sy = zeros(N,1); sz = zeros(N,1);
   *   rx = cos(pi); ry = sin(pi); rz = 0;
   *   nx = 0; ny = 0; nz = 1;
   *   SSx = 0; SSy = 1; SSz = 0;
   *   dtheta = 2*pi/N;
   *   for i=1:N
   *    px = cos(i*dtheta);
   *    py = sin(i*dtheta);
   *    pz = 0;
   *    phi = acos(px*rx + py*ry + pz*rz);
   *    if (nx * (ry*pz-py*rz) -ny * (rx*pz-px*rz) + nz * (rx*py-px*ry)) < 0.0
   *      phi = 2*pi - phi;
   *    end
   *    cp = cos(phi);
   *    sp = sin(phi);
   *    sx(i) = cp*SSx - sp*SSy;
   *    sy(i) = sp*SSx + cp*SSy;
   *    sz(i) = 0;
   *    x(i) = px;
   *    y(i) = py;
   *    z(i) = pz;
   *   end
   *   figure()
   *   plot(x,y,'rx');
   *   hold on;
   *   for i=1:N
   *    x0 = x(i);
   *    y0 = y(i);
   *    x1 = x0 + sx(i);
   *    y1 = y0 + sy(i);
   *    plot([x0 x1],[y0 y1],'-bo');
   *   end
   */
  template<typename MT>
  inline void compute_structure_map_from_rotational_sweep(
                                             mesh_array::T4Mesh const & mesh
                                            , mesh_array::VertexAttribute<typename MT::real_type, mesh_array::T4Mesh> const & X
                                            , mesh_array::VertexAttribute<typename MT::real_type, mesh_array::T4Mesh> const & Y
                                            , mesh_array::VertexAttribute<typename MT::real_type, mesh_array::T4Mesh> const & Z
                                            , typename MT::vector3_type const & axis
                                            , typename MT::vector3_type const & ref
                                            , typename MT::vector3_type const & s
                                            , mesh_array::VertexAttribute<typename MT::vector3_type, mesh_array::T4Mesh> & structure_map
                                               )
  {
    using std::acos;

    typedef typename MT::value_traits    VT;
    typedef typename MT::real_type       T;
    typedef typename MT::vector3_type    V3;
    typedef typename MT::quaternion_type Q;

    T  const two_pi = VT::two()*VT::pi();
    V3 const n      = tiny::unit(axis);
    V3 const r      = tiny::unit(ref);

    structure_map.bind(mesh);

    for ( size_t idx = 0u; idx < mesh.vertex_size(); ++idx)
    {
      mesh_array::Vertex const vertex = mesh.vertex( idx );

      T  const x = X(vertex);
      T  const y = Y(vertex);
      T  const z = Z(vertex);
      V3 const p = tiny::unit(V3::make(x,y,z));

      bool const flipped = tiny::inner_prod( tiny::cross( r, p ), n ) < VT::zero();
      T    const psi     = acos( tiny::inner_prod(p, r) );
      T    const theta   = flipped ? (two_pi - psi) : psi;

      Q  const R     = Q::Ru( theta, n);
      V3 const s_new = tiny::rotate(R, s);
      
      structure_map(vertex) = s_new;
    }
  }

}// namespace prox


//PROX_COMPUTE_STRUCTURE_MAP_FROM_ROTATIONAL_SWEEP_H
#endif
