#include <mass.h>

#include <tiny_is_finite.h>
#include <tiny_is_number.h>
#include <tiny_value_traits.h>

#include <cassert>

namespace mass
{  
  template<typename T>
  Properties<T> translate_to_model_frame(
                                         T const & dx
                                         , T const & dy
                                         , T const & dz
                                         , Properties<T> const & Ibody
                                         )
  {
    using namespace tiny;
    
    typedef ValueTraits<T> value_traits;
    
    assert( is_number( Ibody.m_m )           || !"mass must be a number"       );
    assert( is_finite( Ibody.m_m )           || !"mass must be a finite number");      
    assert( Ibody.m_m > value_traits::zero() || !"mass must be positive"       );
    assert( is_number( dx )                  || !"x must be a number"          );
    assert( is_finite( dx )                  || !"x must be a finite number"   );      
    assert( is_number( dy )                  || !"y must be a number"          );
    assert( is_finite( dy )                  || !"y must be a finite number"   );      
    assert( is_number( dz )                  || !"z must be a number"          );
    assert( is_finite( dz )                  || !"z must be a finite number"   );      
    assert( is_number( Ibody.m_Ixx )         || !"Ixx must be a number"        );
    assert( is_finite( Ibody.m_Ixx )         || !"Ixx must be a finite number" );      
    assert( is_number( Ibody.m_Iyy )         || !"Iyy must be a number"        );
    assert( is_finite( Ibody.m_Iyy )         || !"Iyy must be a finite number" );      
    assert( is_number( Ibody.m_Izz )         || !"Izz must be a number"        );
    assert( is_finite( Ibody.m_Izz )         || !"Izz must be a finite number" );      
    assert( is_number( Ibody.m_Ixy )         || !"Ixy must be a number"        );
    assert( is_finite( Ibody.m_Ixy )         || !"Ixy must be a finite number" );      
    assert( is_number( Ibody.m_Ixz )         || !"Ixz must be a number"        );
    assert( is_finite( Ibody.m_Ixz )         || !"Ixz must be a finite number" );      
    assert( is_number( Ibody.m_Iyz )         || !"Iyz must be a number"        );
    assert( is_finite( Ibody.m_Iyz )         || !"Iyz must be a finite number" );      
    
    Properties<T> Imodel;
    
    Imodel.m_Ixx = Ibody.m_Ixx + Ibody.m_m * (dy*dy + dz*dz);
    Imodel.m_Iyy = Ibody.m_Iyy + Ibody.m_m * (dx*dx + dz*dz);
    Imodel.m_Izz = Ibody.m_Izz + Ibody.m_m * (dx*dx + dy*dy);
    Imodel.m_Ixy = Ibody.m_Ixy - Ibody.m_m * (dx*dy);      
    Imodel.m_Ixz = Ibody.m_Ixz - Ibody.m_m * (dx*dz);      
    Imodel.m_Iyz = Ibody.m_Iyz - Ibody.m_m * (dy*dz);
    Imodel.m_x   = dx;
    Imodel.m_y   = dy;
    Imodel.m_z   = dz;
    Imodel.m_m   = Ibody.m_m;
    
    return Imodel;
  }
  
  
  template<typename T>
  Properties<T> translate_to_body_frame( Properties<T> const & Imodel )
  {
    using namespace tiny;
    
    typedef ValueTraits<T> value_traits;
    
    assert( is_number( Imodel.m_m )           || !"mass must be a number"       );
    assert( is_finite( Imodel.m_m )           || !"mass must be a finite number");      
    assert( Imodel.m_m > value_traits::zero() || !"mass must be positive"       );
    assert( is_number( Imodel.m_x )              || !"x must be a number"          );
    assert( is_finite( Imodel.m_x )              || !"x must be a finite number"   );      
    assert( is_number( Imodel.m_y )              || !"y must be a number"          );
    assert( is_finite( Imodel.m_y )              || !"y must be a finite number"   );      
    assert( is_number( Imodel.m_z )              || !"z must be a number"          );
    assert( is_finite( Imodel.m_z )              || !"z must be a finite number"   );      
    assert( is_number( Imodel.m_Ixx )            || !"Ixx must be a number"        );
    assert( is_finite( Imodel.m_Ixx )            || !"Ixx must be a finite number" );      
    assert( is_number( Imodel.m_Iyy )            || !"Iyy must be a number"        );
    assert( is_finite( Imodel.m_Iyy )            || !"Iyy must be a finite number" );      
    assert( is_number( Imodel.m_Izz )            || !"Izz must be a number"        );
    assert( is_finite( Imodel.m_Izz )            || !"Izz must be a finite number" );      
    assert( is_number( Imodel.m_Ixy )            || !"Ixy must be a number"        );
    assert( is_finite( Imodel.m_Ixy )            || !"Ixy must be a finite number" );      
    assert( is_number( Imodel.m_Ixz )            || !"Ixz must be a number"        );
    assert( is_finite( Imodel.m_Ixz )            || !"Ixz must be a finite number" );      
    assert( is_number( Imodel.m_Iyz )            || !"Iyz must be a number"        );
    assert( is_finite( Imodel.m_Iyz )            || !"Iyz must be a finite number" );      
    
    Properties<T> Ibody;
    
    T const x = - Imodel.m_x;
    T const y = - Imodel.m_y;
    T const z = - Imodel.m_z;
    
    Ibody.m_Ixx = Imodel.m_Ixx - Imodel.m_m * (y*y + z*z);      
    Ibody.m_Iyy = Imodel.m_Iyy - Imodel.m_m * (x*x + z*z);      
    Ibody.m_Izz = Imodel.m_Izz - Imodel.m_m * (x*x + y*y);
    Ibody.m_Ixy = Imodel.m_Ixy + Imodel.m_m * (x*y);      
    Ibody.m_Ixz = Imodel.m_Ixz + Imodel.m_m * (x*z);      
    Ibody.m_Iyz = Imodel.m_Iyz + Imodel.m_m * (y*z);
    Ibody.m_m   = Imodel.m_m;
    
    return Ibody;
  }
  
  
  template
  Properties<float> translate_to_model_frame(
                                             float const & dx
                                             , float const & dy
                                             , float const & dz
                                             , Properties<float> const & Ibody
                                             );
  
  template
  Properties<double> translate_to_model_frame(
                                              double const & dx
                                              , double const & dy
                                              , double const & dz
                                              , Properties<double> const & Ibody
                                              );
  
  
  template
  Properties<float> translate_to_body_frame( Properties<float> const & Imodel );
  
  template
  Properties<double> translate_to_body_frame( Properties<double> const & Imodel );
  
  
  
} // namespace mass
