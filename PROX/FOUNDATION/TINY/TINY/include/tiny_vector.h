#ifndef TINY_VECTOR_H
#define TINY_VECTOR_H

#include <tiny_container.h>
#include <tiny_value_traits.h>
#include <tiny_random.h>

namespace tiny
{
  
  template <size_t N, typename T>
  class Vector 
  : public detail::Container<1,N,T>
  {
  public:
    
    typedef          detail::Container<1, N, T> 			  base_class_type;
    typedef typename T::real_type   		                real_type;
    typedef typename T::op_type     		                op_type;
    typedef          ValueTraits<real_type>		          value_traits;
    
  public:
    
    Vector () 
		: base_class_type() 
		{}
    
    ~Vector () {} 
    
    Vector ( Vector const & v ) 
		: base_class_type(v)
		{}
    
		explicit Vector ( real_type const & value ) 
		: base_class_type(value)
		{}
    
    Vector & operator = (Vector const & v)
    {
      base_class_type::operator=(v);
      return *this;
		}
    
  public:
    
    static Vector<3,T> make( real_type const & x, real_type const & y, real_type const & z )
    {
      Vector<3,T> v;
      v(0) = x;
      v(1) = y;
      v(2) = z;
      return v;      
    }
    
    static Vector<4,T> make( real_type const & x, real_type const & y, real_type const & z, real_type const & w )
    {
      Vector<4,T> v;
      v(0) = x;
      v(1) = y;
      v(2) = z;
      v(3) = w;
      return v;      
    }
    
    static Vector<3,T> zero( )
    {
      Vector<3,T> v;
      v(0) = value_traits::zero();
      v(1) = value_traits::zero();
      v(2) = value_traits::zero();
      return v;      
    }
    
    static Vector<3,T> i( )
    {
      Vector<3,T> v;
      v(0) = value_traits::one();
      v(1) = value_traits::zero();
      v(2) = value_traits::zero();
      return v;      
    }
    
    static Vector<3,T> j( )
    {
      Vector<3,T> v;
      v(0) = value_traits::zero();
      v(1) = value_traits::one();
      v(2) = value_traits::zero();
      return v;      
    }
    
    static Vector<3,T> k( )
    {
      Vector<3,T> v;
      v(0) = value_traits::zero();
      v(1) = value_traits::zero();
      v(2) = value_traits::one();
      return v;      
    }
    
    static Vector random ( real_type const & lower, real_type const & upper)
    {      
      typedef typename Vector::iterator   iterator;
      
      Random<real_type> value(lower,upper);
      Vector v;
      iterator i     = v.begin();
      iterator end   = v.end();
      for (; i!=end ;++i)
        *i = value();
      
      return v;
    }
    
    static Vector random () { return random(value_traits::zero(), value_traits::one());  }

  }; 
  
} // namespace tiny

// TINY_VECTOR_H
#endif 
