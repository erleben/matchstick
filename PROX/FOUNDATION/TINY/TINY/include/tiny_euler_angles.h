#ifndef TINY_EULER_ANGLES_H
#define TINY_EULER_ANGLES_H

#include <tiny_matrix.h>
#include <tiny_quaternion.h>
#include <tiny_functions.h>  // for clamp
#include <tiny_is_number.h>
#include <tiny_is_finite.h>
#include <cmath>            // for cos, atan2, and asin
#include <cassert>

namespace tiny
{
  
  /**
   * Computes Euler Angles.
   * The resulting order is: xyz, i.e. first x, then y and finally z
   *
   *
   * @param R    A rotation matrix
   *
   * @param rx   Upon return holds the rotation around x-axis in radians.
   * @param ry   Upon return holds the rotation around y-axis in radians.
   * @param rz   Upon return holds the rotation around z-axis in radians.
   *
   * @return     If return value is true then we have experienced gimabll lock
   *             and can not gurantee unqiue Euler angles for rx and rz.
   */
  template<typename TT>
  inline bool XYZ_euler_angles(
                               tiny::Matrix<3u,3u,TT> const & R
                               , typename tiny::Matrix<3u,3u,TT>::real_type & rx
                               , typename tiny::Matrix<3u,3u,TT>::real_type & ry
                               , typename tiny::Matrix<3u,3u,TT>::real_type & rz
                               )
  {
    using std::atan2;
    using std::asin;
    
    typedef typename tiny::Matrix<3u,3u,TT>::real_type     T;
    typedef typename tiny::Matrix<3u,3u,TT>::value_traits  VT;
    

    //
    // We know
    //
    //               | 1   0    0  |
    //          Rx = | 0   cx  -sx |
    //               | 0   sx   cx |
    //
    //               |  cy   0   sy |
    //          Ry = |  0    1    0 |
    //               | -sy   0   cy |
    //
    //               | cz   -sz   0 |
    //          Rz = | sz    cz   0 |
    //               | 0     0    1 |
    //
    //             R =  Rz Ry Rx
    //
    //       | cy*cz           cz*sx*sy - cx*sz      cx*cz*sy + sx*sz |
    //  R =  | cy*sz           cx*cz + sx*sy*sz     -cz*sx + cx*sy*sz |
    //       |  -sy              cy*sx                   cx*cy        |
    //

    T const & r00 = R(0,0);
    T const & r01 = R(0,1);
    T const & r02 = R(0,2);
    T const & r10 = R(1,0);
    T const & r20 = R(2,0);
    T const & r21 = R(2,1);
    T const & r22 = R(2,2);

    // We observe
    //
    //      atan2(y,x) =  atan2(sin(theta), cos(theta) ) =  theta
    //
    //

    bool gimbal_lock = false;

    if ( r20 >= VT::one() )
    {
      // Since r20 = -sy this must mean sy = -1 and so we must have
      // ry = -pi/2 and from this we conclude cy = 0
      //
      // Looking at
      //
      //   [r01 r02] =  [ cz*sx*sy-cx*sz       cx*cz*sy+sx*sz ]
      //
      // And using the knowledge that sy = -1 and cy = 0 we reduce to
      //
      //   [r01 r02] =  [ -cz*sx - cx*sz       -cx*cz + sx*sz ]
      //
      // For this specific value of ry we have gimball lock as x and z axes are
      // aligned. Hence, we pick rx = 0 from which it follows that cx = 1 and
      // sx = 0. Using this knowledge we find
      //
      //   [r01 r02] =  [ - sz       -cz ]
      //
      // From which we notice that   rz = atan2(   -r01, - r02 )
      //
      rz =   VT::numeric_cast( atan2(- r01, -r02) );
      ry = - VT::pi_2();
      rx =   VT::zero();
      gimbal_lock = true;
    }
    else if ( r20 <= -VT::one() )
    {
      // Since r20 = -sy this must mean sy = 1 and so we must have ry = pi/2 and from this we conclude cy = 0
      //
      // Looking at
      //
      //   [r01 r02] =  [ cz*sx*sy-cx*sz       cx*cz*sy+sx*sz ]
      //
      // And using the knowledge that sy = 1 and cy = 0 we reduce to
      //
      //   [r01 r02] =  [ cz*sx - cx*sz       cx*cz + sx*sz ]
      //
      // For this specific value of ry we have gimball lock as x and z axes are
      // aligned. Hence, we pick rx = 0 from which it follows that cx = 1 and
      // sx = 0. Using this knowledge we find
      //
      //   [r01 r02] =  [ -sz    cz ]
      //
      // From which we notice that   rz = atan2(   -r01, r02 )
      //
      rz =   VT::numeric_cast( atan2(-r01, r02) );
      ry =   VT::pi_2();
      rx =   VT::zero();
      gimbal_lock = true;
    }
    else
    {
      rz = VT::numeric_cast( atan2(r10, r00) );
      ry = VT::numeric_cast( asin(-r20)      );
      rx = VT::numeric_cast( atan2(r21, r22) );
      gimbal_lock = false;
    }

    assert( is_number(rx)      || !"XYZ_euler_angles(): nan");
    assert( is_number(ry)      || !"XYZ_euler_angles(): nan");
    assert( is_number(rz)      || !"XYZ_euler_angles(): nan");

    assert( is_finite(rx)      || !"XYZ_euler_angles(): inf");
    assert( is_finite(ry)      || !"XYZ_euler_angles(): inf");
    assert( is_finite(rz)      || !"XYZ_euler_angles(): inf");

    assert(  rx >= -VT::pi()   || !"XYZ_euler_angles(): out of range");
    assert(  ry >= -VT::pi_2() || !"XYZ_euler_angles(): out of range");
    assert(  rz >= -VT::pi()   || !"XYZ_euler_angles(): out of range");
    assert(  rx <=  VT::pi()   || !"XYZ_euler_angles(): out of range");
    assert(  ry <=  VT::pi_2() || !"XYZ_euler_angles(): out of range");
    assert(  rz <=  VT::pi()   || !"XYZ_euler_angles(): out of range");

    return gimbal_lock;
  }
  
  /**
   * Computes ZYZ Euler Angles.
   *
   * @param Q    A quaternion representing some given orientation for which we wish to find the corresponding ZYZ Euler parameters.
   *
   * @param phi    The first rotation around the Z-axis.
   * @param psi    The rotation around the Y-axis.
   * @param theta  The first rotation around the Z-axis.
   *
   * @return     If return value is true then we have computed unique euler angle
   *             values otherwise our solution might not be unique
   */
  template<typename TT>
  inline void ZYZ_euler_angles(
                               tiny::Quaternion<TT> const & Q
                               , typename tiny::Quaternion<TT>::real_type & phi
                               , typename tiny::Quaternion<TT>::real_type & psi
                               , typename tiny::Quaternion<TT>::real_type & theta
                               )
  {
    using std::atan2;
    using std::cos;

    typedef          tiny::Quaternion<TT>                  QT;
    typedef typename tiny::Quaternion<TT>::vector3_type    V;
    typedef typename tiny::Quaternion<TT>::value_traits    VT;
    typedef typename tiny::Quaternion<TT>::real_type       T;
    
    phi   = VT::zero();
    psi   = VT::zero();
    theta = VT::zero();
    
    // Here phi, psi and theta defines the relative rotation, Q, such that
    //
    // Q ~ Rz( phi )*Ry( psi )*Rz(theta);
    //
    // Our taks is to find phi, psi, and theta given Q
    //
    // We exploit the following idea below to reduce the problem. We
    // use a clever test-vector, k = [0 0 1]^T and try to rotate this
    // vector with the given rotation. That is
    //
    // Q k Q^*  = Rz( phi )*Ry( psi )*Rz(theta) k =  Rz( phi )*Ry( psi ) k;
    //
    // denoting Q k Q^* = u, a unit vector, we no longer need to worry about theta.

    V const u = rotate( Q, V::k() );

    // Now we must have
    //
    //  u = Rz(phi) Ry(psi) [0 0 1]^T
    //
    // From this we must have
    //
    //  | u_x |   |  cos(phi) -sin(phi) 0 | |  cos(psi) 0 sin(psi) |   |0|
    //  | u_y | = |  sin(phi)  cos(phi) 0 | |  0        1 0        |   |0|
    //  | u_z |   |  0         0        1 | | -sin(psi) 0 cos(psi) |   |1|
    //
    // Multipling once
    //
    //  | u_x |   |  cos(phi) -sin(phi) 0 | |  sin(psi) |  
    //  | u_y | = |  sin(phi)  cos(phi) 0 | |   0       | 
    //  | u_z |   |  0         0        1 | |  cos(psi) |  
    //
    // Multipling twice
    //
    //  | u_x |   |  cos(phi)  sin(psi) |  
    //  | u_y | = |  sin(phi)  sin(psi) | 
    //  | u_z |   |  cos(psi)           |  
    //
    // From the third equation we solve
    //
    //  psi = acos( u_z )
    //
    // This forces psi to always be in the internval [0..pi].
    //
    T const u_z = clamp( u(2), -VT::one(), VT::one());
    assert(is_number(u_z)               || !"ZYZ_euler_angles(): not an number encountered" );
    assert(u_z <= VT::one()             || !"ZYZ_euler_angles(): u_z was too big"           );
    assert(u_z >= -VT::one()            || !"ZYZ_euler_angles(): u_z was too small"         );
    
    psi = VT::numeric_cast( acos(u_z)   );
    assert(is_number(psi)               || !"ZYZ_euler_angles(): psi was not an number encountered");
    assert(psi <= VT::pi()              || !"ZYZ_euler_angles(): psi was too big"                  );
    assert(psi >= VT::zero()            || !"ZYZ_euler_angles(): psi was too small"                );
    //
    // We know that sin(psi) is allways going to be positive, which mean
    // that we can divide the second equation by the first equation and
    // obtain
    //
    //  sin(phi)/cos(phi) = tg(phi) = u_y/u_x
    //
    // From this we have
    //
    //  phi = arctan2( u_y, u_x )
    //
    // That means that phi will always be in the interval [-pi..pi].
    //
    // Observe that if psi is zero then u_y and u_x is both zero and our
    // approach will alway compute phi to be the value zero. The case
    // is actually worse than it seems. Because with psi is zero ZYZ are
    // in a gimbal lock where the two Z-axis transformations are completely
    // aligned.
    //
    //
    T const too_small = VT::numeric_cast( 0.0001 );
    if(psi<too_small)
    {
      //
      // Our solution is to use another clever test vector 
      //
      V const w = rotate( Q, V::i());
      
      T const w_x = w(0);
      T const w_y = w(1);

      assert(is_number(w_x)   || !"ZYZ_euler_angles(): w_x was not an number encountered");
      assert(is_number(w_y)   || !"ZYZ_euler_angles(): w_y not an number encountered");

      phi = VT::numeric_cast( atan2(w_y,w_x) );

      assert(is_number(phi)   || !"ZYZ_euler_angles(): phi was not an number encountered");
      assert(phi <=  VT::pi() || !"ZYZ_euler_angles(): phi was too big");
      assert(phi >= -VT::pi() || !"ZYZ_euler_angles(): phi was too small");
      
      //
      // psi was too close to zero so we are in a gimbal lock, we simply keep theta zero
      //
      return;
    }
    else
    {
      // We are not close to gimbal lock, so we can safely
      T const u_x = u(0);
      T const u_y = u(1);

      assert(is_number(u_x)   || !"ZYZ_euler_angles(): u_x was not an number encountered");
      assert(is_number(u_y)   || !"ZYZ_euler_angles(): u_y not an number encountered");

      phi = VT::numeric_cast( atan2(u_y,u_x) );

      assert(is_number(phi)   || !"ZYZ_euler_angles(): phi was not an number encountered");
      assert(phi <=  VT::pi() || !"ZYZ_euler_angles(): phi was too big");
      assert(phi >= -VT::pi() || !"ZYZ_euler_angles(): phi was too small");
    }
    
    
    //
    // So now we have 
    //
    //   Qzy =~ Rz( phi )*Ry( psi );
    //
    // and from this we know
    //
    //   Q = Qzy Qz(theta);
    //
    // so
    //
    //  (Qzy^* Q) = Qz(theta)
    //
    // We also know
    //
    //  (Qzy^* Q) = [s,v] = [ cos(theta/2) , sin(theta/2) * k ]
    //
    // where s is a value_type and v is a 3-vector. k is a unit z-axis and
    // theta is a rotation along that axis.
    //
    // we can get theta/2 by:
    //
    //   theta/2 = atan2 ( sin(theta/2) , cos(theta/2) )
    //
    // but we can not get sin(theta/2) directly, only its absolute value:
    //
    //   |v| = |sin(theta/2)| * |k|  = |sin(theta/2)|
    //
    QT Qy;
    QT Qz;
    QT H;

    Qy = QT::Ry(psi);
    Qz = QT::Rz(phi);
    H = prod( conj( prod( Qz , Qy ) ), Q );
    
    V const w = rotate(H,V::i());
    
    T const w_x = w(0);
    T const w_y = w(1);

    assert(is_number(w_x)     || !"ZYZ_euler_angles(): w_x was not an number encountered");
    assert(is_number(w_y)     || !"ZYZ_euler_angles(): w_y not an number encountered"    );

    theta = VT::numeric_cast( atan2(w_y,w_x) );

    assert(is_number(theta)   || !"ZYZ_euler_angles(): phi was not an number encountered");
    assert(theta <=  VT::pi() || !"ZYZ_euler_angles(): phi was too big"                  );
    assert(theta >= -VT::pi() || !"ZYZ_euler_angles(): phi was too small"                );
    
    //T ct2 = Q.s();       //---   cos(theta/2)
    //T st2 = norm( v ); //---  |sin(theta/2)|
    
    //// First try positive choice of sin(theta/2)
    //theta = value_traits::two()* atan2(st2,ct2);
    
    return;
  }
  
} // namespace tiny

//TINY_EULER_ANGLES_H
#endif
