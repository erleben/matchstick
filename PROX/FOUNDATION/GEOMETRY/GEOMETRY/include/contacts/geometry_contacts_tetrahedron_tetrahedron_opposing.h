#ifndef GEOMETRY_CONTACTS_TETRAHEDRON_TETRAHEDRON_OPPOSING_H
#define GEOMETRY_CONTACTS_TETRAHEDRON_TETRAHEDRON_OPPOSING_H

#include <contacts/geometry_contacts_callback.h>

#include <types/geometry_tetrahedron.h>
#include <types/geometry_triangle.h>

#include <closest_points/geometry_closest_points_line_line.h>

#include <overlap/geometry_overlap_triangle_triangle.h>

#include <tiny_precision.h>        // needed for tiny::working_precision

#include <cmath>                   // needed for std::min and std::max
#include <vector>

namespace geometry
{
  namespace details
  {
    template<typename V>
    class UnscaledPlane
    {
    public:

      V m_normal;    ///< The plane normal NOT necessary a unit-normal
      V m_point;     ///< A point on the plane (any point)

    };

    template<typename V>
    inline UnscaledPlane<V> make_unscaled_plane(V const & p0,V const & p1,V const & p2)
    {
      UnscaledPlane<V> plane;

      typedef typename V::real_type T;

      V const m0 = cross(p1-p0, p2-p0);
      V const m1 = cross(p2-p1, p0-p1);
      V const m2 = cross(p0-p2, p1-p2);

      T const l0 = inner_prod(m0,m0);
      T const l1 = inner_prod(m1,m1);
      T const l2 = inner_prod(m2,m2);

      if (l0 >= l1 && l0 >= l2)
      {
        plane.m_normal = m0;
        plane.m_point  = p0;
      }
      if (l1 >= l0 && l1 >= l2)
      {
        plane.m_normal = m1;
        plane.m_point  = p1;
      }
      if (l2 >= l1 && l2 >= l0)
      {
        plane.m_normal = m2;
        plane.m_point  = p2;
      }

      return plane;
    }

    template<typename V>
    inline UnscaledPlane<V> make_unscaled_plane(Triangle<V> const & triangle)
    {
      return make_unscaled_plane(triangle.p(0),triangle.p(1),triangle.p(2));
    }

    template<typename V>
    inline UnscaledPlane<V> make_unscaled_plane(V const & normal, V const & point)
    {
      UnscaledPlane<V> plane;

      plane.m_normal = normal;
      plane.m_point  = point;

      return plane;
    }

    template<typename V>
    inline typename V::real_type get_signed_distance( V const & q, UnscaledPlane<V> const & plane)
    {
      return inner_prod( plane.m_normal, q - plane.m_point);
    }

    template<typename V>
    inline bool inside_planes(V const & q, std::vector<UnscaledPlane<V> > const & planes)
    {
      assert(planes.size() == 4u || !"inside_planes(): internal error");

      typedef typename V::value_traits  VT;
      typedef typename V::real_type     T;

      T const d0 = get_signed_distance( q, planes[0] );
      T const d1 = get_signed_distance( q, planes[1] );
      T const d2 = get_signed_distance( q, planes[2] );
      T const d3 = get_signed_distance( q, planes[3] );

      if (d0 > VT::zero())
        return false;
      if (d1 > VT::zero())
        return false;
      if (d2 > VT::zero())
        return false;
      if (d3 > VT::zero())
        return false;

      return true;
    }

    template<typename V>
    inline V make_intersection(V const & a, V const & b, UnscaledPlane<V> const & plane)
    {
      using std::fabs;

      typedef typename V::real_type     T;

      T const dA = fabs( get_signed_distance( a, plane )  );
      T const dB = fabs( get_signed_distance( b, plane )  );
      T const t = dA / (dA+dB);
      V const q = a + (b-a)*t;

      return q;
    }

    template<typename V>
    inline typename V::real_type make_intersection_parameter(V const & a, V const & b, UnscaledPlane<V> const & plane)
    {
      using std::fabs;

      typedef typename V::real_type     T;

      T const dA = fabs( get_signed_distance( a, plane )  );
      T const dB = fabs( get_signed_distance( b, plane )  );
      T const t = dA / (dA+dB);

      return t;
    }

    template<typename V>
    inline bool is_crossing_plane(V const & a, V const & b, UnscaledPlane<V> const & plane)
    {
      typedef typename V::value_traits  VT;
      typedef typename V::real_type     T;

      T const dA = get_signed_distance( a, plane );
      T const dB = get_signed_distance( b, plane );

      if( dA >= VT::zero() && dB >= VT::zero() )
        return false;

      if( dA <= VT::zero() && dB <= VT::zero() )
        return false;

      return true;
    }

    /**
     * This method determines the contact normal to be the normal-direction 
     * that are defined by the two most opposing surfaces.
     */
    template< typename V>
    inline bool pick_most_opposing_surface_normal(
                            Tetrahedron<V> const & tetA
                            , Tetrahedron<V> const & tetB
                            , std::vector<bool> const & surface_A
                            , std::vector<bool> const & surface_B
                            , V & n
                            )
    {
      using std::min;
      using std::max;

      typedef typename V::value_traits VT;
      typedef typename V::real_type     T;

      bool found_normal = false;

      T best_fit = VT::zero();

      for(unsigned int a = 0u; a < 4u; ++a)
      {
        if (!surface_A[a])
          continue;

        Triangle<V>               const triangleA = get_opposite_face(a, tetA);
        details::UnscaledPlane<V> const planA     = details::make_unscaled_plane(triangleA);

        V const nA = unit(planA.m_normal);

        for(unsigned int b = 0u; b < 4u; ++b)
        {
          if (!surface_B[b])
            continue;

          Triangle<V>               const triangleB = get_opposite_face(b, tetB);
          details::UnscaledPlane<V> const planB     = details::make_unscaled_plane(triangleB);

          V const nB = unit(planB.m_normal);

          T const test = tiny::inner_prod(nA, nB);

          if (test >= VT::zero() ) // surfaces must be opposing each other
            continue;

          //--- Now we know that planes are opposing each other
          //--- Next we will test if A is in front of B face and vice versa

          V const & pB = tetB.p(b);
          V const & pA = tetA.p(a);

          T const testA =  tiny::inner_prod( planA.m_normal, pB - planA.m_point) ;
          T const testB =  tiny::inner_prod( planB.m_normal, pA - planB.m_point) ;

          if (testA < VT::zero() && testB < VT::zero())
            continue;

          if( test < best_fit)
          {
            n = nA;
            best_fit = test;
            found_normal = true;
          }

        }
      }
      return found_normal;
    }

    /**
     *  The new friction direction finder (Should probably be located somewhere else)
     *  Currently it is just one vector thus, the interpolation is just s
     * @tparam V
     * @param p - point of intrest
     * @param s - direction
     * @return resultant s vector
     */
    template<typename V>
    inline V interpolate( V const & p, std::vector<V> const & s_map, Tetrahedron<V> const & A)
    {
      typedef typename V::real_type T;
      typedef typename V::value_traits VT;
      T w0 = VT::zero();
      T w1 = VT::zero();
      T w2 = VT::zero();
      T w3 = VT::zero();
      geometry::barycentric(A.p(0), A.p(1), A.p(2), A.p(3), p, w0,w1,w2,w3);
      V const s = w0*s_map[0]+w1*s_map[1]+w2*s_map[2]+w3*s_map[3];
      return s;
    }

    /**
     * This sub-routine just generates all possible intersection points
     * between the two tetrahedra and then projects them onto the contact
     * plane using the contact normal information. Finally, it computes
     * penetration depth based on the given normal direction and filters 
     * away any redundant contact points before reporting them with the
     * callback function.
     */
    template< typename V>
    inline bool generate_contacts_from_intersection(
                                                    Tetrahedron<V> const & A
                                                    , Tetrahedron<V> const & B
                                                    , ContactsCallback<V> & callback
                                                    , std::vector<V> const & structure_map_A
                                                    , std::vector<V> const & structure_map_B
                                                    , V const & n
                                                    )
    {
      using std::min;
      using std::max;

      typedef typename V::value_traits VT;
      typedef typename V::real_type     T;

      std::vector<V> contacts;
      contacts.reserve(16);

      std::vector<Triangle<V> >                trianglesA(4u);
      std::vector<Triangle<V> >                trianglesB(4u);

      std::vector<details::UnscaledPlane<V> >  planesA(4u);
      std::vector<details::UnscaledPlane<V> >  planesB(4u);

      for (unsigned int v =0u; v < 4u; ++v)
      {
        trianglesA[v] = get_opposite_face( v, A );
        trianglesB[v] = get_opposite_face( v, B );
        planesA[v]    = details::make_unscaled_plane(trianglesA[v]);
        planesB[v]    = details::make_unscaled_plane(trianglesB[v]);
      }

      for (unsigned int v =0u; v < 4u; ++v)
      {
        if( details::inside_planes( B.p(v), planesA) )
          contacts.push_back(B.p(v));

        if( details::inside_planes( A.p(v), planesB) )
          contacts.push_back(A.p(v));
      }

      unsigned int edge_table[6][2] = {
        {0, 1}
        , {0, 2}
        , {0, 3}
        , {1, 2}
        , {1, 3}
        , {2, 3}
      };

      for (unsigned int e = 0u; e < 6u; ++e)
      {
        unsigned int const i = edge_table[e][0u];
        unsigned int const j = edge_table[e][1u];

        V const & Ai = A.p(i);
        V const & Aj = A.p(j);
        V const & Bi = B.p(i);
        V const & Bj = B.p(j);

        for (unsigned int p = 0u; p < 4u; ++p)
        {
          details::UnscaledPlane<V> const & planeA = planesA[p];
          details::UnscaledPlane<V> const & planeB = planesB[p];

          if (details::is_crossing_plane(Bi, Bj, planeA) )
          {
            V const qB = details::make_intersection(Bi, Bj, planeA);

            if( details::inside_planes(qB, planesA) )
              contacts.push_back(qB);
          }

          if (details::is_crossing_plane(Ai, Aj, planeB) )
          {
            V const qA = details::make_intersection( Ai, Aj, planeB);

            if( details::inside_planes(qA, planesB) )
              contacts.push_back(qA);
          }
        }
      }

      if(contacts.empty())
      {
        return false;
      }

      // Now we know we have found a bunch of contacts, we will now try to find
      // the best "contact plane" and project all contact points onto that
      // plane and compute penetration measures with respect to that plane too.
      T min_val = VT::highest();
      T max_val = VT::lowest();

      {
        for( typename std::vector<V>::iterator p = contacts.begin(); p!= contacts.end(); ++p)
        {
          T const d = inner_prod( (*p), n );

          min_val = min( min_val, d );
          max_val = max( max_val, d );
        }
      }

      T const depth = min_val - max_val;

      // Project contact points onto cotact plane
      V const mid =  n * (max_val + min_val)*VT::half();

      for( typename std::vector<V>::iterator p = contacts.begin(); p!= contacts.end(); ++p)
      {
        (*p) = (*p) - inner_prod( n, ( (*p) - mid ) ) * n;
      }

      // Some contacts might have been projected to the same point in the
      // contact plane, so we filter away redundant information before
      // using the callback to report the computed contact point.
      typename std::vector<V>::iterator p    = contacts.begin();
      for(; p!= contacts.end(); ++p)
      {
        bool unique = true;
        
        typename std::vector<V>::iterator q = contacts.begin();
        for(; q != p; ++q)
        {
          if( tiny::norm_1( (*q) - (*p) ) < tiny::working_precision<T>())
          {
            unique = false;
            break;
          }
        }
        
        if(unique)
        {
          V const Sa = interpolate( (*p),structure_map_A, A);
          V const Sb = interpolate( (*p),structure_map_B, B);
          callback( (*p), n, depth, Sa, Sb);
        }
      }
      
      return true;
    }

  }// end of namespace details

  /**
   * Algorithm type tags for calling the appropriate contact point generation algorithm that is desired.
   */
  struct MOST_OPPOSING_SURFACES {};

  template< typename V>
  inline bool contacts_tetrahedron_tetrahedron(
                                               Tetrahedron<V> const & A
                                               , Tetrahedron<V> const & B
                                               , ContactsCallback<V> & callback
                                               , std::vector<bool> const & surface_A
                                               , std::vector<bool> const & surface_B
                                               , std::vector<V> const & structure_map_A
                                               , std::vector<V> const & structure_map_B
                                               , MOST_OPPOSING_SURFACES const & /*algorithm_tag*/
  )
  {
    assert(surface_A.size() == 4u                                          || !"contacts_tetrahedron_tetrahedron(): internal error, must have four surface map values");
    assert( (surface_A[0] || surface_A[1] || surface_A[2] || surface_A[3]) || !"contacts_tetrahedron_tetrahedron(): internal error, tetrahedron A must have at least one surface face");
    assert(surface_B.size() == 4u                                          || !"contacts_tetrahedron_tetrahedron(): internal error, must have four surface map values");
    assert( (surface_B[0] || surface_B[1] || surface_B[2] || surface_B[3]) || !"contacts_tetrahedron_tetrahedron(): internal error, tetrahedron B must have at least one surface face");

    using std::min;
    using std::max;

    bool const overlap = overlap_tetrahedron_tetrahedron(A, B);
    if(!overlap)
      return false;

    V n; // The contact normal to be used

    bool const found_normal = details::pick_most_opposing_surface_normal(A,B,surface_A,surface_B, n);

    if(!found_normal)
      return false;

    return details::generate_contacts_from_intersection(A, B, callback, structure_map_A, structure_map_B,n);
  }

}// end namespace geometry

// GEOMETRY_CONTACTS_TETRAHEDRON_TETRAHEDRON_OPPOSING_H
#endif
