#include <geometry.h>
#include <tiny.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

typedef tiny::MathTypes<float> MT;
typedef MT::quaternion_type    Q;
typedef MT::vector3_type       V;
typedef MT::real_type          T;
typedef MT::value_traits       VT;


BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(overlap_tri_tri_test)
{
  // B inside A
  {
    V const A0   = V::make(0.0, 0.0, 0.0);
    V const A1   = V::make(1.0, 0.0, 0.0);
    V const A2   = V::make(0.0, 1.0, 0.0);

    V const B0   = V::make(0.1, 0.1, 0.1);
    V const B1   = V::make(0.9, 0.0, 0.0);
    V const B2   = V::make(0.0, 0.9, 0.0);

    geometry::Triangle<V> triA = geometry::make_triangle<V>(A0, A1, A2);
    geometry::Triangle<V> triB = geometry::make_triangle<V>(B0, B1, B2);

    bool const test1 = geometry::overlap_triangle_triangle(triA, triB);
    bool const test2 = geometry::overlap_triangle_triangle(triB, triA);

    BOOST_CHECK(test1);
    BOOST_CHECK(test2);
  }

  unsigned int permutation[6][3] = {
    {0, 1, 2},
    {0, 2, 1},
    {1, 2, 0},
    {1, 0, 2},
    {2, 0, 1},
    {2, 1, 0}
  };

  // Touching edge-edge cases
  {
    std::vector<V> A(3u);
    std::vector<V> B(3u);

    A[0]   = V::make(0.0, 0.0, 0.0);
    A[1]   = V::make(1.0, 0.0, 0.0);
    A[2]   = V::make(0.0, 1.0, 0.0);

    B[0]   = V::make( 0.5, 0.0,-0.5);
    B[1]   = V::make( 0.5, 0.0, 0.5);
    B[2]   = V::make( 0.5,-1.0, 0.0);

    for (unsigned int i=0u;i<6u;++i)
    {
      for (unsigned int j=0u;j<6u;++j)
      {
        geometry::Triangle<V> triA = geometry::make_triangle<V>(
                                                                        A[permutation[i][0]]
                                                                      , A[permutation[i][1]]
                                                                      , A[permutation[i][2]]
                                                                      );
        geometry::Triangle<V> triB = geometry::make_triangle<V>(
                                                                        B[permutation[j][0]]
                                                                      , B[permutation[j][1]]
                                                                      , B[permutation[j][2]]
                                                                      );

        bool const test1 = geometry::overlap_triangle_triangle(triA, triB);
        bool const test2 = geometry::overlap_triangle_triangle(triB, triA);

        BOOST_CHECK(test1);
        BOOST_CHECK(test2);

      }
    }


  }

  // Separating edge-edge cases
  {
    std::vector<V> A(3u);
    std::vector<V> B(3u);

    A[0]   = V::make(0.0, 0.0, 0.0);
    A[1]   = V::make(1.0, 0.0, 0.0);
    A[2]   = V::make(0.0, 1.0, 0.0);

    B[0]   = V::make( 0.5, -0.01,-0.5);
    B[1]   = V::make( 0.5, -0.01, 0.5);
    B[2]   = V::make( 0.5, -1.01, 0.0);

    for (unsigned int i=0u;i<6u;++i)
    {
      for (unsigned int j=0u;j<6u;++j)
      {
        geometry::Triangle<V> triA = geometry::make_triangle<V>(
                                                                   A[permutation[i][0]]
                                                                   , A[permutation[i][1]]
                                                                   , A[permutation[i][2]]
                                                                   );
        geometry::Triangle<V> triB = geometry::make_triangle<V>(
                                                                   B[permutation[j][0]]
                                                                   , B[permutation[j][1]]
                                                                   , B[permutation[j][2]]
                                                                   );

        bool const test1 = geometry::overlap_triangle_triangle(triA, triB);
        bool const test2 = geometry::overlap_triangle_triangle(triB, triA);

        BOOST_CHECK(!test1);
        BOOST_CHECK(!test2);

      }
    }
    
    
  }

  // Separated by face cases
  {
    std::vector<V> A(3u);
    std::vector<V> B(3u);

    A[0]   = V::make(0.0, 0.0, 0.0);
    A[1]   = V::make(1.0, 0.0, 0.0);
    A[2]   = V::make(0.0, 1.0, 0.0);

    B[0]   = V::make( 1.1,  0.0, 0.1);
    B[1]   = V::make( 1.1,  0.0, 1.1);
    B[2]   = V::make( 1.1, -1.0, 0.6);

    for (unsigned int i=0u;i<6u;++i)
    {
      for (unsigned int j=0u;j<6u;++j)
      {
        geometry::Triangle<V> triA = geometry::make_triangle<V>(
                                                                   A[permutation[i][0]]
                                                                   , A[permutation[i][1]]
                                                                   , A[permutation[i][2]]
                                                                   );
        geometry::Triangle<V> triB = geometry::make_triangle<V>(
                                                                   B[permutation[j][0]]
                                                                   , B[permutation[j][1]]
                                                                   , B[permutation[j][2]]
                                                                   );

        bool const test1 = geometry::overlap_triangle_triangle(triA, triB);
        bool const test2 = geometry::overlap_triangle_triangle(triB, triA);

        BOOST_CHECK(!test1);
        BOOST_CHECK(!test2);

      }
    }
  }

}

BOOST_AUTO_TEST_SUITE_END();
