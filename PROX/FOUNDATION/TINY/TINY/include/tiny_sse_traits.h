#ifndef TINY_SSE_TRAITS_H
#define TINY_SSE_TRAITS_H

#include <tiny_aligned_16.h> // for ALIGNED_16 macro
#include <float.h>
#include <cstddef>          // for size_t
#include <xmmintrin.h>      // for __mm128
#include <emmintrin.h>      // for __mm128i
#include <cmath>

// More information can be found at
//
//  http://developer.apple.com/documentation/Performance/Conceptual/Accelerate_sse_migration/migration_sse_translation/chapter_4_section_2.html
//
//and
//
//  http://ompf.org/forum/viewtopic.php?f=11&t=613


#define FLT_NINF (*(float*)_epi32_ninf)
#define FLT_PINF (*(float*)_epi32_pinf)

#define EPI32_CONST(name,x)         static ALIGNED_16 int   const _epi32_##name[4] = {x,x,x,x};
#define EPI32_CONST4(name,x,y,z,w)  static ALIGNED_16 int   const _epi32_##name[4] = {w,z,y,x};
#define PS_CONST(name,x)            static ALIGNED_16 float const _ps_##name[4]    = {x,x,x,x};
#define PS_CONST4(name,x,y,z,w)     static ALIGNED_16 float const _ps_##name[4]    = {w,z,y,x};

namespace tiny
{
  /**
   * The SSE vector extension found in newer Intel architectures
   * provides eight 128 bit-wide registers (XMM0-XMM7), capable of
   * operating on multiple element simultainously.
   */

  EPI32_CONST(sign_mask,0x80000000)
  EPI32_CONST(inv_sign_mask,0x7FFFFFFF)
  EPI32_CONST(mant_mask,0x7F800000)
  EPI32_CONST(inv_mant_mask,0x807FFFFF)
  EPI32_CONST(min_norm_pos,0x00800000)

  EPI32_CONST(1,1)
  EPI32_CONST(2,2)
  EPI32_CONST(7,7)
  EPI32_CONST(127,127)
  EPI32_CONST(ninf,0xFF800000)
  EPI32_CONST(pinf,0x7F800000)

  PS_CONST(2pow23,8388608.0f)
  PS_CONST(1,1.0f)

  int       imask = 0xffffffff;
  long long lmask = 0xffffffffffffffffLL;  // 2009-07-11 Kenny: GCC 4.2 warning: 'tiny::<unnamed>::lmask' defined but not used
  float     fmask = *((float*)&imask);
  float     dmask = *((double*)&fmask);

  static const __m128 maxf     = (__m128) { 0x1.0p23f, 0x1.0p23f, 0x1.0p23f, 0x1.0p23f };
  static const __m128 onesf    = (__m128) { 0x1.0p0f, 0x1.0p0f, 0x1.0p0f, 0x1.0p0f };
  static const __m128 zerosf   = (__m128) { 0x0, 0x0, 0x0, 0x0 };
  static const __m128 mask0000 = (__m128) { 0x0,0x0,0x0,0x0 };
  static const __m128 mask0001 = (__m128) { 0x0,0x0,0x0,fmask };
  static const __m128 mask0011 = (__m128) { 0x0,0x0,fmask,fmask };
  static const __m128 mask0111 = (__m128) { 0x0,fmask,fmask,fmask};
  static const __m128 mask1111 = (__m128) _mm_load_ps1(&fmask);
  static const __m128 mask1110 = (__m128) { fmask,fmask,fmask,0x0 };
  static const __m128 mask1100 = (__m128) { fmask,fmask,0x0,0x0 };
  static const __m128 mask1000 = (__m128) { fmask,0x0,0x0,0x0 };
  static const __m128d onesd   = _mm_set_pd1(1.0);
  static const __m128d zerosd  = _mm_set_pd1(0.0);


  static inline __m128 _mm_fabs_ps ( __m128 value)
  {
    return  (__m128) _mm_srli_epi32( _mm_slli_epi32( (__m128i) value, 1 ), 1 );
  }

  static inline __m128 _mm_round_ps(__m128 x)
  {
    float vector[4];
    _mm_store_ps(vector,x);
    for (size_t i=0;i<4;++i)
      vector[i]= ::round(vector[i]);
    return _mm_load_ps(vector);
    //#const __m128 t=_mm_or_ps( _mm_and_ps( _mm_load_ps((const float*)_epi32_sign_mask),x),	_mm_load_ps(_ps_2pow23) );
    //return _mm_sub_ps(_mm_add_ps(x,t),t);
  }

  /*static inline __m128 _mm_random_ps()
  {
  float rand[4];

  rand[0]=drand48();
  rand[1]=drand48();
  rand[2]=drand48();
  rand[3]=drand48();

  return _mm_load_ps(rand);
  }*/

  static inline __m128 _mm_floor_ps(__m128 v)
  {
    static const __m128 twoTo23 =_mm_load_ps((const float*)_ps_2pow23);
    __m128 b = _mm_fabs_ps(v);
    __m128 d = _mm_sub_ps( _mm_add_ps( _mm_add_ps( _mm_sub_ps( v, twoTo23 ), twoTo23 ), twoTo23 ), twoTo23 ); //the meat of floor
    __m128 largeMaskE = (__m128) _mm_cmpgt_ps( b, twoTo23 ); //-1 if v >= 2**23
    __m128 g = (__m128) _mm_cmplt_ps( v, d ); //check for possible off by one error
    __m128 h = _mm_cvtepi32_ps( (__m128i) g ); //convert positive check result to -1.0, negative to 0.0
    __m128 t = _mm_add_ps( d, h ); //add in the error if there is one
    //Select between output result and input value based on v >= 2**23
    v = _mm_and_ps( v, largeMaskE );
    t = _mm_andnot_ps( largeMaskE, t );
    return _mm_or_ps( t, v );
  }

  static inline __m128 _mm_ceil_ps(__m128 x)
  {
    const __m128 t = _mm_round_ps(x);
    return _mm_add_ps(t,_mm_and_ps(_mm_cmpgt_ps(x,t),_mm_load_ps(_ps_1)));
  }

  static inline __m128 _mm_negate_ps( __m128 v )
  {
    return _mm_sub_ps(_mm_setzero_ps(),v);
  }

  template <typename real_type_>
  class SSETraits{};

  /*
  SSE registers used in sp fp (single precision floating point)
  mode (__m128-type) allows for four 32 bits floating point
  elements in one register, with the following layout:

  SSE register: | fp3 | fp2 | fp1 | fp0 | = 128 bits

  Each 32 bit element is stored in IEEE-754 fp sp format:

  | s (1 bit) | exponent (8 bits) | mantissa (23 bits) |
  */

  template<>
  class SSETraits<float>
  {
  public:

    typedef float 		real_type;
    typedef  __m128		op_type;

    enum { stride = 4 };

    // Load/Store
    // Loads scalar into all register elements
    static op_type set_op_type (real_type const & scalar)
    {
      return _mm_load_ps1(&scalar); //  return _mm_set_ps1( scalar );
    }

    static op_type load_op_type (real_type const * scalar)
    {
      return _mm_load_ps(scalar); //  return _mm_set_ps1( scalar );
    }

    // Store all register elements to array
    static real_type * store_op_type (real_type * const array, op_type const & ot)
    {
      _mm_store_ps(array,ot);
      return array;
    }

    /*                // Store specific register element into scalar
    template <size_t idx>
    static real_type& store_op_type (real_type & scalar, op_type const & ot)
    {
    if (idx==0)
    {
    _mm_store_ss(&scalar,ot);
    }
    else
    {
    op_type tmp = _mm_shuffle_ps(ot,ot,_MM_SHUFFLE(3,2,1,idx));
    _mm_store_ss(&scalar,tmp);
    }
    return scalar;
    }

    template <size_t idx>
    static real_type store_op_type (op_type const & ot)
    {
    real_type scalar;
    if (idx==0)
    {
    _mm_store_ss(&scalar,ot);
    }
    else
    {
    op_type tmp = _mm_shuffle_ps(ot,ot,_MM_SHUFFLE(3,2,1,idx));
    _mm_store_ss(&scalar,tmp);
    }
    return scalar;
    }

    static real_type& store_op_type (real_type & scalar, op_type const & ot, size_t idx)
    {
    assert(idx<stride);
    op_type tmp = ot;;

    switch (idx)
    {
    case 0: break;
    case 1: tmp = _mm_shuffle_ps(tmp,tmp,_MM_SHUFFLE(3,2,1,1)); break;
    case 2: tmp = _mm_shuffle_ps(tmp,tmp,_MM_SHUFFLE(3,2,1,2)); break;
    case 3: tmp = _mm_shuffle_ps(tmp,tmp,_MM_SHUFFLE(3,2,1,3)); break;
    }
    _mm_store_ss(&scalar,tmp);
    return scalar;
    }

    static real_type store_op_type (op_type const & ot, size_t idx)
    {
    assert(idx<stride);
    real_type scalar;
    op_type tmp = ot;;

    switch (idx)
    {
    case 0: break;
    case 1: tmp = _mm_shuffle_ps(tmp,tmp,_MM_SHUFFLE(3,2,1,1)); break;
    case 2: tmp = _mm_shuffle_ps(tmp,tmp,_MM_SHUFFLE(3,2,1,2)); break;
    case 3: tmp = _mm_shuffle_ps(tmp,tmp,_MM_SHUFFLE(3,2,1,3)); break;
    }
    _mm_store_ss(&scalar,tmp);
    return scalar;
    }
    */

    /**
    * Pads remainder of __m128 with value
    * N determines how many elements to be padded
    */
    template<size_t N>
    static void set_pad_value(op_type & ot, real_type value)
    {
      op_type const pad = _mm_load_ps1(&value);
      switch (N)
      {
      case 1:
        ot = _mm_add_ps(_mm_and_ps(mask1110,ot),_mm_and_ps(mask0001,pad));
        break;
      case 2:
        ot = _mm_add_ps(_mm_and_ps(mask1100,ot),_mm_and_ps(mask0011,pad));
        break;
      case 3:
        ot = _mm_add_ps(_mm_and_ps(mask1000,ot),_mm_and_ps(mask0111,pad));
        break;
      }
    }

    // Boolean operations

    static op_type equal (op_type const & lhs, op_type const & rhs)
    {
      const op_type epsilon = _mm_set_ps1(real_type(1.0/8192.0));
      return _mm_and_ps(onesf,_mm_cmplt_ps(_mm_fabs_ps(_mm_sub_ps(lhs,rhs)), epsilon ));
    }

    static op_type not_equal (op_type const & lhs, op_type const & rhs)
    {
      return _mm_and_ps(onesf,_mm_cmpneq_ps(lhs,rhs));
    }

    static op_type less_than (op_type const & lhs, op_type const & rhs)
    {
      return _mm_and_ps(onesf,_mm_cmplt_ps(lhs,rhs));
    }

    static op_type greater_than (op_type const & lhs, op_type const & rhs)
    {
      return _mm_and_ps(onesf,_mm_cmpgt_ps(lhs,rhs));
    }

    static op_type less_than_or_equal (op_type const & lhs, op_type const & rhs)
    {
      return _mm_and_ps(onesf,_mm_cmple_ps(lhs,rhs));
    }

    static op_type greater_than_or_equal (op_type const & lhs, op_type const & rhs)
    {
      return _mm_and_ps(onesf,_mm_cmpge_ps(lhs,rhs));
    }

    // Basic Arithmetics

    static op_type add (op_type const & lhs, op_type const & rhs)
    {
      return _mm_add_ps(lhs,rhs);
    }

    static op_type sub (op_type const & lhs, op_type const & rhs)
    {
      return _mm_sub_ps(lhs,rhs);
    }

    static op_type mul (op_type const & lhs, op_type const & rhs)
    {
      return _mm_mul_ps(lhs,rhs);
    }

    static op_type div (op_type const & lhs, op_type const & rhs)
    {
      return _mm_div_ps(lhs,rhs);
    }

    static op_type& add_assign (op_type & lhs, op_type const & rhs)
    {
      return lhs = _mm_add_ps(lhs,rhs);
    }

    static op_type& sub_assign (op_type & lhs, op_type const & rhs)
    {
      return lhs = _mm_sub_ps(lhs,rhs);
    }

    static op_type& mul_assign (op_type & lhs, op_type const & rhs)
    {
      return lhs = _mm_mul_ps(lhs,rhs);
    }

    static op_type& div_assign (op_type & lhs, op_type const & rhs)
    {
      return lhs = _mm_div_ps(lhs,rhs);
    }

    static op_type negate(op_type const & rhs)
    {
      return _mm_negate_ps(rhs);
    }

    // Various functions
    static op_type sign (op_type const & ot)
    {
      op_type pos = _mm_cmpgt_ps(ot,zerosf);
      op_type neg = _mm_cmplt_ps(ot,zerosf);

      return _mm_add_ps( _mm_and_ps(pos,onesf) , _mm_and_ps(negate(onesf),neg));
    }

    /*static op_type& random (op_type & ot)
    {
    return ot = _mm_random_ps();
    }*/

    static op_type round (op_type const & ot)
    {
      return _mm_round_ps(ot);
    }
    static op_type max (op_type const & lhs, op_type const & rhs)
    {
      return _mm_max_ps(lhs,rhs);
    }

    static op_type& max_assign (op_type & lhs, op_type const & rhs)
    {
      return lhs = _mm_max_ps(lhs,rhs);
    }

    static op_type min (op_type const & lhs, op_type const & rhs)
    {
      return _mm_min_ps(lhs,rhs);
    }

    static op_type& min_assign (op_type & lhs, op_type const & rhs)
    {
      return lhs = _mm_min_ps(lhs,rhs);
    }

    static op_type abs (op_type const & ot)
    {
      return _mm_fabs_ps(ot);
    }

    static op_type floor (op_type const & ot)
    {
      return _mm_floor_ps(ot);
    }

    static op_type ceil (op_type const & ot)
    {
      return  _mm_ceil_ps(ot);
    }

    static op_type sqrt (op_type const & ot)
    {
      return _mm_sqrt_ps(ot);
    }
    static op_type rsqrt (op_type const & ot)
    {
      return _mm_rsqrt_ps(ot);
    }

    // Horizontal functions -------------------------

    static real_type sum (op_type const & ot)
    {
      const op_type t = _mm_add_ps(_mm_shuffle_ps(ot,ot,0xB1),ot);
      float result;
      _mm_store_ss(&result,_mm_add_ps(_mm_shuffle_ps(t,t,0x4E),t));
      return result;
    }

    static real_type mul (op_type const & ot)
    {                	
      const op_type t = _mm_mul_ps(_mm_shuffle_ps(ot,ot,0xB1),ot);
      float result;
      _mm_store_ss(&result,_mm_mul_ps(_mm_shuffle_ps(t,t,0x4E),t));
      return result;
    }

    static real_type min (op_type const & ot)
    {
      const op_type t = _mm_min_ps(_mm_shuffle_ps(ot,ot,0xB1),ot);
      float result;
      _mm_store_ss(&result,_mm_min_ps(_mm_shuffle_ps(t,t,0x4E),t));
      return result;
    }

    static real_type max (op_type const & ot)
    {
      const op_type t = _mm_max_ps(_mm_shuffle_ps(ot,ot,0xB1),ot);
      float result;
      _mm_store_ss(&result,_mm_max_ps(_mm_shuffle_ps(t,t,0x4E),t));
      return result;
    }
  };


} // namespace tiny

// TINY_SSE_TRAITS_H
#endif 
