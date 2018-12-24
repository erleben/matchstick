#ifndef PROX_CONTACT_POINT_H
#define PROX_CONTACT_POINT_H

#include <prox_rigid_body.h>

#include <tiny_precision.h>   // needed for working precision

#include <cmath> // needed for std::fabs
#include <cassert> // needed for assert

namespace prox
{
  
  template< typename M >
  class ContactPoint
  {
  public:
    
    typedef typename M::real_type       real_type;
    typedef typename M::vector3_type    vector3_type;
    typedef          RigidBody<M>       body_type;
    
  protected:
    
    vector3_type     m_position;
    vector3_type     m_normal;
    real_type        m_depth;
    
    body_type *      m_body_i;
    body_type *      m_body_j;

    vector3_type m_Sa; ///< structure direction of object A
    vector3_type m_Sb; ///< structure direction of object B
    vector3_type m_S;  ///< structure direction of the two objects combined
    
  public:

    ContactPoint()
    : m_position(vector3_type::make(real_type(0),real_type(0),real_type(0)))
    , m_normal(vector3_type::make(real_type(0),real_type(0),real_type(0)))
    , m_depth(real_type(0))
    , m_body_i(0)
    , m_body_j(0)
    , m_Sa(vector3_type::make(real_type(1),real_type(0),real_type(0)))
    , m_Sb(vector3_type::make(real_type(1),real_type(0),real_type(0)))
    , m_S(vector3_type::make(real_type(0),real_type(0),real_type(0)))
    {}
    
    virtual ~ContactPoint(){}
    
    ContactPoint (ContactPoint const & point)
    {
      *this = point;
    }
    
    ContactPoint & operator= (ContactPoint const & point)
    {
      assert(point.m_body_i  || !"ContactPoint::operator=(): body i pointer was null ");
      assert(point.m_body_j  || !"ContactPoint::operator=(): body j pointer was null ");

      if( this != &point)
      {
        this->m_position    = point.m_position;
        this->m_normal      = point.m_normal;
        this->m_depth       = point.m_depth;
        this->m_body_i      = point.m_body_i;
        this->m_body_j      = point.m_body_j;
        this->m_Sa          = point.m_Sa;
        this->m_Sb          = point.m_Sb;
        this->m_S           = point.m_S;
      }
      return *this;
    }
    
    vector3_type const & get_position()  const {  return this->m_position; }
    vector3_type const & get_normal()    const {  return this->m_normal;   }
    real_type    const & get_depth()     const {  return this->m_depth;    }
    body_type    const * get_body_i()    const {  return this->m_body_i;   }
    body_type    const * get_body_j()    const {  return this->m_body_j;   }
    vector3_type const & get_Sa()        const {  return this->m_Sa;       }
    vector3_type const & get_Sb()        const {  return this->m_Sb;       }
    vector3_type const & get_S()         const {  return this->m_S;        }

    
    void set_position(vector3_type const & p)  {  this->m_position = p;    }
    void set_normal(vector3_type const & n)    {  this->m_normal = n;      }
    void set_depth(real_type const & d)        {  this->m_depth = d;       }
    void set_Sa(vector3_type const & d)        {  this->m_Sa = d;          }
    void set_Sb(vector3_type const & d)        {  this->m_Sb = d;          }
    void set_S(vector3_type const & d)         {  this->m_S = d;           }

      void set_body_i(body_type const * body_i)
    {
      assert(body_i  || !"ContactPoint::set_body_i(): body i pointer was null ");

      this->m_body_i = const_cast<body_type*>(body_i); // 2009-11-25 Kenny: hmm can we not get rid of const casts?
    }

    void set_body_j(body_type const * body_j)
    {
      assert(body_j  || !"ContactPoint::set_body_i(): body j pointer was null ");

      this->m_body_j = const_cast<body_type*>(body_j);   // 2009-11-25 Kenny: hmm can we not get rid of const casts?
    }

  public:

    bool operator== (ContactPoint const & point) const
    {
      using std::fabs;

      typedef typename M::real_type       T;

      T const precision = tiny::working_precision<T>();

      if(this->m_body_i != point.m_body_i)
        return false;

      if(this->m_body_j != point.m_body_j)
        return false;

      T const p_diff1 = fabs( point.m_position(1) - this->m_position(1) ) ;
      T const p_diff0 = fabs( point.m_position(0) - this->m_position(0) ) ;
      T const p_diff2 = fabs( point.m_position(2) - this->m_position(2) ) ;

      if( p_diff1  > precision )
        return false;

      if( p_diff0  > precision )
        return false;

      if( p_diff2  > precision )
        return false;

      T const n_diff1 = fabs( point.m_normal(1) - this->m_normal(1) ) ;
      T const n_diff0 = fabs( point.m_normal(0) - this->m_normal(0) ) ;
      T const n_diff2 = fabs( point.m_normal(2) - this->m_normal(2) ) ;

      if( n_diff1  > precision )
        return false;

      if( n_diff0  > precision )
        return false;

      if( n_diff2  > precision )
        return false;

      T const d_diff = fabs( point.m_depth - this->m_depth ) ;

      if( d_diff  > precision )
        return false;

      return true;
    }

    bool operator< (ContactPoint const & point) const
    {
      if( this->m_body_i < point.m_body_i )
        return true;
      if( this->m_body_i > point.m_body_i )
        return false;

      if( this->m_body_j < point.m_body_j )
          return true;
      if( this->m_body_j > point.m_body_j )
        return false;

      if( this->m_position(1) < point.m_position(1) )
        return true;
      if( this->m_position(1) > point.m_position(1) )
        return false;

      if( this->m_position(0) < point.m_position(0) )
        return true;
      if( this->m_position(0) > point.m_position(0) )
        return false;


      if( this->m_position(2) < point.m_position(2) )
        return true;
      if( this->m_position(2) > point.m_position(2) )
        return false;

      if( this->m_normal(1) < point.m_normal(1) )
        return true;
      if(  this->m_normal(1) > point.m_normal(1) )
        return false;


      if( this->m_normal(0) < point.m_normal(0) )
        return true;
      if( this->m_normal(0) > point.m_normal(0) )
        return false;

      if(  this->m_normal(2) < point.m_normal(2) )
        return true;
      if(  this->m_normal(2) > point.m_normal(2) )
        return false;

      if(  this->m_depth < point.m_depth )
        return true;
      if(  this->m_depth > point.m_depth )
        return false;

      return false;
    }

  };
}// namespace prox

// PROX_CONTACT_POINT_H
#endif 
