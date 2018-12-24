#ifndef PROX_STEPPER_PARAMS_H
#define PROX_STEPPER_PARAMS_H

#include <prox_enums.h>    // Needed for common type defs 

#include <tiny_is_number.h>
#include <tiny_is_finite.h>

namespace prox
{    
  
  template< typename MT >
  class StepperParams
  {
  public:

    typedef typename MT::real_type           T;
    typedef typename MT::value_traits        VT;
        
  protected:
    
    stepper_type    m_stepper;              ///< The time stepper type.
    T               m_min_gap;              ///< The minimum allowable gap (=penetration) that will not cause stabilization to kick in
    T               m_max_gap;              ///< The maximum possible gap (=penetration) to correct for during stabilization
    T               m_gap_reduction;        ///< The amount of gap (=penetration) to reduce during stabilization
    bool            m_pre_stabilization;    ///< Flag to turn pre stabilization on/off
    bool            m_post_stabilization;   ///< Flag to turn post stabilization on/off
    bool            m_contact_reduction;    ///< Flat to turn on contact filter reduction, post-filter that removed redundant contacts.
    bool            m_bounce_on;            ///< Flag to turn bouncing completely off, default bounce is on.

  public:

    stepper_type const & stepper()       const { return this->m_stepper; }

    T const & min_gap()       const { return this->m_min_gap; }
    T const & max_gap()       const { return this->m_max_gap; }
    T const & gap_reduction() const { return this->m_gap_reduction; }    
    
    bool const & pre_stabilization()  const { return this->m_pre_stabilization;  }
    bool const & post_stabilization() const { return this->m_post_stabilization; }
    bool const & contact_reduction() const { return this->m_contact_reduction; }
    bool const & bounce_on() const { return this->m_bounce_on; }

    void set_min_gap(T const & value)
    {
      assert(value >= VT::zero() || !"set_min_gap(): value must be nonnegative");
      assert(is_number(value)    || !"set_min_gap(): value must be a number");
      assert(is_finite(value)    || !"set_min_gap(): value must be a finite value");
      
      this->m_min_gap = value;
    }

    void set_max_gap(T const & value)
    {
      assert(value > VT::zero() || !"set_max_gap(): value must be positive");
      assert(is_number(value)   || !"set_max_gap(): value must be a number");
      assert(is_finite(value)   || !"set_max_gap(): value must be a finite value");
      
      this->m_max_gap = value;
    }
    
    void set_gap_reduction(T const & value)
    {
      assert(value >  VT::zero() || !"set_gap_reduction(): value must be positive");
      assert(value <= VT::one()  || !"set_gap_reduction(): value must be less than equal one");
      assert(is_number(value)    || !"set_gap_reduction(): value must be a number");
      assert(is_finite(value)    || !"set_gap_reduction(): value must be a finite value");
      
      this->m_gap_reduction = value; 
    }

    void set_stepper(stepper_type const & value)
    {
      this->m_stepper = value;
    }
    
    void set_pre_stabilization(bool const & value)
    {
      this->m_pre_stabilization = value;
    }

    void set_post_stabilization(bool const & value)
    {
      this->m_post_stabilization = value;
    }

    void set_contact_reduction(bool const & value)
    {
      this->m_contact_reduction = value;
    }

    void set_bounce_on(bool const & value)
    {
      this->m_bounce_on = value;
    }

  public:
    
    StepperParams()
    : m_stepper(semi_implicit)
    , m_min_gap(VT::numeric_cast(0.001f) )
    , m_max_gap(VT::numeric_cast(0.01f) )
    , m_gap_reduction(VT::numeric_cast(0.5f) )
    , m_pre_stabilization( true )
    , m_post_stabilization( true )
    , m_contact_reduction( true )
    , m_bounce_on(true)
    {}
    
  };

} // namespace prox

// PROX_STEPPER_PARAMS_H
#endif 
