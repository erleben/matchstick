#ifndef PROX_MATCHSTICK_MODEL_H
#define PROX_MATCHSTICK_MODEL_H

#include <tiny_functions.h> // Needed for clamp

#include <cstdlib>  // for std::size_t

namespace prox
{
  
  template< typename math_policy >
  class MatchStickModel 
  {
  public:
    
    typedef typename math_policy::real_type          real_type;
    typedef typename math_policy::vector3_type       vector3_type;
    typedef typename math_policy::value_traits       value_traits;
    typedef typename math_policy::quaternion_type    quaternion_type;

  protected:
    
    real_type    m_e;               // Coefficient of Restitution
    real_type    m_mu_isotropic;    // Coefficient of Friction for purely Isotropic Coulomb Model
    vector3_type m_mu_anisotropic;  // Coefficient of Friction for extreme anisotropic Coulomb Model

  public:

    MatchStickModel()
    : m_e(value_traits::zero())
    , m_mu_isotropic(value_traits::half())
    , m_mu_anisotropic(vector3_type::make(value_traits::half(),value_traits::one(),value_traits::one()))
    {}

  public:

    /**
     * Compute the friction coefficients parameterized by the characteristics
     * structure directions of two materials.
     *
     * @oaram Sa  Structure direction of material A given wrt world coordinate system.
     * @oaram Sb  Structure direction of material B given wrt world coordinate system.
     *
     * @return    The actual coefficients of friction.
     */
    vector3_type compute_coefficients_of_friction(
                                                  vector3_type const & Sa
                                                  , vector3_type const & Sb
                                                  ) const
    {
      using std::acos;

      vector3_type SSb = Sb;
      if (tiny::inner_prod(Sa, SSb) < value_traits::zero())
      {
        SSb = - SSb;
      }

      real_type const cos_theta = tiny::clamp( tiny::inner_prod(Sa,SSb), -value_traits::one(), value_traits::one() );
      real_type const theta     = acos(cos_theta);
      
      // TODO Check up on this equation?
      real_type const beta      = value_traits::one() - value_traits::two()*theta/value_traits::pi();

      vector3_type const    C_iso = vector3_type::make(m_mu_isotropic,m_mu_isotropic,m_mu_isotropic);
      vector3_type const &  C_an  = this->m_mu_anisotropic;
      vector3_type const    mu    = C_iso + beta * (C_an - C_iso);

      return mu;
    }

    /**
     * Compute the contact frame based on the characteristics
     * structure directions of two materials and current contact normal.
     *
     * @oaram Sa  Structure direction of material A given wrt world coordinate system.
     * @oaram Sb  Structure direction of material B given wrt world coordinate system.
     * @param s   Upon return holds the first axis of the contact frame.
     * @param t   Upon return holds the second axis of the contact frame.
     * @oaram n   The contact normal.
     */
    void compute_contact_frame(
                               vector3_type   & Sa
                               , vector3_type & Sb
                               , vector3_type & s
                               , vector3_type & t
                               , vector3_type const & n
                               ) const
    {
      real_type const too_small = value_traits::numeric_cast(10e-5);

      if (tiny::inner_prod(Sa, Sb) < value_traits::zero())
      {
        Sb = - Sb;
      }

      s = tiny::unit(Sa + Sb);

      real_type const a = tiny::inner_prod(n, s);

      s = tiny::unit(s - (a*n) );

      real_type const length_squared = tiny::inner_prod(s, s);

      if (length_squared > too_small )
      {
        t = tiny::unit(tiny::cross(n, s));
      }
      else
      {
        tiny::orthonormal_vectors( s, t, n );
      }
    }

  public:

    void set_coefficient_of_isotropic_friction(real_type const & mu)
    {
      this->m_mu_isotropic = mu;
    }

    void set_coefficients_of_anisotropic_friction(
                                                  real_type const & mu_x
                                                  , real_type const & mu_y
                                                  , real_type const & mu_z
                                                  )
    {
      this->m_mu_anisotropic[0] = mu_x;
      this->m_mu_anisotropic[1] = mu_y;
      this->m_mu_anisotropic[2] = mu_z;
    }

    real_type const &  get_coefficient_of_isotropic_friction() const
    {
      return this->m_mu_isotropic;
    }

    vector3_type const & get_coefficients_of_anisotropic_friction() const
    {
      return this->m_mu_anisotropic;
    }

    real_type    const & get_coefficient_of_restitution() const
    {
      return this->m_e;
    }
    
    void set_coefficient_of_restitution(real_type const & e)
    {
      this->m_e = e;
    }

  };
} // namespace prox

// PROX_MATCHSTICK_MODEL_H
#endif 
