#ifndef GEOMETRY_LINE_H
#define GEOMETRY_LINE_H

namespace geometry
{
  
  template<typename V>
  class Line
  {
  protected:
    
    V m_point;
    V m_direction;
    
  public:
    
    V       & point()           { return this->m_point;      }
    V       & direction()       { return this->m_direction;  }
    V const & point()     const { return this->m_point;      }
    V const & direction() const { return this->m_direction;  }
    
  public:
    
    Line()
    : m_point()
    , m_direction()
    {}
    
    Line(V const & point, V const & direction)
    : m_point(point)
    , m_direction( unit( direction) )
    {}

    Line(Line const & line)
    {
      *this = line;
    }

    Line & operator=(Line const & line)
    {
      if(this != &line)
      {
        this->m_point      = line.m_point;
        this->m_direction  = line.m_direction;
      }
      return *this;
    }
  
  };

  struct FROM_POINTS {};
  struct FROM_DIRECTION {};

  template<typename V>
  inline Line<V> make_line(V const & a, V const & b, FROM_POINTS const & )
  {
    return Line<V>( a, b - a  );
  }

  template<typename V>
  inline Line<V> make_line(V const & point, V const & direction, FROM_DIRECTION const & )
  {
    return Line<V>( point, direction );
  }

  template<typename V>
  inline Line<V> make_line(V const & a, V const & b)
  {
    return make_line( a, b, FROM_POINTS() );
  }

}// namespace geometry

// GEOMETRY_LINE_H
#endif