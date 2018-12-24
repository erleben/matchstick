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

BOOST_AUTO_TEST_CASE(inside_obb_test)
{

  {
    V const center   = V::make(0.0, 0.0, 0.0);
    V const half_ext = V::make(1.0, 1.0, 1.0);
    Q const q        = Q::identity();

    geometry::OBB<MT> A = geometry::make_obb<MT>(center, q, half_ext);

    V const p0 = V::make( 0.0,  0.0,  0.0);
    V const p1 = V::make( 1.0,  1.0,  1.0);
    V const p2 = V::make( 2.0,  2.0,  2.0);

    BOOST_CHECK( geometry::inside_obb(p0, A) == true );
    BOOST_CHECK( geometry::inside_obb(p1, A) == true );
    BOOST_CHECK( geometry::inside_obb(p2, A) == false );
  }

  {
    V const center   = V::make(0.0, 0.0, 0.0);
    V const half_ext = V::make(1.0, 1.0, 1.0);
    Q const q        = Q::Rx( VT::pi_half() );

    geometry::OBB<MT> A = geometry::make_obb<MT>(center, q, half_ext);

    V const p0 = geometry::transform_from_obb( V::make( 0.0,  0.0,  0.0), A);
    V const p1 = geometry::transform_from_obb(V::make( 1.0,  1.0,  1.0), A);
    V const p2 = geometry::transform_from_obb(V::make( 2.0,  2.0,  2.0), A);

    BOOST_CHECK( geometry::inside_obb(p0, A) == true );
    BOOST_CHECK( geometry::inside_obb(p1, A) == true );
    BOOST_CHECK( geometry::inside_obb(p2, A) == false );
  }



}

BOOST_AUTO_TEST_SUITE_END();
