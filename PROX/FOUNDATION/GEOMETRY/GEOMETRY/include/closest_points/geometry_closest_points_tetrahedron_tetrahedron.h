#ifndef GEOMETRY_CLOSEST_POINTS_TETRAHEDRON_TETRAHEDRON_H
#define GEOMETRY_CLOSEST_POINTS_TETRAHEDRON_TETRAHEDRON_H

#include <types/geometry_tetrahedron.h>
#include <types/geometry_triangle.h>
#include <contacts/geometry_contacts_tetrahedron_tetrahedron_opposing.h>  // Needed for UnscaledPlane stuff

#include <tiny.h>

#include <vector>
#include <cassert>

namespace geometry
{
  template< typename V>
  inline void closest_points_tetrahedron_tetrahedron(
                                                     Tetrahedron<V> const & A
                                                     , Tetrahedron<V> const & B
                                                     , V & a
                                                     , V & b
                                                     , typename V::real_type & min_distance
                                                     )
  {
    typedef typename V::value_traits VT;
    typedef typename V::real_type     T;

    unsigned int const edge[6][2] = {
      {0,  1}
      , {0,  2}
      , {0,  3}
      , {1,  2}
      , {1,  3}
      , {2,  3}
    };


    std::vector<Triangle<V> >                trianglesA(4u);
    std::vector<Triangle<V> >                trianglesB(4u);
    std::vector<details::UnscaledPlane<V> >  planesA(4u);
    std::vector<details::UnscaledPlane<V> >  planesB(4u);

    //--- Pre-computation ---- -------------------------------------------------
    for (unsigned int v =0u; v < 4u; ++v)
    {
      trianglesA[v] = get_opposite_face( v, A );
      trianglesB[v] = get_opposite_face( v, B );
      planesA[v]    = details::make_unscaled_plane(trianglesA[v]);
      planesB[v]    = details::make_unscaled_plane(trianglesB[v]);
    }

    min_distance = VT::infinity();

    //--- Search for V-V cases -------------------------------------------------

    for (unsigned int i =0u; i < 4u; ++i)
    {
      V const & ai = A.p(i);

      for (unsigned int j =0u; j < 4u; ++j)
      {
        V const & bj = B.p(j);

        T const distance = tiny::norm(ai-bj);

        a            = (distance < min_distance) ? ai : a;
        b            = (distance < min_distance) ? bj : b;
        min_distance =  distance < min_distance  ? distance : min_distance;
      }
    }

    //--- Search for V-E cases -------------------------------------------------
    for (unsigned int i =0u; i < 4u; ++i)
    {
      // Test vertex of A against edge from B
      for (unsigned int k =0u; k < 6u; ++k)
      {
        V const & p_a  = A.p(i);
        V const & p_b0 = B.p(edge[k][0]);
        V const & p_b1 = B.p(edge[k][1]);

        V const da    = p_a  - p_b0;
        V const db    = p_b1 - p_b0;

        T const t = (tiny::inner_prod(da, db)/tiny::inner_prod(db, db));

        if (t <= VT::zero())
          continue;

        if (t >= VT::one())
          continue;

        V const p_b      = p_b0 + t * db;
        V const ortho    = p_a - p_b;
        T const distance = tiny::norm(ortho);

        a            = (distance < min_distance) ? p_a      : a;
        b            = (distance < min_distance) ? p_b      : b;
        min_distance =  distance < min_distance  ? distance : min_distance;
      }

      // Test V of B against E of A
      for (unsigned int k =0u; k < 6u; ++k)
      {
        V const & p_b  = B.p(i);
        V const & p_a0 = A.p(edge[k][0]);
        V const & p_a1 = A.p(edge[k][1]);

        V const db    = p_b  - p_a0;
        V const da    = p_a1 - p_a0;

        T const t = (tiny::inner_prod(da, db)/tiny::inner_prod(da, da));

        if (t <= VT::zero())
          continue;

        if (t >= VT::one())
          continue;

        V const p_a      = p_a0 + t * da;
        V const ortho    = p_a - p_b;
        T const distance = tiny::norm(ortho);

        a            = (distance < min_distance) ? p_a      : a;
        b            = (distance < min_distance) ? p_b      : b;
        min_distance =  distance < min_distance  ? distance : min_distance;
      }

    }

    //--- Search for E-E cases -------------------------------------------------
    for (unsigned int k = 0u; k < 6u; ++k)
    {
      V const & ai = A.p(edge[k][0u]);
      V const & aj = A.p(edge[k][1u]);
      V const   da = tiny::unit(aj - ai);

      for (unsigned int m = 0u; m < 6u; ++m)
      {
        V const & bi = B.p(edge[m][0u]);
        V const & bj = B.p(edge[m][1u]);
        V const   db = tiny::unit(bj - bi);

        V const r  =   bi - ai;
        T const k  =   tiny::inner_prod(da, db);
        T const q1 =   tiny::inner_prod(da,r);
        T const q2 = - tiny::inner_prod(db,r);
        T const w  =   VT::one() - k*k;

        // Test if edges too close to parallel
        if(fabs(w) < tiny::working_precision<T>())
          continue;

        // Compute edge-parameters corresponding to closest points
        T const t = (q1 + k*q2)/w;
        T const s = (q2 + k*q1)/w;

        // Test if closest points are interior on edges
        if (t<= VT::zero())
          continue;

        if (s<= VT::zero())
          continue;

        if (t>= tiny::norm(aj - ai))
          continue;

        if (s>= tiny::norm(bj - bi))
          continue;

        V const p_a      = ai + t*da;
        V const p_b      = bi + s*db;
        T const distance = tiny::norm(p_a - p_b);

        a            = (distance < min_distance) ? p_a      : a;
        b            = (distance < min_distance) ? p_b      : b;
        min_distance =  distance < min_distance  ? distance : min_distance;
      }
    }

    //--- Search for V-F cases -------------------------------------------------
    for (unsigned int i = 0u; i < 4u; ++i)
    {
      // Vs of A against triangles of B
      for (unsigned int k = 0u; k < 4u; ++k)
      {
        V const p_a = A.p(i);

        bool inside_voronoi_planes = true;

        V const & n_b  = planesB[k].m_normal;

        for(unsigned int m = 0u; m < 3u;++m)
        {
          unsigned int const n = (m + 1) % 3;

          V const & p_b0 = trianglesB[k].p(m);
          V const & p_b1 = trianglesB[k].p(n);
          V const   e_b  = p_b1 - p_b0;

          V const vp_n  = tiny::cross(n_b, e_b);
          T const tst  = tiny::inner_prod( p_a - p_b0, vp_n);

          inside_voronoi_planes = tst < VT::zero() ? false : inside_voronoi_planes;
        }

        if (!inside_voronoi_planes)
          continue;

        V const & q_b      = planesB[k].m_point;
        V const   p_b      =  p_a -  n_b * tiny::inner_prod(n_b, p_a - q_b)/tiny::inner_prod(n_b, n_b);
        T const   distance = tiny::inner_prod( p_a - q_b, n_b)/ tiny::norm( n_b );

        if(distance < VT::zero() )
          continue;

        a            = (distance < min_distance) ? p_a   : a;
        b            = (distance < min_distance) ? p_b   : b;
        min_distance =  distance < min_distance  ? distance : min_distance;
      }

      // Vs of B against triangles of A
      for (unsigned int k = 0u; k < 4u; ++k)
      {
        V const p_b = B.p(i);

        bool inside_voronoi_planes = true;

        V const & n_a  = planesA[k].m_normal;

        for(unsigned int m = 0u; m < 3u;++m)
        {
          unsigned int const n = (m + 1) % 3;

          V const & p_a0 = trianglesA[k].p(m);
          V const & p_a1 = trianglesA[k].p(n);
          V const   e_a  = p_a1 - p_a0;

          V const vp_n  = tiny::cross(n_a, e_a);
          T const tst  = tiny::inner_prod( p_b - p_a0, vp_n);

          inside_voronoi_planes = tst < VT::zero() ? false : inside_voronoi_planes;
        }

        if (!inside_voronoi_planes)
          continue;

        V const & q_a      = planesA[k].m_point;
        V const   p_a      =  p_b -  n_a * tiny::inner_prod(n_a, p_b - q_a)/tiny::inner_prod(n_a, n_a);
        T const   distance = tiny::inner_prod( p_b - q_a, n_a)/ tiny::norm( n_a );

        if(distance < VT::zero() )
          continue;

        a            = (distance < min_distance) ? p_a   : a;
        b            = (distance < min_distance) ? p_b   : b;
        min_distance =  distance < min_distance  ? distance : min_distance;
      }
    }
  }

}// end namespace geometry

// GEOMETRY_CLOSEST_POINTS_TETRAHEDRON_TETRAHEDRON_H
#endif
