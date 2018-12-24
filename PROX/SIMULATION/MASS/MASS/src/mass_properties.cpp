#include <mass.h>

#include <tiny_is_finite.h>
#include <tiny_is_number.h>
#include <tiny_value_traits.h>

#include <cassert>
#include <ostream>   

namespace mass
{
  //-------------------------------------------------------------------------
  template<typename T>  
  Properties<T>::Properties()
  : m_m( tiny::ValueTraits<T>::zero())
  , m_Ixx( tiny::ValueTraits<T>::zero())
  , m_Iyy( tiny::ValueTraits<T>::zero())
  , m_Izz( tiny::ValueTraits<T>::zero())
  , m_Ixy( tiny::ValueTraits<T>::zero())
  , m_Ixz( tiny::ValueTraits<T>::zero())
  , m_Iyz( tiny::ValueTraits<T>::zero())
  , m_x( tiny::ValueTraits<T>::zero())
  , m_y( tiny::ValueTraits<T>::zero())
  , m_z( tiny::ValueTraits<T>::zero())
  , m_Qs( tiny::ValueTraits<T>::one())
  , m_Qx( tiny::ValueTraits<T>::zero())
  , m_Qy( tiny::ValueTraits<T>::zero())
  , m_Qz( tiny::ValueTraits<T>::zero())
  {}
  //-------------------------------------------------------------------------  
  template<typename T>  
  Properties<T>::~Properties(){}
  //-------------------------------------------------------------------------  
  template<typename T>  
  Properties<T>::Properties( Properties<T> const & cpy ) { (*this) = cpy; }
  //-------------------------------------------------------------------------  
  template<typename T>  
  Properties<T> & Properties<T>::operator=( Properties<T> const & lhs )
  {
    if (this != &lhs)
    {
      this->m_m   = lhs.m_m;
      this->m_Ixx = lhs.m_Ixx;
      this->m_Iyy = lhs.m_Iyy;
      this->m_Izz = lhs.m_Izz;
      this->m_Ixy = lhs.m_Ixy;
      this->m_Ixz = lhs.m_Ixz;
      this->m_Iyz = lhs.m_Iyz;
      this->m_x   = lhs.m_x;
      this->m_y   = lhs.m_y;
      this->m_z   = lhs.m_z;
      this->m_Qs  = lhs.m_Qs;
      this->m_Qx  = lhs.m_Qx;
      this->m_Qy  = lhs.m_Qy;
      this->m_Qz  = lhs.m_Qz;
    }
    return *this;
  }
  //-------------------------------------------------------------------------  
  template<typename T>  
  bool Properties<T>::operator==( Properties<T> const & lhs ) const
  {
    if( this->m_m   != lhs.m_m   ) return false;
    if( this->m_Ixx != lhs.m_Ixx ) return false;
    if( this->m_Iyy != lhs.m_Iyy ) return false;
    if( this->m_Izz != lhs.m_Izz ) return false;
    if( this->m_Ixy != lhs.m_Ixy ) return false;
    if( this->m_Ixz != lhs.m_Ixz ) return false;
    if( this->m_Iyz != lhs.m_Iyz ) return false;
    if( this->m_x   != lhs.m_x   ) return false;
    if( this->m_y   != lhs.m_y   ) return false;
    if( this->m_z   != lhs.m_z   ) return false;
    if( this->m_Qs  != lhs.m_Qs  ) return false;
    if( this->m_Qx  != lhs.m_Qx  ) return false;
    if( this->m_Qy  != lhs.m_Qy  ) return false;
    if( this->m_Qz  != lhs.m_Qz  ) return false;
    
    return true;
  }
  //-------------------------------------------------------------------------  
  template<typename T>  
  bool Properties<T>::is_body_space() const { return !(this->is_model_space()); }
  //-------------------------------------------------------------------------  
  template<typename T>  
	bool Properties<T>::is_model_space() const
  {
    using namespace tiny;    
    typedef ValueTraits<T> VT;
    
    assert( this->m_m    >= VT::zero() || !"Internal error");
    assert( this->m_Ixx  >= VT::zero() || !"Internal error");
    assert( this->m_Iyy  >= VT::zero() || !"Internal error");
    assert( this->m_Izz  >= VT::zero() || !"Internal error");

    assert( is_number( this->m_m )     || !"Internal error");
    assert( is_number( this->m_Ixx )   || !"Internal error");
    assert( is_number( this->m_Iyy )   || !"Internal error");
    assert( is_number( this->m_Izz )   || !"Internal error");
    assert( is_number( this->m_Ixy )   || !"Internal error");
    assert( is_number( this->m_Ixz )   || !"Internal error");
    assert( is_number( this->m_Iyz )   || !"Internal error");
    assert( is_number( this->m_x )     || !"Internal error");
    assert( is_number( this->m_y )     || !"Internal error");
    assert( is_number( this->m_z )     || !"Internal error");
    assert( is_number( this->m_Qs )    || !"Internal error");
    assert( is_number( this->m_Qx )    || !"Internal error");
    assert( is_number( this->m_Qy )    || !"Internal error");
    assert( is_number( this->m_Qz )    || !"Internal error");

    assert( is_finite( this->m_m )     || !"Internal error");
    assert( is_finite( this->m_Ixx )   || !"Internal error");
    assert( is_finite( this->m_Iyy )   || !"Internal error");
    assert( is_finite( this->m_Izz )   || !"Internal error");
    assert( is_finite( this->m_Ixy )   || !"Internal error");
    assert( is_finite( this->m_Ixz )   || !"Internal error");
    assert( is_finite( this->m_Iyz )   || !"Internal error");
    assert( is_finite( this->m_x )     || !"Internal error");
    assert( is_finite( this->m_y )     || !"Internal error");
    assert( is_finite( this->m_z )     || !"Internal error");
    assert( is_finite( this->m_Qs )    || !"Internal error");
    assert( is_finite( this->m_Qx )    || !"Internal error");
    assert( is_finite( this->m_Qy )    || !"Internal error");
    assert( is_finite( this->m_Qz )    || !"Internal error");
    
    if( this->m_Ixy != VT::zero() ) return true;
    if( this->m_Ixz != VT::zero() ) return true;
    if( this->m_Iyz != VT::zero() ) return true;
    if( this->m_x   != VT::zero() ) return true;
    if( this->m_y   != VT::zero() ) return true;
    if( this->m_z   != VT::zero() ) return true;
    if( this->m_Qs  != VT::one()  ) return true;
    if( this->m_Qx  != VT::zero() ) return true;
    if( this->m_Qy  != VT::zero() ) return true;
    if( this->m_Qz  != VT::zero() ) return true;
    
    return false;
  }
  //-------------------------------------------------------------------------  
  template<typename T>  
  void Properties<T>::clear()
  {
    using namespace tiny;    
    typedef ValueTraits<T> VT;
    
    this->m_m   = VT::zero();
    this->m_Ixx = VT::zero();
    this->m_Iyy = VT::zero();
    this->m_Izz = VT::zero();
    this->m_Ixy = VT::zero();
    this->m_Ixz = VT::zero();
    this->m_Iyz = VT::zero();
    this->m_x   = VT::zero();
    this->m_y   = VT::zero();
    this->m_z   = VT::zero();
    this->m_Qs  = VT::one();
    this->m_Qx  = VT::zero();
    this->m_Qy  = VT::zero();
    this->m_Qz  = VT::zero();
  }
  //-------------------------------------------------------------------------
   
  template<typename T>
  std::ostream & operator<< (std::ostream & o, Properties<T> const & P)
  {
    o << "  m = " << P.m_m   << std::endl;
    o << "Ixx = " << P.m_Ixx << std::endl;
    o << "Iyy = " << P.m_Iyy << std::endl;
    o << "Izz = " << P.m_Izz << std::endl;
    o << "Ixy = " << P.m_Ixy << std::endl;
    o << "Ixz = " << P.m_Ixz << std::endl;
    o << "Iyz = " << P.m_Iyz << std::endl;
    o << "  x = " << P.m_x   << std::endl;
    o << "  y = " << P.m_y   << std::endl;
    o << "  z = " << P.m_z   << std::endl;
    o << " Qs = " << P.m_Qs  << std::endl;
    o << " Qx = " << P.m_Qx  << std::endl;
    o << " Qy = " << P.m_Qy  << std::endl;
    o << " Qz = " << P.m_Qz  << std::endl;
    return o;
  }
  //-------------------------------------------------------------------------
  
  template class Properties<float>;
  template class Properties<double>;
  
  template
  std::ostream & operator<< (std::ostream & o, Properties<float> const & P);

  template
  std::ostream & operator<< (std::ostream & o, Properties<double> const & P);

} // namespace mass
