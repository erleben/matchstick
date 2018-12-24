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

BOOST_AUTO_TEST_CASE(tetrahedron_test)
{

  {
    geometry::Tetrahedron<V> A;
    BOOST_CHECK( geometry::is_valid(A) == true );

    BOOST_CHECK_CLOSE(A.point(0)(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(0)(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(0)(2), 0.0, 0.01 );

    BOOST_CHECK_CLOSE(A.point(1)(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(1)(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(1)(2), 0.0, 0.01 );

    BOOST_CHECK_CLOSE(A.point(2)(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(2)(1), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(2)(2), 0.0, 0.01 );

    BOOST_CHECK_CLOSE(A.point(3)(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(3)(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(3)(2), 1.0, 0.01 );

    T const vol = geometry::get_signed_volume(A);

    BOOST_CHECK_CLOSE(vol, 1.0/6.0, 0.01 );


    geometry::Triangle<V> const face_i = geometry::get_opposite_face(0, A);
    geometry::Triangle<V> const face_j = geometry::get_opposite_face(1, A);
    geometry::Triangle<V> const face_k = geometry::get_opposite_face(2, A);
    geometry::Triangle<V> const face_m = geometry::get_opposite_face(3, A);


    //  face opposite vertex 0:  123

    BOOST_CHECK_CLOSE(face_i.point(0)(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(face_i.point(0)(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(face_i.point(0)(2), 0.0, 0.01 );

    BOOST_CHECK_CLOSE(face_i.point(1)(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(face_i.point(1)(1), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(face_i.point(1)(2), 0.0, 0.01 );

    BOOST_CHECK_CLOSE(face_i.point(2)(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(face_i.point(2)(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(face_i.point(2)(2), 1.0, 0.01 );

    //  face opposite vertex 1:  032
    BOOST_CHECK_CLOSE(face_j.point(0)(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(face_j.point(0)(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(face_j.point(0)(2), 0.0, 0.01 );

    BOOST_CHECK_CLOSE(face_j.point(1)(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(face_j.point(1)(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(face_j.point(1)(2), 1.0, 0.01 );

    BOOST_CHECK_CLOSE(face_j.point(2)(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(face_j.point(2)(1), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(face_j.point(2)(2), 0.0, 0.01 );

    //  face opposite vertex 2:  013
    BOOST_CHECK_CLOSE(face_k.point(0)(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(face_k.point(0)(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(face_k.point(0)(2), 0.0, 0.01 );

    BOOST_CHECK_CLOSE(face_k.point(1)(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(face_k.point(1)(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(face_k.point(1)(2), 0.0, 0.01 );

    BOOST_CHECK_CLOSE(face_k.point(2)(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(face_k.point(2)(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(face_k.point(2)(2), 1.0, 0.01 );

    //  face opposite vertex 3:  021
    BOOST_CHECK_CLOSE(face_m.point(0)(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(face_m.point(0)(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(face_m.point(0)(2), 0.0, 0.01 );

    BOOST_CHECK_CLOSE(face_m.point(1)(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(face_m.point(1)(1), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(face_m.point(1)(2), 0.0, 0.01 );

    BOOST_CHECK_CLOSE(face_m.point(2)(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(face_m.point(2)(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(face_m.point(2)(2), 0.0, 0.01 );
  }

  {
    V const p0 = V::make(0.0, 0.0, 0.0);
    V const p1 = V::make(0.0, 1.0, 0.0);
    V const p2 = V::make(1.0, 0.0, 0.0);
    V const p3 = V::make(0.0, 0.0, 1.0);

    geometry::Tetrahedron<V> A = geometry::make_tetrahedron(p0,p1,p2,p3);
    BOOST_CHECK( geometry::is_valid(A) == true );

    BOOST_CHECK_CLOSE(A.point(0)(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(0)(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(0)(2), 0.0, 0.01 );

    BOOST_CHECK_CLOSE(A.point(1)(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(1)(1), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(1)(2), 0.0, 0.01 );

    BOOST_CHECK_CLOSE(A.point(2)(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(2)(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(2)(2), 0.0, 0.01 );

    BOOST_CHECK_CLOSE(A.point(3)(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(3)(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.point(3)(2), 1.0, 0.01 );

    T const vol = geometry::get_signed_volume(A);

    BOOST_CHECK_CLOSE(vol, -1.0/6.0, 0.01 );
  }

  {
    V const p0 = V::make(0.0, 0.0, 0.0);
    V const p1 = V::make(0.0, 1.0, 0.0);
    V const p2 = V::make(1.0, 0.0, 0.0);
    V const p3 = V::make(0.0, 0.0, 1.0);

    geometry::Tetrahedron<V> A = geometry::make_tetrahedron(p0,p1,p2,p3);

    V const v = V::make(-1.0, -1.0, -1.0);
    V const p = A.get_support_point(v);

    BOOST_CHECK_CLOSE(p(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(p(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(p(2), 0.0, 0.01 );
  }
  {
    V const p0 = V::make(0.0, 0.0, 0.0);
    V const p1 = V::make(0.0, 1.0, 0.0);
    V const p2 = V::make(1.0, 0.0, 0.0);
    V const p3 = V::make(0.0, 0.0, 1.0);

    geometry::Tetrahedron<V> A = geometry::make_tetrahedron(p0,p1,p2,p3);

    V const v = V::make( 1.0, 0.0, 0.0);
    V const p = A.get_support_point(v);

    BOOST_CHECK_CLOSE(p(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(p(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(p(2), 0.0, 0.01 );
  }
  {
    V const p0 = V::make(0.0, 0.0, 0.0);
    V const p1 = V::make(0.0, 1.0, 0.0);
    V const p2 = V::make(1.0, 0.0, 0.0);
    V const p3 = V::make(0.0, 0.0, 1.0);

    geometry::Tetrahedron<V> A = geometry::make_tetrahedron(p0,p1,p2,p3);

    V const v = V::make( 0.0, 0.0, 1.0);
    V const p = A.get_support_point(v);

    BOOST_CHECK_CLOSE(p(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(p(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(p(2), 1.0, 0.01 );
  }


}

BOOST_AUTO_TEST_SUITE_END();
