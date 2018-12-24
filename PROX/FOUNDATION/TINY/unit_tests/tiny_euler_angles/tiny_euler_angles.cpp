#include <tiny_math_types.h>
#include <tiny_type_traits.h>
#include <tiny_quaternion.h>
#include <tiny_quaternion_functions.h>
#include <tiny_matrix_functions.h>
#include <tiny_euler_angles.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>


typedef tiny::MathTypes<double>  MT;
typedef MT::quaternion_type      Q;
typedef MT::matrix3x3_type       M;
typedef MT::real_type            T;
typedef MT::value_traits         VT;


void do_zyz_test( T const & phi_in, T const & psi_in, T const & theta_in)
{
  Q Q_in;
  Q Q_out;
  Q identity = Q::identity();
  Q Qz1;
  Q Qy;
  Q Qz2;

  T const too_small = 10e-7;

  T phi_out   = 0.0;
  T psi_out   = 0.0;
  T theta_out = 0.0;

  Qz1 = Q::Rz(theta_in);
  Qy = Q::Ry(psi_in);
  Qz2 = Q::Rz(phi_in);

  Q_in = tiny::prod( Qz2 , tiny::prod( Qy , Qz1) );

  tiny::ZYZ_euler_angles(Q_in,phi_out,psi_out,theta_out);

  if(psi_in > 0.0)
  {
    // we only want to do this if we are not in a gimbal lock
    Qz1 = Q::Rz(theta_out);
    Qy = Q::Ry(psi_out);
    Qz2 = Q::Rz(phi_out);
    Q_out = tiny::prod( Qz2 , tiny::prod( Qy , Qz1) );
    identity = tiny::prod( tiny::conj(Q_out), Q_in );

    T const s = fabs( fabs(identity.real())  - 1.0 );
    T const v0 = fabs(identity.imag()(0));
    T const v1 = fabs(identity.imag()(1));
    T const v2 = fabs(identity.imag()(2));

    BOOST_CHECK( s < too_small);
    BOOST_CHECK( v0 < too_small);
    BOOST_CHECK( v1 < too_small);
    BOOST_CHECK( v2 < too_small);

    T const dphi = fabs(phi_in - phi_out);
    T const dpsi = fabs(psi_in - psi_out);
    T const dtheta = fabs(theta_in - theta_out);
    BOOST_CHECK( dphi < too_small);
    BOOST_CHECK( dpsi < too_small);
    BOOST_CHECK( dtheta < too_small);
  }
  else
  {
    // In gimbal lock phi and theta behaves strangely
    BOOST_CHECK_CLOSE( 0.0, theta_out, 0.01);
    double const dpsi = fabs(psi_out);
    BOOST_CHECK( dpsi < too_small);

    T new_phi = phi_in + theta_in;
    T const pi     = 3.1415926535897932384626433832795;
    T const two_pi = 2.0*pi;
    while(new_phi>pi) new_phi -= two_pi;
    while(new_phi<-pi) new_phi += two_pi;

    T const dphi = fabs(new_phi - phi_out);
    BOOST_CHECK( dphi < too_small);
  }
}


void SAFE_CHECK_CLOSE( double const & l, double const & r, double const tol  )
{
  using std::fabs;

  double const ALMOST_ZERO = 10e-10;

  if( fabs(l) < ALMOST_ZERO || fabs(r) < ALMOST_ZERO )
    BOOST_CHECK( fabs(l-r) < ALMOST_ZERO );
  else
    BOOST_CHECK_CLOSE(l, r, tol);
}


void do_xyz_test( T const & alpha_in, T const & beta_in, T const & gamma_in)
{
  // Q Qx   = Q::Rx(alpha_in);
  // Q Qy   = Q::Ry(beta_in);
  // Q Qz   = Q::Rz(gamma_in);
  // Q Q_in = tiny::prod( Qz, tiny::prod( Qy , Qx) );
  // M R_in = tiny::make(Q_in);

  M const Rx_in = M::Rx(alpha_in);
  M const Ry_in = M::Ry(beta_in);
  M const Rz_in = M::Rz(gamma_in);
  M const R_in  = Rz_in * Ry_in * Rx_in;

  double alpha_out = 0.0;
  double beta_out  = 0.0;
  double gamma_out = 0.0;

  tiny::XYZ_euler_angles(R_in, alpha_out, beta_out, gamma_out);

  M const Rx_out = M::Rx(alpha_out);
  M const Ry_out = M::Ry(beta_out);
  M const Rz_out = M::Rz(gamma_out);
  M const R_out  = Rz_out * Ry_out * Rx_out;

  {
    SAFE_CHECK_CLOSE(R_in(0,0), R_out(0,0), 0.1);
    SAFE_CHECK_CLOSE(R_in(0,1), R_out(0,1), 0.1);
    SAFE_CHECK_CLOSE(R_in(0,2), R_out(0,2), 0.1);
    SAFE_CHECK_CLOSE(R_in(1,0), R_out(1,0), 0.1);
    SAFE_CHECK_CLOSE(R_in(1,1), R_out(1,1), 0.1);
    SAFE_CHECK_CLOSE(R_in(1,2), R_out(1,2), 0.1);
    SAFE_CHECK_CLOSE(R_in(2,0), R_out(2,0), 0.1);
    SAFE_CHECK_CLOSE(R_in(2,1), R_out(2,1), 0.1);
    SAFE_CHECK_CLOSE(R_in(2,2), R_out(2,2), 0.1);
  }

}


BOOST_AUTO_TEST_SUITE(tiny_euler_angles);

BOOST_AUTO_TEST_CASE(ZYZ)
{
  size_t N = 15;

  double const pi     = 3.1415926535897932384626433832795;
  double const two_pi = 2.0*pi;
  double const delta = (two_pi)/(N-1);

  double phi = -pi+delta;
  for(;phi<pi;)
  {
    double psi = 0.0;
    for(;psi<pi;)
    {
      double theta = -pi+delta;
      for(;theta<pi;)
      {
        do_zyz_test( phi, psi, theta );
        theta += delta;
      }
      psi += delta;
    }
    phi += delta;
  }
}


BOOST_AUTO_TEST_CASE(XYZ)
{
  double const pi     = VT::pi();
  unsigned int N      = 40;
  double const dpi    = 2.0*pi/(N-1);

  for (unsigned int i = 0; i < N;++i)
  {
    for (unsigned int j = 0; j < N;++j)
    {
      for (unsigned int k = 0; k < N;++k)
      {
        double const alpha = -pi + i*dpi;
        double const beta  = (-pi + j*dpi)/2.0;
        double const gamma = -pi + k*dpi;

        do_xyz_test( alpha, beta, gamma );
      }
    }
  }
}


BOOST_AUTO_TEST_SUITE_END();
