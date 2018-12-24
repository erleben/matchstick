#ifndef TINY_FUNCTIONS_H
#define TINY_FUNCTIONS_H

#include <tiny_constants.h> // for zero, one, degree and radian

#include <algorithm>       // for min and max
#include <cmath>           // for fabs and sin

/**
 * The following functions are defined below:
 * - clamp (and the variations: clamp_min, clamp_max, clamp_zero_one)
 * - fac (faculty)
 * - sgn (sign function)
 * - sinc
 */	
namespace tiny
{
    /**
     * clamp function to clamp a value to be in the interval (min_value; max_value).
     *
     * @param value      The value to be clamped.
     * @param min_value  The minimum allowed value.
     * @param max_value  The maximum allowed value (ohhh, really?).
     * @return           The clamped value. If value already is in (min_value; max_value) no clamping is performed.
     */
    template<typename T>
    inline T clamp(T const & value, T const & min_value, T const & max_value)
    {
      assert(min_value <= max_value || !"max_value cannot be less than min_value");
      using std::min;
      using std::max;

      return T(min(max_value, max(min_value, value)));
    }

    /**
     * clamp function to clamp a value never to be less than min_value.
     *
     * @param value      The value to be clamped.
     * @param min_value  The minimum allowed value.
     * @return           The clamped value if value is less than min_value, otherwise the original value is returned.
     */
    template<typename T>
    inline T clamp_min(T const & value, T const & min_value)
    {
    	using std::max;
      return clamp(value, min_value, max(value, min_value));
    }

    /**
     * clamp function to clamp a value never to be greater than max_value.
     *
     * @param value      The value to be clamped.
     * @param max_value  The maximum allowed value.
     * @return           The clamped value if value is greater than max_value, otherwise the original value is returned.
     */
    template<typename T>
    inline T clamp_max(T const & value, T const & max_value)
    {
    	using std::min;
      return clamp(value, min(value, max_value), max_value);
    }

    /**
     * clamp function to easily clamp a value between 0 and 1.
     * note: this function is mostly usable for T = some real type.
     *
     * @param value      The value to be clamped.
     * @return           The clamped value. If value already is in (0; 1) no clamping is performed.
     */
    template<typename T>
    inline T clamp_zero_one(T const & value)
    {
      return clamp(value, detail::zero<T>(), detail::one<T>());
    }

    template<typename T>
    inline T fac(unsigned long n)
    {
      // TODO what about implicit type conversions? This could have been done more elegangtly using partial specialization  
      unsigned long val = 1;
      for(; n > 0; val *= n--);
      return T(val);
    }

    template<typename T>
    inline T sgn(T const & val)
    {
      return val > detail::zero<T>() ? detail::one<T>() : val < detail::zero<T>() ? - detail::one<T>() : detail::zero<T>();
    }

    /**
    * Compute Sinc Function.
    * The implementation of this method was greatly inspired by the
    * one in Open Dynamics Engine v. 0.039
    *
    * This method returns sin(x)/x. this has a singularity at 0 so special
    * handling is needed for small arguments.
    *
    * @param x
    * @return   The value of sin(x)/x
    */
    template<typename T>
    inline T sinc(T & x)
    {
      using std::fabs;
      using std::sin;

      static T const tiny   = tiny::detail::numeric_cast<T>(1.0e-4);
      static T const factor = tiny::detail::numeric_cast<T>(0.166666666666666666667);

      //--- if |x| < 1e-4 then use a taylor series expansion. this two term expansion
      //--- is actually accurate to one LS bit within this range if double precision
      //--- is being used - so don't worry!
      return (fabs(x) < tiny) ? (detail::one<T>() - x*x*factor) : (sin(x)/x);
    }

}  // namespace tiny

// TINY_FUNCTIONS_H
#endif
