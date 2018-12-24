#include <mass.h>

#include <tiny_is_finite.h>
#include <tiny_is_number.h>
#include <tiny_value_traits.h>

#include <cassert>

namespace mass
{
  
  template<typename T>
  Properties<T> compute_cone(
                    T const & density
                    , T const & base_radius
                    , T const & height
                    )
  {
    using namespace tiny;
    
    typedef ValueTraits<T> VT;
    
    T const pi    = VT::pi();
    T const three = VT::three();
    T const k1    = VT::numeric_cast( 1.0 / 10.0 );
    T const k2    = VT::numeric_cast( 3.0 / 20.0 );
    T const k3    = VT::numeric_cast( 3.0 / 10.0 );
    
    assert( is_number( density )     || !"density must be a number"            );
    assert( is_finite( density )     || !"density must be a finite number"     );      
    assert( density > VT::zero()     || !"density must be positive"            );
    assert( is_number( base_radius ) || !"base_radius must be a  number"       );
    assert( is_finite( base_radius ) || !"base_radius must be a finite number" );
    assert( base_radius > VT::zero() || !"base_radius must be positive"        );
    assert( is_number( height )      || !"height must be a number"             );
    assert( is_finite( height )      || !"height must be a finite number"      );
    assert( height > VT::zero()      || !"height must be positive"             );
    
    T const R      = base_radius;
    T const h      = height;
    T const volume = pi*R*R*h / three;
    T const mass   = density * volume;

    Properties<T> value;
    
    value.m_m   = mass;
    value.m_Ixx = k1*mass*h*h + k2*mass*R*R;
    value.m_Iyy = k3*mass*R*R;
    value.m_Izz = value.m_Ixx;
    value.m_y   = h / VT::four();
    
    return value;
  }
  
  template Properties<float> compute_cone<float>(
                                    float const & density
                                    , float const & base_radius
                                    , float const & height
                                    );
  
  
  template Properties<double> compute_cone<double>(
                                     double const & density
                                     , double const & base_radius
                                     , double const & height
                                     );
  
} // namespace mass
