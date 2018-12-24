#ifndef TINY_COMMON_OPS_POLICY_H
#define TINY_COMMON_OPS_POLICY_H

namespace tiny
{
  namespace detail
  {
    
    /* Common OpsPolicy class
     *
     * This class implements the default policy for basic ops,
     * that is how the operators common for all Container derived types
     * are implemented.
     */
    template <typename M>
    class CommonOpsPolicy
      {
      protected:
        
        typedef typename M::type_traits  type_traits;
        
        enum {stride = type_traits::stride};
        enum {N = M::N};
        enum {I = M::I};
        enum {J = M::J};
        // 2009-07-13 Kenny: Optmize calculations and document this
        enum {remainder = (stride-(J%stride))%stride };
        enum {last = ((J+remainder-stride)*(remainder>0) + J*(remainder==0)) }; // 2009-07-13 Kenny: Seems to be start of last op_type in a row
        
      public:
        
        typedef typename M::op_type		  	          op_type;
        typedef typename M::real_type               real_type;
        typedef typename M::value_traits            value_traits;
        
      public:
                
        // Boolean operation -----------------------------------------
        static M equal (M const & lhs,  M const & rhs)
        {
          M result;
          
          for (size_t i = 0; i < I ; ++i)
          {
            for(size_t j=0 ; j < J ; j+=stride )
            {
              M::accessor::cast(result,i,j) = type_traits::equal(M::accessor::cast(lhs,i,j), M::accessor::cast(rhs,i,j));
            }
            // 2009-07-14 Kenny: warning C4127: conditional expression is constant
            if(remainder)// Necessary becase test will set remainder values to 1.0
              type_traits::template set_pad_value<remainder>(M::accessor::cast(result,i,last), value_traits::zero());
          }
          
          return result;
        }
        
        static M not_equal (M const & lhs, M const & rhs)
        {
          M result;
          
          for (size_t i = 0; i < I ; ++i)
          {
            for(size_t j=0 ; j < J ; j+=stride )
            {
              M::accessor::cast(result,i,j) = type_traits::not_equal( M::accessor::cast(lhs,i,j), M::accessor::cast(rhs,i,j));
            }
          }
          return result;
        }
        
        static M less_than (M const & lhs, M const & rhs)
        {
          M result;
          
          for(size_t i=0 ; i < I ; ++i )
          {
            for(size_t j=0 ; j < J ; j+=stride )
            {
              M::accessor::cast(result,i,j) = type_traits::less_than(M::accessor::cast(lhs,i,j), M::accessor::cast(rhs,i,j));
            }
          }
          return result;
        }
        
        static M greater_than (M const & lhs, M const & rhs)
        {
          M result;
          
          for(size_t i=0 ; i < I ; i++)
          {
            for(size_t j=0 ; j < J ; j+=stride )
            {
              M::accessor::cast(result,i,j) = type_traits::greater_than(M::accessor::cast(lhs,i,j), M::accessor::cast(rhs,i,j));
            }
          }
          return result;
        }
        
        static M less_than_or_equal (M const & lhs, M const & rhs)
        {
          M result;
          
          for (size_t i = 0; i < I ; ++i)
          {
            for(size_t j = 0 ; j < J ; j+=stride )
            {  
              M::accessor::cast(result,i,j)  = type_traits::less_than_or_equal(M::accessor::cast(lhs,i,j), M::accessor::cast(rhs,i,j));
            }
            // 2009-07-14 Kenny: warning C4127: conditional expression is constant
            if(remainder) // Necessary becase test will set remainder values to 1.0
              type_traits::template set_pad_value<remainder>(M::accessor::cast(result,i,last), value_traits::zero());
          }
          return result;
        }
        
        static M greater_than_or_equal (M const & lhs, M const & rhs)
        {
          M result;
          
          for(size_t i = 0 ; i < I ; ++i)
          {
            for(size_t j = 0 ; j < J ; j+=stride )
            {
              M::accessor::cast(result,i,j) = type_traits::greater_than_or_equal(M::accessor::cast(lhs,i,j), M::accessor::cast(rhs,i,j));
            }
            // 2009-07-14 Kenny: warning C4127: conditional expression is constant
            if(remainder) // Necessary becase test will set remainder values to 1.0
              type_traits::template set_pad_value<remainder>(M::accessor::cast(result,i,last), value_traits::zero());
          }
          return result;
        }
        
        // Basic Arithmetics ---------------------------------------------
        
        static M add (M const & lhs, M const & rhs )
        {
          M result(lhs);
          return add_assign(result,rhs);
        }
        
        static M sub (M const & lhs, M const & rhs )
        {
          M result(lhs);
          return sub_assign(result,rhs);
        }
        
//        static M mul (M const & lhs, M const & rhs )
//        {
//          M result(lhs);
//          return mul_assign(result,rhs);
//        }
//        
//        static M div (M const & lhs, M const & rhs )
//        {
//          M result(lhs);
//          return div_assign(result,rhs);
//        }
        
        static M& add_assign (M & lhs, M const & rhs )
        {
          
          for(size_t i = 0 ; i<I ; ++i)
          {
            for(size_t j = 0 ; j<J ; j+=stride )
            {
              type_traits::add_assign(M::accessor::cast(lhs,i,j), M::accessor::cast(rhs,i,j));
            }
          }
          return lhs;
        }
        
        static M& sub_assign (M & lhs, M const & rhs )
        {
          for(size_t i = 0 ; i<I ; ++i)
          {
            for(size_t j = 0 ; j<J ; j+=stride )
            {
              type_traits::sub_assign(M::accessor::cast(lhs,i,j), M::accessor::cast(rhs,i,j));
            }
          }
          return lhs;
        }
        
//        static M& mul_assign (M & lhs, M const & rhs )
//        {
//          for(size_t i = 0 ; i<I ; ++i)
//          {
//            for(size_t j=0 ; j<J ; j+=stride )
//            {
//              type_traits::mul_assign(M::accessor::cast(lhs,i,j), M::accessor::cast(rhs,i,j));
//            }
//          }
//          return lhs;
//        }
//        
//        static M& div_assign (M & lhs, M const & rhs )
//        {
//          for(size_t i = 0 ; i<I ; ++i)
//          {
//            for(size_t j=0 ; j<J ; j+=stride )
//            {
//              type_traits::div_assign(M::accessor::cast(lhs,i,j), M::accessor::cast(rhs,i,j));
//            }
//            
//            if(remainder)
//            {
//              op_type tmp = M::accessor::cast(rhs,i,last);
//              type_traits::template set_pad_value<remainder>(tmp,value_traits::one());
//              type_traits::div_assign(M::accessor::cast(lhs,i,last),tmp);
//            }
//          }
//          return lhs;
//        }
        
        static M mul (M const & lhs, real_type const & rhs)
        {
          M result(lhs);
          return mul_assign(result,rhs);
        }
        
        static M div (M const & lhs, real_type const & rhs)
        {
          M result(lhs);
          return div_assign(result,rhs);
        }
        
        static M& mul_assign (M & lhs, real_type const & rhs)
        {
          const op_type op_type_rhs = type_traits::set_op_type(rhs);
          
          for(size_t i = 0 ; i < I ; ++i )
          {
            for(size_t j = 0 ; j < J ; j+=stride )
            {
              type_traits::mul_assign(M::accessor::cast(lhs,i,j), op_type_rhs);
            }
          }
          return lhs;
        }
        
        static M& div_assign (M & lhs, real_type const & rhs)
        {
          assert(rhs != value_traits::zero() || !"Division by zero!");
          const op_type op_type_rhs = type_traits::set_op_type(rhs);
          
          for(size_t i = 0 ; i < I ; ++i)
          {
            for(size_t j = 0 ; j < J ; j+=stride )
            {
              type_traits::div_assign(M::accessor::cast(lhs,i,j), op_type_rhs);
            }
          }
          return lhs;
        }
        
        static M negate (M const & rhs)
        {
          M result;
          
          for(size_t i = 0 ; i < I ; ++i)
          {
            for(size_t j = 0 ; j < J ; j+=stride )
            {
              M::accessor::cast(result,i,j) = type_traits::negate(M::accessor::cast(rhs,i,j));
            }
          }
          return result;
        }
        
        // Various Functions ------------------------------------------------
        
        static M sign (M const & rhs)
        {
          M result;
          
          for(size_t i = 0 ; i < I ; ++i)
          {
            for(size_t j = 0 ; j < J ; j+=stride )
            {
              M::accessor::cast(result,i,j) = type_traits::sign(M::accessor::cast(rhs,i,j));
            }
          }
          return result;
        }
        
      static M round (M const & rhs)
        {
          M result;
          
          for(size_t i = 0 ; i < I ; ++i)
          {
            for(size_t j = 0 ; j < J ; j+=stride )
            {
              M::accessor::cast(result,i,j) = type_traits::round(M::accessor::cast(rhs,i,j));
            }
          }
          return result;
        }
        
        static M min (M const & lhs, M const & rhs)
        {
          M result;
          
          for(size_t i = 0 ; i < I ; ++i)
          {
            for(size_t j = 0 ; j < J ; j+=stride )
            {
              M::accessor::cast(result,i,j) = type_traits::min(M::accessor::cast(lhs,i,j), M::accessor::cast(rhs,i,j));
            }
          }
          return result;
        }
        
        static M max (M const & lhs, M const & rhs)
        {
          M result;
          
          for(size_t i = 0 ; i < I ; ++i)
          {
            for(size_t j = 0 ; j < J ; j+=stride )
            {
              M::accessor::cast(result,i,j) = type_traits::max(M::accessor::cast(lhs,i,j),M::accessor::cast(rhs,i,j));
            }
          }
          return result;
        }
        
        static M abs (M const & rhs)
        {
          M result;
          
          for(size_t i = 0 ; i < I ; ++i)
          {
            for(size_t j = 0 ; j < J ; j+=stride )
            {
              M::accessor::cast(result,i,j) = type_traits::abs(M::accessor::cast(rhs,i,j));
            }
          }
          return result;
        }
        
        static M floor (M const & rhs)
        {
          M result;
          
          for(size_t i = 0 ; i < I ; ++i)
          {
            for(size_t j = 0 ; j < J ; j+=stride )
            {
              M::accessor::cast(result,i,j) = type_traits::floor(M::accessor::cast(rhs,i,j));
            }
          }
          return result;
        }
        
        static M ceil (M const & rhs)
        {
          M result;
          
          for(size_t i = 0 ; i < I ; ++i)
          {
            for(size_t j = 0 ; j < J ; j+=stride )
            {
              M::accessor::cast(result,i,j) = type_traits::ceil(M::accessor::cast(rhs,i,j));
            }
          }
          return result;
        }
        
//        static M sqrt (M const & rhs)
//        {
//          M result;
//          
//          for(size_t i = 0 ; i < I ; ++i)
//          {
//            for(size_t j = 0 ; j < J ; j+=stride )
//            {
//              M::accessor::cast(result,i,j) = type_traits::sqrt(M::accessor::cast(rhs,i,j));
//            }
//          }
//          return result;
//        }
//        
//        static M rsqrt (M const & rhs)
//        {
//          M result;
//          
//          for(size_t i = 0 ; i < I ; ++i)
//          {
//            for(size_t j = 0 ; j < J ; j+=stride )
//            {
//              M::accessor::cast(result,i,j) = type_traits::rsqrt(M::accessor::cast(rhs,i,j));
//            }
//            if (remainder)
//            {
//              op_type tmp = M::accessor::cast(rhs,i,last);
//              type_traits::template set_pad_value<remainder>(tmp,value_traits::one());
//              tmp = type_traits::rsqrt(tmp);
//              type_traits::template set_pad_value<remainder>(tmp,value_traits::zero());
//              M::accessor::cast(result,i,last) = tmp;
//            }
//          }
//          return result;
//        }
        
        // Horizontal functions ----------------------------------
        
        static real_type sum (M const & rhs)
        {
          op_type result = value_traits::zero();  // 2009-07-13 Kenny: Proper initialization?  
          for(size_t i = 0 ; i<I ; ++i)
          {
            for(size_t j=0 ; j<J ; j+=stride )
            {
              type_traits::add_assign(result,M::accessor::cast(rhs,i,j));
            }
            // 2009-07-14 Kenny: warning C4127: conditional expression is constant
            if(remainder) // Necessary otherwise pad-values may end up being the final result
              type_traits::template set_pad_value<remainder>( result, value_traits::zero());
          }
          return type_traits::sum(result);
        }
        
        static real_type min (M const & rhs)
        {
          op_type result = M::accessor::cast(rhs,0,0);
          for(size_t i = 0 ; i<I ; ++i)
          {
            for(size_t j=0 ; j<J ; j+=stride )
            {
              result = type_traits::min(result,M::accessor::cast(rhs,i,j));
            }
            // 2009-07-14 Kenny: warning C4127: conditional expression is constant
            if(remainder) // Necessary otherwise pad-values may end up being the final result
              type_traits::template set_pad_value<remainder>( result, value_traits::highest());
          }
          return type_traits::min(result);
        }
        
        static real_type max (M const & rhs)
        {
          op_type result = M::accessor::cast(rhs,0,0);
          for(size_t i = 0 ; i<I ; ++i)
          {
            for(size_t j=0 ; j<J ; j+=stride )
            {
              result = type_traits::max(result,M::accessor::cast(rhs,i,j));
            }
            // 2009-07-14 Kenny: warning C4127: conditional expression is constant
            if(remainder) // Necessary otherwise pad-values may end up being the final result
              type_traits::template set_pad_value<remainder>(result, value_traits::lowest());
          }
          return type_traits::max(result);
        }
      };
    
  } // namspace detail
} // namspace tiny

// TINY_COMMON_OPS_POLICY_H
#endif 

