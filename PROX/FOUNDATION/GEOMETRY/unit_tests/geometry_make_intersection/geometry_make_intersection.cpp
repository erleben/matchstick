#include <geometry.h>
#include <tiny.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

#include <vector>


BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(intersection)
{
  typedef tiny::MathTypes<float> MT;
  typedef MT::vector3_type       V;
  
  {
    geometry::Plane<V> const A = geometry::make_plane( V::make(0,0,1), -1.0);
    geometry::Plane<V> const B = geometry::make_plane( V::make(0,1,0), -1.0);
    geometry::Line<V>  const L = geometry::make_intersection(A,B);
    
    BOOST_CHECK_CLOSE( geometry::get_distance(L.point(), A), 0.0, 0.01f );
    BOOST_CHECK_CLOSE( geometry::get_distance(L.point(), B), 0.0, 0.01f );
    BOOST_CHECK_CLOSE( inner_prod(L.direction(), A.normal()), 0.0, 0.01f );
    BOOST_CHECK_CLOSE( inner_prod(L.direction(), B.normal()), 0.0, 0.01f );
    BOOST_CHECK_CLOSE( inner_prod(L.direction(), L.direction()), 1.0, 0.01f );
  }

  {
    geometry::Plane<V> const A = geometry::make_plane( V::make(0,0,1), -1.0);
    geometry::Plane<V> const B = geometry::make_plane( V::make(0,1,0), -1.0);
    geometry::Plane<V> const C = geometry::make_plane( V::make(1,0,0), -1.0);
    
    V const p = geometry::make_intersection(A,B,C);
    
    BOOST_CHECK_CLOSE( geometry::get_distance(p, A), 0.0, 0.01f );
    BOOST_CHECK_CLOSE( geometry::get_distance(p, B), 0.0, 0.01f );
    BOOST_CHECK_CLOSE( geometry::get_distance(p, C), 0.0, 0.01f );
  }

}

BOOST_AUTO_TEST_SUITE_END();
