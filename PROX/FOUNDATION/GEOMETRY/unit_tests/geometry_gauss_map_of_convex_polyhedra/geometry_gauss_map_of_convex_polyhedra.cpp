#include <geometry.h>
#include <tiny.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

typedef tiny::MathTypes<float> MT;
typedef MT::vector3_type       V;
typedef MT::real_type          T;


BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(gauss_map_tetrahedron_test)
{

  {
    geometry::Tetrahedron<V> A;

    geometry::GaussMapOfConvexPolyhedra<V>  G = geometry::make_gauss_map(A);

    // Search for face features
    {
      V const s = tiny::unit( V::make(0.0 , 0.0, -1.0) );

      V n;
      std::vector<V> points;

      bool found = G.search_for_feature(s, points, n);

      BOOST_CHECK( found );
      BOOST_CHECK_EQUAL(points.size(), 3);

      BOOST_CHECK_CLOSE(points[0][0], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][1], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][2], 0.0, 0.01);

      BOOST_CHECK_CLOSE(points[1][0], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[1][1], 1.0, 0.01);
      BOOST_CHECK_CLOSE(points[1][2], 0.0, 0.01);

      BOOST_CHECK_CLOSE(points[2][0], 1.0, 0.01);
      BOOST_CHECK_CLOSE(points[2][1], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[2][2], 0.0, 0.01);

      BOOST_CHECK_CLOSE(n[0],  0.0, 0.01);
      BOOST_CHECK_CLOSE(n[1],  0.0, 0.01);
      BOOST_CHECK_CLOSE(n[2], -1.0, 0.01);
    }
    {
      V const s = tiny::unit( V::make(-1.0 , 0.0, 0.0) );

      V n;
      std::vector<V> points;

      bool found = G.search_for_feature(s, points, n);

      BOOST_CHECK( found );
      BOOST_CHECK_EQUAL(points.size(), 3);

      BOOST_CHECK_CLOSE(points[0][0], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][1], 1.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][2], 0.0, 0.01);

      BOOST_CHECK_CLOSE(points[1][0], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[1][1], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[1][2], 0.0, 0.01);

      BOOST_CHECK_CLOSE(points[2][0], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[2][1], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[2][2], 1.0, 0.01);

      BOOST_CHECK_CLOSE(n[0], -1.0, 0.01);
      BOOST_CHECK_CLOSE(n[1],  0.0, 0.01);
      BOOST_CHECK_CLOSE(n[2],  0.0, 0.01);
    }
    {
      V const s = tiny::unit( V::make(0.0 , -1.0, 0.0) );

      V n;
      std::vector<V> points;

      bool found = G.search_for_feature(s, points, n);

      BOOST_CHECK( found );
      BOOST_CHECK_EQUAL(points.size(), 3);

      BOOST_CHECK_CLOSE(points[0][0], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][1], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][2], 0.0, 0.01);

      BOOST_CHECK_CLOSE(points[1][0], 1.0, 0.01);
      BOOST_CHECK_CLOSE(points[1][1], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[1][2], 0.0, 0.01);

      BOOST_CHECK_CLOSE(points[2][0], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[2][1], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[2][2], 1.0, 0.01);

      BOOST_CHECK_CLOSE(n[0],  0.0, 0.01);
      BOOST_CHECK_CLOSE(n[1], -1.0, 0.01);
      BOOST_CHECK_CLOSE(n[2],  0.0, 0.01);
    }
    {
      V const s = tiny::unit( V::make(1.0 , 1.0, 1.0) );

      V n;
      std::vector<V> points;

      bool found = G.search_for_feature(s, points, n);

      BOOST_CHECK( found );
      BOOST_CHECK_EQUAL(points.size(), 3);

      BOOST_CHECK_CLOSE(points[0][0], 1.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][1], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][2], 0.0, 0.01);

      BOOST_CHECK_CLOSE(points[1][0], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[1][1], 1.0, 0.01);
      BOOST_CHECK_CLOSE(points[1][2], 0.0, 0.01);

      BOOST_CHECK_CLOSE(points[2][0], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[2][1], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[2][2], 1.0, 0.01);

      BOOST_CHECK_CLOSE(n[0],  s[0], 0.01);
      BOOST_CHECK_CLOSE(n[1],  s[1], 0.01);
      BOOST_CHECK_CLOSE(n[2],  s[2], 0.01);

    }

    // Search for vertex features
    {
      V const s = tiny::unit( V::make(0.0 , 0.0, 1.0) );

      V n;
      std::vector<V> points;

      bool found = G.search_for_feature(s, points, n);

      BOOST_CHECK( found );
      BOOST_CHECK_EQUAL(points.size(), 1);

      BOOST_CHECK_CLOSE(points[0][0], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][1], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][2], 1.0, 0.01);

      BOOST_CHECK_CLOSE(n[0],  s[0], 0.01);
      BOOST_CHECK_CLOSE(n[1],  s[1], 0.01);
      BOOST_CHECK_CLOSE(n[2],  s[2], 0.01);
      
    }
    {
      V const s = tiny::unit( V::make(0.0 , 1.0, 0.0) );

      V n;
      std::vector<V> points;

      bool found = G.search_for_feature(s, points, n);

      BOOST_CHECK( found );
      BOOST_CHECK_EQUAL(points.size(), 1);

      BOOST_CHECK_CLOSE(points[0][0], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][1], 1.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][2], 0.0, 0.01);

      BOOST_CHECK_CLOSE(n[0],  s[0], 0.01);
      BOOST_CHECK_CLOSE(n[1],  s[1], 0.01);
      BOOST_CHECK_CLOSE(n[2],  s[2], 0.01);
      
    }
    {
      V const s = tiny::unit( V::make(1.0 , 0.0, 0.0) );

      V n;
      std::vector<V> points;

      bool found = G.search_for_feature(s, points, n);

      BOOST_CHECK( found );
      BOOST_CHECK_EQUAL(points.size(), 1);

      BOOST_CHECK_CLOSE(points[0][0], 1.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][1], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][2], 0.0, 0.01);

      BOOST_CHECK_CLOSE(n[0],  s[0], 0.01);
      BOOST_CHECK_CLOSE(n[1],  s[1], 0.01);
      BOOST_CHECK_CLOSE(n[2],  s[2], 0.01);
      
    }
    {
      V const s = tiny::unit( V::make(-1.0 , -1.0, -1.0) );

      V n;
      std::vector<V> points;

      bool found = G.search_for_feature(s, points, n);

      BOOST_CHECK( found );
      BOOST_CHECK_EQUAL(points.size(), 1);

      BOOST_CHECK_CLOSE(points[0][0], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][1], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][2], 0.0, 0.01);

      BOOST_CHECK_CLOSE(n[0],  s[0], 0.01);
      BOOST_CHECK_CLOSE(n[1],  s[1], 0.01);
      BOOST_CHECK_CLOSE(n[2],  s[2], 0.01);
    }

    // Search for edge features
    {
      V const s = tiny::unit( V::make(-1.0 , -1.0,  0.0) );

      V n;
      std::vector<V> points;

      bool found = G.search_for_feature(s, points, n);

      BOOST_CHECK( found );
      BOOST_CHECK_EQUAL(points.size(), 2);

      BOOST_CHECK_CLOSE(points[0][0], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][1], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][2], 0.0, 0.01);

      BOOST_CHECK_CLOSE(points[1][0], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[1][1], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[1][2], 1.0, 0.01);

      BOOST_CHECK_CLOSE(n[0],  s[0], 0.01);
      BOOST_CHECK_CLOSE(n[1],  s[1], 0.01);
      BOOST_CHECK_CLOSE(n[2],  s[2], 0.01);
    }
    {
      V const s = tiny::unit( V::make(-1.0 , 0.0,  -1.0) );

      V n;
      std::vector<V> points;

      bool found = G.search_for_feature(s, points, n);

      BOOST_CHECK( found );
      BOOST_CHECK_EQUAL(points.size(), 2);

      BOOST_CHECK_CLOSE(points[0][0], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][1], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][2], 0.0, 0.01);

      BOOST_CHECK_CLOSE(points[1][0], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[1][1], 1.0, 0.01);
      BOOST_CHECK_CLOSE(points[1][2], 0.0, 0.01);

      BOOST_CHECK_CLOSE(n[0],  s[0], 0.01);
      BOOST_CHECK_CLOSE(n[1],  s[1], 0.01);
      BOOST_CHECK_CLOSE(n[2],  s[2], 0.01);
      
    }
    {
      V const s = tiny::unit( V::make(0.0 , -1.0,  -1.0) );

      V n;
      std::vector<V> points;

      bool found = G.search_for_feature(s, points, n);

      BOOST_CHECK( found );
      BOOST_CHECK_EQUAL(points.size(), 2);

      BOOST_CHECK_CLOSE(points[0][0], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][1], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][2], 0.0, 0.01);

      BOOST_CHECK_CLOSE(points[1][0], 1.0, 0.01);
      BOOST_CHECK_CLOSE(points[1][1], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[1][2], 0.0, 0.01);

      BOOST_CHECK_CLOSE(n[0],  s[0], 0.01);
      BOOST_CHECK_CLOSE(n[1],  s[1], 0.01);
      BOOST_CHECK_CLOSE(n[2],  s[2], 0.01);
    }
    {
      V const s = tiny::unit( V::make(1.0 ,  1.0,  0.5) );

      V n;
      std::vector<V> points;

      bool found = G.search_for_feature(s, points, n);

      BOOST_CHECK( found );
      BOOST_CHECK_EQUAL(points.size(), 2);

      BOOST_CHECK_CLOSE(points[0][0], 1.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][1], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][2], 0.0, 0.01);

      BOOST_CHECK_CLOSE(points[1][0], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[1][1], 1.0, 0.01);
      BOOST_CHECK_CLOSE(points[1][2], 0.0, 0.01);

      BOOST_CHECK_CLOSE(n[0],  s[0], 0.01);
      BOOST_CHECK_CLOSE(n[1],  s[1], 0.01);
      BOOST_CHECK_CLOSE(n[2],  s[2], 0.01);
    }
    {
      V const s = tiny::unit( V::make(1.0 , -0.5,  1.0) );

      V n;
      std::vector<V> points;

      bool found = G.search_for_feature(s, points, n);

      BOOST_CHECK( found );
      BOOST_CHECK_EQUAL(points.size(), 2);

      BOOST_CHECK_CLOSE(points[0][0], 1.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][1], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][2], 0.0, 0.01);

      BOOST_CHECK_CLOSE(points[1][0], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[1][1], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[1][2], 1.0, 0.01);

      BOOST_CHECK_CLOSE(n[0],  s[0], 0.01);
      BOOST_CHECK_CLOSE(n[1],  s[1], 0.01);
      BOOST_CHECK_CLOSE(n[2],  s[2], 0.01);
    }
    {
      V const s = tiny::unit( V::make(-0.5 , 1.0,  1.0) );

      V n;
      std::vector<V> points;

      bool found = G.search_for_feature(s, points, n);

      BOOST_CHECK( found );
      BOOST_CHECK_EQUAL(points.size(), 2);

      BOOST_CHECK_CLOSE(points[0][0], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][1], 1.0, 0.01);
      BOOST_CHECK_CLOSE(points[0][2], 0.0, 0.01);

      BOOST_CHECK_CLOSE(points[1][0], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[1][1], 0.0, 0.01);
      BOOST_CHECK_CLOSE(points[1][2], 1.0, 0.01);

      BOOST_CHECK_CLOSE(n[0],  s[0], 0.01);
      BOOST_CHECK_CLOSE(n[1],  s[1], 0.01);
      BOOST_CHECK_CLOSE(n[2],  s[2], 0.01);
    }
  }

}

BOOST_AUTO_TEST_SUITE_END();
