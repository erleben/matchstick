#ifndef TINY_MATRIX_H
#define TINY_MATRIX_H

#include <tiny_value_traits.h>
#include <tiny_container.h>
#include <tiny_vector.h>
#include <tiny_random.h>

namespace tiny
{
  
  template <size_t I,size_t J, typename T>
  class Matrix 
  : public detail::Container<I,J,T >
  {
  public:
    
    typedef          detail::Container<I,J,T>		    base_class_type;
    typedef typename T::real_type	        		      real_type;
    typedef typename T::op_type     			          op_type;
    typedef          ValueTraits<real_type>         value_traits;
    typedef          Vector<J,T>		                row_type;
    typedef          Vector<I,T>                    column_type;
    
  public:
    
    Matrix()
    : base_class_type()
    {}
    
    explicit Matrix( real_type const & value )
    : base_class_type(value)
    {}
    
    Matrix( Matrix const & cpy )
    : base_class_type(cpy)
    {}
    
    ~Matrix () {}
    
    Matrix &  operator= (Matrix const & rhs)
    {
      base_class_type::operator=( rhs );
      return *this;
    }
    
    column_type get_column_copy (size_t const & c ) const
    {
      assert( (c<J) || !"c was out of bounds");
      
      column_type column;
            
      for (size_t i = 0; i<I; ++i )
        column[i] = this->m_data[ base_class_type::J_padded*i + c];
      
      return column;
    }
    
    row_type get_row_copy ( size_t const & r ) const
    {
      assert( (r<I) || !"r was out of bounds");
      
      row_type row;
      
      size_t const offset = r * base_class_type::J_padded;
      for (size_t j = 0; j<J; ++j ) // iterate over coloums
        row[j] = this->m_data[offset+j];
    
      return row;
    }
    
  public:
    
    static Matrix<I,J,T> identity(  )
    {
      Matrix<I,J,T>  M;
      
      M.clear();
      
      size_t const K = (I<J ? I : J);
      for (size_t i = 0 ; i< K ; ++i)
        M(i,i) = value_traits::one();
      
      return M;
    }

    static Matrix<I,J,T> zero(  )
    {
      return Matrix<I,J,T>( value_traits::zero() );
    }

    static Matrix<3,3, T>  make(
                                typename T::real_type const & m00, typename T::real_type const & m01, typename T::real_type const & m02
                                , typename T::real_type const & m10, typename T::real_type const & m11, typename T::real_type const & m12
                                , typename T::real_type const & m20, typename T::real_type const & m21, typename T::real_type const & m22
                                )
    {
      Matrix<3,3, T> M;
      M(0,0) = m00; M(0,1) = m01; M(0,2) = m02; 
      M(1,0) = m10; M(1,1) = m11; M(1,2) = m12; 
      M(2,0) = m20; M(2,1) = m21; M(2,2) = m22;
      return M;
    }
    
    static Matrix<3,3,T> make_diag( real_type const & d )
    {
      Matrix<3,3,T> M;
      M.clear();
      M(0,0) = M(1,1) = M(2,2) = d;
      return M;
    }
    
    static Matrix<3,3,T> make_diag( real_type const & d0, real_type const & d1, real_type const & d2 )
    {
      Matrix<3,3,T> M;
      M.clear();
      M(0,0)=d0;
      M(1,1)=d1;
      M(2,2)=d2;
      return M;
    }
    
    template<size_t N>
    static Matrix<N,N,T> make_diag( Vector<N,T> const & v )
    {
      Matrix<N,N,T> M;
      M.clear();
      for (size_t i = 0;i<N ; ++i)
        M(i,i) = v(i);
      return M;
    }
    
    /**
     * Creates a rotation matrix.
     * This method returns a general rotation matrix around a specified axe. It assumes that post-multiplication by colum vectors is used.
     *
     * @param radians           The rotation angle in radians.
     * @param axe               A vector. This is the rotation axe.
     */    
    static  Matrix<3,3,T> Ru( real_type const & radians, Vector<3,T> const & axis )
    {
      using std::cos;
      using std::sin;
      
      real_type const cosinus = value_traits::numeric_cast( cos(radians) );
      real_type const sinus   = value_traits::numeric_cast( sin(radians) );
      Vector<3,T> u = unit(axis);
      
      Matrix<3,3, T> R;
      
      //Foley p.227 (5.76)
      R(0,0) = u(0)*u(0) + cosinus*(value_traits::one() - u(0)*u(0));
      R(0,1) = u(0)*u(1)*(value_traits::one()-cosinus) - sinus*u(2);
      R(0,2) = u(0)*u(2)*(value_traits::one()-cosinus) + sinus*u(1);
      R(1,0) = u(0)*u(1)*(value_traits::one()-cosinus) + sinus*u(2);
      R(1,1) = u(1)*u(1) + cosinus*(value_traits::one() - u(1)*u(1));
      R(1,2) = u(1)*u(2)*(value_traits::one()-cosinus) - sinus*u(0);
      R(2,0) = u(0)*u(2)*(value_traits::one()-cosinus) - sinus*u(1);
      R(2,1) = u(1)*u(2)*(value_traits::one()-cosinus) + sinus*u(0);
      R(2,2) = u(2)*u(2) + cosinus*(value_traits::one() - u(2)*u(2));
      
      return R;
    }
    
    
    /**
     * Creates a rotation matrix.
     * This method returns a rotation matrix around the x-axe. It
     * assumes that post-multiplication by colum vectors is used.
     *
     * @param radians           The rotation angle in radians.
     */
    static Matrix<3,3,T> Rx(real_type const & radians)
    {      
      using std::cos;
      using std::sin;
      real_type const cosinus = value_traits::numeric_cast( cos(radians) );
      real_type const sinus   = value_traits::numeric_cast( sin(radians) );
      return Matrix<3,3,T>::make(
                          value_traits::one(), value_traits::zero(),        value_traits::zero(),
                          value_traits::zero(),              cosinus,                      -sinus,
                          value_traits::zero(),                sinus,                      cosinus
                          );
    }
    
    /**
     * Creates a rotation matrix.
     * This method returns a rotation matrix around the y-axe. It
     * assumes that post-multiplication by colum vectors is used.
     *
     * @param radians           The rotation angle in radians.
     */
    static Matrix<3,3,T> Ry(real_type const & radians)
    {      
      using std::cos;
      using std::sin;
      real_type const cosinus = value_traits::numeric_cast( cos(radians) );
      real_type const sinus   = value_traits::numeric_cast( sin(radians) );
      return Matrix<3,3,T>::make(
                          cosinus,   value_traits::zero(),                   sinus,
                          value_traits::zero(),    value_traits::one(),    value_traits::zero(),
                          -sinus,   value_traits::zero(),                 cosinus
                          );
    }
    
    /**
     * Creates a rotation matrix.
     * This method returns a rotation matrix around the z-axe. It assumes that post-multiplication by colum vectors is used.
     *
     * @param radians           The rotation angle in radians.
     */
    static Matrix<3,3,T> Rz(real_type const & radians)
    {      
      using std::cos;
      using std::sin;
      real_type const cosinus = value_traits::numeric_cast( cos(radians) );
      real_type const sinus   = value_traits::numeric_cast( sin(radians) );
      return Matrix<3,3,T>::make(
                          cosinus,                                      -sinus,       value_traits::zero(),
                          sinus,                                       cosinus,       value_traits::zero(),
                          value_traits::zero(),           value_traits::zero(),        value_traits::one()
                          );
    }
    
    static Matrix random ( real_type const & lower, real_type const & upper)
    {      
      typedef typename Matrix::iterator iterator;
      
      Random<real_type> value(lower,upper);
      
      Matrix m;
      
      iterator i     = m.begin();
      iterator end   = m.end();
      for (; i!=end ;++i)
      {
        *i = value();
      }
      return m;
    }
    
    static Matrix random () { return random(value_traits::zero(), value_traits::one());  }
    
    
  };
  
} //  namespace tiny

// TINY_MATRIX_H
#endif 

