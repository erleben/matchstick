#ifndef TINY_QUATERNION_FUNCTIONS_H
#define TINY_QUATERNION_FUNCTIONS_H

#include <tiny_quaternion.h>
#include <tiny_vector.h>
#include <tiny_matrix.h>

#include <tiny_vector_functions.h>   // for cross and inner_prod
#include <tiny_is_number.h>

#include <tiny_quaternion_ops_policy.h>

#include <iosfwd>

namespace tiny
{
  /**
   * Rotation Matrix to Quaternion Conversion Factory Function. 
   * Quaternions can be used to represent rotations, so can matrices. This function converts matrices into quaternions.
   *
   * @param M       A reference to a matrix. This matrix should be a rotation matrix. That is an othogonal matrix.
   *
   * @return        A quaternion corresponding to the rotation specified by the rotation matrix.
   */
  template< typename T>
  Quaternion<T>  make(Matrix<3,3,T> const & M)
  {
    using std::sqrt;
    
    typedef typename T::real_type                  real_type;
    typedef typename Quaternion<T>::value_traits   value_traits;
    
    Quaternion<T> Q;
    
    real_type const & M00 = M(0,0);
    real_type const & M01 = M(0,1);
    real_type const & M02 = M(0,2);
    real_type const & M10 = M(1,0);
    real_type const & M11 = M(1,1);
    real_type const & M12 = M(1,2);
    real_type const & M20 = M(2,0);
    real_type const & M21 = M(2,1);
    real_type const & M22 = M(2,2);
    
    real_type tr = M00 + M11 + M22;
    real_type r;
    
    real_type const half = value_traits::one()/value_traits::two();
    
    if(tr>=value_traits::zero())
    {
      r           = sqrt(tr + value_traits::one());
      Q.real()    = half*r;
      r           = half/r;
      Q.imag()[0] = (M21 - M12) * r;
      Q.imag()[1] = (M02 - M20) * r;
      Q.imag()[2] = (M10 - M01) * r;
    }
    else
    {
      int i = 0;
      if(M11>M00)
        i = 1;
      if(M22>M(i,i))
        i = 2;
      switch(i)
      {
        case 0:
          r           = sqrt((M00 - (M11+M22)) + value_traits::one());
          Q.imag()[0] = half*r;
          r           = half/r;
          Q.imag()[1] = (M01 + M10) * r;
          Q.imag()[2] = (M20 + M02) * r;
          Q.real()    = (M21 - M12) * r;
          break;
        case 1:
          r           = sqrt((M11 - (M22+M00)) + value_traits::one());
          Q.imag()[1] = half*r;
          r           = half/r;
          Q.imag()[2] = (M12 + M21)*r;
          Q.imag()[0] = (M01 + M10)*r;
          Q.real()    = (M02 - M20)*r;
          break;
        case 2:
          r           = sqrt((M22 - (M00+M11)) + value_traits::one());
          Q.imag()[2] = half*r;
          r           = half/r;
          Q.imag()[0] = (M20 + M02) * r;
          Q.imag()[1] = (M12 + M21) * r;
          Q.real()    = (M10 - M01) * r;
          break;
      };
    }
    return Q;
  }
  
  template <typename T>
  inline Quaternion<T> operator*( Quaternion<T> const & q, typename T::real_type const & s )  {    return Quaternion<T>( q.real()*s, q.imag()*s);  }
  
  template <typename T>
  inline Quaternion<T> operator*( typename T::real_type const & s, Quaternion<T> const & q )  {    return Quaternion<T>( q.real()*s, q.imag()*s);  }
  
  template <typename T>
  inline Quaternion<T> operator/( Quaternion<T> const & q, typename T::real_type const & s )  {    return Quaternion<T>( q.real()/s, q.imag()/s);  }
  
//  template <typename T>
//  inline Quaternion<T> operator/( typename T::real_type const & s, Quaternion<T> const & q )  {    return Quaternion<T>( q.real()/s, q.imag()/s);  }

  template<typename T>
  inline Quaternion<T> operator + (Quaternion<T> const & lhs, Quaternion<T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Quaternion<T> > ops_policy;  
    return ops_policy::add(lhs,rhs);
  }
  
  template<typename T>
  inline Quaternion<T>& operator *= (Quaternion<T> & lhs, Quaternion<T> const & rhs)
  {
    typedef detail::QuaternionOpsPolicy< Quaternion<T> > ops_policy;
    return ops_policy::mul_assign(lhs,rhs);
  }
  
  template<typename T>
  inline Quaternion<T> operator * (Quaternion<T> const & lhs, Quaternion<T> const & rhs)
  {
    typedef detail::QuaternionOpsPolicy< Quaternion<T> > ops_policy;
    return ops_policy::mul(lhs,rhs);
  }
  
  template<typename T>
  inline Quaternion<T> prod(Quaternion<T> const & lhs, Quaternion<T> const & rhs)
  {
    return (lhs * rhs);
  }
  
  template<typename T>
  inline Quaternion<T> prod(Quaternion<T> const & a, Vector<3,T> const & b)
  {
    return Quaternion<T>(  - inner_prod(a.imag() , b),   cross(a.imag() , b) + b*a.real()  );
  }
  
  template<typename T>
  inline Quaternion<T> prod(Vector<3,T> const & a, Quaternion<T> const & b)
  {
    return Quaternion<T>( - inner_prod(a , b.imag()),  cross(a , b.imag()) + a*b.real()  );
  }
  
  template<typename T>
  inline Vector<3,T> rotate(Quaternion<T> const & q, Vector<3,T> const & r)
  {
    return prod(  prod(q , r)  , conj(q) ).imag();
  }
  
  template<typename T>
  inline typename T::real_type inner_prod ( Quaternion<T> const & q, Quaternion<T> const & r )
  {
    typedef detail::QuaternionOpsPolicy< Quaternion<T> > ops_policy;
    return ops_policy::inner_prod(q,r);
  }

  template<typename T>
  inline typename T::real_type norm ( Quaternion<T> const & q )
  {
    typedef detail::QuaternionOpsPolicy< Quaternion<T> > ops_policy;
    return ops_policy::norm(q);
  }
    
  template<typename T>
  inline Quaternion<T> conj ( Quaternion<T> const & q )
  {
    typedef detail::QuaternionOpsPolicy< Quaternion<T> > ops_policy;
    return ops_policy::conj(q);
  }

  template<typename T>
  inline Quaternion<T> unit ( Quaternion<T> const & q )
  {
    typedef detail::QuaternionOpsPolicy< Quaternion<T> > ops_policy;
    return ops_policy::unit(q);
  }

  /**
   * Natural Logarithm
   * Returns the Quaternion equal to the natural logarithm of
   * the specified Quaternion.
   *
   * @param q   A reference to an unit quaterion.
   * @return
   */      
  template<typename T>
  inline typename T::real_type log( Quaternion<T> const & q )
  {
    using std::acos;
    using std::sin;
    
    typedef typename T::real_type                 real_type;
    typedef typename Quaternion<T>::value_traits  value_traits;
        
    if(  q == Quaternion<T>(value_traits::one(), value_traits::zero(), value_traits::zero(),value_traits::zero())    )
      return Quaternion<T>(value_traits::zero(),value_traits::zero(),value_traits::zero(),value_traits::zero());
    
    real_type const theta    = value_traits::numeric_cast( acos( q.real() ) );
    real_type const stheta   = value_traits::numeric_cast( sin(theta)    );
    return Quaternion<T>(value_traits::zero(), q.imag()*(theta/stheta));
  }

  /**
   * Orthogonal Quaternion.
   * This method sets this Quaternion to an orthogonal Quaternion
   * of the specififed Quaternion. In otherwords the resulting
   * angle between the specified Quaternion and this Quaternion
   * is pi/2.
   */      
  template<typename T>
  inline Quaternion<T> hat(Quaternion<T> const & q)
  {
    return Quaternion<T> ( q.imag()(2), - q.imag()(1) , q.imag()(0), -q.real()); 
  }
  
  /**
   * Expoent
   * Sets the Quaternion equal to the expoent of
   * the specified Quaternion.
   *
   * @param q    A reference to a pure Quaternion (zero
   *             T part).
   */
  template<typename T>
  inline Quaternion<T> exp(Quaternion<T> const & q)
  {
    using std::sqrt;
    using std::cos;
    using std::sin;
    typedef typename T::real_type                 real_type;
    typedef typename Quaternion<T>::value_traits  value_traits;
    //--- teta^2 x^2 + teta^2 y^2 +teta^2 z^2 =
    //--- teta^2 (x^2 + y^2 + z^2) = teta^2
    real_type const teta = value_traits::numeric_cast(  sqrt(q.imag() *q.imag())  );
    real_type const ct   = value_traits::numeric_cast(  cos(teta)           );
    real_type const st   = value_traits::numeric_cast(  sin(teta)           );
    return Quaternion<T>(ct,q.imag()*st);
  }
  
  /**
   * Linear Interpolation of Quaterions.
   *
   * @param A		Quaternion A
   * @param B		Quaternion B
   * @param w    The weight
   *
   * @return     The resulting Quaternion, (1-w)*A + w*B. Note the resulting
   *             Quaternion may not be a unit quaterion eventhough A and B are
   *             unit quaterions. If a unit Quaternion is needed write
   *             unit(lerp(A,B,w)).
   *
   * @author Spreak
   */
  template<typename T>
  inline Quaternion<T> lerp(Quaternion<T> const & A,Quaternion<T> const & B, typename T::real_type const & w)
  {
    typedef typename Quaternion<T>::value_traits   value_traits;
    assert(w>=value_traits::zero() || !"lerp(): w must not be less than 0");
    assert(w<=value_traits::one()  || !"lerp(): w must not be larger than 1");	  
    typename T::real_type const mw = value_traits::one() - w; 
    return ((mw * A) + (w * B));
  }
  
  /**
   * Spherical Linear Interpolation of Quaterions.
   *
   * @param A
   * @param B
   * @param w        The weight
   *
   * @return          The resulting Quaternion.
   */
  template<typename T>
  inline Quaternion<T> slerp(Quaternion<T> const & A,Quaternion<T> const & B,typename T::real_type const & w)
  {
    typedef typename Quaternion<T>::value_traits   value_traits;
    typedef typename T::real_type                  real_type;
    
    using std::acos;
    using std::sin;
    
    assert(w>=value_traits::zero() || !"slerp(): w must not be less than 0");
    assert(w<=value_traits::one()  || !"slerp(): w must not be larger than 1");	  
    
    real_type const q_tiny = value_traits::numeric_cast( 10e-7 ); 
    real_type norm = inner_prod(A, B);
    
    bool flip = false;
    if( norm < value_traits::zero() )
    {
      norm = -norm;
      flip = true;
    }
    real_type weight = w;
    real_type inv_weight;
    if(value_traits::one() - norm < q_tiny)
    {
      inv_weight = value_traits::one() - weight;
    }
    else
    {
      real_type const theta = value_traits::numeric_cast( acos(norm)                                          );
      real_type const s_val = value_traits::numeric_cast( value_traits::one() / sin(theta)                    ); 
      inv_weight            = value_traits::numeric_cast( sin((value_traits::one() - weight) * theta) * s_val );
      weight                = value_traits::numeric_cast( sin(weight * theta) * s_val                         );
    }
    if(flip)
    {
      weight = -weight;
    }
    return ( inv_weight * A + weight * B);
  }
  
  /**
   * "Cubical" Sphereical Interpolation.
   * In popular terms this correpons to a cubic spline in
   * ordinary 3D space. However it is really a serie of
   * spherical linear interpolations, which defines a
   * cubic on the unit Quaternion sphere.
   *
   * @param q0
   * @param q1
   * @param q2
   * @param q3
   * @param u
   *
   * @return
   */
  template<typename T>
  inline Quaternion<T> squad(
                             Quaternion<T> const & q0
                             , Quaternion<T> const & q1
                             , Quaternion<T> const & q2
                             , Quaternion<T> const & q3
                             , typename T::real_type const & u
                             )
  {
    typedef typename T::real_type                  real_type;
    typedef typename Quaternion<T>::value_traits   value_traits;
    
    assert(u>=value_traits::zero() || !"squad(): u must not be less than 0");
    assert(u<=value_traits::one()  || !"squad(): u must not be larger than 1");	  
    
    real_type const u2 = value_traits::two() *u*(value_traits::one() -u); 
    return slerp( slerp(q0,q3,u), slerp(q1,q2,u), u2);
  }
    
  /**
   * Get Axis Angle Representation.
   * This function converts a unit-quaternion into the
   * equivalent angle-axis representation.
   *
   * @param Q       The quaternion
   * @param axis    Upon return this argument holds value of the equivalent rotation axis.
   * @param theta   Upon return this argument holds value of the equivalent rotation angle.
   */
  template<typename T>
  inline void get_axis_angle(Quaternion<T> const & Q,Vector<3,T> & axis, typename T::real_type & theta)
  {
    using std::atan2;
    
    typedef typename T::real_type                    real_type;
    typedef typename Quaternion<T>::value_traits     value_traits;
    typedef          Vector<3,T>                     V;
    //
    // By definition a unit quaternion Q can be written as
    //
    //    Q = [s,v] = [cos(theta/2), n sin(theta/2)]
    //
    // where n is a unit vector. This is the same as a rotation of
    // theta radian around the axis n.
    //
    //
    // Rotations are difficult to work with for several reasons.
    //
    // Firstly both Q and -Q represent the same rotation. This is
    // easily proven, rotate a arbitary vector r by Q then we have
    //
    //   r^\prime = Q r Q^*
    //
    // Now rotate the same vector by -Q
    //
    //   r^\prime = (-Q) r (-Q)^* = Q r Q^*
    //
    // because -Q = [-s,-v] and (-Q)^* = [-s , v] = - [s,-v]^* = - Q^*.
    //
    // Thus the quaternion representation of a single rotation is not unique.
    //
    // Secondly the rotation it self is not well-posed. A rotation of theta
    // radians around the unit axis n could equally well be done as a rotation
    // of -theta radians around the negative unit axis n.
    //
    // This is seen by straightforward substitution
    //
    //  [ cos(-theta/2), sin(-theta/2) (-n) ] = [ cos(theta/2), sin(theta/2) n ]
    // 
    // Thus we get the same quaternion regardless of whether we
    // use (+theta,+n) or (-theta,-n).
    //
    //
    // From the Quaternion we see that
    //
    //   \frac{v}{\norm{v}}  = \frac{ sin(theta/2) n }{| sin(theta/2) | } = sign(sin(theta/2)) n
    //
    // Thus we can easily get the rotation axis. However, we can not immediately
    // determine the positive rotation axis direction. The problem boils down to the
    // fact that we can not see the sign of the sinus-factor.
    //
    // Let us proceed by setting
    //
    //   x =    cos(theta/2)   =  s
    //   y =  | sin(theta/2) | =  \norm{v}
    //
    // Then we basically have two possibilities for finding theta
    //
    //  theta_1 = 2 atan2( y, x)        equivalent to      sign(sin(theta/2)) = 1
    //
    // or 
    //
    //  theta_2 = 2 atan2( -y, x)       equivalent to      sign(sin(theta/2)) = -1
    //
    // If theta_1 is the solution we have
    //
    //  n = \frac{v}{\norm{v}}
    //
    // If theta_2 is the solution we must have
    //
    //  n = - \frac{v}{\norm{v}}
    //
    // Observe that we always have theta_2 = 2 pi - theta_1. Therefore theta_1 < theta_2.
    //
    // Let us imagine that we always choose $theta_1$ as the solution then
    // the correspoding quaternion for that solution would be
    //
    //         Q_1 = [cos(theta_1/2),  sin(theta_1/2)   \frac{v}{\norm{v}}]
    //             = [s ,  \norm{v}   \frac{v}{\norm{v}}] 
    //             = Q
    //
    // Now if we choose theta_2 as the solution we would have
    //
    //         Q_2 = [cos(theta_2/2),  sin(theta_2/2)   -\frac{v}{\norm{v}}]
    //             = [s ,  -\norm{v}   -\frac{v}{\norm{v}}] 
    //             = [s ,  \norm{v}   \frac{v}{\norm{v}}] 
    //             = Q
    //
    // Thus we observe that regardless of which solution we pick we always have Q = Q_1 = Q_2.
    //
    // At this point one may be confused. However, it should be clear that theta_2 is equivalent
    // to the theta_1 rotation. The difference is simply that theta_2 corresponds to flipping the
    // rotation axis of the theta_1 case.
    //
    real_type const ct2   = Q.real();           //---   cos(theta/2)
    real_type const st2   = norm( Q.imag() );   //---  |sin(theta/2)|
    
    theta = value_traits::two()* atan2(st2,ct2);
    
    assert( st2 >= value_traits::zero()   || !"get_axis_angle(): |sin(theta/2)| must be non-negative");
    assert( theta >= value_traits::zero() || !"get_axis_angle(): theta must be non-negative");
    assert( is_number(theta)              || !"get_axis_angle(): NaN encountered");
    
    axis = st2 > value_traits::zero() ? Q.imag() / st2 : V( value_traits::zero() );
  }
  
  /**
   *  Get Rotation Angle wrt. an Axis.
   * Think of it as if you have a fixated body A so only body B is allowd to move.
   *
   * Let BF_B' indicate the initial orientation of body B's body frame
   * and BF_B the current orientation, now BF_A should be thought of as
   * being immoveable ie. constant.
   *
   *   Q_initial : BF_A -> BF_B'
   *   Q_cur     : BF_A -> BF_B
   *
   * Now we must have the relation
   *
   *   Q_rel Q_initial = Q_cur
   *
   * From which we deduce
   *
   *   Q_rel = Q_cur conj(Q_initial)
   *
   * And we see that
   *
   *   Q_rel : BF_B' -> BF_B
   *
   * That is how much the body frame of body B have rotated (measured
   * with respect to the fixed body frame A).
   *
   *
   * @param Q_rel    Rotation from initial orientation of B to current orientation of B.
   * @param axis     The rotation axis in the body frame of body B.
   *
   * @return     The angle in radians.
   */
  template<typename T>
  inline T get_angle(Quaternion<T> const & Q_rel, Vector<3,T> const & axis)
  {
    typedef typename T::real_type                  real_type;
    typedef typename Quaternion<T>::value_traits   value_traits;
    
    using std::atan2;
    
    //--- The angle between the two bodies is extracted from the Quaternion Q_rel
    //---
    //---    [s,v] = [ cos(theta/2) , sin(theta/2) * u ]
    //---
    //--- where s is a value_type and v is a 3-vector. u is a unit axis and
    //--- theta is a rotation along that axis.
    //---
    //--- we can get theta/2 by:
    //---
    //---    theta/2 = atan2 ( sin(theta/2) , cos(theta/2) )
    //---
    //--- but we can not get sin(theta/2) directly, only its absolute value:
    //---
    //---    |v| = |sin(theta/2)| * |u|  = |sin(theta/2)|
    //---
    //--- using this value will have a strange effect.
    //---
    //--- Recall that there are two Quaternion representations of a given
    //--- rotation, q and -q.
    //---
    //--- Typically as a body rotates along the axis it will go through a
    //--- complete cycle using one representation and then the next cycle
    //--- will use the other representation.
    //---
    //--- This corresponds to u pointing in the direction of the joint axis and
    //--- then in the opposite direction. The result is that theta
    //--- will appear to go "backwards" every other cycle.
    //---
    //--- Here is a fix: if u points "away" from the direction of the joint
    //--- axis (i.e. more than 90 degrees) then use -q instead of q. This
    //--- represents the same rotation, but results in the cos(theta/2) value
    //--- being sign inverted.
    real_type const ct2 = Q_rel.real();           //---   cos(theta/2)
    real_type const st2 = norm( Q_rel.imag() );   //---  |sin(theta/2)|
    real_type theta = value_traits::zero();
    
    //--- Remember that Q_rel : BF_B' -> BF_B, so we need the axis in body B's local frame
    if( Q_rel.imag() * axis  >= value_traits::zero())
    {
      //--- u points in direction of axis.
      theta = value_traits::two()* atan2(st2,ct2);
    }
    else
    {
      //--- u points in opposite direction.
      theta = value_traits::two() * atan2(st2,-ct2);
    }
    //--- The angle we get will be between 0..2*pi, but we want
    //--- to return angles between -pi..pi
    if (theta > value_traits::pi())   
      theta -= value_traits::two()*value_traits::pi();
    
    //--- The angle we've just extracted has the wrong sign (Why???).
    theta = -theta;
    //
    // Say we have a rotation, R, relating the coordinates of two frames X and Y, now
    // let the coordinates of the vector v be given in frame X as
    //
    //    [v]_X
    //
    // And in frame Y as
    //
    //    [v]_Y
    //
    // Now R is defined such that
    //
    //    R [v]_X = [v]_Y
    //
    // That is it chagnes the coordinates of v from X to Y.
    //
    // This is pretty straightforward, but there is some subtlety in it, say
    // frame Y is rotated theta radians around the z-axis, then the rotation
    // matrix relating the coordinates is the opposite rotation.
    //
    // What we want to measure is how much the frame axis have rotated, but
    // what we are given is a rotation transforming coordinates, therefor
    // we need to flip the sign of the extracted angle!!!
    return theta;
  }
  
  template<typename T>
  inline std::ostream & operator<< (std::ostream & o,Quaternion<T> const & q)
  {
    o << "[" << q.real() << "," << q.imag()(0) << "," << q.imag()(1) << "," << q.imag()(2) << "]";
    return o;
  }
  
  template<typename T>
  inline std::istream & operator>>(std::istream & i,Quaternion<T> & q)
  {
    char dummy;
    i >> dummy;
    i >> q.real();
    i >> dummy;
    i >> q.imag()(0);
    i >> dummy;
    i >> q.imag()(1);
    i >> dummy;
    i >> q.imag()(2);
    i >> dummy;
    return i;
  }
  
  
} // namespace tiny

// TINY_QUATERNION_FUNCTIONS_H
#endif 

