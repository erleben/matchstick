#include <mass.h>

#include <tiny_is_finite.h>
#include <tiny_is_number.h>
#include <tiny_value_traits.h>
#include <tiny_precision.h>

#include <tiny_math_types.h>
#include <tiny_matrix_functions.h>       // needed for R = tiny::make( Q )
#include <tiny_quaternion_functions.h>   // needed for Q = tiny::make( R )
#include <tiny_eigen3x3.h>

#include <cassert>
#include <cmath>                         // needed for std::fabs

namespace mass
{
  
  namespace detail
  {
    template<typename T>
    void rotate(
                T const & R00
                , T const & R01
                , T const & R02
                , T const & R10
                , T const & R11
                , T const & R12
                , T const & R20
                , T const & R21
                , T const & R22
                , T & Ixx
                , T & Iyy
                , T & Izz
                , T & Ixy
                , T & Ixz
                , T & Iyz                      
                )
    {
      using namespace tiny;
      using std::fabs;
      
      typedef ValueTraits<T> value_traits;
      
      assert( is_number( R00 )  || !"R00 must be a number"       );
      assert( is_finite( R00 )  || !"R00 must be a finite number");      
      assert( is_number( R01 )  || !"R01 must be a number"       );
      assert( is_finite( R01 )  || !"R01 must be a finite number");      
      assert( is_number( R02 )  || !"R02 must be a number"       );
      assert( is_finite( R02 )  || !"R02 must be a finite number");      
      assert( is_number( R10 )  || !"R10 must be a number"       );
      assert( is_finite( R10 )  || !"R10 must be a finite number");      
      assert( is_number( R11 )  || !"R11 must be a number"       );
      assert( is_finite( R11 )  || !"R11 must be a finite number");      
      assert( is_number( R12 )  || !"R12 must be a number"       );
      assert( is_finite( R12 )  || !"R12 must be a finite number");      
      assert( is_number( R20 )  || !"R20 must be a number"       );
      assert( is_finite( R20 )  || !"R20 must be a finite number");      
      assert( is_number( R21 )  || !"R21 must be a number"       );
      assert( is_finite( R21 )  || !"R21 must be a finite number");      
      assert( is_number( R22 )  || !"R22 must be a number"       );
      assert( is_finite( R22 )  || !"R22 must be a finite number");      
      assert( is_number( Ixx )  || !"Ixx must be a number"       );
      assert( is_finite( Ixx )  || !"Ixx must be a finite number");      
      assert( is_number( Iyy )  || !"Iyy must be a number"       );
      assert( is_finite( Iyy )  || !"Iyy must be a finite number");      
      assert( is_number( Izz )  || !"Izz must be a number"       );
      assert( is_finite( Izz )  || !"Izz must be a finite number");      
      assert( is_number( Ixy )  || !"Ixy must be a number"       );
      assert( is_finite( Ixy )  || !"Ixy must be a finite number");      
      assert( is_number( Ixz )  || !"Ixz must be a number"       );
      assert( is_finite( Ixz )  || !"Ixz must be a finite number");      
      assert( is_number( Iyz )  || !"Iyz must be a number"       );
      assert( is_finite( Iyz )  || !"Iyz must be a finite number");      
      
      assert(  fabs(R00*R00 + R01*R01 + R02*R02 - value_traits::one()) <= working_precision<T>() || !"R is not special orthonormal matrix"); 
      assert(  fabs(R10*R10 + R11*R11 + R12*R12 - value_traits::one()) <= working_precision<T>() || !"R is not special orthonormal matrix"); 
      assert(  fabs(R20*R20 + R21*R21 + R22*R22 - value_traits::one()) <= working_precision<T>() || !"R is not special orthonormal matrix"); 
      assert(  fabs(R00*R10 + R01*R11 + R02*R12 ) <= working_precision<T>()                      || !"R is not special orthonormal matrix"); 
      assert(  fabs(R00*R20 + R01*R21 + R02*R22 ) <= working_precision<T>()                      || !"R is not special orthonormal matrix"); 
      assert(  fabs(R10*R20 + R11*R21 + R12*R22 ) <= working_precision<T>()                      || !"R is not special orthonormal matrix"); 
      
      //--- The formulas below was computed using Matlab, that is
      //---syms R00 R01 R02  R10 R11 R12 R20 R21 R22 real;
      //---syms Jxx Jyy Jzz Jxy Jxz Jyz real;
      //---I = [ Jxx, Jxy , Jxz; Jxy, Jyy, Jyz; Jxz, Jyz, Jzz]
      //---R = [ R00, R01 , R02; R10, R11, R12; R20, R21, R22]
      //---simplify(R*I*R')
      T const & two = value_traits::two();
      
      T const Jxx = Ixx;
      T const Jyy = Iyy;
      T const Jzz = Izz;
      T const Jxy = Ixy;
      T const Jxz = Ixz;
      T const Jyz = Iyz;
      
      Ixx = (R00*R00*Jxx + R01*R01*Jyy + R02*R02*Jzz) + two*(R00*R01*Jxy + R00*R02*Jxz + R01*R02*Jyz);
      Iyy = (R10*R10*Jxx + R11*R11*Jyy + R12*R12*Jzz) + two*(R10*R11*Jxy + R10*R12*Jxz + R11*R12*Jyz); 
      Izz = (R20*R20*Jxx + R21*R21*Jyy + R22*R22*Jzz) + two*(R20*R21*Jxy + R20*R22*Jxz + R21*R22*Jyz); 
      
      Ixy = (R10*R00*Jxx + R11*R01*Jyy + R12*R02*Jzz) + (R10*R01 + R11*R00)*Jxy + (R12*R01 + R11*R02)*Jyz + (R12*R00 + R10*R02)*Jxz;
      Ixz = (R20*R00*Jxx + R21*R01*Jyy + R22*R02*Jzz) + (R20*R01 + R21*R00)*Jxy + (R22*R00 + R20*R02)*Jxz + (R21*R02 + R22*R01)*Jyz; 
      Iyz = (R10*R20*Jxx + R11*R21*Jyy + R12*R22*Jzz) + (R10*R21 + R11*R20)*Jxy + (R10*R22 + R12*R20)*Jxz + (R11*R22 + R12*R21)*Jyz; 
    }
    
  } // end of namespace detail
  
  template<typename T>
  Properties<T> rotate(
                       T const & qs
                       , T const & qx
                       , T const & qy
                       , T const & qz
                       , Properties<T> const & I
                       )
  {
    using namespace tiny;
    using std::fabs;
    
    typedef          MathTypes<T>             types;
    typedef typename types::quaternion_type   Q;
    typedef typename types::matrix3x3_type    M;
    typedef typename types::value_traits      VT;
    
    assert( is_number( qs )  || !"qs must be a number"       );
    assert( is_finite( qs )  || !"qs must be a finite number");      
    assert( is_number( qx )  || !"qx must be a number"       );
    assert( is_finite( qx )  || !"qx must be a finite number");      
    assert( is_number( qy )  || !"qy must be a number"       );
    assert( is_finite( qy )  || !"qy must be a finite number");      
    assert( is_number( qz )  || !"qz must be a number"       );
    assert( is_finite( qz )  || !"qz must be a finite number");      
    assert(  fabs( (qs*qs + qx*qx + qy*qy + qz*qz)  - VT::one() ) <= working_precision<T>() || !"Q is not unit quaterion"); 
    
    Properties<T> value = I;
    
    Q const q1 = Q( I.m_Qs, I.m_Qx, I.m_Qy, I.m_Qz);
    Q const q2 = Q( qs, qx, qy, qz);    // Create a tiny quaterion from the given input
    M const R = make( q2 );             // Using tiny to convert unit-quaterion to corresponding rotation matrix 
    
    // Now simply use matrix version of this function
    detail::rotate(
                   R(0,0), R(0,1), R(0,2)
                   , R(1,0), R(1,1), R(1,2)
                   , R(2,0), R(2,1), R(2,2)
                   , value.m_Ixx, value.m_Iyy, value.m_Izz
                   , value.m_Ixy, value.m_Ixz, value.m_Iyz                      
                   );
    
    Q const q3 = q2*q1;
    
    value.m_Qs = q3.real();
    value.m_Qx = q3.imag()[0];
    value.m_Qy = q3.imag()[1];
    value.m_Qz = q3.imag()[2];
    
    return value;
  }
  
  template<typename T>
  Properties<T> rotate_to_body_frame( Properties<T> const & Imodel )
  {
    
    using namespace tiny;
    using std::fabs;
    
    typedef          MathTypes<T>             types;
    typedef typename types::quaternion_type   Q;
    typedef typename types::matrix3x3_type    M;
    typedef typename types::vector3_type      V;
    typedef typename types::value_traits      VT;
    
    assert( is_number( Imodel.m_Ixx )  || !"Ixx must be a number"       );
    assert( is_finite( Imodel.m_Ixx )  || !"Ixx must be a finite number");      
    assert( is_number( Imodel.m_Iyy )  || !"Ixx must be a number"       );
    assert( is_finite( Imodel.m_Iyy )  || !"Ixx must be a finite number");      
    assert( is_number( Imodel.m_Izz )  || !"Ixx must be a number"       );
    assert( is_finite( Imodel.m_Izz )  || !"Ixx must be a finite number");      
    assert( is_number( Imodel.m_Ixy )  || !"Ixy must be a number"       );
    assert( is_finite( Imodel.m_Ixy )  || !"Ixy must be a finite number");      
    assert( is_number( Imodel.m_Ixz )  || !"Ixz must be a number"       );
    assert( is_finite( Imodel.m_Ixz )  || !"Ixz must be a finite number");      
    assert( is_number( Imodel.m_Iyz )  || !"Iyz must be a number"       );
    assert( is_finite( Imodel.m_Iyz )  || !"Iyz must be a finite number");
    
    Properties<T> value = Imodel;
    
    assert( fabs(VT::one() - Imodel.m_Qs) < working_precision<T>() || !"Internal error"  );
    assert( fabs(Imodel.m_Qx) < working_precision<T>()             || !"Internal error"  );
    assert( fabs(Imodel.m_Qy) < working_precision<T>()             || !"Internal error"  );
    assert( fabs(Imodel.m_Qz) < working_precision<T>()             || !"Internal error"  );
    
    //--- Convert input into a tiny matrix that holds the inertia tensor matrix 
    M const A = M::make(  
                        Imodel.m_Ixx, Imodel.m_Ixy, Imodel.m_Ixz
                        , Imodel.m_Ixy, Imodel.m_Iyy, Imodel.m_Iyz
                        , Imodel.m_Ixz, Imodel.m_Iyz, Imodel.m_Izz 
                        );
    
    //--- Do eigenvalue decomposition, A = R diag(d) R^T,
    M R;
    V d;
    eigen(A, R, d);
    
    //--- Next we select a permuation of the eigenvalues such that d0 >= d1 >= d2
    size_t perm[6][3];
    perm[0][0] = 0; perm[0][1] =1; perm[0][2] =2;
    perm[1][0] = 0; perm[1][1] =2; perm[1][2] =1;
    perm[2][0] = 1; perm[2][1] =0; perm[2][2] =2;
    perm[3][0] = 2; perm[3][1] =0; perm[3][2] =1;
    perm[4][0] = 1; perm[4][1] =2; perm[4][2] =0;
    perm[5][0] = 2; perm[5][1] =1; perm[5][2] =0;

    size_t order[3] = {0u, 0u, 0u};
    for(size_t choice = 0;choice<6u;++choice)
    {
      size_t i =  perm[choice][0];
      size_t j =  perm[choice][1];
      size_t k =  perm[choice][2];
      
      if( d(i) >= d(j) && d(j) >= d(k))
      {
        order[0] = i; order[1] = j; order[2] = k;
        break;
      }
    }
    
    //--- Now we create the rotation matrix corresponding the new eigenvalue order
    M RR = M::make(  R(0, order[0]), R(0, order[1]), R(0, order[2])
                   , R(1, order[0]), R(1, order[1]), R(1, order[2])
                   , R(2, order[0]), R(2, order[1]), R(2, order[2])
                   );
    
    //--- We need to make sure that the eigenvectors form a special orthogonal matrix
    if( det(RR) < VT::zero() )
    {
      RR(0,0) = -RR(0,0);
      RR(1,0) = -RR(1,0);
      RR(2,0) = -RR(2,0);
    }
    
    Q const q = make( RR );  // Convert rotation matrix to corresponding quaternion
    
    // Convert from tiny data types into raw POD types
    value.m_Qs = q.real();
    value.m_Qx = q.imag()[0];
    value.m_Qy = q.imag()[1];
    value.m_Qz = q.imag()[2];
    
    value.m_Ixx = d(order[0]);
    value.m_Iyy = d(order[1]);
    value.m_Izz = d(order[2]);
    value.m_Ixy = VT::zero();
    value.m_Ixz = VT::zero();
    value.m_Iyz = VT::zero();
    
    return value;
  }
  
  template  
  Properties<float> rotate<float>(
                                  float const & qs
                                  , float const & qx
                                  , float const & qy
                                  , float const & qz
                                  , Properties<float> const & I
                                  );
  
  template  
  Properties<double> rotate<double>(
                                    double const & qs
                                    , double const & qx
                                    , double const & qy
                                    , double const & qz
                                    , Properties<double> const & I
                                    );
  
  template
  Properties<float> rotate_to_body_frame( Properties<float> const & Imodel );
  
  template
  Properties<double> rotate_to_body_frame( Properties<double> const & Imodel );
  
} // namespace mass
