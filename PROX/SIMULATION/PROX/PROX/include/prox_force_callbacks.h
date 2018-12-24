#ifndef PROX_FORCE_CALLBACKS_H
#define PROX_FORCE_CALLBACKS_H

#include <prox_rigid_body.h>

#include <cassert>

namespace prox
{
  template<typename M>
  class ForceCallback
  {
  public:

    typedef          RigidBody<M>      B;
    typedef typename M::vector3_type   V;
    typedef typename M::real_type      T;
    typedef typename M::value_traits   VT;

  protected:

    size_t m_idx;

  public:

    size_t const & get_idx() const { return this->m_idx; }
    void set_idx(size_t const & idx ) { this->m_idx = idx; }

  public:

    ForceCallback()
    : m_idx(0u)
    {}

    virtual ~ForceCallback(){}

  public:

    virtual void compute_force_and_torque( B const & body, V & force, V & torque) const = 0;

  };

  template<typename M>
  class Gravity : public ForceCallback<M>
  {
  public:

    typedef          RigidBody<M>      B;
    typedef typename M::vector3_type   V;
    typedef typename M::real_type      T;
    typedef typename M::value_traits   VT;

  protected:

    T m_acceleration;
    V m_up;

  public:

    T const & acceleration() const { return this->m_acceleration; }
    T       & acceleration()       { return this->m_acceleration; }

    V const & up() const { return this->m_up; }
    V       & up()       { return this->m_up; }

  public:

    Gravity()
    : m_acceleration( VT::numeric_cast(9.81))
    , m_up( V::make(VT::zero(), VT::one(), VT::zero())   )
    {}

  public:

    void compute_force_and_torque( B const & body, V & force, V & torque) const
    {
      assert(this->m_acceleration >= VT::zero() || !"Gravity::compute_force_and_torque(): acceleration must be non-negative");

      force = - this->m_up * (body.get_mass() * this->m_acceleration);
      torque = V::zero();
    }

  };

  template<typename M>
  class Damping : public ForceCallback<M>
  {
  public:

    typedef          RigidBody<M>      B;
    typedef typename M::vector3_type   V;
    typedef typename M::real_type      T;
    typedef typename M::value_traits   VT;

  protected:

    T m_linear;
    T m_angular;

  public:

    T const & linear() const { return this->m_linear; }
    T       & linear()       { return this->m_linear; }

    T const & angular() const { return this->m_angular; }
    T       & angular()       { return this->m_angular; }

  public:

    Damping()
    : m_linear( VT::numeric_cast(0.01)  )
    , m_angular( VT::numeric_cast(0.01) )
    {}

  public:

    void compute_force_and_torque( B const & body, V & force, V & torque) const
    {
      assert(this->m_linear  >= VT::zero() || !"Damping::compute_force_and_torque(): damping must be non-negative");
      assert(this->m_angular >= VT::zero() || !"Damping::compute_force_and_torque(): damping must be non-negative");


      force = - (body.get_velocity() * m_linear);
      torque = - (body.get_spin()     * m_angular);
    }
    
  };

  template<typename M>
  class Pin : public ForceCallback<M>
  {
  public:

    typedef          RigidBody<M>      B;
    typedef typename M::vector3_type   V;
    typedef typename M::real_type      T;
    typedef typename M::value_traits   VT;

  protected:

    T m_tau;     ///< Characteristic damping time, use 1/6 of time-step size, usually gives nice results?
    V m_target;  ///< Target position that must be hit
    V m_anchor;  ///< Anchor point in local body space coordinates. This the point on the body that should be pinned to target.

  public:

    T const & tau() const { return this->m_tau; }
    T       & tau()       { return this->m_tau; }

    V const & target() const { return this->m_target; }
    V       & target()       { return this->m_target; }

    V const & anchor() const { return this->m_anchor; }
    V       & anchor()       { return this->m_anchor; }

  public:

  Pin()
    : m_tau (VT::numeric_cast( 0.1 ))
    , m_target ( V::zero() )
    , m_anchor(V::zero() )
    {}

  public:

    void compute_force_and_torque( B const & body, V & force, V & torque) const
    {
      using std::min;
      using std::max;

      assert( this->m_tau > VT::zero() || !"MoveToPoint::compute_force_and_torque(): tau must be positive");

      V const r      = tiny::rotate(body.get_orientation(), this->m_anchor);
      V const D      = this->m_target -  (r + body.get_position());
      T const l      = tiny::norm( D );
      V const n      = tiny::unit( D );
      T const v      = tiny::inner_prod(body.get_velocity(), n);
      T const m      = body.get_mass();
      T const b      = (VT::two() *m) / this->m_tau;
      T const k      = m / (this->m_tau*this->m_tau);

      force  =  ( k*l    -  b*v   )*n;

      torque = tiny::cross(r, force);
    }

  };

} //namespace prox

// PROX_FORCE_CALLBACKS_H
#endif 
