#ifndef PROX_POSITION_UPDATE_H
#define PROX_POSITION_UPDATE_H

#include <cmath>
#include <stdexcept>

namespace prox
{
  
  template<typename math_policy>
  inline void position_update( 
                              typename math_policy::vector7_type const& q,
                              typename math_policy::vector6_type const & u,
                              typename math_policy::real_type const & dt,
                              typename math_policy::vector7_type & qnew,
                              math_policy const & /*tag*/ 
                              ) 
  {
    typedef typename math_policy::vector3_type     vector3_type;
    typedef typename math_policy::quaternion_type  quaternion_type;
    
    typedef typename math_policy::block6x1_type block6x1_type;
    typedef typename math_policy::block7x1_type block7x1_type;
    
    typedef typename math_policy::value_traits  value_traits;
    
    size_t const N = u.size();
    
    if( q.size() != N )
      throw std::logic_error("position_update(): s has incorrect dimension");
    
    // 2009-08-04 Kenny: You are right this seems pointless. In fact I think the correct behaviour should be to get the number 
    // of blocks from q, then verify if u has the same number of blocks, and lastly if qnew is big enough to hold the results  
    if( u.size() != N )// 2009-08-03 Sarah: is this not redundant since N = u.size()?
      throw std::logic_error("position_update(): u has incorrect dimension");
    
    if(&q != &qnew)
    {
      qnew.resize( N );
    }
    
    float const dt_half = dt / value_traits::two();
    
    for(size_t i = 0; i<N; ++i)
    {
      block7x1_type & qnew_b = qnew( i );
      block7x1_type const& q_b = q( i );
      block6x1_type const& u_b = u( i );
      
      qnew_b(0) = q_b(0) + dt * u_b(0);
      qnew_b(1) = q_b(1) + dt * u_b(1);
      qnew_b(2) = q_b(2) + dt * u_b(2);
      
      
      quaternion_type Q;
      Q.real()    = q_b(3);   
      Q.imag()(0) = q_b(4);   
      Q.imag()(1) = q_b(5);
      Q.imag()(2) = q_b(6);
      
      vector3_type W;
      W(0) = u_b(3);
      W(1) = u_b(4);
      W(2) = u_b(5);
      
      bool const finite = true;  // Kenny: Hmm, this might have to be a user-specified paramter?
      
      if( finite)
      {
        // Do a finitedimensional update instead
        //
        // Create the quaternion, H, corresonding to a rotation of
        //
        //    \theta = \Delta t \norm{\vec \omega}
        //
        // Around the unit-axis
        //
        //     \vec n = \frac{\vec \omega}{  \norm{\vec \omega} }
        //
        // That is
        //
        //    H = [ \cos\left( \frac{\theta}{2} \right), \sin\left( \frac{\theta}{2} \right) \vec n ]
        //    
        // Now perform the update of the orientation simply by
        //
        //   Q \leftarrow  H Q
        //
        // There should be no need to normalize Q after this operation.
        float const radian = tiny::norm( W )*dt;
        vector3_type axis  = tiny::unit( W );
        quaternion_type R = quaternion_type::Ru( radian, axis ); 
        Q = tiny::prod(R,Q);
      }
      else
      {
        //--- Just do an infinitesimal rotation update (i.e. a forward Euler step)
        Q = Q + (tiny::prod(W , Q) * dt_half);
      }
      
      //--- To counter-act numerical problems
      Q = tiny::unit(Q);
      
      qnew_b(3) = Q.real();
      qnew_b(4) = Q.imag()(0);
      qnew_b(5) = Q.imag()(1);
      qnew_b(6) = Q.imag()(2);
    }
  }
} // namespace prox

// PROX_POSITION_UPDATE_H
#endif 
