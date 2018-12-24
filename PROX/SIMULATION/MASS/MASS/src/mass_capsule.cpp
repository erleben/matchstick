#include <mass.h>

#include <tiny_is_finite.h>
#include <tiny_is_number.h>
#include <tiny_value_traits.h>

#include <cassert>

namespace mass
{
  
  template<typename T>
  Properties<T> compute_capsule(
                         T const & density
                       , T const & radius
                       , T const & half_height
                       )
  {
    using namespace tiny;
    
    typedef ValueTraits<T> value_traits;
    
    assert( is_number( density )                || !"density must be a number"           );
    assert( is_finite( density )                || !"density must be a finite number"    );      
    assert( density > value_traits::zero()      || !"density must be positive"           );    
    assert( is_number( radius )                 || !"radius must be a  number"           );
    assert( is_finite( radius )                 || !"radius must be a finite number"     );
    assert( radius >= value_traits::zero()      || !"radius must be non-negative"        );
    assert( is_number( half_height )            || !"half_height must be a number"       );
    assert( is_finite( half_height )            || !"half_height must be a finite number");
    assert( half_height >= value_traits::zero() || !"half_height must be non-negative"   );
  
    Properties<T> const I_sphere   = compute_sphere  ( density, radius );
    Properties<T> const I_cylinder = compute_cylinder( density, radius, half_height );
    Properties<T>       value      = I_cylinder;
    
    value.m_m   += I_sphere.m_m;
    value.m_Iyy += I_sphere.m_Iyy;
    
    // Only xx and zz parts of inertia tensor should increase. The
    // yy part is unchanegd so that the products of inertia will be zero!

    Properties<T> I_half_sphere = I_sphere;

    I_half_sphere.m_m = I_sphere.m_m * value_traits::half();
    
    I_half_sphere = translate_to_model_frame( value_traits::zero(), half_height, value_traits::zero(), I_half_sphere);
    
    assert( I_half_sphere.m_Ixx >= I_sphere.m_Ixx       || !"Inertia transformation failed" ); 
    assert( I_half_sphere.m_Iyy == I_sphere.m_Iyy       || !"Inertia transformation failed" ); 
    assert( I_half_sphere.m_Izz >= I_sphere.m_Izz       || !"Inertia transformation failed" ); 
    assert( I_half_sphere.m_Ixy == value_traits::zero() || !"Inertia transformation failed" ); 
    assert( I_half_sphere.m_Ixz == value_traits::zero() || !"Inertia transformation failed" ); 
    assert( I_half_sphere.m_Iyz == value_traits::zero() || !"Inertia transformation failed" ); 
    
    value.m_Ixx  += I_half_sphere.m_Ixx;
    value.m_Izz  += I_half_sphere.m_Izz;    
    
    return value;
  }
  
  
  template Properties<float> compute_capsule<float>(
                                       float const & density
                                       , float const & radius
                                       , float const & half_height
                                       );
  
  template Properties<double> compute_capsule<double>(
                                        double const & density
                                        , double const & radius
                                        , double const & half_height
                                        );
  
  
  
} // namespace mass
