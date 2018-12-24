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

BOOST_AUTO_TEST_CASE(overlap_obb_obb_test)
{
  // touching bottom-top faces
  {
    V const centerA   = V::make(0.0, 0.0, 0.0);
    V const half_extA = V::make(1.0, 1.0, 1.0);
    Q const qA        = Q::identity();

    geometry::OBB<MT> obbA = geometry::make_obb<MT>(centerA, qA, half_extA);

    V const centerB   = V::make(0.0, -2.0, 0.0);
    V const half_extB = V::make(1.0, 1.0, 1.0);
    Q const qB        = Q::Ry(VT::pi_quarter());

    geometry::OBB<MT> obbB = geometry::make_obb<MT>(centerB, qB, half_extB);

    bool const test1 = geometry::overlap_obb_obb(obbA,obbB);
    bool const test2 = geometry::overlap_obb_obb(obbB,obbA);

    BOOST_CHECK(test1);
    BOOST_CHECK(test2);
  }
  // touching left-right faces
  {
    V const centerA   = V::make(0.0, 0.0, 0.0);
    V const half_extA = V::make(1.0, 1.0, 1.0);
    Q const qA        = Q::identity();

    geometry::OBB<MT> obbA = geometry::make_obb<MT>(centerA, qA, half_extA);

    V const centerB   = V::make(2.0, 0.0, 0.0);
    V const half_extB = V::make(1.0, 1.0, 1.0);
    Q const qB        = Q::Rx(VT::pi_quarter());

    geometry::OBB<MT> obbB = geometry::make_obb<MT>(centerB, qB, half_extB);

    bool const test1 = geometry::overlap_obb_obb(obbA,obbB);
    bool const test2 = geometry::overlap_obb_obb(obbB,obbA);

    BOOST_CHECK(test1);
    BOOST_CHECK(test2);
  }
  // touching front-back faces
  {
    V const centerA   = V::make(0.0, 0.0, 0.0);
    V const half_extA = V::make(1.0, 1.0, 1.0);
    Q const qA        = Q::identity();

    geometry::OBB<MT> obbA = geometry::make_obb<MT>(centerA, qA, half_extA);

    V const centerB   = V::make(0.0, 0.0, 2.0);
    V const half_extB = V::make(1.0, 1.0, 1.0);
    Q const qB        = Q::Rz(VT::pi_quarter());

    geometry::OBB<MT> obbB = geometry::make_obb<MT>(centerB, qB, half_extB);

    bool const test1 = geometry::overlap_obb_obb(obbA,obbB);
    bool const test2 = geometry::overlap_obb_obb(obbB,obbA);

    BOOST_CHECK(test1);
    BOOST_CHECK(test2);
  }
  // separating bottom-top faces
  {
    V const centerA   = V::make(0.0, 0.0, 0.0);
    V const half_extA = V::make(1.0, 1.0, 1.0);
    Q const qA        = Q::identity();

    geometry::OBB<MT> obbA = geometry::make_obb<MT>(centerA, qA, half_extA);

    V const centerB   = V::make(0.0, -2.1, 0.0);
    V const half_extB = V::make(1.0, 1.0, 1.0);
    Q const qB        = Q::Ry(VT::pi_quarter());

    geometry::OBB<MT> obbB = geometry::make_obb<MT>(centerB, qB, half_extB);

    bool const test1 = geometry::overlap_obb_obb(obbA,obbB);
    bool const test2 = geometry::overlap_obb_obb(obbB,obbA);

    BOOST_CHECK(!test1);
    BOOST_CHECK(!test2);
  }
  // separating left-right faces
  {
    V const centerA   = V::make(0.0, 0.0, 0.0);
    V const half_extA = V::make(1.0, 1.0, 1.0);
    Q const qA        = Q::identity();

    geometry::OBB<MT> obbA = geometry::make_obb<MT>(centerA, qA, half_extA);

    V const centerB   = V::make(2.1, 0.0, 0.0);
    V const half_extB = V::make(1.0, 1.0, 1.0);
    Q const qB        = Q::Rx(VT::pi_quarter());

    geometry::OBB<MT> obbB = geometry::make_obb<MT>(centerB, qB, half_extB);

    bool const test1 = geometry::overlap_obb_obb(obbA,obbB);
    bool const test2 = geometry::overlap_obb_obb(obbB,obbA);

    BOOST_CHECK(!test1);
    BOOST_CHECK(!test2);
  }
  // separating front-back faces
  {
    V const centerA   = V::make(0.0, 0.0, 0.0);
    V const half_extA = V::make(1.0, 1.0, 1.0);
    Q const qA        = Q::identity();

    geometry::OBB<MT> obbA = geometry::make_obb<MT>(centerA, qA, half_extA);

    V const centerB   = V::make(0.0, 0.0, 2.1);
    V const half_extB = V::make(1.0, 1.0, 1.0);
    Q const qB        = Q::Rz(VT::pi_quarter());

    geometry::OBB<MT> obbB = geometry::make_obb<MT>(centerB, qB, half_extB);

    bool const test1 = geometry::overlap_obb_obb(obbA,obbB);
    bool const test2 = geometry::overlap_obb_obb(obbB,obbA);
    
    BOOST_CHECK(!test1);
    BOOST_CHECK(!test2);
  }
  // A inside of B
  {
    V const centerA   = V::make(0.0, 0.0, 0.0);
    V const half_extA = V::make(0.5, 0.5, 0.5);
    Q const qA        = Q::identity();

    geometry::OBB<MT> obbA = geometry::make_obb<MT>(centerA, qA, half_extA);

    V const centerB   = V::make(0.0, 0.0, 0.0);
    V const half_extB = V::make(1.0, 1.0, 1.0);
    Q const qB        = Q::Ry(VT::pi_quarter());

    geometry::OBB<MT> obbB = geometry::make_obb<MT>(centerB, qB, half_extB);

    bool const test1 = geometry::overlap_obb_obb(obbA,obbB);
    bool const test2 = geometry::overlap_obb_obb(obbB,obbA);

    BOOST_CHECK(test1);
    BOOST_CHECK(test2);
  }
  // B inside of A
  {
    V const centerA   = V::make(0.0, 0.0, 0.0);
    V const half_extA = V::make(1.0, 1.0, 1.0);
    Q const qA        = Q::identity();

    geometry::OBB<MT> obbA = geometry::make_obb<MT>(centerA, qA, half_extA);

    V const centerB   = V::make(0.0, 0.0, 0.0);
    V const half_extB = V::make(0.5, 0.5, 0.5);
    Q const qB        = Q::Ry(VT::pi_quarter());

    geometry::OBB<MT> obbB = geometry::make_obb<MT>(centerB, qB, half_extB);

    bool const test1 = geometry::overlap_obb_obb(obbA,obbB);
    bool const test2 = geometry::overlap_obb_obb(obbB,obbA);

    BOOST_CHECK(test1);
    BOOST_CHECK(test2);
  }
  // touching edge-edge-case
  {
    V const centerA   = V::make(0.0, 0.0, 0.0);
    V const half_extA = V::make(1.0, 1.0, 1.0);
    Q const qA        = Q::identity();

    geometry::OBB<MT> obbA = geometry::make_obb<MT>(centerA, qA, half_extA);

    V const centerB   = V::make(2.0, 2.0, 0.0);
    V const half_extB = V::make(1.0, 1.0, 1.0);
    Q const qB        = Q::Ru(VT::pi_half(), centerB );

    geometry::OBB<MT> obbB = geometry::make_obb<MT>(centerB, qB, half_extB);

    bool const test1 = geometry::overlap_obb_obb(obbA,obbB);
    bool const test2 = geometry::overlap_obb_obb(obbB,obbA);

    BOOST_CHECK(test1);
    BOOST_CHECK(test2);
  }
  // separating edge-edge-case
  {
    V const centerA   = V::make(0.0, 0.0, 0.0);
    V const half_extA = V::make(1.0, 1.0, 1.0);
    Q const qA        = Q::identity();

    geometry::OBB<MT> obbA = geometry::make_obb<MT>(centerA, qA, half_extA);

    V const centerB   = V::make(2.01, 2.01, 0.0);
    V const half_extB = V::make(1.0, 1.0, 1.0);
    Q const qB        = Q::Ru(VT::pi_half(), centerB );

    geometry::OBB<MT> obbB = geometry::make_obb<MT>(centerB, qB, half_extB);

    bool const test1 = geometry::overlap_obb_obb(obbA,obbB);
    bool const test2 = geometry::overlap_obb_obb(obbB,obbA);

    BOOST_CHECK(!test1);
    BOOST_CHECK(!test2);
  }

}

BOOST_AUTO_TEST_SUITE_END();
