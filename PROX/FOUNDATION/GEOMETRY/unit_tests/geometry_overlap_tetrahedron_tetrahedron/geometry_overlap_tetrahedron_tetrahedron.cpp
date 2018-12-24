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

BOOST_AUTO_TEST_CASE(overlap_tet_tet_test)
{
  // B inside A, all SAT tests should fail
  {
    V const Ap0   = V::make(0.0, 0.0, 0.0);
    V const Ap1   = V::make(1.0, 0.0, 0.0);
    V const Ap2   = V::make(0.0, 1.0, 0.0);
    V const Ap3   = V::make(0.0, 0.0, 1.0);

    V const Bp0   = V::make(0.1, 0.1, 0.1);
    V const Bp1   = V::make(0.9, 0.0, 0.0);
    V const Bp2   = V::make(0.0, 0.9, 0.0);
    V const Bp3   = V::make(0.0, 0.0, 0.9);

    geometry::Tetrahedron<V> tetA = geometry::make_tetrahedron<V>(Ap0, Ap1, Ap2, Ap3);
    geometry::Tetrahedron<V> tetB = geometry::make_tetrahedron<V>(Bp0, Bp1, Bp2, Bp3);

    bool const test1 = geometry::overlap_tetrahedron_tetrahedron(tetA, tetB);
    bool const test2 = geometry::overlap_tetrahedron_tetrahedron(tetB, tetA);

    BOOST_CHECK(test1);
    BOOST_CHECK(test2);
  }

  unsigned int permutation[6][4] = {
    {1, 3, 2, 0},
    {0, 3, 1, 2},
    {0, 1, 2, 3},
    {2, 3, 0, 1},
    {1, 2, 0, 3},
    {2, 0, 1, 3}
  };

  // Touching edge-edge cases
  {
    std::vector<V> A(4u);
    std::vector<V> B(4u);

    A[0]   = V::make(0.0, 0.0, 0.0);
    A[1]   = V::make(1.0, 0.0, 0.0);
    A[2]   = V::make(0.0, 1.0, 0.0);
    A[3]   = V::make(0.0, 0.0, 1.0);

    B[0]   = V::make( 1.0, -1.0,  0.5);
    B[1]   = V::make(-1.0,  1.0,  0.5);
    B[2]   = V::make(-1.0, -1.0, -1.0);
    B[3]   = V::make(-1.0, -1.0,  1.0);



    for (unsigned int i=0u;i<6u;++i)
    {
      for (unsigned int j=0u;j<6u;++j)
      {


        geometry::Tetrahedron<V> tetA = geometry::make_tetrahedron<V>(
                                                                        A[permutation[i][0]]
                                                                      , A[permutation[i][1]]
                                                                      , A[permutation[i][2]]
                                                                      , A[permutation[i][3]]
                                                                      );
        geometry::Tetrahedron<V> tetB = geometry::make_tetrahedron<V>(
                                                                        B[permutation[j][0]]
                                                                      , B[permutation[j][1]]
                                                                      , B[permutation[j][2]]
                                                                      , B[permutation[j][3]]
                                                                      );

        bool const test1 = geometry::overlap_tetrahedron_tetrahedron(tetA, tetB);
        bool const test2 = geometry::overlap_tetrahedron_tetrahedron(tetB, tetA);

        BOOST_CHECK(test1);
        BOOST_CHECK(test2);

      }
    }


  }

  // Separating edge-edge cases
  {
    std::vector<V> A(4u);
    std::vector<V> B(4u);

    A[0]   = V::make(0.01, 0.01, 0.01);
    A[1]   = V::make(1.01, 0.01, 0.01);
    A[2]   = V::make(0.01, 1.01, 0.01);
    A[3]   = V::make(0.01, 0.01, 1.01);

    B[0]   = V::make( 1.0, -1.0,  0.5);
    B[1]   = V::make(-1.0,  1.0,  0.5);
    B[2]   = V::make(-1.0, -1.0, -1.0);
    B[3]   = V::make(-1.0, -1.0,  1.0);



    for (unsigned int i=0u;i<6u;++i)
    {
      for (unsigned int j=0u;j<6u;++j)
      {


        geometry::Tetrahedron<V> tetA = geometry::make_tetrahedron<V>(
                                                                      A[permutation[i][0]]
                                                                      , A[permutation[i][1]]
                                                                      , A[permutation[i][2]]
                                                                      , A[permutation[i][3]]
                                                                      );
        geometry::Tetrahedron<V> tetB = geometry::make_tetrahedron<V>(
                                                                      B[permutation[j][0]]
                                                                      , B[permutation[j][1]]
                                                                      , B[permutation[j][2]]
                                                                      , B[permutation[j][3]]
                                                                      );

        bool const test1 = geometry::overlap_tetrahedron_tetrahedron(tetA, tetB);
        bool const test2 = geometry::overlap_tetrahedron_tetrahedron(tetB, tetA);

        BOOST_CHECK(!test1);
        BOOST_CHECK(!test2);
        
      }
    }
    
    
  }

  // Separated by face cases
  {
    V const A0   = V::make(0.0, 0.0, 0.0);
    V const A1   = V::make(1.0, 0.0, 0.0);
    V const A2   = V::make(0.0, 1.0, 0.0);
    V const A3   = V::make(0.0, 0.0, 1.0);

    std::vector<V> offset(4);

    offset[0] = V::make( 2.0,  2.0,  2.0);
    offset[1] = V::make(-2.0,  0.0,  0.0);
    offset[2] = V::make( 0.0, -2.0,  0.0);
    offset[3] = V::make( 0.0,  0.0, -2.0);

    geometry::Tetrahedron<V> tetA = geometry::make_tetrahedron<V>(A0, A1, A2, A3);

    for (unsigned int k = 0u; k < 4u; ++k)
    {

      geometry::Tetrahedron<V> tetB = geometry::make_tetrahedron<V>(
                                                                    A0 + offset[k]
                                                                    , A1 + offset[k]
                                                                    , A2 + offset[k]
                                                                    , A3 + offset[k]
                                                                    );

      bool const test1 = geometry::overlap_tetrahedron_tetrahedron(tetA, tetB);
      bool const test2 = geometry::overlap_tetrahedron_tetrahedron(tetB, tetA);

      BOOST_CHECK(!test1);
      BOOST_CHECK(!test2);
      
    }
    
  }
}

BOOST_AUTO_TEST_SUITE_END();
