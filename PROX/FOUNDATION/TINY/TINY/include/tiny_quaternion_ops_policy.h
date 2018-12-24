#ifndef TINY_QUATERNION_OPS_POLICY_H
#define TINY_QUATERNION_OPS_POLICY_H

#include <tiny_vector.h>
#include <tiny_vector_functions.h>  // for unit and inner_prod

#include <cmath> 
#include <cassert>

namespace tiny
{
  namespace detail
  {
    
    template <typename Q>
    class QuaternionOpsPolicy
      {
      protected:
        
        typedef typename Q::type_traits         type_traits;
        typedef typename Q::value_traits   	    value_traits;
        typedef typename type_traits::op_type		op_type;
        
      public:
        
        typedef typename type_traits::real_type  real_type;
        typedef typename Q::vector3_type         vector3_type;
        
        static Q & mul_assign (Q & lhs, Q const &rhs)
        {
          real_type	    const   a = lhs.real(); // temporary backup
          vector3_type	const & A = lhs.imag();   // for readability
          real_type	    const & b = rhs.real();   // for readability
          vector3_type	const & B = rhs.imag();   // for readability
          lhs.real() = a*b - tiny::inner_prod( A, B );
          lhs.imag() = a*B + A*b + cross(A,B);
          return lhs;
        }
        
        static Q mul (Q const & lhs, Q const &rhs)
        {
          Q q(lhs);
          return mul_assign(q,rhs);
        }
        
        static real_type inner_prod(Q const & q, Q const & r)	
        {
          typedef Vector<4,type_traits>             vector4_type;
          typedef VectorOpsPolicy< vector4_type >   ops_policy;
          return ops_policy::inner_prod(*((vector4_type*)&q),*((vector4_type*)&r)); // Actually a vector inner_prod :)
        }
        
        static real_type norm(Q const & q)	
        {
          using std::sqrt;
          return sqrt(  inner_prod(q,q) );
        }
        
        static Q conj (Q const & q)
        {          
          return Q(q.real(), -q.imag());
        }
        
        static Q unit(Q const & q)
        {
          using std::sqrt;
          using std::fabs;
          
          real_type l = norm(q);
          
          if(fabs(l) > value_traits::zero())
            return Q(q.real()/l , q.imag()/l) ;
          return Q(value_traits::zero(),value_traits::zero(),value_traits::zero(),value_traits::zero());
        }
        
      };
    
  } // namespace detail
} // namespace tiny

// TINY_QUATERNION_OPS_POLICY_H
#endif 
