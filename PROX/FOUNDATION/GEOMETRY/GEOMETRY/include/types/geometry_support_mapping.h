#ifndef GEOMETRY_SUPPORT_MAPPING
#define GEOMETRY_SUPPORT_MAPPING

namespace geometry
{
  
  template<typename V>
  class SupportMapping
  {    
  public:

    /**
     * Get Support Point of Shape.
     *
     * A support point is simply the extreme point of a shape in a given direction.
     *
     * @param v    The support direction.
     *
     * @return     The support point.
     */
    virtual V get_support_point( V const & v ) const = 0;

  };
    
} // namespace geometry

// GEOMETRY_SUPPORT_MAPPING
#endif
