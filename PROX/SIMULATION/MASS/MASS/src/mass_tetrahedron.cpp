#include <mass.h>

#include <tiny_is_finite.h>
#include <tiny_is_number.h>
#include <tiny_value_traits.h>

#include <cassert>
#include <cmath>         // needed for std::fabs

namespace mass
{
  
  template<typename T>
  Properties<T> compute_tetrahedron(
                           T const & density
                           , T const & x1
                           , T const & y1
                           , T const & z1
                           , T const & x2
                           , T const & y2
                           , T const & z2
                           , T const & x3
                           , T const & y3
                           , T const & z3
                           , T const & x4
                           , T const & y4
                           , T const & z4
                           )
  {
    using namespace tiny;
    using std::fabs;
    
    typedef ValueTraits<T> value_traits;
    
    assert( is_number( density )            || !"density must be a number"       );
    assert( is_finite( density )            || !"density must be a finite number");      
    assert( density > value_traits::zero()  || !"density must be positive"       );
    
    assert( is_number( x1 ) || !"x1 must be a  number"      );
    assert( is_finite( x1 ) || !"x1 must be a finite number");
    assert( is_number( y1 ) || !"y1 must be a  number"      );
    assert( is_finite( y1 ) || !"y1 must be a finite number");
    assert( is_number( z1 ) || !"z1 must be a  number"      );
    assert( is_finite( z1 ) || !"z1 must be a finite number");
    assert( is_number( x2 ) || !"x2 must be a  number"      );
    assert( is_finite( x2 ) || !"x2 must be a finite number");
    assert( is_number( y2 ) || !"y2 must be a  number"      );
    assert( is_finite( y2 ) || !"y2 must be a finite number");
    assert( is_number( z2 ) || !"z2 must be a  number"      );
    assert( is_finite( z2 ) || !"z2 must be a finite number");
    assert( is_number( x3 ) || !"x3 must be a  number"      );
    assert( is_finite( x3 ) || !"x3 must be a finite number");
    assert( is_number( y3 ) || !"y3 must be a  number"      );
    assert( is_finite( y3 ) || !"y3 must be a finite number");
    assert( is_number( z3 ) || !"z3 must be a  number"      );
    assert( is_finite( z3 ) || !"z3 must be a finite number");
    assert( is_number( x4 ) || !"x4 must be a  number"      );
    assert( is_finite( x4 ) || !"x4 must be a finite number");
    assert( is_number( y4 ) || !"y4 must be a  number"      );
    assert( is_finite( y4 ) || !"y4 must be a finite number");
    assert( is_number( z4 ) || !"z4 must be a  number"      );
    assert( is_finite( z4 ) || !"z4 must be a finite number");
    
    // This implementation is based on ideas from: F. Tonon: Explicit Exact Formulas for
    // the 3-D Tetrahedron Inertia Tensor in Terms of its Vertex Coordinates. Journal of
    // Mathematics and Statistics, Volume 1, Issue 1, Pages 8-11 , 2005.
    
    T const x_poly = x1*x1 + x2*x2 + x3*x3 + x4*x4 + x1*x2 + x1*x3 + x1*x4 + x2*x3 + x2*x4 + x3*x4;
    T const y_poly = y1*y1 + y2*y2 + y3*y3 + y4*y4 + y1*y2 + y1*y3 + y1*y4 + y2*y3 + y2*y4 + y3*y4;
    T const z_poly = z1*z1 + z2*z2 + z3*z3 + z4*z4 + z1*z2 + z1*z3 + z1*z4 + z2*z3 + z2*z4 + z3*z4;
    T const two    = value_traits::two();
    
    T const E00 = (x2 - x1);
    T const E01 = (x3 - x1);
    T const E02 = (x4 - x1);
    T const E10 = (y2 - y1);
    T const E11 = (y3 - y1);
    T const E12 = (y4 - y1);
    T const E20 = (z2 - z1);
    T const E21 = (z3 - z1);
    T const E22 = (z4 - z1);
    
    T const j   = fabs( 
                       E00 * (  E11*E22 - E21*E12 ) 
                       -
                       E01 * (E10*E22 - E20*E12) 
                       +
                       E02 * ( E10*E21 - E20*E11) 
                       ); 
    
    T const volume = j / value_traits::numeric_cast(6.0) ;
    
    T const mass = density * volume;
    
    
    Properties<T> value;
    
    value.m_m = mass;
    
    value.m_Ixx = density*j*( y_poly + z_poly ) / value_traits::numeric_cast( 60.0 );
    value.m_Iyy = density*j*( x_poly + z_poly ) / value_traits::numeric_cast( 60.0 );
    value.m_Izz = density*j*( x_poly + y_poly ) / value_traits::numeric_cast( 60.0 );
    
    value.m_Ixy = density*j*(
                     two*x1*y1 + x1*y2     + x1*y3     + x1*y4      +
                     x2*y1     + two*x2*y2 + x2*y3     + x2*y4      +
                     x3*y1     + x3*y2     + two*x3*y3 + x3*y4      +
                     x4*y1     + x4*y2     + x4*y3     + two*x4*y4
                     ) / value_traits::numeric_cast( 120.0 );
    value.m_Ixz = density*j*(
                     two*x1*z1 + x1*z2     + x1*z3     + x1*z4      +
                     x2*z1     + two*x2*z2 + x2*z3     + x2*z4      +
                     x3*z1     + x3*z2     + two*x3*z3 + x3*z4      +
                     x4*z1     + x4*z2     + x4*z3     + two*x4*z4
                     ) / value_traits::numeric_cast( 120.0 );
    value.m_Iyz = density*j*(
                     two*y1*z1 + y1*z2     + y1*z3     + y1*z4      +
                     y2*z1     + two*y2*z2 + y2*z3     + y2*z4      +
                     y3*z1     + y3*z2     + two*y3*z3 + y3*z4      +
                     y4*z1     + y4*z2     + y4*z3     + two*y4*z4
                     ) / value_traits::numeric_cast( 120.0 );
    
    value.m_x = (x1+x2+x3+x4) / value_traits::four();
    value.m_y = (y1+y2+y3+y4) / value_traits::four();
    value.m_z = (z1+z2+z3+z4) / value_traits::four();
    
    return value;
  }
  
  template Properties<float> compute_tetrahedron<float>(
                                           float const & density
                                           , float const & x1
                                           , float const & y1
                                           , float const & z1
                                           , float const & x2
                                           , float const & y2
                                           , float const & z2
                                           , float const & x3
                                           , float const & y3
                                           , float const & z3
                                           , float const & x4
                                           , float const & y4
                                           , float const & z4
                                           );
  
  template Properties<double> compute_tetrahedron<double>(
                                            double const & density
                                            , double const & x1
                                            , double const & y1
                                            , double const & z1
                                            , double const & x2
                                            , double const & y2
                                            , double const & z2
                                            , double const & x3
                                            , double const & y3
                                            , double const & z3
                                            , double const & x4
                                            , double const & y4
                                            , double const & z4
                                            );
  
} // namespace mass
