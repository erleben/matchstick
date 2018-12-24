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

BOOST_AUTO_TEST_CASE(inside_aabb_test)
{

  {
    V const min_coord = V::make(-1.0, -1.0, -1.0);
    V const max_coord = V::make( 1.0,  1.0,  1.0);

    geometry::AABB<V> A = geometry::make_aabb(min_coord, max_coord);


    V const p0 = V::make( 0.0,  0.0,  0.0);
    V const p1 = V::make( 1.0,  1.0,  1.0);
    V const p2 = V::make( 2.0,  2.0,  2.0);

    BOOST_CHECK( geometry::inside_aabb(p0, A) == true );
    BOOST_CHECK( geometry::inside_aabb(p1, A) == true );
    BOOST_CHECK( geometry::inside_aabb(p2, A) == false );

  }


}

BOOST_AUTO_TEST_SUITE_END();
