#ifndef PROX_UPDATE_INERTIA_TENSOR_H
#define PROX_UPDATE_INERTIA_TENSOR_H

#include <tiny_is_number.h>
#include <tiny_is_finite.h>
#include <tiny_precision.h>

#include <util_log.h>

#include <cmath>    // Needed for std::fabs
#include <cassert>

namespace prox
{
  namespace detail
  {

    /**
     * Inertia Update Method.
     *
     * Computes, I_world = R I_body R^T. This method have been optimized to
     * avoid having to transpose the orientation matrix and exploit
     * symmetry of the inertia tensor.
     *
     * @note            Further optimization may be possible by exploiting common sub-terms.
     *
     * @param R         The orientation of the body frame with repect the world frame as a rotation matrix.
     * @param I_body    An inertia tensor with respect to the body frame
     * @param I_world   Upon return this argument holds the inertia tensor with respect to the orientation of the world frame.
     *
     * @tparam M        The 3-by-3 matrix type to use for this update.
     */
    template<typename MT>
    inline void update_inertia_tensor(
                                        typename MT::matrix3x3_type const & R
                                      , typename MT::matrix3x3_type const & I_body
                                      , typename MT::matrix3x3_type       & I_world
                                      , bool                        const & verbose = false
                                      )
    {
      using std::fabs;

      typedef typename MT::real_type        T;
      typedef typename MT::value_traits     VT;

      assert(is_number(I_body(0,0)) || !"update_inertia_tensor(): Nan");
      assert(is_number(I_body(0,1)) || !"update_inertia_tensor(): Nan");
      assert(is_number(I_body(0,2)) || !"update_inertia_tensor(): Nan");
      assert(is_number(I_body(1,0)) || !"update_inertia_tensor(): Nan");
      assert(is_number(I_body(1,1)) || !"update_inertia_tensor(): Nan");
      assert(is_number(I_body(1,2)) || !"update_inertia_tensor(): Nan");
      assert(is_number(I_body(2,0)) || !"update_inertia_tensor(): Nan");
      assert(is_number(I_body(2,1)) || !"update_inertia_tensor(): Nan");
      assert(is_number(I_body(2,2)) || !"update_inertia_tensor(): Nan");

      assert(is_finite(I_body(0,0)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(I_body(0,1)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(I_body(0,2)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(I_body(1,0)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(I_body(1,1)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(I_body(1,2)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(I_body(2,0)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(I_body(2,1)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(I_body(2,2)) || !"update_inertia_tensor(): Inf");

      assert(is_number(R(0,0)) || !"update_inertia_tensor(): Nan");
      assert(is_number(R(0,1)) || !"update_inertia_tensor(): Nan");
      assert(is_number(R(0,2)) || !"update_inertia_tensor(): Nan");
      assert(is_number(R(1,0)) || !"update_inertia_tensor(): Nan");
      assert(is_number(R(1,1)) || !"update_inertia_tensor(): Nan");
      assert(is_number(R(1,2)) || !"update_inertia_tensor(): Nan");
      assert(is_number(R(2,0)) || !"update_inertia_tensor(): Nan");
      assert(is_number(R(2,1)) || !"update_inertia_tensor(): Nan");
      assert(is_number(R(2,2)) || !"update_inertia_tensor(): Nan");

      assert(is_finite(R(0,0)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(R(0,1)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(R(0,2)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(R(1,0)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(R(1,1)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(R(1,2)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(R(2,0)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(R(2,1)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(R(2,2)) || !"update_inertia_tensor(): Inf");

      if(verbose)
      {
        util::Log logging;

        //--- In body frame inertia censor should be a "positive" diagonal
        //--- tensor, otherwise data is corrupt

        if( I_body(0,0) <= VT::zero() )
        {
          logging << "update_inertia_tensor(): I_body(0,0) was non-positive" << util::Log::newline();
        }
        if( I_body(1,1) <= VT::zero() )
        {
          logging << "update_inertia_tensor(): I_body(1,1) was non-positive" << util::Log::newline();
        }
        if( I_body(2,2) <= VT::zero() )
        {
          logging << "update_inertia_tensor(): I_body(2,2) was non-positive" << util::Log::newline();
        }
        if( fabs(I_body(0,1)) <= tiny::working_precision<T>() )
        {
          logging << "update_inertia_tensor(): I_body(0,1) was non-zero" << util::Log::newline();
        }
        if( fabs(I_body(0,2)) <= tiny::working_precision<T>() )
        {
          logging << "update_inertia_tensor(): I_body(0,2) was non-zero" << util::Log::newline();
        }
        if( fabs(I_body(1,0)) <= tiny::working_precision<T>() )
        {
          logging << "update_inertia_tensor(): I_body(1,0) was non-zero" << util::Log::newline();
        }
        if( fabs(I_body(1,2)) <= tiny::working_precision<T>() )
        {
          logging << "update_inertia_tensor(): I_body(1,2) was non-zero" << util::Log::newline();
        }
        if( fabs(I_body(2,0)) <= tiny::working_precision<T>() )
        {
          logging << "update_inertia_tensor(): I_body(2,0) was non-zero" << util::Log::newline();
        }
        if( fabs(I_body(2,1)) <= tiny::working_precision<T>() )
        {
          logging << "update_inertia_tensor(): I_body(2,1) was non-zero" << util::Log::newline();
        }

        //--- R should be an orthonormal matrix, R^T R = I
        T const I00 = R(0,0)*R(0,0) + R(1,0)*R(1,0) + R(2,0)*R(2,0);
        T const I11 = R(0,1)*R(0,1) + R(1,1)*R(1,1) + R(2,1)*R(2,1);
        T const I22 = R(0,2)*R(0,2) + R(1,2)*R(1,2) + R(2,2)*R(2,2);
        T const I01 = R(0,0)*R(0,1) + R(1,0)*R(1,1) + R(2,0)*R(2,1);
        T const I02 = R(0,0)*R(0,2) + R(1,0)*R(1,2) + R(2,0)*R(2,2);
        T const I12 = R(0,1)*R(0,2) + R(1,1)*R(1,2) + R(2,1)*R(2,2);

        if( fabs(VT::one()-I00) <= tiny::working_precision<T>() )
        {
          logging << "update_inertia_tensor(): R was not sufficient orthonormal" << util::Log::newline();
        }
        if( fabs(VT::one()-I11) <= tiny::working_precision<T>() )
        {
          logging << "update_inertia_tensor(): R was not sufficient orthonormal" << util::Log::newline();
        }
        if( fabs(VT::one()-I22) <= tiny::working_precision<T>() )
        {
          logging << "update_inertia_tensor(): R was not sufficient orthonormal" << util::Log::newline();
        }
        if( fabs(I01) <= tiny::working_precision<T>() )
        {
          logging << "update_inertia_tensor(): R was not sufficient orthonormal" << util::Log::newline();
        }
        if( fabs(I02) <= tiny::working_precision<T>() )
        {
          logging << "update_inertia_tensor(): R was not sufficient orthonormal" << util::Log::newline();
        }
        if( fabs(I12) <= tiny::working_precision<T>() )
        {
          logging << "update_inertia_tensor(): R was not sufficient orthonormal" << util::Log::newline();
        }
      }


      //--- 2004-08-30 Kenny: The formulas below was generated using Matlab
      //---
      //--- syms R00 R01 R02  R10 R11 R12 R20 R21 R22 real;
      //--- syms I00 I01 I02  I10 I11 I12 I20 I21 I22 real;
      //--- I = [ I00, I01 , I02; I01, I11, I12; I02, I12, I22]
      //--- R = [ R00, R01 , R02; R10, R11, R12; R20, R21, R22]
      //--- W = R*I*R'
      //--- simplify(W)
      //---

      I_world(0,0) = R(0,0)*R(0,0)*I_body(0,0) + 2*R(0,0)*R(0,1)*I_body(0,1)+2*R(0,0)*R(0,2)*I_body(0,2)+R(0,1)*R(0,1)*I_body(1,1)+2*R(0,1)*R(0,2)*I_body(1,2)+R(0,2)*R(0,2)*I_body(2,2);
      I_world(1,1) = R(1,0)*R(1,0)*I_body(0,0)+2*R(1,0)*R(1,1)*I_body(0,1)+2*R(1,0)*R(1,2)*I_body(0,2)+R(1,1)*R(1,1)*I_body(1,1)+2*R(1,1)*R(1,2)*I_body(1,2)+R(1,2)*R(1,2)*I_body(2,2);
      I_world(2,2) = R(2,0)*R(2,0)*I_body(0,0)+2*R(2,0)*R(2,1)*I_body(0,1)+2*R(2,0)*R(2,2)*I_body(0,2)+R(2,1)*R(2,1)*I_body(1,1)+2*R(2,1)*R(2,2)*I_body(1,2)+R(2,2)*R(2,2)*I_body(2,2);
      I_world(1,0) = I_world(0,1) = R(1,0)*R(0,0)*I_body(0,0)+R(1,0)*R(0,1)*I_body(0,1)+R(1,0)*R(0,2)*I_body(0,2)+R(1,1)*R(0,0)*I_body(0,1)+R(1,1)*R(0,1)*I_body(1,1)+R(1,1)*R(0,2)*I_body(1,2)+R(1,2)*R(0,0)*I_body(0,2)+R(1,2)*R(0,1)*I_body(1,2)+R(1,2)*R(0,2)*I_body(2,2);
      I_world(2,0) = I_world(0,2) = R(2,0)*R(0,0)*I_body(0,0)+R(2,0)*R(0,1)*I_body(0,1)+R(2,0)*R(0,2)*I_body(0,2)+R(2,1)*R(0,0)*I_body(0,1)+R(2,1)*R(0,1)*I_body(1,1)+R(2,1)*R(0,2)*I_body(1,2)+R(2,2)*R(0,0)*I_body(0,2)+R(2,2)*R(0,1)*I_body(1,2)+R(2,2)*R(0,2)*I_body(2,2);
      I_world(2,1) = I_world(1,2) = R(1,0)*R(2,0)*I_body(0,0)+R(1,0)*R(2,1)*I_body(0,1)+R(1,0)*R(2,2)*I_body(0,2)+R(1,1)*R(2,0)*I_body(0,1)+R(1,1)*R(2,1)*I_body(1,1)+R(1,1)*R(2,2)*I_body(1,2)+R(1,2)*R(2,0)*I_body(0,2)+R(1,2)*R(2,1)*I_body(1,2)+R(1,2)*R(2,2)*I_body(2,2);

      assert(is_number(I_world(0,0)) || !"update_inertia_tensor(): Nan");
      assert(is_number(I_world(0,1)) || !"update_inertia_tensor(): Nan");
      assert(is_number(I_world(0,2)) || !"update_inertia_tensor(): Nan");
      assert(is_number(I_world(1,0)) || !"update_inertia_tensor(): Nan");
      assert(is_number(I_world(1,1)) || !"update_inertia_tensor(): Nan");
      assert(is_number(I_world(1,2)) || !"update_inertia_tensor(): Nan");
      assert(is_number(I_world(2,0)) || !"update_inertia_tensor(): Nan");
      assert(is_number(I_world(2,1)) || !"update_inertia_tensor(): Nan");
      assert(is_number(I_world(2,2)) || !"update_inertia_tensor(): Nan");

      assert(is_finite(I_world(0,0)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(I_world(0,1)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(I_world(0,2)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(I_world(1,0)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(I_world(1,1)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(I_world(1,2)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(I_world(2,0)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(I_world(2,1)) || !"update_inertia_tensor(): Inf");
      assert(is_finite(I_world(2,2)) || !"update_inertia_tensor(): Inf");
    }

  } // namespace detail
} // namespace prox

// PROX_UPDATE_INERTIA_TENSOR_H
#endif 
