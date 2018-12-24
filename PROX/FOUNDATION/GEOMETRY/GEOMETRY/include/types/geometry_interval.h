#ifndef GEOMETRY_INTERVAL_H
#define GEOMETRY_INTERVAL_H

#include <tiny_value_traits.h>
#include <tiny_is_number.h>
#include <tiny_is_finite.h>

namespace geometry
{
  
  template<typename T>
  class Interval
  {
  protected:
    
    typedef tiny::ValueTraits<T> VT;
    
    T m_lower;
    T m_upper;
    
  public:
    
    T const & lower() const { return this->m_lower; }
    T       & lower()       { return this->m_lower; }
    T const & upper() const { return this->m_upper; }
    T       & upper()       { return this->m_upper; }
    
  public:
    
    Interval()
    : m_lower( VT::highest() )
    , m_upper( VT::lowest() )
    {}
    
  };
  
}// namespace geometry

// GEOMETRY_INTERVAL_H
#endif