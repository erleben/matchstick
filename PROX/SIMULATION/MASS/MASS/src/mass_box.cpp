#include <mass.h>

#include <tiny_is_finite.h>
#include <tiny_is_number.h>
#include <tiny_value_traits.h>

#include <cassert>

namespace mass
{
  
  template<typename T>
  Properties<T> compute_box(
                   T const & density
                   , T const & half_width
                   , T const & half_height
                   , T const & half_depth                                            
                   )
  {
    using namespace tiny;
    
    typedef ValueTraits<T> value_traits;
    
    assert( is_number( density )                || !"density must be a number"           );
    assert( is_finite( density )                || !"density must be a finite number"    );      
    assert( density > value_traits::zero()      || !"density must be positive"           );    
    assert( is_number( half_width )             || !"half_width must be a  number"       );
    assert( is_finite( half_width )             || !"half_width must be a finite number" );
    assert( half_width >= value_traits::zero()  || !"half_width must be non-negative"    );
    assert( is_number( half_height )            || !"half_height must be a number"       );
    assert( is_finite( half_height )            || !"half_height must be a finite number");
    assert( half_height >= value_traits::zero() || !"half_height must be non-negative"   );
    assert( is_number( half_depth )             || !"half_depth must be a number"        );
    assert( is_finite( half_depth )             || !"half_depth must be a finite number" );
    assert( half_depth >= value_traits::zero()  || !"half_depth must be non-negative"    );
    
    T const & h0   = half_width;
    T const & h1   = half_height;
    T const & h2   = half_depth;
    T const volume = value_traits::eight()*h0*h1*h2;
    T const mass   = density*volume;
    
    Properties<T> value;
    
    value.m_m    = mass;
    value.m_Ixx  = ( (h1*h1 + h2*h2 ) * mass) / value_traits::three();
    value.m_Iyy  = ( (h0*h0 + h2*h2 ) * mass) / value_traits::three();
    value.m_Izz  = ( (h0*h0 + h1*h1 ) * mass) / value_traits::three();
    
    return value;
  }
  
  template Properties<float> compute_box<float>(
                                    float const & density
                                    , float const & half_width
                                    , float const & half_height
                                    , float const & half_depth                                            
                                    );
  
  template Properties<double> compute_box<double>(
                                     double const & density
                                     , double const & half_width
                                     , double const & half_height
                                     , double const & half_depth                                            
                                     );
    
} // namespace mass
