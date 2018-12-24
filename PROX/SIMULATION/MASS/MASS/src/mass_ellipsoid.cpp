#include <mass.h>

#include <tiny_is_finite.h>
#include <tiny_is_number.h>
#include <tiny_value_traits.h>

#include <cassert>

namespace mass
{
  
  template<typename T>
  Properties<T> compute_ellipsoid(
                         T const & density
                         , T const & a
                         , T const & b
                         , T const & c
                         )
  {
    using namespace tiny;
    
    typedef ValueTraits<T> value_traits;
    
    assert( is_number( density )            || !"density must be a number"       );
    assert( is_finite( density )            || !"density must be a finite number");      
    assert( density > value_traits::zero()  || !"density must be positive"       );    
    assert( is_number( a )                  || !"a must be a  number"       );
    assert( is_finite( a )                  || !"a must be a finite number" );
    assert( a > value_traits::zero()        || !"a must be positive"        );
    assert( is_number( b )                  || !"b must be a  number"       );
    assert( is_finite( b )                  || !"b must be a finite number" );
    assert( b > value_traits::zero()        || !"b must be positive"        );
    assert( is_number( c )                  || !"c must be a  number"       );
    assert( is_finite( c )                  || !"c must be a finite number" );
    assert( c > value_traits::zero()        || !"c must be positive"        );
    
    T const volume  = ( value_traits::four()*value_traits::pi()*a*b*c ) / value_traits::three();
    T const mass            = density * volume;
    T const five    = value_traits::numeric_cast( 5.0 );
    
    Properties<T> value;
    
    value.m_m    = mass;
    value.m_Ixx  = mass * (b*b + c*c) / five;
    value.m_Iyy  = mass * (a*a + c*c) / five;
    value.m_Izz  = mass * (a*a + b*b) / five;
    
    return value;
  }
  
  template Properties<float> compute_ellipsoid<float>(
                                         float const & density
                                         , float const & a
                                         , float const & b
                                         , float const & c
                                         );
  
  template Properties<double> compute_ellipsoid<double>(
                                          double const & density
                                          , double const & a
                                          , double const & b
                                          , double const & c
                                          );
  
} // namespace mass
