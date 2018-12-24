#include <geometry.h>
#include <tiny_math_types.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(raycast_tetrahedron)
{

  using std::sqrt;

  typedef tiny::MathTypes<double>   MT;
  typedef MT::vector3_type          V;
  typedef MT::real_type             T;
  typedef MT::value_traits          VT;


  V const p0 = V::make(0.0, 0.0, 0.0);
  V const p1 = V::make(1.0, 0.0, 0.0);
  V const p2 = V::make(0.0, 1.0, 0.0);
  V const p3 = V::make(0.0, 0.0, 1.0);

  geometry::Tetrahedron<V> const tetrahedron = geometry::make_tetrahedron(p0,p1,p2,p3);

  // Ray hitting on bottom of tetrahedron
  {
    V                const p   = V::make( 0.0, 0.0, -1.0);
    V                const r   = V::make( 0.2, 0.2,  1.0);
    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool const hit = geometry::compute_raycast_tetrahedron(ray, tetrahedron, q, length );

    BOOST_CHECK( hit );
    BOOST_CHECK_CLOSE( length, tiny::norm(r), 0.01);
    BOOST_CHECK_CLOSE( q(0),  0.2, 0.01);
    BOOST_CHECK_CLOSE( q(1),  0.2, 0.01);
    BOOST_CHECK_CLOSE( q(2),  0.0, 0.01);
  }
  // Ray missing tetrahedron
  {
    V                const p   = V::make( 0.0, 0.0, -1.0);
    V                const r   = V::make( -0.2, -0.2,  1.0);
    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool const hit = geometry::compute_raycast_tetrahedron(ray, tetrahedron, q, length );

    BOOST_CHECK( !hit );
  }
  // Ray hitting on bottom of tetrahedron only bottom is surface
  {
    std::vector<bool> surface_map(4, false);

    surface_map[3] = true;

    V                const p   = V::make( 0.0, 0.0, -1.0);
    V                const r   = V::make( 0.2, 0.2,  1.0);
    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool const hit = geometry::compute_raycast_tetrahedron(ray, tetrahedron, q, length, surface_map );

    BOOST_CHECK( hit );
    BOOST_CHECK_CLOSE( length, tiny::norm(r), 0.01);
    BOOST_CHECK_CLOSE( q(0),  0.2, 0.01);
    BOOST_CHECK_CLOSE( q(1),  0.2, 0.01);
    BOOST_CHECK_CLOSE( q(2),  0.0, 0.01);
  }
  // Ray from above hitting bottom of tetrahedron only bottom is surface
  {
    std::vector<bool> surface_map(4, false);

    surface_map[3] = true;

    V                const p   = V::make( 0.0, 0.0,  1.0);
    V                const r   = V::make( 0.2, 0.2, -1.0);
    geometry::Ray<V> const ray = geometry::make_ray(p, r);

    T       length = VT::zero();
    V       q      = V::zero();

    bool const hit = geometry::compute_raycast_tetrahedron(ray, tetrahedron, q, length, surface_map );

    BOOST_CHECK( !hit );
  }

  // Stress testing... generating a bunch of random rays that all are hitting the oblique top plane
  for (unsigned int samples = 0u; samples < 1000u; ++samples)
  {
    V                const noise           = V::random(0.0, 1.0);
    T                const v1              = noise(0);
    T                const v2              = noise(1);
    T                const v3              = 1.0 - v1 - v2;
    T                const w1              = (v3 < 0.0) ?  v1/(1.0-v3) : v1;
    T                const w2              = (v3 < 0.0) ?  v2/(1.0-v3) : v2;
    T                const w3              = (v3 < 0.0) ?  0.0 : v3;

    if( w3==0.0) // We do not wish to test for exact edge cases --- they are sensitive to finite precision errors
      continue;
    if( w3==0.0)  // We do not wish to test for exact edge cases --- they are sensitive to finite precision errors
      continue;
    if( w3==0.0)  // We do not wish to test for exact edge cases --- they are sensitive to finite precision errors
      continue;

    BOOST_CHECK_CLOSE( (w1+w2+w3),  1.0, 0.01);
    
    BOOST_CHECK( w1 >= 0.0);
    BOOST_CHECK( w2 >= 0.0);
    BOOST_CHECK( w3 >= 0.0);

    BOOST_CHECK( w1 <= 1.0);
    BOOST_CHECK( w2 <= 1.0);
    BOOST_CHECK( w3 <= 1.0);

    V                const hit_point       = w1*p1 + w2*p2 + w3*p3;
    V                const ray_origin      = noise*10.0;
    V                const ray_direction   = hit_point - ray_origin;
    geometry::Ray<V> const ray             = geometry::make_ray(ray_origin, ray_direction);

    T       length = VT::zero();
    V       q      = V::zero();

    bool const hit = geometry::compute_raycast_tetrahedron(ray, tetrahedron, q, length );

    BOOST_CHECK( hit );
    BOOST_CHECK_CLOSE( length, tiny::norm(ray_direction), 0.01);

    for(unsigned int i = 0;i<3u;++i)
    {
      if(fabs(hit_point(i)) < tiny::working_precision<T>() )
        BOOST_CHECK_SMALL( q(i),  tiny::working_precision<T>());
      else
        BOOST_CHECK_CLOSE( q(i),  hit_point(i), 0.01);
    }
  }


}

BOOST_AUTO_TEST_SUITE_END();
