#ifndef TINY_VECTOR_FUNCTIONS_H
#define TINY_VECTOR_FUNCTIONS_H

#include <tiny_vector.h>
#include <tiny_random.h>

#include <tiny_common_ops_policy.h>
#include <tiny_vector_ops_policy.h>

#include <fstream>

namespace tiny
{
  template <size_t N, typename T>
  inline bool operator == (Vector<N,T> const & lhs, Vector<N,T> const & rhs)
  {
    for(size_t n = 0;n<N;++n)
    {
      typename T::real_type const & a = lhs(n);
      typename T::real_type const & b = rhs(n);
      if( (a>b) || (a<b) )
        return false;
    }
    return true;
  }
  
  template <size_t N, typename T>
  inline bool operator != (Vector<N,T> const & lhs, Vector<N,T> const & rhs) { return !(lhs==rhs); }  
  
  template <size_t N, typename T>
  inline  Vector<N,T>  equal(Vector<N,T> const & lhs, Vector<N,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::equal(lhs,rhs);
  }
  
  template <size_t N, typename T>
  inline  Vector<N,T> not_equal (Vector<N,T> const & lhs, Vector<N,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::not_equal(lhs,rhs);
  }
  
  template <size_t N, typename T>
  inline  Vector<N,T> operator < ( Vector<N,T> const & lhs, Vector<N,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::less_than(lhs,rhs);
  }
  
  template <size_t N, typename T>
  inline Vector<N,T> operator > (Vector<N,T> const & lhs, Vector<N,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::greater_than(lhs,rhs);
  }
  
  template <size_t N, typename T>
  inline Vector<N,T> operator <= (Vector<N,T> const & lhs, Vector<N,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::less_than_or_equal(lhs,rhs);
  }
  
  template <size_t N, typename T>
  inline Vector<N,T> operator >= (Vector<N,T> const & lhs, Vector<N,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::greater_than_or_equal(lhs,rhs);
  }
  
  template <size_t N, typename T>
  inline Vector<N,T> operator + (Vector<N,T> const & lhs, Vector<N,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::add(lhs,rhs);
  }
  
  template <size_t N, typename T>
  inline Vector<N,T> operator - (Vector<N,T> const & lhs, Vector<N,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::sub(lhs,rhs);
  }
  
  template <size_t N, typename T>
  inline Vector<N,T> & operator += (Vector<N,T> & lhs, Vector<N,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::add_assign(lhs,rhs);
  }
  
  template <size_t N, typename T>
  inline Vector<N,T>& operator -= (Vector<N,T> & lhs, Vector<N,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::sub_assign(lhs,rhs);
  }
  
  template <size_t N, typename T>
  inline  Vector<N,T> operator * (typename T::real_type const & s, Vector<N,T> const & rhs)
  {
    return rhs * s;
  }
  
  template <size_t N, typename T>
  inline  Vector<N,T> operator * (Vector<N,T> const & rhs, typename T::real_type const & s)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::mul(rhs,s);
  }
  
  template <size_t N, typename T>
  inline  Vector<N,T>& operator *= (Vector<N,T>  & lhs, typename T::real_type const & s)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::mul_assign(lhs,s);
  }
  
  template <size_t N, typename T>
  inline Vector<N,T> operator / (Vector<N,T> const & lhs, typename T::real_type const & s)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::div(lhs,s);
  }
  
  template <size_t N, typename T>
  inline Vector<N,T>& operator /= (Vector<N,T> & lhs, typename T::real_type const & s)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::div_assign(lhs,s);
  }
  
  template <size_t N, typename T>
  inline  Vector<N,T> operator - (Vector<N,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::negate(rhs);
  }
  
  template <size_t N, typename T>
  inline  Vector<N,T> sign (Vector<N,T> const & A)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::sign(A);
  }
  
  template <size_t N, typename T>
  inline Vector<N,T> round (Vector<N,T> const & A)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::round(A);
  }
  
  template <size_t N, typename T>
  inline Vector<N,T> min (Vector<N,T> const & lhs, Vector<N,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::min(lhs,rhs);
  }
  
  template <size_t N, typename T>
  inline  Vector<N,T> max (Vector<N,T> const & lhs, Vector<N,T> const & rhs)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::max(lhs,rhs);
  }
  
  template <size_t N, typename T>
  inline  Vector<N,T> abs (Vector<N,T> const & A)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::abs(A);
  }
  
  template <size_t N, typename T>
  inline  Vector<N,T> floor (Vector<N,T> const & A)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::floor(A);
  }
  
  template <size_t N, typename T>
  inline  Vector<N,T> ceil (Vector<N,T> const & A)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::ceil(A);
  }
  
  
  template <size_t N, typename T>
  inline  typename T::real_type min (Vector<N,T> const & A)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::min(A);
  }
  
  template <size_t N, typename T>
  inline  typename T::real_type max (Vector<N,T> const & A)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::max(A);
  }
  
  template <size_t N, typename T>
  inline typename T::real_type sum (Vector<N,T> const & A)
  {
    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::sum(A);
  }
  
  template <size_t N, typename T>
  inline  typename T::real_type  inner_prod (Vector<N,T> const & lhs, Vector<N,T> const & rhs)
  {
    typedef detail::VectorOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::inner_prod(lhs,rhs);
  }
  
  template <size_t N, typename T>
  inline  typename T::real_type operator * (Vector<N,T> const & lhs, Vector<N,T> const & rhs)
  {
    typedef detail::VectorOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::inner_prod(lhs,rhs);
  }
  
  template <size_t N, typename T>
  inline  Vector<N,T> cross (Vector<N,T> const & lhs, Vector<N,T> const & rhs)
  {
    typedef detail::VectorOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::cross(lhs,rhs);
  }
  
  template <size_t N, typename T>
  inline typename T::real_type norm(Vector<N,T> const & v)
  {
    typedef detail::VectorOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::norm(v);
  }
  
  template <size_t N, typename T>
  inline typename T::real_type norm_1(Vector<N,T> const & v)
  {
    return max ( abs ( v ) );
  }
  
  template <size_t N, typename T>
  inline  Vector<N,T> unit(Vector<N,T> const & v)
  {
    typedef detail::VectorOpsPolicy< Vector<N,T> > ops_policy;  
    return ops_policy::unit(v);
  }
  
  template<size_t N,typename T>
  inline std::ostream & operator<< (std::ostream & o, Vector<N,T> const & A)
  {
    o << "[";      
    o <<  A(0);
    for(size_t n =1;n<N;++n)
    {
      o << "," << A(n);
    }
    o << "]";
    return o;
  }
  
  template<size_t N,typename T>
  inline std::istream & operator>>(std::istream & in,Vector<N,T> & A)
  {
    char dummy;
    in >> dummy;      
    in >>  A(0);
    for(size_t n =1;n<N;++n)
    {
      in >> dummy >> A(n);
    }
    in >> dummy;    
    return in;
  }
  
  //  template <size_t N, typename T>
  //  inline Vector<N,T> operator / (Vector<N,T> const & lhs, Vector<N,T> const & rhs)
  //  {
  //    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
  //    return ops_policy::div(lhs,rhs);
  //  }
  //  template <size_t N, typename T>
  //  inline  Vector<N,T>& operator /= (Vector<N,T> & lhs, Vector<N,T> const & rhs)
  //  {
  //    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
  //    return ops_policy::div_assign(lhs,rhs);
  //  }
  //  template <size_t N, typename T>
  //  inline  Vector<N,T> & operator *= (Vector<N,T> & lhs, Vector<N,T> const & rhs)
  //  {
  //    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
  //    return ops_policy::mul_assign(lhs,rhs);
  //  }
  //  template <size_t N, typename T>
  //  inline  Vector<N,T> operator / (typename T::real_type const & s, Vector<N,T> const & rhs)
  //  {
  //    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
  //    return ops_policy::div(rhs,s); // Reverted on purpose
  //  }
  //  template <size_t N, typename T>
  //  inline  Vector<N,T> sqrt (Vector<N,T> const & A)
  //  {
  //    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
  //    return ops_policy::sqrt(A);
  //  }
  //  
  //  template <size_t N, typename T>
  //  inline  Vector<N,T> rsqrt (Vector<N,T> const & A)
  //  {
  //    typedef detail::CommonOpsPolicy< Vector<N,T> > ops_policy;  
  //    return ops_policy::rsqrt(A);
  //  }
  
  /**
   * Compute Orthonormal Vectors.
   * Compute unit vectors of a right handed coordinate frame, given initial z-axis (k-vector).
   *
   * @param i  Upon return contains a orthogonal vector to j and k
   * @param j  Upon return contains a orthogonal vector to i and k
   * @param k  A given direction for the last vector, assumed to be a unit-vector.
   */
  template<typename T>
  inline void orthonormal_vectors( Vector<3,T> & i, Vector<3,T> & j, Vector<3,T> const & k )
  {
    typedef typename Vector<3,T>::value_traits   value_traits;
        
    Vector<3,T> m_abs_k = abs( k);
    
    if ( m_abs_k( 0 ) > m_abs_k( 1 ) )
    {
      if ( m_abs_k( 0 ) > m_abs_k( 2 ) )
        i = Vector<3,T>::make( value_traits::zero(), value_traits::one(), value_traits::zero() );
      else
        i = Vector<3,T>::make( value_traits::one(), value_traits::zero(), value_traits::zero() );
    }
    else
    {
      if ( m_abs_k( 1 ) > m_abs_k( 2 ) )
        i = Vector<3,T>::make( value_traits::zero(), value_traits::zero(), value_traits::one() );
      else
        i = Vector<3,T>::make( value_traits::one(), value_traits::zero(), value_traits::zero() );
    }
    j = unit( cross(k,i) );
    i = cross(j,k);
  }

  /**
   * Truncate to Tolerance.
   *
   * @param p   Vector to be truncated
   * @param tol Tolerance
   *
   * @return    The return value is the truncated vector
   */
  template<typename T>
  inline Vector<3,T> truncate(Vector<3,T> const & p, typename T::real_type const & tol)
  {
    typedef typename Vector<3,T>::value_traits   value_traits;
    typedef typename T::real_type                real_type;
    
    using std::fabs;

    assert(is_number(tol)             || !"truncate(): nan");
    assert(is_finite(tol)             || !"truncate(): inf");
    assert(tol > value_traits::zero() || !"truncate(): tol must be postive");

    real_type const x = (fabs(p(0)) > tol) ? p(0) : value_traits::zero();
    real_type const y = (fabs(p(1)) > tol) ? p(1) : value_traits::zero();
    real_type const z = (fabs(p(2)) > tol) ? p(2) : value_traits::zero();

    assert(is_number(x)   || !"truncate(): nan");
    assert(is_finite(x)   || !"truncate(): inf");
    assert(is_number(y)   || !"truncate(): nan");
    assert(is_finite(y)   || !"truncate(): inf");
    assert(is_number(z)   || !"truncate(): nan");
    assert(is_finite(z)   || !"truncate(): inf");

    return Vector<3,T>::make( x, y, z);
  }

} // namespace tiny

// TINY_VECTOR_FUNCTIONS_H
#endif 
