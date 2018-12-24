#ifndef GEOMETRY_GAUSS_MAP_OF_CONVEX_POLYHEDRA_H
#define GEOMETRY_GAUSS_MAP_OF_CONVEX_POLYHEDRA_H

#include <types/geometry_tetrahedron.h>

#include <tiny.h>

#include <cmath>
#include <vector>
#include <map>
#include <cassert>

namespace geometry
{
  namespace details
  {

    class EdgeLookup
    {
    protected:

      std::vector< std::map<unsigned int, unsigned int> >  m_data;  ///< maps edge (i,j) to a linear index k

      unsigned int m_unique_edges;

    public:

      EdgeLookup(unsigned int const & N)
      : m_data(N)
      , m_unique_edges(0u)
      {
      }

    public:

      unsigned int size() const { return m_unique_edges; }

      void create( unsigned int const & i, unsigned int const & j)
      {
        using std::min;
        using std::max;

        unsigned int const & s = min(i,j);
        unsigned int const & t = max(i,j);

        std::map<unsigned int, unsigned int>::const_iterator lookup = m_data[s].find(t);

        if( lookup == m_data[s].end() )
        {
          m_data[s].insert(
                           std::pair<unsigned int, unsigned int>(t, m_unique_edges)
                           );

          ++m_unique_edges;
        }
      }

      unsigned int get_index( unsigned int const & i, unsigned int const & j) const
      {
        using std::min;
        using std::max;

        unsigned int const & s = min(i,j);
        unsigned int const & t = max(i,j);
        
        return  m_data[s].at(t);
      }
    };

  }// end namespace details


  template<typename V>
  class GaussMapOfConvexPolyhedra
  {
  public:

    typedef typename V::real_type       T;
    typedef typename V::value_traits    VT;
    
  protected:

    class MapOfFace
    {
    public:

      std::vector<unsigned int>   m_point_indices;
    };

    class MapOfEdge
    {
    public:

      unsigned int m_point0_idx;
      unsigned int m_point1_idx;
      unsigned int m_left_face_idx;
      unsigned int m_right_face_idx;

    };

    class MapOfVertex
    {
    public:
      
      std::vector<unsigned int> m_edge_indices;
      
    };

  protected:

    std::vector<V>            m_normals;
    std::vector<V>            m_points;

    std::vector<MapOfFace>    m_faces;
    std::vector<MapOfEdge>    m_edges;
    std::vector<MapOfVertex>  m_vertices;

    T                         m_tolerance;   ///< Tolerance specified allowed inaccuracy
                                             ///< in terms of "angle". This is represented
                                             ///< as the cosine of the allowed angle error
                                             ///< in determning the proper "feature".

  protected:

    bool inside_map_of_face(V const & n, unsigned int const & face_idx) const
    {
      using std::min;
      using std::max;
      using std::fabs;

      assert(face_idx < this->m_faces.size() || !"inside_map_of_face(): face index was out of bounds");

      V const & m  = this->m_normals[face_idx];

      assert(  fabs( VT::one() - tiny::norm(n) ) < tiny::working_precision<T>() || !"inside_map_of_face(): n was not unit vector" );
      assert(  fabs( VT::one() - tiny::norm(m) ) < tiny::working_precision<T>() || !"inside_map_of_face(): m was not unit vector" );

      T const   dot      = tiny::inner_prod(n, m);
      T const   safe_dot = max(-VT::one(), min(VT::one(), dot ) );

      return (safe_dot >= this->m_tolerance);
    }

    bool inside_map_of_edge(V const & n, unsigned int const & edge_idx) const
    {
      using std::min;
      using std::max;
      using std::fabs;

      assert(edge_idx < this->m_edges.size() || !"inside_map_of_edge(): edge index was out of bounds");

      unsigned int const & i0  = this->m_edges[edge_idx].m_point0_idx;
      unsigned int const & i1  = this->m_edges[edge_idx].m_point1_idx;
      unsigned int const & lf  = this->m_edges[edge_idx].m_left_face_idx;
      unsigned int const & rf  = this->m_edges[edge_idx].m_right_face_idx;

      V const & p0  = this->m_points[i0];
      V const & p1  = this->m_points[i1];
      V const & l   = this->m_normals[ lf ];
      V const & r   = this->m_normals[ rf ];

      assert(  fabs( VT::one() - tiny::norm(n) ) < tiny::working_precision<T>() || !"inside_map_of_edge(): n was not unit vector" );
      assert(  fabs( VT::one() - tiny::norm(l) ) < tiny::working_precision<T>() || !"inside_map_of_edge(): l was not unit vector" );
      assert(  fabs( VT::one() - tiny::norm(r) ) < tiny::working_precision<T>() || !"inside_map_of_edge(): r was not unit vector" );

      V const d        = p1 - p0;
      V const n0       = n - (tiny::inner_prod(n,d)*d / tiny::inner_prod(d,d));

      assert( is_finite(n0(0)) || !"inside_map_of_edge(): inf ");
      assert( is_number(n0(0)) || !"inside_map_of_edge(): nan ");
      assert( is_finite(n0(1)) || !"inside_map_of_edge(): inf ");
      assert( is_number(n0(1)) || !"inside_map_of_edge(): nan ");
      assert( is_finite(n0(2)) || !"inside_map_of_edge(): inf ");
      assert( is_number(n0(2)) || !"inside_map_of_edge(): nan ");

      // Test if projected normal is too close to orign (means normal
      // is no-where close to the gauss map of the edge)
      if ( tiny::inner_prod(n0,n0) < tiny::working_precision<T>() )
        return false;

      V const n1       = tiny::unit(n0);
      T const dot      = tiny::inner_prod(n, n1);

      assert( is_finite(dot) || !"inside_map_of_edge(): inf ");
      assert( is_number(dot) || !"inside_map_of_edge(): nan ");

      T const safe_dot = max(-VT::one(), min(VT::one(), dot ) );

      if (safe_dot < this->m_tolerance)
        return false;

      // Determine if arc angle is acute or reflex angle

      T const space_product =  tiny::inner_prod( d, tiny::cross(l,r) );

      assert( is_finite(space_product) || !"inside_map_of_edge(): inf ");
      assert( is_number(space_product) || !"inside_map_of_edge(): nan ");

      bool const accute = space_product > VT::zero();

      if(accute)
      {
        T const space_product_l  = tiny::inner_prod( d, tiny::cross(l,n) );
        T const space_product_r  = tiny::inner_prod( d, tiny::cross(n,r) );

        assert( is_finite(space_product_l) || !"inside_map_of_edge(): inf ");
        assert( is_number(space_product_l) || !"inside_map_of_edge(): nan ");
        assert( is_finite(space_product_r) || !"inside_map_of_edge(): inf ");
        assert( is_number(space_product_r) || !"inside_map_of_edge(): nan ");

        bool const inside = space_product_r >= VT::zero() && space_product_l>= VT::zero();

        return inside;
      }
      else
      {
        T const space_product_r  = tiny::inner_prod( d, tiny::cross(r,n) );
        T const space_product_l  = tiny::inner_prod( d, tiny::cross(n,l) );

        assert( is_finite(space_product_l) || !"inside_map_of_edge(): inf ");
        assert( is_number(space_product_l) || !"inside_map_of_edge(): nan ");
        assert( is_finite(space_product_r) || !"inside_map_of_edge(): inf ");
        assert( is_number(space_product_r) || !"inside_map_of_edge(): nan ");

        bool const outside = space_product_r > VT::zero() && space_product_l > VT::zero();

        return ! outside;
      }

      return false;
    }

    bool inside_map_of_vertex(V const & n, unsigned int const & vertex_idx) const
    {
      assert( vertex_idx < this->m_vertices.size() || !"inside_map_of_vertex(): vertex_idx was out of bounds");

      MapOfVertex const & vertex = this->m_vertices[vertex_idx];

      unsigned int const N  = vertex.m_edge_indices.size();

      for (unsigned int i = 0u; i < N; ++i)
      {
        unsigned int  const   edge_idx = vertex.m_edge_indices[i];

        assert( edge_idx < this->m_edges.size() || !"inside_map_of_vertex(): edge_idx was out of bounds");

        MapOfEdge const & edge     = this->m_edges[edge_idx];

        bool should_flip_edge = edge.m_point1_idx == vertex_idx;

        unsigned int const & i0 = should_flip_edge ? edge.m_point1_idx     : edge.m_point0_idx;
        unsigned int const & i1 = should_flip_edge ? edge.m_point0_idx     : edge.m_point1_idx;
        unsigned int const & lf = should_flip_edge ? edge.m_right_face_idx : edge.m_left_face_idx;
        unsigned int const & rf = should_flip_edge ? edge.m_left_face_idx  : edge.m_right_face_idx;

        assert( i0 < this->m_vertices.size() || !"inside_map_of_vertex(): i0 was out of bounds");
        assert( i1 < this->m_vertices.size() || !"inside_map_of_vertex(): i1 was out of bounds");
        assert( lf < this->m_faces.size()    || !"inside_map_of_vertex(): lf was out of bounds");
        assert( rf < this->m_faces.size()    || !"inside_map_of_vertex(): rf was out of bounds");

        V const & p0  = this->m_points[i0];
        V const & p1  = this->m_points[i1];
        V const & l   = this->m_normals[ lf ];
        V const & r   = this->m_normals[ rf ];

        assert(  fabs( VT::one() - tiny::norm(n) ) < tiny::working_precision<T>() || !"inside_map_of_vertex(): n was not unit vector" );
        assert(  fabs( VT::one() - tiny::norm(l) ) < tiny::working_precision<T>() || !"inside_map_of_vertex(): l was not unit vector" );
        assert(  fabs( VT::one() - tiny::norm(r) ) < tiny::working_precision<T>() || !"inside_map_of_vertex(): r was not unit vector" );

        V const d = p1 - p0;

        T const space_product =  tiny::inner_prod( d, tiny::cross(l,r) );

        assert( is_finite(space_product) || !"inside_map_of_vertex(): inf ");
        assert( is_number(space_product) || !"inside_map_of_vertex(): nan ");

        bool const accute = space_product > VT::zero();

        if(accute)
        {
          // l and r forms an acute angle around d
          T const product = tiny::inner_prod(n, tiny::cross(  r, l ) );

          bool const inside_arc = product > VT::zero();

          if (!inside_arc)
            return false;
        }
        else
        {
          // l and r forms a reflex angle around d
          T const product =    tiny::inner_prod(n, tiny::cross(  l, r ) );
          
          bool const inside_arc = product > VT::zero();
          
          if (!inside_arc)
            return false;
        }
      }
      return true;
    }
    
  public:

    /**
     * Set Tolerance using units of radians.
     * Tolerance is defind to be cos(angle) where "angle" is allowed cone angle error.
     *
     *
     * @param  angle    The allowed angle error given in radians.
     */
    void set_tolerance_in_radians( T const & angle )
    {
      using std::cos;

      assert( is_finite(angle)   || !"set_tolerance(): nan");
      assert( is_number(angle)   || !"set_tolerance(): nan");
      assert(angle >= VT::zero() || !"set_tolerance(): Illegal tolerance");
      assert(angle <  VT::pi()   || !"set_tolerance(): Illegal tolerance");

      this->m_tolerance = cos(angle);
    }

    /**
     * Set Tolerance using units of degrees.
     * Tolerance is defind to be cos(angle) where "angle" is allowed cone angle error.
     *
     *
     * @param  angle    The allowed angle error given in degrees.
     */
    void set_tolerance_in_degrees( T const & angle)
    {
      this->set_tolerance_in_radians( VT::convert_to_radians(angle) );
    }

    /**
     * Tolerance is defind to be cos(angle) where "angle" is allowed cone angle error.
     */
    T const & tolerance() const { return this->m_tolerance; }

    /**
     * Search Gauss-Map for the geometric feature (face, edge or vertex)
     * whos "normal cone" contains the specified direction vector s.
     *
     * The search uses a tolerance to "fatten" the normal cones. This is
     * to make it more robust. Precedence is given to higher dimensional
     * features.
     *
     * @param s          A unit direction vector
     * @param points     The points that define the geometric feature. If a
     *                   vertex only one points, if and edge then two points,
     *                   if a face then at least 3 points.
     * @param n          A "best" normal vector for the found feature (this is
     *                   likely to be close to s, but it could be slightly
     *                   different if a large tolerance is used.
     *
     * @return           If a feature is found then return values is true otherwise
     *                   it is false.
     */
    bool search_for_feature(V const & s, std::vector<V> & points, V & n ) const
    {
      for(unsigned int idx = 0u; idx < this->m_faces.size(); ++idx)
      {
        if( inside_map_of_face(s, idx) )
        {
          MapOfFace const & face = this->m_faces[idx];

          unsigned int const N = face.m_point_indices.size();

          points.resize(N);

          for(unsigned int k = 0u; k < N; ++k)
          {
            points[k] = this->m_points[ face.m_point_indices[k] ];
          }

          n = this->m_normals[idx];
          return true;
        }
      }
      for(unsigned int idx = 0u; idx < this->m_edges.size(); ++idx)
      {
        if( inside_map_of_edge(s, idx) )
        {
          MapOfEdge const & edge = this->m_edges[idx];

          points.resize(2);
          points[0] = this->m_points[edge.m_point0_idx];
          points[1] = this->m_points[edge.m_point1_idx];
          n         = s;
          return true;
        }
      }
      for(unsigned int idx = 0u; idx < this->m_vertices.size(); ++idx)
      {
        if( inside_map_of_vertex(s, idx) )
        {
          points.resize(1);
          points[0] = this->m_points[idx];
          n         = s;
          return true;
        }
      }
      return false;
    }

    /**
     * Create Gauss-Map.
     * It is implicilty assumed that user has specified a convex polyhedra
     * with no open holes. No testing is done to verify if input data is correct.
     *
     * @param   points     The coordinates of all the vertices.
     * @params  normals    Unit outward pointing normals of the faces.
     * @param   faces      A face-index array. The k'th element
     *                     contains an array of the vertex indices in
     *                     CCW order that makes up the k'th face.
     */
    void create(
                std::vector<V> const & points
                , std::vector<V> const & normals
                , std::vector<  std::vector< unsigned int> > const & faces
                )
    {
      assert( normals.size() == faces.size()   || !"create(): illegal input");
      assert( normals.size() >= 3u             || !"create(): not enough faces/normals to define a positive volume polyhedra");
      assert( points.size()  >= 4u             || !"create(): not enough points to define a positive volume polyhedra");

      this->m_points  = points;
      this->m_normals = normals;

      this->m_vertices.resize( points.size() );
      this->m_faces.resize( normals.size() );

      // Create face maps
      unsigned int const N = faces.size();
      for (unsigned int n=0u; n<N; ++n)
      {
        unsigned int const K = faces[n].size();

        this->m_faces[n].m_point_indices.resize(K);

        for (unsigned int k=0u; k<K; ++k)
        {
          this->m_faces[n].m_point_indices[k] = faces[n][k];
        }
      }

      // Allocate space for edge maps
      details::EdgeLookup edge_lookup( points.size() );

      {
        for (unsigned int n=0u; n<N; ++n)
        {
          unsigned int const K = faces[n].size();
          for (unsigned int k=0u; k<K; ++k)
          {
            unsigned int const i = faces[n][k];
            unsigned int const j = faces[n][(k+1u) % K];
            edge_lookup.create(i,j);
          }
        }
      }

      this->m_edges.resize( edge_lookup.size() );

      // Create edge maps
      for (unsigned int n=0u; n<N; ++n)
      {
        unsigned int const K = faces[n].size();

        for (unsigned int k=0u; k<K; ++k)
        {
          unsigned int const i = faces[n][k];
          unsigned int const j = faces[n][(k+1u) % K];
          unsigned int const m = edge_lookup.get_index(i,j);

          if( i < j)
          {
            this->m_edges[m].m_point0_idx = i;
            this->m_edges[m].m_point1_idx = j;
            this->m_edges[m].m_left_face_idx  = n;
          }
          else
          {
            this->m_edges[m].m_point0_idx = j;
            this->m_edges[m].m_point1_idx = i;
            this->m_edges[m].m_right_face_idx = n;
          }
        }
      }

      // Create vertex maps
      unsigned int const M = this->m_edges.size();
      for (unsigned int m = 0u; m<M; ++m)
      {
        MapOfEdge const & edge = this->m_edges[m];

        this->m_vertices[edge.m_point0_idx].m_edge_indices.push_back( m );
        this->m_vertices[edge.m_point1_idx].m_edge_indices.push_back( m );
      }

    }

    /**
     * For some polyhedra only their coordinates change during simulations,
     * but their connectivity is unchanged. In such cases it can be computational
     * advantage to just update a pre-computed Gauss-Map with new geometry
     * information rather than rebuilding all the connectivity information
     * once again.
     */
    void update(
                std::vector<V> const & points
                , std::vector<V> const & normals
                )
    {
      assert( points.size() == this->m_points.size()   || !"update(): gauss map was not created");
      assert( normals.size() == this->m_normals.size() || !"update(): gauss map was not created");

      this->m_points  = points;
      this->m_normals = normals;
    }

  public:

    GaussMapOfConvexPolyhedra()
    {
      this->set_tolerance_in_degrees( VT::four() );
    }

  };


  template<typename V>
  inline GaussMapOfConvexPolyhedra<V> make_gauss_map( Tetrahedron<V> const & A)
  {
    GaussMapOfConvexPolyhedra<V> gauss_map;

    std::vector<V> points(4u);
    std::vector<V> normals(4u);

    std::vector< std::vector< unsigned int > > faces;

    faces.resize(4);
    faces[0].resize(3u);
    faces[1].resize(3u);
    faces[2].resize(3u);
    faces[3].resize(3u);

    faces[0][0]  = 1;
    faces[0][1]  = 2;
    faces[0][2]  = 3;

    faces[1][0]  = 2;
    faces[1][1]  = 0;
    faces[1][2]  = 3;

    faces[2][0]  = 0;
    faces[2][1]  = 1;
    faces[2][2]  = 3;

    faces[3][0]  = 0;
    faces[3][1]  = 2;
    faces[3][2]  = 1;

    points[0] = A.p(0);
    points[1] = A.p(1);
    points[2] = A.p(2);
    points[3] = A.p(3);

    for(unsigned int k= 0u; k< 4;++k)
    {
      V const & p0 = points[  faces[k][0] ];
      V const & p1 = points[  faces[k][1] ];
      V const & p2 = points[  faces[k][2] ];

      normals[k]   = unit( cross( p1-p0, p2-p0  ) );
    }

    gauss_map.create( points, normals, faces );

    return gauss_map;
  }


  template<typename V>
  inline void update_gauss_map( GaussMapOfConvexPolyhedra<V> & gauss_map, Tetrahedron<V> const & A)
  {
    std::vector<V> points(4u);
    std::vector<V> normals(4u);

    std::vector< std::vector< unsigned int > > faces;

    faces.resize(4);
    faces[0].resize(3u);
    faces[1].resize(3u);
    faces[2].resize(3u);
    faces[3].resize(3u);

    faces[0][0]  = 1;
    faces[0][1]  = 2;
    faces[0][2]  = 3;

    faces[1][0]  = 2;
    faces[1][1]  = 0;
    faces[1][2]  = 3;

    faces[2][0]  = 0;
    faces[2][1]  = 1;
    faces[2][2]  = 3;

    faces[3][0]  = 0;
    faces[3][1]  = 2;
    faces[3][2]  = 1;

    points[0] = A.p(0);
    points[1] = A.p(1);
    points[2] = A.p(2);
    points[3] = A.p(3);

    for(unsigned int k= 0u; k< 4;++k)
    {
      V const & p0 = points[  faces[k][0] ];
      V const & p1 = points[  faces[k][1] ];
      V const & p2 = points[  faces[k][2] ];

      normals[k]   = unit( cross( p1-p0, p2-p0  ) );
    }

    gauss_map.update( points, normals );
  }


}// namespace geometry

// GEOMETRY_GAUSS_MAP_OF_CONVEX_POLYHEDRA_H
#endif
