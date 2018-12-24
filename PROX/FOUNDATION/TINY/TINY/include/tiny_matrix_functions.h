#ifndef TINY_MATRIX_FUNCTIONS_H
#define TINY_MATRIX_FUNCTIONS_H

#include <tiny_matrix.h>
#include <tiny_quaternion.h>
#include <tiny_common_ops_policy.h>
#include <tiny_matrix_ops_policy.h>

#include <tiny_vector_functions.h>  // for cross, inner_prod and norm

#include <cmath>                   // for sin, cos, min, max, fabs
#include <iosfwd>

namespace tiny
{
  
  template<size_t I,size_t J,typename T>
  inline std::ostream & operator<< (std::ostream & o, Matrix<I,J,T> const & A)
  {
    o << "[";
    for(size_t i =0;i<I;++i)
    {
      o <<  A(i,0);
      for(size_t j =1;j<J;++j)
      {
        o << "," << A(i,j);
      }
      if(i<(I-1))
        o << ";";
    }
    o << "]";
    return o;
  }
  
  template<size_t I,size_t J,typename T>
  inline std::istream & operator>>(std::istream & in,Matrix<I,J,T> & A)
  {
    char dummy;
    
    in >> dummy;
    for(size_t i =0;i<I;++i)
    {
      in >>  A(i,0);
      for(size_t j =1;j<J;++j)
      {
        in >> dummy;
        in >>  A(i,j);
      }
      if(i<(I-1))
        in >> dummy;
    }
    in >> dummy;    
    return in;
  }
    
  template <size_t I, size_t J, typename T>
  inline Matrix<I,J,T> operator + (Matrix<I,J,T> const & lhs, Matrix<I,J,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::add(lhs,rhs);
  }
  
  template <size_t I, size_t J, typename T>
  inline Matrix<I,J,T> operator - (Matrix<I,J,T> const & lhs, Matrix<I,J,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::sub(lhs,rhs);
  }  
  
  template <size_t I, size_t J, typename T>
  inline Matrix<I,J,T>& operator += (Matrix<I,J,T> & lhs, Matrix<I,J,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::add_assign(lhs,rhs);
  }
  
  template <size_t I, size_t J, typename T>
  inline Matrix<I,J,T>& operator -= (Matrix<I,J,T> & lhs, Matrix<I,J,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::sub_assign(lhs,rhs);
  }
  
  template <size_t I, size_t J, typename T>
  inline Matrix<I,J,T> operator * (typename T::real_type const & s, Matrix<I,J,T> const & A) { return A * s; }
  
  template <size_t I, size_t J, typename T>
  inline Matrix<I,J,T> prod (typename T::real_type const & s, Matrix<I,J,T> const & A) { return s*A; }
  
  template <size_t I, size_t J, typename T>
  inline Matrix<I,J,T> operator * (Matrix<I,J,T> const & A, typename T::real_type const & s)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::mul(A,s);
  }
  
  template <size_t I, size_t J, typename T>
  inline Matrix<I,J,T> prod (Matrix<I,J,T> const & A, typename T::real_type const & s) { return A * s; }

  template <size_t I, size_t J, typename T>
  inline Matrix<I,J,T>& operator *= (Matrix<I,J,T>  & lhs, typename T::real_type const & s)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::mul_assign(lhs,s);
  }
  
  template <size_t I, size_t J, typename T>
  inline Matrix<I,J,T> operator / (Matrix<I,J,T> const & lhs, typename T::real_type const & s)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::div(lhs,s);
  }
  
  template <size_t I, size_t J, typename T>
  inline  Matrix<I,J,T>& operator /= (Matrix<I,J,T> & lhs, typename T::real_type const & s)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::div_assign(lhs,s);
  }
  
//  template <size_t I, size_t J, typename T>
//  inline  Matrix<I,J,T> operator / (typename T::real_type const & s, Matrix<I,J,T> const & rhs)
//  {
//    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
//    return ops_policy::div(rhs,s); // Reverted on purpose
//  }
  
  template <size_t I, size_t J, typename T>
  inline  Matrix<I,J,T> operator - (Matrix<I,J,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::negate(rhs);
  }

  template <size_t I, size_t J, size_t K, typename T >
  inline Matrix<I,K,T> operator * (Matrix<I,J,T> const & lhs, Matrix<J,K,T> const & rhs)
  {         
    typedef detail::MatrixOpsPolicy< Matrix<I,K,T> > ops_policy;
    return ops_policy::mul_matrix(lhs,rhs);
  }
  
  template <size_t I, size_t J, size_t K, typename T >
  inline Matrix<I,K,T> prod(Matrix<I,J,T> const & lhs, Matrix<J,K,T> const & rhs) { return lhs * rhs; }
  
  //  template <size_t N, typename T >
  //  inline Matrix<N,N,T> & operator *= (Matrix<N,N, T> & rhs, Matrix<N,N, T> const & lhs)
  //  {
  //    typedef detail::MatrixOpsPolicy< Matrix<N,N,T> > ops_policy;
  //    return ops_policy::mul_matrix(rhs,lhs);
  //  }
  
  template<typename T>
  inline Matrix<3,3,T> inverse( Matrix<3,3,T> const & A)
  {
    typedef Matrix<3,3,T>             M;
    typedef typename T::real_type     real_type;
    
    //From messer p.283 we know
    //
    //  -1     1
    // A   = -----  adj A
    //       det A
    //
    //                      i+j
    // ij-cofactor of A = -1    det A
    //                               ij
    //
    // i,j entry of the adjoint.
    //                                   i+j
    // adjoint A   = ji-cofactor = A = -1    det A
    //          ij                                ji
    //
    // As it can be seen the only numerical error
    // in these calculations is from the resolution
    // of the scalars. So it is a very accurate method.
    //
    M adj;
    adj(0,0) = A(1,1)*A(2,2) - A(2,1)*A(1,2);
    adj(1,1) = A(0,0)*A(2,2) - A(2,0)*A(0,2);
    adj(2,2) = A(0,0)*A(1,1) - A(1,0)*A(0,1);
    adj(0,1) = A(1,0)*A(2,2) - A(2,0)*A(1,2);
    adj(0,2) = A(1,0)*A(2,1) - A(2,0)*A(1,1);
    adj(1,0) = A(0,1)*A(2,2) - A(2,1)*A(0,2);
    adj(1,2) = A(0,0)*A(2,1) - A(2,0)*A(0,1);
    adj(2,0) = A(0,1)*A(1,2) - A(1,1)*A(0,2);
    adj(2,1) = A(0,0)*A(1,2) - A(1,0)*A(0,2);
    real_type det = A(0,0)*adj(0,0) -  A(0,1)*adj(0,1) +   A(0,2)*adj(0,2);
    if(det)
    {
      adj(0,1) = -adj(0,1);
      adj(1,0) = -adj(1,0);
      adj(1,2) = -adj(1,2);
      adj(2,1) = -adj(2,1);
      return trans(adj)/det;
    }
    
    return M::identity();
  }
  
  template<typename T>
  inline typename T::real_type trace(Matrix<3,3,T> const & A)
  {
    return (A(0,0) + A(1,1) + A(2,2));
  }
  
  template <typename T>
  inline typename T::real_type det(Matrix<3,3, T> const & A)
  {
    return A(0,0)*(A(1,1)*A(2,2) - A(2,1)*A(1,2)) -  A(0,1)*(A(1,0)*A(2,2) - A(2,0)*A(1,2)) +   A(0,2)*(A(1,0)*A(2,1) - A(2,0)*A(1,1));
  }
	
  template <typename T>
  inline bool is_symmetric(Matrix<3,3, T> M, typename T::real_type const & threshold)
  {
    typedef typename Matrix<3,3, T>::value_traits  value_traits;
    
    using std::fabs;
    
    assert(threshold >= value_traits::zero() || !"is_symmetric(): threshold must be non-negative");
    
    if(fabs(M(0,1)-M(1,0))>threshold)      return false;
    if(fabs(M(0,2)-M(2,0))>threshold)      return false;
    if(fabs(M(1,2)-M(2,1))>threshold)      return false;
    return true;
  }

  template <size_t I, size_t J, typename T >
  inline  Matrix<J,I, T> trans( Matrix<I, J,T> const & m )
  {
    typedef detail::MatrixOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::trans(m);
  }
  
  template <size_t I, size_t J, typename T>
  inline  Matrix<I,J,T> abs (Matrix<I,J,T> const & A)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::abs(A);
  }
  
  template <size_t I, size_t J, typename T>
  inline typename T::real_type min (Matrix<I,J,T> const & A)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::min(A);
  }
  
  template <size_t I, size_t J, typename T>
  inline typename T::real_type max (Matrix<I,J,T> const & A)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::max(A);
  }
  
  template <size_t I, size_t J, typename T>
  inline Matrix<I,J,T> sign (Matrix<I,J,T> const & A)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::sign(A);
  }
  
  template <size_t I, size_t J, typename T>
  inline  Matrix<I,J,T> round (Matrix<I,J,T> const & A)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::round(A);
  }
  
  template <size_t I, size_t J, typename T>
  inline  Matrix<I,J,T> min (Matrix<I,J,T> const & lhs, Matrix<I,J,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::min(lhs,rhs);
  }
  
  template <size_t I, size_t J, typename T>
  inline  Matrix<I,J,T>  max (Matrix<I,J,T> const & lhs, Matrix<I,J,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::max(lhs,rhs);
  }
  
  template <size_t I, size_t J, typename T>
  inline  Matrix<I,J,T> floor (Matrix<I,J,T> const & A)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::floor(A);
  }
  
  template <size_t I, size_t J, typename T>
  inline  Matrix<I,J,T> ceil (Matrix<I,J,T> const & A)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::ceil(A);
  }

  template <size_t I, size_t J, typename T>
  inline  typename T::real_type sum (Matrix<I,J,T> const & A)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::sum(A);
  }
  
  template<size_t I, size_t J, typename T>
  inline typename T::real_type norm_1(Matrix<I,J,T> const & A)
  {
    return max ( abs( A )  );
  }
  
  template <size_t I, size_t J, typename T>
  inline  Matrix<I,J,T> equal (Matrix<I,J,T> const & lhs, Matrix<I,J,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::equal(lhs,rhs);
  }
  
  template <size_t I, size_t J, typename T>
  inline Matrix<I,J,T>  not_equal (Matrix<I,J,T> const & lhs, Matrix<I,J,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::not_equal(lhs,rhs);
  }

  template <size_t I, size_t J, typename T>
  inline Matrix<I,J,T> operator < ( Matrix<I,J,T> const & lhs, Matrix<I,J,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::less_than(lhs,rhs);
  }
  
  template <size_t I, size_t J, typename T>
  inline Matrix<I,J,T> operator > (Matrix<I,J,T> const & lhs, Matrix<I,J,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::greater_than(lhs,rhs);
  }
  
  template <size_t I, size_t J, typename T>
  inline Matrix<I,J,T> operator <= (Matrix<I,J,T> const & lhs, Matrix<I,J,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::less_than_or_equal(lhs,rhs);
  }
  
  template <size_t I, size_t J, typename T>
  inline  Matrix<I,J,T> operator >= (Matrix<I,J,T> const & lhs, Matrix<I,J,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::greater_than_or_equal(lhs,rhs);
  }
    
  template <size_t I, size_t J, typename T >
  inline  Vector<I,T> operator * (Matrix<I,J, T> const & rhs, Vector<J,T> const & lhs)
  {
    typedef detail::MatrixOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::mul_vector(rhs,lhs);
  }
  
  template <size_t I, size_t J, typename T >
  inline  Vector<I,T> prod(Matrix<I,J, T> const & rhs, Vector<J,T> const & lhs)  { return rhs * lhs; }
  
  template <size_t I, size_t J, typename T >
  inline  Vector<I,T> operator * ( Vector<J,T> const & rhs, Matrix<I,J, T> const & lhs)
  {
    typedef detail::MatrixOpsPolicy< Matrix<I,J,T> > ops_policy;
    return ops_policy::mul_vector(lhs,rhs);
  }
  
  template <size_t I, size_t J, typename T >
  inline  Vector<I,T> prod( Vector<J,T> const & rhs, Matrix<I,J, T> const & lhs)
  {
    return rhs*lhs;
  }
  
  template <typename T>
  inline Matrix<3,3,T> outer_prod( Vector<3,T> const & v1, Vector<3,T> const & v2 )
  {
    return Matrix<3,3,T>::make(
                               ( v1( 0 ) * v2( 0 ) ), ( v1( 0 ) * v2( 1 ) ), ( v1( 0 ) * v2( 2 ) ),
                               ( v1( 1 ) * v2( 0 ) ), ( v1( 1 ) * v2( 1 ) ), ( v1( 1 ) * v2( 2 ) ),
                               ( v1( 2 ) * v2( 0 ) ), ( v1( 2 ) * v2( 1 ) ), ( v1( 2 ) * v2( 2 ) ) 
                               );
  }
  
  /**
   * Sets up the cross product matrix.
   * This method is usefull for expression the cross product as a matrix multiplication.
   *
   * @param  v       A reference to a vector. This is first argument of the cross product.
   */
  template<typename T>
  inline Matrix<3,3,T> star(Vector<3,T> const & v)
  {
    typedef typename Matrix<3,3,T>::value_traits  value_traits;
    
    //--- Changes a cross-product into a matrix multiplication.
    //--- Rewrites the component of a vector3_type cross-product as a matrix.
    //--- a x b = a*b = ba*
    return Matrix<3,3,T>::make(   
                               value_traits::zero(),                   -v(2),                     v(1),  
                               v(2),     value_traits::zero(),                   -v(0),
                               -v(1),                     v(0),    value_traits::zero()
                               );
  }
  
  template <typename T>
  inline  Matrix<3,3,T> ortonormalize(Matrix<3,3, T>  const & A)
  {
    typedef          Vector<3,T>   vector3_type;
    typedef typename T::real_type  real_type;
    
    vector3_type  row0; row0[0]=A(0,0); row0[1]=A(0,1); row0[2]=A(0,2);
    vector3_type  row1; row1[0]=A(1,0); row1[1]=A(1,1); row1[2]=A(1,2);
    vector3_type  row2; row2[0]=A(2,0); row2[1]=A(2,1); row2[2]=A(2,2);
    
    real_type const l0 = norm(row0);
    if(l0) 
      row0 /= l0;
    
    row1 -=  row0 * inner_prod(row0 , row1);
    real_type const l1 = norm(row1);
    if(l1) 
      row1 /= l1;
    
    row2 = cross( row0 , row1);
    
    Matrix<3,3,T> result;
    
    result(0,0) = row0[0]; result(0,1) = row0[1]; result(0,2) = row0[2];
    result(1,0) = row1[0]; result(1,1) = row1[1]; result(1,2) = row1[2];
    result(2,0) = row2[0]; result(2,1) = row2[1]; result(2,2) = row2[2];
    return result;
	}
  
  /**
   * Convert unit quaternion to a matrix.
   * This function performs a conversion of a quaternion that represents
   * a rotation into the correponding rotation matrix.
   *
   * @param q      A reference to a quaternion.
   * @return       A corresponding rotation matrix.
   */
  template<typename T>
  inline Matrix<3,3,T>  make(Quaternion<T> const & q)
  {
    typedef          Matrix<3,3,T>    M;
    typedef typename M::value_traits  value_traits;
    
    M m;
    m(0,0) = value_traits::one() - value_traits::two() * ( (q.imag()(1)*q.imag()(1)) + (q.imag()(2)*q.imag()(2)));
    m(1,1) = value_traits::one() - value_traits::two() * ( (q.imag()(0)*q.imag()(0)) + (q.imag()(2)*q.imag()(2)));
    m(2,2) = value_traits::one() - value_traits::two() * ( (q.imag()(1)*q.imag()(1)) + (q.imag()(0)*q.imag()(0)));
    m(1,0) =                       value_traits::two() * ( (q.imag()(0)*q.imag()(1)) + (q.real()*q.imag()(2)));
    m(0,1) =                       value_traits::two() * ( (q.imag()(0)*q.imag()(1)) - (q.real()*q.imag()(2)));
    m(2,0) =                       value_traits::two() * (-(q.real()*q.imag()(1))    + (q.imag()(0)*q.imag()(2)));
    m(0,2) =                       value_traits::two() * ( (q.real()*q.imag()(1))    + (q.imag()(0)*q.imag()(2)));
    m(2,1) =                       value_traits::two() * ( (q.imag()(2)*q.imag()(1)) + (q.real()*q.imag()(0)));
    m(1,2) =                       value_traits::two() * ( (q.imag()(2)*q.imag()(1)) - (q.real()*q.imag()(0)));
    return m;
  }
  
//  template <size_t I, size_t J, typename T>
//  inline Matrix<I,J,T> operator / (Matrix<I,J,T> const & lhs, Matrix<I,J,T> const & rhs)
//  {
//    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
//    return ops_policy::div(lhs,rhs);
//  }
//  
//  template <size_t I, size_t J, typename T>
//  inline Matrix<I,J,T>& operator *= (Matrix<I,J,T> & lhs, Matrix<I,J,T> const & rhs)
//  {
//    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
//    return ops_policy::mul_assign(lhs,rhs);
//  }
//  
//  template <size_t I, size_t J, typename T>
//  inline Matrix<I,J,T>& operator /= (Matrix<I,J,T> & lhs, Matrix<I,J,T> const & rhs)
//  {
//    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
//    return ops_policy::div_assign(lhs,rhs);
//  }
//    
//  template <size_t I, size_t J, typename T>
//  inline  Matrix<I,J,T> sqrt (Matrix<I,J,T> const & A)
//  {
//    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
//    return ops_policy::sqrt(A);
//  }
//  
//  template <size_t I, size_t J, typename T>
//  inline  Matrix<I,J,T> rsqrt (Matrix<I,J,T> const & A)
//  {
//    typedef detail::CommonOpsPolicy< Matrix<I,J,T> > ops_policy;
//    return ops_policy::rsqrt(A);
//  }
      
} //  namespace tiny

// TINY_MATRIX_FUNCTIONS_H
#endif

