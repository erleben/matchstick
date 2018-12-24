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

BOOST_AUTO_TEST_CASE(overlap_obb_capsule_test)
{
  // Separated on plus x-axis of OBB
  {
    V const center   = V::make(0.0, 0.0, 0.0);
    V const half_ext = V::make(1.0, 1.0, 1.0);
    Q const q        = Q::identity();

    V const point0   = V::make(2.1,  0.0, 0.0);
    V const point1   = V::make(3.1,  2.1, 2.1);
    T const radius   = 1.0;

    geometry::OBB<MT>    const & obb = geometry::make_obb<MT>(center, q, half_ext);
    geometry::Capsule<V> const & cap = geometry::make_capsule(radius, point0, point1);

    bool const test = geometry::overlap_obb_capsule(obb, cap);

    BOOST_CHECK(!test);
  }
  // Overlap from plus x-side of OBB
  {
    V const center   = V::make(0.0, 0.0, 0.0);
    V const half_ext = V::make(1.0, 1.0, 1.0);
    Q const q        = Q::identity();

    V const point0   = V::make(1.9,  0.0, 0.0);
    V const point1   = V::make(3.1,  2.1, 2.1);
    T const radius   = 1.0;

    geometry::OBB<MT>    const & obb = geometry::make_obb<MT>(center, q, half_ext);
    geometry::Capsule<V> const & cap = geometry::make_capsule(radius, point0, point1);

    bool const test = geometry::overlap_obb_capsule(obb, cap);

    BOOST_CHECK(test);
  }
  // Separated on negative x-axis of OBB
  {
    V const center   = V::make(0.0, 0.0, 0.0);
    V const half_ext = V::make(1.0, 1.0, 1.0);
    Q const q        = Q::identity();

    V const point0   = V::make(-2.1,  0.0, 0.0);
    V const point1   = V::make(-3.1,  2.1, 2.1);
    T const radius   = 1.0;

    geometry::OBB<MT>    const & obb = geometry::make_obb<MT>(center, q, half_ext);
    geometry::Capsule<V> const & cap = geometry::make_capsule(radius, point0, point1);

    bool const test = geometry::overlap_obb_capsule(obb, cap);

    BOOST_CHECK(!test);
  }
  // Overlap from negative x-side of OBB
  {
    V const center   = V::make(0.0, 0.0, 0.0);
    V const half_ext = V::make(1.0, 1.0, 1.0);
    Q const q        = Q::identity();

    V const point0   = V::make(-1.9,  0.0, 0.0);
    V const point1   = V::make(-3.1,  2.1, 2.1);
    T const radius   = 1.0;

    geometry::OBB<MT>    const & obb = geometry::make_obb<MT>(center, q, half_ext);
    geometry::Capsule<V> const & cap = geometry::make_capsule(radius, point0, point1);

    bool const test = geometry::overlap_obb_capsule(obb, cap);
    
    BOOST_CHECK(test);
  }
  // Separated on plus y-axis of OBB
  {
    V const center   = V::make(0.0, 0.0, 0.0);
    V const half_ext = V::make(1.0, 1.0, 1.0);
    Q const q        = Q::identity();

    V const point0   = V::make(0.0, 2.1,  0.0);
    V const point1   = V::make(2.1, 3.1,  2.1);
    T const radius   = 1.0;

    geometry::OBB<MT>    const & obb = geometry::make_obb<MT>(center, q, half_ext);
    geometry::Capsule<V> const & cap = geometry::make_capsule(radius, point0, point1);

    bool const test = geometry::overlap_obb_capsule(obb, cap);

    BOOST_CHECK(!test);
  }
  // Overlap from plus y-side of OBB
  {
    V const center   = V::make(0.0, 0.0, 0.0);
    V const half_ext = V::make(1.0, 1.0, 1.0);
    Q const q        = Q::identity();

    V const point0   = V::make(0.0,  1.9, 0.0);
    V const point1   = V::make(2.1,  3.1, 2.1);
    T const radius   = 1.0;

    geometry::OBB<MT>    const & obb = geometry::make_obb<MT>(center, q, half_ext);
    geometry::Capsule<V> const & cap = geometry::make_capsule(radius, point0, point1);

    bool const test = geometry::overlap_obb_capsule(obb, cap);

    BOOST_CHECK(test);
  }
  // Separated on negative y-axis of OBB
  {
    V const center   = V::make(0.0, 0.0, 0.0);
    V const half_ext = V::make(1.0, 1.0, 1.0);
    Q const q        = Q::identity();

    V const point0   = V::make(0.0,  -2.1, 0.0);
    V const point1   = V::make(2.1,  -3.1, 2.1);
    T const radius   = 1.0;

    geometry::OBB<MT>    const & obb = geometry::make_obb<MT>(center, q, half_ext);
    geometry::Capsule<V> const & cap = geometry::make_capsule(radius, point0, point1);

    bool const test = geometry::overlap_obb_capsule(obb, cap);

    BOOST_CHECK(!test);
  }
  // Overlap from negative y-side of OBB
  {
    V const center   = V::make(0.0, 0.0, 0.0);
    V const half_ext = V::make(1.0, 1.0, 1.0);
    Q const q        = Q::identity();

    V const point0   = V::make(0.0, -1.9, 0.0);
    V const point1   = V::make(2.1, -3.1, 2.1);
    T const radius   = 1.0;

    geometry::OBB<MT>    const & obb = geometry::make_obb<MT>(center, q, half_ext);
    geometry::Capsule<V> const & cap = geometry::make_capsule(radius, point0, point1);

    bool const test = geometry::overlap_obb_capsule(obb, cap);
    
    BOOST_CHECK(test);
  }
  // Separated on plus z-axis of OBB
  {
    V const center   = V::make(0.0, 0.0, 0.0);
    V const half_ext = V::make(1.0, 1.0, 1.0);
    Q const q        = Q::identity();

    V const point0   = V::make(0.0, 0.0, 2.1);
    V const point1   = V::make(2.1, 2.1, 3.1);
    T const radius   = 1.0;

    geometry::OBB<MT>    const & obb = geometry::make_obb<MT>(center, q, half_ext);
    geometry::Capsule<V> const & cap = geometry::make_capsule(radius, point0, point1);

    bool const test = geometry::overlap_obb_capsule(obb, cap);

    BOOST_CHECK(!test);
  }
  // Overlap from plus z-side of OBB
  {
    V const center   = V::make(0.0, 0.0, 0.0);
    V const half_ext = V::make(1.0, 1.0, 1.0);
    Q const q        = Q::identity();

    V const point0   = V::make(0.0, 0.0,  1.9);
    V const point1   = V::make(2.1, 2.1,  3.1);
    T const radius   = 1.0;

    geometry::OBB<MT>    const & obb = geometry::make_obb<MT>(center, q, half_ext);
    geometry::Capsule<V> const & cap = geometry::make_capsule(radius, point0, point1);

    bool const test = geometry::overlap_obb_capsule(obb, cap);

    BOOST_CHECK(test);
  }
  // Separated on negative z-axis of OBB
  {
    V const center   = V::make(0.0, 0.0, 0.0);
    V const half_ext = V::make(1.0, 1.0, 1.0);
    Q const q        = Q::identity();

    V const point0   = V::make(0.0, 0.0,  -2.1);
    V const point1   = V::make(2.1, 2.1,  -3.1);
    T const radius   = 1.0;

    geometry::OBB<MT>    const & obb = geometry::make_obb<MT>(center, q, half_ext);
    geometry::Capsule<V> const & cap = geometry::make_capsule(radius, point0, point1);

    bool const test = geometry::overlap_obb_capsule(obb, cap);

    BOOST_CHECK(!test);
  }
  // Overlap from negative z-side of OBB
  {
    V const center   = V::make(0.0, 0.0, 0.0);
    V const half_ext = V::make(1.0, 1.0, 1.0);
    Q const q        = Q::identity();

    V const point0   = V::make(0.0, 0.0, -1.9);
    V const point1   = V::make(2.1, 2.1, -3.1);
    T const radius   = 1.0;

    geometry::OBB<MT>    const & obb = geometry::make_obb<MT>(center, q, half_ext);
    geometry::Capsule<V> const & cap = geometry::make_capsule(radius, point0, point1);

    bool const test = geometry::overlap_obb_capsule(obb, cap);
    
    BOOST_CHECK(test);
  }
  // Separated by z-OBB X capsule axis
  {
    V const center   = V::make(0.0, 0.0, 0.0);
    V const half_ext = V::make(1.0, 1.0, 1.0);
    Q const q        = Q::identity();

    V const point0   = V::make( 0.0,  -3.0, 0.0);
    V const point1   = V::make( 3.0,   0.0, 0.0);
    T const radius   = 0.5;

    geometry::OBB<MT>    const & obb = geometry::make_obb<MT>(center, q, half_ext);
    geometry::Capsule<V> const & cap = geometry::make_capsule(radius, point0, point1);

    bool const test = geometry::overlap_obb_capsule(obb, cap);

    BOOST_CHECK(!test);
  }
  // Overlap by z-OBB X capsule axis
  {
    V const center   = V::make(0.0, 0.0, 0.0);
    V const half_ext = V::make(1.0, 1.0, 1.0);
    Q const q        = Q::identity();

    V const point0   = V::make( 0.0,  -3.0, 0.0);
    V const point1   = V::make( 3.0,   0.0, 0.0);
    T const radius   = 1.0;

    geometry::OBB<MT>    const & obb = geometry::make_obb<MT>(center, q, half_ext);
    geometry::Capsule<V> const & cap = geometry::make_capsule(radius, point0, point1);

    bool const test = geometry::overlap_obb_capsule(obb, cap);

    BOOST_CHECK(test);
  }
}

BOOST_AUTO_TEST_SUITE_END();
