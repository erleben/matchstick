#include <mass.h>

#include <tiny_is_finite.h>
#include <tiny_is_number.h>
#include <tiny_value_traits.h>

#include <cassert>

namespace mass
{
  
  template<typename T>
  Properties<T> compute_cylinder(
                        T const & density
                        , T const & radius
                        , T const & half_height
                        )
  {
    using namespace tiny;
    
    typedef ValueTraits<T> value_traits;
    
    assert( is_number( density )                 || !"density must be a number"           );
    assert( is_finite( density )                 || !"density must be a finite number"    );      
    assert( density > value_traits::zero()       || !"density must be positive"           );    
    assert( is_number( radius )                  || !"radius must be a  number"           );
    assert( is_finite( radius )                  || !"radius must be a finite number"     );
    assert( radius > value_traits::zero()        || !"radius must be positive"            );
    assert( is_number( half_height )             || !"half_height must be a  number"      );
    assert( is_finite( half_height )             || !"half_height must be a finite number");
    assert( half_height > value_traits::zero()   || !"half_height must be positive"       );
    
    T const & r     = radius;
    T const & h     = value_traits::two()*half_height;
    T const volume  = value_traits::pi()*r*r*h;
    T const mass    = density*volume;
    T const twelve  = value_traits::numeric_cast( 12.0 );
    T const four    = value_traits::four();
    
    Properties<T> value;
    
    value.m_m   = mass;
    value.m_Ixx = (((r*r)/four) + ((h*h)/twelve)) * mass;
    value.m_Izz = value.m_Ixx;
    value.m_Iyy = mass*r*r*value_traits::half();
    
    return value;
  }
  
  template Properties<float> compute_cylinder<float>(
                                        float const & density
                                        , float const & radius
                                        , float const & half_height
                                        );
  
  template Properties<double> compute_cylinder<double>(
                                         double const & density
                                         , double const & radius
                                         , double const & half_height
                                         );
  
} // namespace mass
