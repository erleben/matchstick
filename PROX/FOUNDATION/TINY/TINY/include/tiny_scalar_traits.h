#ifndef TINY_SCALAR_TRAITS_H
#define TINY_SCALAR_TRAITS_H

#include <cstddef> // for size_t
#include <cmath>   // for various math functions

#include <tiny_constants.h>
#include <tiny_functions.h> // for sgn(const T&)

namespace tiny
{
  
  template<typename T>
  class ScalarTraits
    {
    public:
      
      typedef T  real_type;
      typedef T  op_type;
      
      enum { stride = 1 };
      
      /**
       * op_type Creation
       *
       *  Builds an op_type from a scalar by copying the scalar into all elements of the op_type.
       *  \param[in] scalar The Scalar to be copied into the op_type.
       *  \return op_type with all elements set to scalar value.
       */
      static op_type set_op_type(real_type const & scalar) 
      { 
        return scalar;
      }
      
      static op_type load_op_type(real_type const * scalars) 
      { 
        return scalars[0];
      }
      
      
      static real_type* store_op_type(real_type * const rt, op_type const & ot) 
      { 
        *rt = ot;
        return rt;
      }
      
      template<size_t N>
      static op_type& set_pad_value(op_type & ot, real_type /*value*/)
      {
        return ot;
      }
      
      // Boolean operations ---------------------------------------------------------------------------
      
      /*! Equality Test
       *
       *   Performs elementwise equality test on the two provided op_type arguments,
       *   and returns an op_type containing the result of each comparison.
       *  \param[in] lhs left hand side op_type of the operation.
       *  \param[in] rhs right hand side op_type of the operation.
       *   \return op_type with result of each comparions.
       */
      static op_type equal(op_type const & lhs, op_type const & rhs)
      {
        return fabs(lhs-rhs)<real_type(1.0/8192.0); // Reimplement with ulps
        //return lhs==rhs;  // reimplement with threshold
      }
      
      /*! Non-Equality Test
       *
       *   Performs elementwise not-equal tests on the two provided op_type arguments,
       *   and returns an op_type containing the result of each comparison.
       *  \param[in] lhs left hand side op_type of the operation.
       *  \param[in] rhs right hand side op_type of the operation.
       *   \return op_type with result of each comparions.
       */
      static op_type not_equal(op_type const & lhs, op_type const & rhs)
      {
        return !(lhs==rhs); // reimplement with threshold
      }
      
      /*! Less-Than Test
       *
       *   Performs elementwise less-than tests on the two provided op_type arguments,
       *   and returns an op_type containing the result of each comparison.
       *   \param[in] lhs left hand side op_type of the operation.
       *   \param[in] rhs right hand side op_type of the operation.
       *   \return op_type with result of each comparions.
       */
      static op_type less_than(op_type const & lhs, op_type const & rhs)
      {
        return lhs<rhs;
      }
      
      /*! Greater-Than Test
       *
       *   Performs elementwise less-than tests on the two provided op_type arguments,
       *   and returns an op_type containing the result of each comparison.
       *  \param[in] lhs left hand side op_type of the operation.
       *  \param[in] rhs right hand side op_type of the operation.
       *   \return op_type with result of each comparions.
       */
      static op_type greater_than(op_type const & lhs, op_type const & rhs) 			{ return lhs>rhs; }
      
      /*! Less-Than-Or-Equal Test
       *
       *   Performs elementwise less-than-or-equal tests on the two provided op_type arguments,
       *   and returns an op_type containing the result of each comparison.
       *  \param[in] lhs left hand side op_type of the operation.
       *  \param[in] rhs right hand side op_type of the operation.
       *   \return op_type with result of each comparions.
       */
      static op_type less_than_or_equal(op_type const & lhs, op_type const & rhs) 	{ return lhs<=rhs; }
      
      
      /*! Greater-Than-Or-Equal Test
       *
       *   Performs elementwise greater-than-or-equal tests on the two provided op_type arguments,
       *   and returns an op_type containing the result of each comparison.
       *  \param[in] lhs left hand side op_type of the operation.
       *  \param[in] rhs right hand side op_type of the operation.
       *   \return op_type with result of each comparions.
       */
      static op_type greater_than_or_equal(op_type const & lhs, op_type const & rhs)	{ return lhs>=rhs; }
      
      // Basic Arithmetics -----------------------------------------------------------
      /*! Addition Operation
       *
       *  Adds the two provided op_type parameters elements-wise and returns a new op_type with the
       *  result of the operation.
       *  \param[in] lhs left hand side op_type of the operation.
       *  \param[in] rhs right hand side op_type of the operation.
       *  \return op_type with result of each addition.
       */
      static op_type add(op_type const & lhs, op_type const & rhs)
      {
        op_type tmp = lhs;
        return ScalarTraits::add_assign(tmp,rhs);
        //op_type tmp(lhs);
        //return ScalarTraits::template add_assign<1>(op_type(lhs),rhs);
      }
      
      /*! Subtraction Operation
       *
       *  Subtracts element-wise the second parameter from the first parameter and a new op_type with the
       *  result of the operation.
       *  \param[in] lhs left hand side op_type of the operation.
       *  \param[in] rhs right hand side op_type of the operation.
       *  \return op_type with result of each addition.
       */
      static op_type  sub(op_type const & lhs, op_type const & rhs)
      {
        op_type tmp = lhs;
        return ScalarTraits::sub_assign(tmp,rhs);
      }
      
      /*! Multiplication Operation
       *
       *  Multiplies the two provided op_type parameters elements-wise and returns a new op_type with the
       *  result of the operation.
       *  \param[in] lhs left hand side op_type of the operation.
       *  \param[in] rhs right hand side op_type of the operation.
       *  \return op_type with result of each multiplication.
       */
      static op_type  mul(op_type const & lhs, op_type const & rhs)
      {
        op_type tmp = lhs;
        return ScalarTraits::mul_assign(tmp,rhs);
      }
      
      /*! Division Operation
       *
       *  Divides element-wise the first parameter by the second parameter and a new op_type with the
       *  result of the operation.
       *  \param[in] lhs left hand side op_type of the operation.
       *  \param[in] rhs right hand side op_type of the operation.
       *  \return op_type with result of each division.
       */
      static op_type div(op_type const & lhs, op_type const & rhs)
      {
        op_type tmp = lhs;
        return ScalarTraits::div_assign(tmp,rhs);
      }
      
      /*! Addition And Assignment Operation
       *
       *  Adds element-wise the rhs parameter from the lhs parameter and stores the result in the lhs parameter.
       *  \param[in] lhs left hand side op_type of the operation.
       *  \param[in] rhs right hand side op_type of the operation.
       *  \return reference to the lhs parameter.
       */
      static op_type& add_assign(op_type & lhs, op_type const & rhs)
      {
        return lhs+=rhs;
      }
      
      /*! Subtraction And Assignment Operation
       *
       *  Subtracts element-wise the rhs parameter from the lhs parameter and stores the result in the lhs parameter.
       *  \param[in] lhs left hand side op_type of the operation.
       *  \param[in] rhs right hand side op_type of the operation.
       *  \return reference to the lhs parameter.
       */
      static op_type& sub_assign(op_type & lhs, op_type const & rhs)
      {
        return (lhs-=rhs);
      }
      
      /*! Multiplication And Assignment Operation
       *
       *  Multiplies element-wise the rhs parameter with the lhs parameter and stores the result in the lhs parameter.
       *  \param[in] lhs left hand side op_type of the operation.
       *  \param[in] rhs right hand side op_type of the operation.
       *  \return reference to the lhs parameter.
       */
      static op_type& mul_assign(op_type & lhs, op_type const & rhs)	
      { 
        return lhs*=rhs; 
      }
      
      /*! Division And Assignment Operation
       *
       *  Divides element-wise the lhs parameter by the rhs parameter and stores the result in the lhs parameter.
       *  \param[in] lhs left hand side op_type of the operation.
       *  \param[in] rhs right hand side op_type of the operation.
       *  \return reference to the lhs parameter.
       */
      static op_type& div_assign(op_type & lhs, op_type const & rhs)
      {
        return lhs/=rhs;
      }
      
      /*! Negation Operation
       *
       *  Negates all elements in the ot parameter and stores the result a new op_type.
       *  \param[in] ot op_type to be negated.
       *  \return op_type with the result of the operation.
       */
      static op_type negate(op_type const & rhs)
      {
        return -rhs;
      }
      
      // Various functions ----------------------------------------------------------------------
      /*! Sign Evaluation
       *
       *  Evaluates the sign of each element and returns an op_type with the results:
       *    A result of 1 indicates positive sign.
       *    A result of 0 indidates undefined sign (evaluated value was zero).
       *    A result of -1 indicates negative sign.
       *  \param[in] ot op_type to be sign evaluated.
       *  \return op_type with the result of the operation.
       */
      static op_type sign (op_type const & ot)
      {
        return sgn(ot);
      }
      
      /*! Random Operation
       *
       *  Replaces provided parameters elements with random values.
       *  \param[out] ot op_type to recieve random values.
       *  \return reference to parameter.
       */
      /*static op_type&	random (op_type & ot)
       {
       return ot = drand48(); // Replcae with something crossplatform
       }*/
      
      /*! Round Operation
       *
       *  Rounds each element of provided parameter to nearest integer, away from zero
       *  \param[in] ot op_type to be rounded.
       *  \return op_type with rounded values.
       */
      static op_type round (op_type const & ot)
      {
        // 2009-07-14 Kenny: round is only part of cmath in C99 compliant compilers.
#ifdef __GNUC__
        return ::round(ot);
#else
        using std::floor;
        using std::ceil;
        return ot>tiny::detail::zero<op_type>() ?  floor( ot+tiny::detail::half<op_type>()) :  ceil( ot-tiny::detail::half<op_type>());
#endif
      }
      
      /*! Max Test
       *
       *   Performs elementwise max tests on the two provided op_type arguments,
       *   and returns an op_type containing the result of each comparison.
       *   \param[in] lhs left hand side op_type of the operation.
       *   \param[in] rhs right hand side op_type of the operation.
       *   \return op_type with result of each comparions.
       */
      static op_type max (op_type const & lhs, op_type const & rhs)
      {
        return lhs>=rhs?lhs:rhs;
      }
      
      static op_type& max_assign (op_type & lhs, op_type const & rhs)
      {
        return lhs = (lhs>=rhs?lhs:rhs);
      }
      
      
      /*! Min Test
       *
       *   Performs elementwise max tests on the two provided op_type arguments,
       *   and returns an op_type containing the result of each comparison.
       *   \param[in] lhs left hand side op_type of the operation.
       *   \param[in] rhs right hand side op_type of the operation.
       *   \return op_type with result of each comparions.
       */
      static op_type min (op_type const & lhs, op_type const & rhs)
      {
        return lhs<=rhs?lhs:rhs;
      }
      
      static op_type& min_assign (op_type & lhs, op_type const & rhs)
      {
        return lhs = (lhs<=rhs?lhs:rhs);
      }
      
      
      /*! Absolute Value Evaluation
       *
       *  Evaluates the absolute value of each element of provided parameter to nearest integer.
       *  \param[in] ot op_type to be evaluated.
       *  \return op_type with absolute values.
       */
      static op_type abs (op_type const & ot)
      {
        return fabs(ot); // TODO: verify
      }
      
      /*! Floor Operation
       *
       *  Rounds down each element of provided parameter to nearest integral value.
       *  \param[in] ot op_type to be floored.
       *  \return op_type with floored values.
       */
      static op_type floor (op_type const & ot)
      {
        return ::floor(ot);
      } // TODO: reimplement
      
      /*! Ceil Operation
       *
       *  Rounds up each element of provided parameter to smallest integral value greater than
       *  the element itself.
       *  \param[in] ot op_type to be ceiled.
       *  \return op_type with ceiled values.
       */
      static op_type  ceil (op_type const & ot)
      {
        return ::ceil(ot);
      } // TODO: reimplement
      
      /*! Square Root Operation
       *
       *  Calculates the square root of each element of provided parameter.
       *  \param[in] ot op_type to perform square root on.
       *  \return op_type with square root values.
       */
      static op_type  sqrt (op_type const & ot)
      {
        return ::sqrt(ot);
      } // TODO: reimplement
      
      /*! Reciprocal Square Root Operation
       *
       *  Calculates the reciprocal square root (1/sqrt) of each element of provided parameter.
       *  \param[in] ot op_type to perform reciprocal square root on.
       *  \return op_type with reciprocal square root values.
       */
      static op_type  rsqrt (op_type const & ot)
      {
        return real_type(1)/sqrt(ot);
      } // TODO: reimplement
      
      // Horizontal functions -------------------------------------------------------------
      
      /*! Horizontal Summation Operation
       *
       * Calculates the sum of all elements in provided op_type parameter
       * \param[in] ot op_type to be summed.
       * \return real_type sum of operation.
       */
      static real_type sum (op_type const & ot) 
      { 
        return ot; 
      }
      
      /*! Horizontal Multiplication Operation
       *
       * Calculates the product of all elements in provided op_type parameter
       * \param[in] ot op_type to be multiplied.
       * \return real_type product of operation.
       */
      static real_type mul (op_type const & ot)
      { 
        return ot;
      }
      
      /*! Horizontal Min Operation
       *
       * Evaluates the min value of all elements in provided op_type parameter
       * \param[in] ot op_type to be evaluated.
       * \return real_type min value of operation.
       */
      static real_type min (op_type const & ot) 						
      { 
        return ot;
      }
      
      /*! Horizontal Max Operation
       *
       * Evaluates the max value of all elements in provided op_type parameter
       * \param[in] ot op_type to be evaluated.
       * \return real_type max value of operation.
       */
      static real_type max (op_type const & ot)
      { 
        return ot; 
      }
    };
  
} // namespace tiny

//TINY_SCALAR_TRAITS_H
#endif
