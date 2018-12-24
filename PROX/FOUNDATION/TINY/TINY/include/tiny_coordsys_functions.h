#ifndef TINY_COORDSYS_FUNCTIONS_H
#define TINY_COORDSYS_FUNCTIONS_H

#include <tiny_coordsys.h>
#include <tiny_precision.h>
#include <tiny_quaternion.h>
#include <tiny_quaternion_functions.h>
#include <iosfwd>   // for std::istream and std::ostream

namespace tiny
{
    
    /**
     * This method assumes that the point is in this coordinate system.
     * In other words this method maps local points into non local
     * points:
     *
     * BF -> WCS
     *
     * Let p be the point and let f designate the function of this
     * method then we have
     *
     * [p]_WCS = f(p)
     *
     */
    template<typename T>
    inline Vector<3,T> xform_point(CoordSys<T> const & X, Vector<3,T> const & p) {    return rotate(X.Q(), p) + X.T();  }
    
    /**
     * This method assumes that the vector is in this
     * coordinate system. That is it maps the vector
     * from BF into WCS.
     */
    template<typename T>
    inline Vector<3,T> xform_vector(CoordSys<T> const & X, Vector<3,T> const & v) { return rotate(X.Q(), v); }
    
    /**
     * Transform Matrix.
     *
     * @param O   A reference to a rotation matrix, which should be transformed by X.
     */
    template<typename T>
    inline Matrix<3,3,T> xform_matrix(CoordSys<T> const & X, Matrix<3,3,T> const & O) { return make(X.Q()) * O; }
    
    /**
     * Coordinate Transformation Product.
     * This function should be used to concatenate coordinate transformations.
     * In terms of homogeneous coordinates L and R corresponds to the matrices.
     *
     * L =  | R_l  T_l |
     *      |  0    1  |
     *
     * R =  | R_r  T_r |
     *      |  0    1  |
     *
     * This function computes the equivalent of the product
     *
     *   X = L R
     *
     *    =  | R_l  T_l |  | R_r  T_r |
     *       |  0    1  |  |  0    1  |
     *
     *   =   | R_l R_r    R_l T_r + T_l |
     *       |    0             1       |
     *
     * @param L   The left coordinate transformation
     * @param R   The right coordinate transformation
     *
     * @return   The coordinate transformation corresponding to the product L*R.
     */
    template<typename T>
    inline CoordSys<T> prod(CoordSys<T> const & L, CoordSys<T> const & R)
    {
        return CoordSys<T>(    rotate(L.Q(), R.T() )  +  L.T() , unit( prod( L.Q() , R.Q()) ) );
    }
    
    /**
     * Inverse Transform.
     *
     * If we have
     *
     * BF -> WCS
     *
     * Then we want to find
     *
     * WCS -> BF
     *
     */
    template<typename T>
    inline CoordSys<T> inverse(CoordSys<T> const & X)
    {
        //---
        //---   p' = R p + T
        //---
        //---  =>
        //---
        //---   p = R^{-1} p' + R^{-1}(-T)
        //---
        return CoordSys<T>(  rotate(conj(X.Q()), -X.T()) , conj(X.Q()) );
    }
    
    /**
     * Model Update Transform.
     * This function computes the necessary transform needed in
     * order to transform coordinates from one local frame
     * into another local frame. This utility is useful when
     * one wants to do model updates instead of world updates.
     *
     * In mathematical terms we have two transforms:
     *
     * C1 : H -> G
     * C2 : F -> G
     *
     * And we want to find
     *
     * C3 : H -> F
     *
     * This transform is computed and assigned to this coordinate
     * system.
     *
     * Very important: Note that this method finds the transform A -> B.
     */
    template<typename T>
    inline CoordSys<T> model_update(Vector<3,T> const & TA, Quaternion<T> const & QA, Vector<3,T> const & TB, Quaternion<T> const & QB)
    {
        typedef typename T::real_type    R;

        typedef          ValueTraits<R>  VT;
        //---
        //---  p' = RA p + TA         (*1)  from A->WCS
        //---
        //---  p = RB^T (p' - TB)     (*2)  from WCS-B
        //---
        //--- Insert (*1) into (*2)  A -> B
        //---
        //---   p = RB^T ( RA p + TA - TB)
        //---     =  RB^T  RA p + RB^T (TA - TB)
        //--- So
        //---   R = RB^T  RA
        //---   T = RB^T (TA - TB)
        //---
        Quaternion<T> q;
        
        if(fabs(VT::one()- tiny::inner_prod(QA, QB)) < working_precision<R>())
        {
            q = Quaternion<T>::identity();
        }
        else
        {
            q = unit( prod( conj(QB), QA) );
        }
        return CoordSys<T>( rotate( conj(QB), (TA - TB)) ,  q);
    }
    
    template<typename T>
    inline CoordSys<T> model_update(CoordSys<T> const & A, CoordSys<T> const & B)
    {
        return model_update(A.T(),A.Q(),B.T(),B.Q());
    }
    
    template<typename T>
    inline std::ostream & operator<< (std::ostream & o, CoordSys<T> const & C)
    {
        o << "[" << C.T() << "," << C.Q() << "]";
        return o;
    }
    
    template<typename T>
    inline std::istream & operator>>(std::istream & i, CoordSys<T> & C)
    {
        char dummy;
        i >> dummy;
        i >> C.T();
        i >> dummy;
        i >> C.Q();
        i >> dummy;
        return i;
    }
    
} // namespace tiny

//TINY_COORDSYS_FUNCTIONS_H
#endif
