#ifndef TINY_CONSTANTS_H
#define TINY_CONSTANTS_H

#include <boost/numeric/conversion/bounds.hpp>  // for  boost::numeric::bounds<T>::lowest() and  boost::numeric::bounds<T>::highest()
#include <boost/cast.hpp>                       // for boost::numeric_cast

#include <cmath>

#ifdef _MSC_VER
#  ifndef M_PI
#    define M_PI 3.14159265358979323846 /* pi */
#  endif
#  ifndef M_PI_2
#    define M_PI_2 1.57079632679489661923 /* pi/2 */
#  endif
#  ifndef M_PI_4
#    define M_PI_4 0.78539816339744830962  /* pi/4 */
#  endif
#endif

namespace tiny
{

    namespace detail
    {

      template <typename T>
      inline T zero();

      template <>
      inline float zero<float>() { return 0.0f; }

      template <>
      inline double zero<double>() { return 0.0; }

      template <>
      inline int zero<int>() { return 0; }

      template <>
      inline long unsigned int zero<long unsigned int>() { return 0; }

      template <>
      inline unsigned int zero<unsigned int>() { return 0u; }

      template <typename T>
      inline T one();

      template <>
      inline float one<float>() { return 1.0f; }

      template <>
      inline double one<double>() { return 1.0; }

      template <>
      inline int one<int>() { return 1; }

      template <>
      inline unsigned int one<unsigned int>() { return 1u; }

      template <typename T>
      inline T two();

      template <>
      inline float two<float>() { return 2.0f; }

      template <>
      inline double two<double>() { return 2.0; }

      template <>
      inline int two<int>() { return 2; }

      template <>
      inline unsigned int two<unsigned int>() { return 2u; }

      template <typename T>
      inline T three();

      template <>
      inline float three<float>() { return 3.0f; }

      template <>
      inline double three<double>() { return 3.0; }

      template <>
      inline int three<int>() { return 3; }

      template <>
      inline unsigned int three<unsigned int>() { return 3u; }

      template <typename T>
      inline T four();

      template <>
      inline float four<float>() { return 4.0f; }

      template <>
      inline double four<double>() { return 4.0; }

      template <>
      inline int four<int>() { return 4; }

      template <>
      inline unsigned int four<unsigned int>() { return 4u; }

      template <typename T>
      inline T eight();

      template <>
      inline float eight<float>() { return 8.0f; }

      template <>
      inline double eight<double>() { return 8.0; }

      template <>
      inline int eight<int>() { return 8; }

      template <>
      inline unsigned int eight<unsigned int>() { return 8u; }

      template <typename T>
      inline T half();

      template <>
      inline float half<float>() { return 0.5f; }

      template <>
      inline double half<double>() { return 0.5; }

      template<typename T>
      inline T pi() { return boost::numeric_cast<T>(M_PI); }

      template<typename T>
      inline T pi_half() {  return boost::numeric_cast<T>(M_PI_2); }

      template<typename T>
      inline T pi_quarter() {  return boost::numeric_cast<T>(M_PI_4); }

      template <typename T>
      inline T highest() { return boost::numeric::bounds<T>::highest(); }

      template <typename T>
      inline T lowest() { return boost::numeric::bounds<T>::lowest(); }

      template <typename T>
      inline T infinity(){ return highest<T>(); }

      template<typename T>
      inline T radians_per_degree() {  return boost::numeric_cast<T>(0.017453292519943295769236907684886); }

      template<typename T>
      inline T degrees_per_radian() {  return boost::numeric_cast<T>(57.295779513082320876798154814105); }

      template<typename R, typename T>
      inline R numeric_cast(T const & value) {  return boost::numeric_cast<R>(value); }
            
    } // namespace detail
  
}  // namespace tiny

// TINY_CONSTANTS_H
#endif
