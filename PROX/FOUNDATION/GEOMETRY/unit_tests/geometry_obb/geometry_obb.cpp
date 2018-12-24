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

BOOST_AUTO_TEST_CASE(obb_test)
{

  {
    geometry::OBB<MT> A;
    BOOST_CHECK( geometry::is_valid(A) == true );

    BOOST_CHECK_CLOSE(A.center()(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.center()(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.center()(2), 0.0, 0.01 );

    BOOST_CHECK_CLOSE(A.half_extent()(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(A.half_extent()(1), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(A.half_extent()(2), 1.0, 0.01 );

    BOOST_CHECK_CLOSE(A.orientation().real(), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(A.orientation().imag()(0), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.orientation().imag()(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.orientation().imag()(2), 0.0, 0.01 );

    V const p000 = geometry::get_local_corner(0, A);
    V const p001 = geometry::get_local_corner(1, A);
    V const p010 = geometry::get_local_corner(2, A);
    V const p011 = geometry::get_local_corner(3, A);
    V const p100 = geometry::get_local_corner(4, A);
    V const p101 = geometry::get_local_corner(5, A);
    V const p110 = geometry::get_local_corner(6, A);
    V const p111 = geometry::get_local_corner(7, A);

    BOOST_CHECK_CLOSE(p000(0), -1.0, 0.01 );
    BOOST_CHECK_CLOSE(p000(1), -1.0, 0.01 );
    BOOST_CHECK_CLOSE(p000(2), -1.0, 0.01 );

    BOOST_CHECK_CLOSE(p001(0),  1.0, 0.01 );
    BOOST_CHECK_CLOSE(p001(1), -1.0, 0.01 );
    BOOST_CHECK_CLOSE(p001(2), -1.0, 0.01 );

    BOOST_CHECK_CLOSE(p010(0), -1.0, 0.01 );
    BOOST_CHECK_CLOSE(p010(1),  1.0, 0.01 );
    BOOST_CHECK_CLOSE(p010(2), -1.0, 0.01 );

    BOOST_CHECK_CLOSE(p011(0),  1.0, 0.01 );
    BOOST_CHECK_CLOSE(p011(1),  1.0, 0.01 );
    BOOST_CHECK_CLOSE(p011(2), -1.0, 0.01 );

    BOOST_CHECK_CLOSE(p100(0), -1.0, 0.01 );
    BOOST_CHECK_CLOSE(p100(1), -1.0, 0.01 );
    BOOST_CHECK_CLOSE(p100(2),  1.0, 0.01 );

    BOOST_CHECK_CLOSE(p101(0),  1.0, 0.01 );
    BOOST_CHECK_CLOSE(p101(1), -1.0, 0.01 );
    BOOST_CHECK_CLOSE(p101(2),  1.0, 0.01 );

    BOOST_CHECK_CLOSE(p110(0), -1.0, 0.01 );
    BOOST_CHECK_CLOSE(p110(1),  1.0, 0.01 );
    BOOST_CHECK_CLOSE(p110(2),  1.0, 0.01 );

    BOOST_CHECK_CLOSE(p111(0),  1.0, 0.01 );
    BOOST_CHECK_CLOSE(p111(1),  1.0, 0.01 );
    BOOST_CHECK_CLOSE(p111(2),  1.0, 0.01 );

  }

  {
    V const center = V::make(1.0, 2.0, 3.0);
    V const half_ext = V::make(4.0, 5.0, 6.0);
    Q const q    = Q::Rx(VT::pi_half());

    geometry::OBB<MT> A = geometry::make_obb<MT>(center, q, half_ext);

    BOOST_CHECK( geometry::is_valid(A) == true );

    BOOST_CHECK_CLOSE(A.center()(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(A.center()(1), 2.0, 0.01 );
    BOOST_CHECK_CLOSE(A.center()(2), 3.0, 0.01 );

    BOOST_CHECK_CLOSE(A.half_extent()(0), 4.0, 0.01 );
    BOOST_CHECK_CLOSE(A.half_extent()(1), 5.0, 0.01 );
    BOOST_CHECK_CLOSE(A.half_extent()(2), 6.0, 0.01 );

    BOOST_CHECK_CLOSE(A.orientation().real(), 0.707106769, 0.01 );
    BOOST_CHECK_CLOSE(A.orientation().imag()(0), 0.707106769, 0.01 );
    BOOST_CHECK_CLOSE(A.orientation().imag()(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(A.orientation().imag()(2), 0.0, 0.01 );

  }


  {
    V const center = V::make(1.0, 2.0, 3.0);
    V const half_ext = V::make(4.0, 5.0, 6.0);
    Q const q    = Q::Rx(VT::pi_half());

    geometry::OBB<MT> const A = geometry::make_obb<MT>(center, q, half_ext);
    geometry::OBB<MT> const B(A);
    geometry::OBB<MT> const C = B;

    BOOST_CHECK( geometry::is_valid(B) == true );
    BOOST_CHECK( geometry::is_valid(C) == true );

    BOOST_CHECK_CLOSE(B.center()(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(B.center()(1), 2.0, 0.01 );
    BOOST_CHECK_CLOSE(B.center()(2), 3.0, 0.01 );
    BOOST_CHECK_CLOSE(B.half_extent()(0), 4.0, 0.01 );
    BOOST_CHECK_CLOSE(B.half_extent()(1), 5.0, 0.01 );
    BOOST_CHECK_CLOSE(B.half_extent()(2), 6.0, 0.01 );
    BOOST_CHECK_CLOSE(B.orientation().real(), 0.707106769, 0.01 );
    BOOST_CHECK_CLOSE(B.orientation().imag()(0), 0.707106769, 0.01 );
    BOOST_CHECK_CLOSE(B.orientation().imag()(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(B.orientation().imag()(2), 0.0, 0.01 );

    BOOST_CHECK_CLOSE(C.center()(0), 1.0, 0.01 );
    BOOST_CHECK_CLOSE(C.center()(1), 2.0, 0.01 );
    BOOST_CHECK_CLOSE(C.center()(2), 3.0, 0.01 );
    BOOST_CHECK_CLOSE(C.half_extent()(0), 4.0, 0.01 );
    BOOST_CHECK_CLOSE(C.half_extent()(1), 5.0, 0.01 );
    BOOST_CHECK_CLOSE(C.half_extent()(2), 6.0, 0.01 );
    BOOST_CHECK_CLOSE(C.orientation().real(), 0.707106769, 0.01 );
    BOOST_CHECK_CLOSE(C.orientation().imag()(0), 0.707106769, 0.01 );
    BOOST_CHECK_CLOSE(C.orientation().imag()(1), 0.0, 0.01 );
    BOOST_CHECK_CLOSE(C.orientation().imag()(2), 0.0, 0.01 );
  }







}

BOOST_AUTO_TEST_SUITE_END();
