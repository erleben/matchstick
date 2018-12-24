#include <mass.h>

#include <tiny_is_finite.h>
#include <tiny_is_number.h>
#include <tiny_value_traits.h>

#include <cassert>

namespace mass
{
  
  template<typename T>
  Properties<T> compute_conical_solid(
                                      T const & density
                                      , T const & bottom_radius
                                      , T const & top_radius
                                      , T const & height
                                      )
  {
    using namespace tiny;
    
    typedef ValueTraits<T> VT;
    
    assert( top_radius < bottom_radius || !"top radius must be smaller than bottom radius"); 
    
    assert( is_number( density )       || !"density must be a number"               );
    assert( is_finite( density )       || !"density must be a finite number"        );      
    assert( density > VT::zero()       || !"density must be positive"               );    
    assert( is_number( bottom_radius ) || !"bottom radius must be a  number"        );
    assert( is_finite( bottom_radius ) || !"bottom radius must be a finite number"  );
    assert( bottom_radius > VT::zero() || !"bottom radius must be positive"         );
    assert( is_number( top_radius )    || !"top radius must be a  number"           );
    assert( is_finite( top_radius )    || !"top radius must be a finite number"     );
    assert( top_radius > VT::zero()    || !"top radius must be positive"            );
    assert( is_number( height )        || !"height must be a  number"               );
    assert( is_finite( height )        || !"height must be a finite number"         );
    assert( height > VT::zero()        || !"height must be positive"                );
    
    // First we create a few constants to make the code more readable    
    T const & a      = bottom_radius;
    T const & b      = top_radius;
    T const & h      = height;
    
    T const H  = a*h/(a-b);              // Height of full cone with base a that exactly encloses the capped cone
    T const L  = H-h;                    // Height of cone tip (empty space) that is capped by the frustum
    
    assert(H>VT::zero() || !"internal error");
    assert(L>VT::zero() || !"internal error");
    
    Properties<T> cone = compute_cone(density, a, H);
    Properties<T> top  = compute_cone(density, b, L);
    
    T const tmp = top.m_y;  // Save center of mass position of top before moving it around
    
    top = translate_to_body_frame( top );
    top = translate_to_model_frame( VT::zero(), h + tmp, VT::zero(), top );
    
    Properties<T> value;
    
    // Now we can think of the full cone as the compound of the
    // cone tip and the conical.
    value.m_Ixx   = cone.m_Ixx - top.m_Ixx;
    value.m_Iyy   = cone.m_Iyy - top.m_Iyy;
    value.m_Izz   = cone.m_Izz - top.m_Izz;
    value.m_m     = cone.m_m  - top.m_m;
    
    value.m_y = (cone.m_m*cone.m_y - top.m_m*top.m_y)/ value.m_m;

    return value;
  }
  
  template Properties<float> compute_conical_solid<float>(
                                                          float const & density
                                                          , float const & bottom_radius
                                                          , float const & top_radius
                                                          , float const & height
                                                          );
  
  template Properties<double> compute_conical_solid<double>(
                                                            double const & density
                                                            , double const & bottom_radius
                                                            , double const & top_radius
                                                            , double const & height
                                                            );
  
} // namespace mass
