#ifndef GEOMETRY_CONTACTS_TETRAHEDRON_TETRAHEDRON_CLOSEST_POINTS_H
#define GEOMETRY_CONTACTS_TETRAHEDRON_TETRAHEDRON_CLOSEST_POINTS_H

#include <contacts/geometry_contacts_tetrahedron_tetrahedron_opposing.h>  // Needed for UnscaledPlane stuff
#include <types/geometry_tetrahedron.h>
#include <types/geometry_triangle.h>
#include <tiny.h>

#include <vector>
#include <cassert>

namespace geometry
{
  struct CLOSEST_POINTS {};

  template< typename V>
  inline bool contacts_tetrahedron_tetrahedron(
                                               Tetrahedron<V> const & A
                                               , Tetrahedron<V> const & B
                                               , ContactsCallback<V> & callback
                                               , std::vector<bool> const & surface_A
                                               , std::vector<bool> const & surface_B
                                               , std::vector<V> const & structure_map_A
                                               , std::vector<V> const & structure_map_B
                                               , CLOSEST_POINTS const & /*algorithm_tag*/
  )
  {

    using std::min;
    using std::max;

    typedef typename V::real_type    T;
    typedef typename V::value_traits VT;

    T const too_far_away      = VT::numeric_cast(0.02); // 2 times the value of the collision envelope
    T const too_small         = tiny::working_precision<T>();

    unsigned int const edge_to_vertex[6][2] = {
        {0,  1}
      , {0,  2}
      , {0,  3}
      , {1,  2}
      , {1,  3}
      , {2,  3}
    };
    unsigned int const edge_to_face[6][2]   = {
        {2,  3}
      , {3,  1}
      , {1,  2}
      , {0,  3}
      , {2,  0}
      , {0,  1}
    };
    unsigned int const vertex_to_face[4][3] = {
        {1,  2,  3}
      , {0,  3,  2}
      , {0,  1,  3}
      , {0,  2,  1}
    };
//    unsigned int const face_to_vertex[4][3] = {
//        {1,  2,  3}
//      , {0,  3,  2}
//      , {0,  1,  3}
//      , {0,  2,  1}
//    };

    std::vector<bool>                vertex_A_is_surface(4u,false);
    std::vector<bool>                vertex_B_is_surface(4u,false);
    std::vector<bool>                edge_A_is_surface(6u,false);
    std::vector<bool>                edge_B_is_surface(6u,false);

    std::vector<bool>                vertex_A_is_used(4u,false);
    std::vector<bool>                vertex_B_is_used(4u,false);
    std::vector<bool>                edge_A_is_used(6u,false);
    std::vector<bool>                edge_B_is_used(6u,false);

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
    for (unsigned int v =0u; v < 4u; ++v)
    {
      vertex_A_is_surface[v] =  surface_A[ vertex_to_face[v][0u] ]
                             || surface_A[ vertex_to_face[v][1u] ]
                             || surface_A[ vertex_to_face[v][2u] ];

      vertex_B_is_surface[v] =  surface_B[ vertex_to_face[v][0u] ]
                             || surface_B[ vertex_to_face[v][1u] ]
                             || surface_B[ vertex_to_face[v][2u] ];
    }
    for (unsigned int e =0u; e < 6u; ++e)
    {
      edge_A_is_surface[e] =  surface_A[ edge_to_face[e][0u] ]
                           || surface_A[ edge_to_face[e][1u] ];

      edge_B_is_surface[e] =  surface_B[ edge_to_face[e][0u] ]
                           || surface_B[ edge_to_face[e][1u] ];
    }

    unsigned int count = 0u;
    //--- Search for V-F cases -------------------------------------------------
    {
      // Vs of A against triangles of B
      for (unsigned int i = 0u; i < 4u; ++i)
      {
        if(vertex_A_is_used[i])
          continue;

        if(!vertex_A_is_surface[i])
          continue;

        for (unsigned int k = 0u; k < 4u; ++k)
        {
          if(!surface_B[k])
            continue;

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

          if(distance > too_far_away)
            continue;

          if(distance <= VT::zero() )
            continue;

          V const n = - tiny::unit(n_b); // (p_b - p_a) / distance;
          V const p = (p_a + p_b)*VT::half();
          T const depth = too_far_away - distance;


          V const Sa = details::interpolate( p,structure_map_A, A);
          V const Sb = details::interpolate( p,structure_map_B, B);
          callback( p, n, depth, Sa, Sb);

          ++count;

          vertex_A_is_used[i] = true;
        }
      }

      // Vs of B against triangles of A
      for (unsigned int i = 0u; i < 4u; ++i)
      {
        if(vertex_B_is_used[i])
          continue;

        if(!vertex_B_is_surface[i])
          continue;

        for (unsigned int k = 0u; k < 4u; ++k)
        {
          if(!surface_A[k])
            continue;

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

          if(distance > too_far_away)
            continue;

          if(distance <= VT::zero() )
            continue;
          
          V const n = tiny::unit(n_a);  // (p_b - p_a) / distance;
          V const p = (p_a + p_b)*VT::half();
          T const depth = too_far_away - distance;
          

          V const Sa = details::interpolate( p,structure_map_A, A);
          V const Sb = details::interpolate( p,structure_map_B, B);
          callback( p, n, depth, Sa, Sb);

          ++count;
          
          vertex_B_is_used[i] = true;
        }
      }
    }

    //--- Search for E-E cases -------------------------------------------------
    for (unsigned int k = 0u; k < 6u; ++k)
    {
      if(edge_A_is_used[k])
        continue;

      if(!edge_A_is_surface[k])
        continue;

      if(vertex_A_is_used[edge_to_vertex[k][0u]] && vertex_A_is_used[edge_to_vertex[k][1u]])
        continue;


      V const & ai = A.p(edge_to_vertex[k][0u]);
      V const & aj = A.p(edge_to_vertex[k][1u]);
      V const   da = tiny::unit(aj - ai);

      for (unsigned int m = 0u; m < 6u; ++m)
      {
        if(edge_B_is_used[m])
          continue;

        if(!edge_B_is_surface[m])
          continue;

        if(vertex_B_is_used[edge_to_vertex[k][0u]] && vertex_B_is_used[edge_to_vertex[k][1u]])
          continue;

        V const & bi = B.p(edge_to_vertex[m][0u]);
        V const & bj = B.p(edge_to_vertex[m][1u]);
        V const   db = tiny::unit(bj - bi);

        V const r  =   bi - ai;
        T const k  =   tiny::inner_prod(da, db);
        T const q1 =   tiny::inner_prod(da,r);
        T const q2 = - tiny::inner_prod(db,r);
        T const w  =   VT::one() - k*k;

        // Test if edges too close to parallel
        bool const too_parallel = fabs(w) < too_small;

        if( too_parallel )
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

        if(distance > too_far_away)
          continue;

        if(distance == VT::zero() )
          continue;

        V  ndir = tiny::unit( tiny::cross( aj-ai, bj-ai ) );
        
        if( tiny::inner_prod(p_a, ndir) > tiny::inner_prod(p_b, ndir) )
          ndir = -ndir;
        
        V const n = ndir; // (p_b - p_a) / distance;
        V const p = (p_a + p_b)*VT::half();
        T const depth = too_far_away - distance;


        V const Sa = details::interpolate( p,structure_map_A, A);
        V const Sb = details::interpolate( p,structure_map_B, B);
        callback( p, n, depth, Sa, Sb);

        ++count;

        edge_A_is_used[k] = true;
        edge_B_is_used[m] = true;
      }
    }

    //--- Search for E-F cases -------------------------------------------------
    {
      // Edges of A against triangles of B
      for (unsigned int i = 0u; i < 6u; ++i)
      {
        if(edge_A_is_used[i])
          continue;

        if(!edge_A_is_surface[i])
          continue;

        for (unsigned int k = 0u; k < 4u; ++k)
        {
          if(!surface_B[k])
            continue;

          V const p_a0 = A.p( edge_to_vertex[i][0u] );
          V const p_a1 = A.p( edge_to_vertex[i][1u] );

          if (!details::is_crossing_plane(p_a0, p_a1, planesB[k]) )
            continue;

          V const p_a = details::make_intersection(p_a0, p_a1, planesB[k]);

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

          V const & q_b       = planesB[k].m_point;
          T const   distance0 = tiny::inner_prod( p_a0 - q_b, n_b)/ tiny::norm( n_b );
          T const   distance1 = tiny::inner_prod( p_a1 - q_b, n_b)/ tiny::norm( n_b );

          V const n = - n_b / tiny::norm( n_b );
          V const p = p_a;
          T const depth = min( distance0, distance1 );


          V const Sa = details::interpolate( p,structure_map_A, A);
          V const Sb = details::interpolate( p,structure_map_B, B);
          callback( p, n, depth, Sa, Sb);

          ++count;

          edge_A_is_used[i] = true;
        }
      }

      // Edges of B against triangles of A
      for (unsigned int i = 0u; i < 6u; ++i)
      {
        if(vertex_B_is_used[i])
          continue;

        if(!vertex_B_is_surface[i])
          continue;

        for (unsigned int k = 0u; k < 4u; ++k)
        {
          if(!surface_A[k])
            continue;

          V const p_b0 = B.p( edge_to_vertex[i][0u] );
          V const p_b1 = B.p( edge_to_vertex[i][1u] );

          if (!details::is_crossing_plane(p_b0, p_b1, planesA[k]) )
            continue;

          V const p_b = details::make_intersection(p_b0, p_b1, planesA[k]);

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

          V const & q_a       = planesA[k].m_point;
          T const   distance0 = tiny::inner_prod( p_b0 - q_a, n_a)/ tiny::norm( n_a );
          T const   distance1 = tiny::inner_prod( p_b1 - q_a, n_a)/ tiny::norm( n_a );

          V const n = n_a / tiny::norm( n_a );
          V const p = p_b;
          T const depth = min( distance0, distance1 );

          V const Sa = details::interpolate( p,structure_map_A, A);
          V const Sb = details::interpolate( p,structure_map_B, B);
          callback( p, n, depth, Sa, Sb);

          ++count;

          edge_B_is_used[i] = true;
        }
      }
    }

    return count > 0u;
  }

}// end namespace geometry

// GEOMETRY_CONTACTS_TETRAHEDRON_TETRAHEDRON_CLOSEST_POINTS_H
#endif
