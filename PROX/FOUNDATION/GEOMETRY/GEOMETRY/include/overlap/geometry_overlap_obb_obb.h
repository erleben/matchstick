#ifndef GEOMETRY_OVERLAP_OBB_OBB
#define GEOMETRY_OVERLAP_OBB_OBB

#include <types/geometry_obb.h>

#include <tiny.h>

#include <cmath>
#include <vector>

namespace geometry
{
  namespace detail
  {
    /**
     * This function generates all possible separating axes between the two specified OBBs.
     */
    template<typename MT>
    inline void compute_obb_obb_sat_axes(
                                         OBB<MT> const & A
                                         , OBB<MT> const & B
                                         , std::vector<typename MT::vector3_type> & axes
                                         )
    {
      using std::min;
      using std::max;
      using std::fabs;

      typedef typename MT::vector3_type    V;
      typedef typename MT::matrix3x3_type  M;
      typedef typename MT::real_type       T;
      typedef typename MT::value_traits    VT;

      M const R_a = tiny::make(A.orientation());
      M const R_b = tiny::make(B.orientation());

        //    V const A0  = tiny::truncate( R_a.get_column_copy(0), tiny::working_precision<T>() );
        //    V const A1  = tiny::truncate( R_a.get_column_copy(1), tiny::working_precision<T>() );
        //    V const A2  = tiny::truncate( R_a.get_column_copy(2), tiny::working_precision<T>() );
        //    V const B0  = tiny::truncate( R_b.get_column_copy(0), tiny::working_precision<T>() );
        //    V const B1  = tiny::truncate( R_b.get_column_copy(1), tiny::working_precision<T>() );
        //    V const B2  = tiny::truncate( R_b.get_column_copy(2), tiny::working_precision<T>() );

      V const A0  = R_a.get_column_copy(0);
      V const A1  = R_a.get_column_copy(1);
      V const A2  = R_a.get_column_copy(2);

      assert( fabs( VT::one() - inner_prod(A0,A0) ) < tiny::working_precision<T>() || !"compute_obb_obb_sat_axes(): logic error");
      assert( fabs( VT::one() - inner_prod(A1,A1) ) < tiny::working_precision<T>() || !"compute_obb_obb_sat_axes(): logic error");
      assert( fabs( VT::one() - inner_prod(A2,A2) ) < tiny::working_precision<T>() || !"compute_obb_obb_sat_axes(): logic error");
      assert( fabs( inner_prod(A0,A1) ) < tiny::working_precision<T>()             || !"compute_obb_obb_sat_axes(): logic error");
      assert( fabs( inner_prod(A0,A2) ) < tiny::working_precision<T>()             || !"compute_obb_obb_sat_axes(): logic error");
      assert( fabs( inner_prod(A1,A2) ) < tiny::working_precision<T>()             || !"compute_obb_obb_sat_axes(): logic error");

      V const B0  = R_b.get_column_copy(0);
      V const B1  = R_b.get_column_copy(1);
      V const B2  = R_b.get_column_copy(2);

      assert( fabs( VT::one() - inner_prod(B0,B0) ) < tiny::working_precision<T>() || !"compute_obb_obb_sat_axes(): logic error");
      assert( fabs( VT::one() - inner_prod(B1,B1) ) < tiny::working_precision<T>() || !"compute_obb_obb_sat_axes(): logic error");
      assert( fabs( VT::one() - inner_prod(B2,B2) ) < tiny::working_precision<T>() || !"compute_obb_obb_sat_axes(): logic error");
      assert( fabs( inner_prod(B0,B1) ) < tiny::working_precision<T>()             || !"compute_obb_obb_sat_axes(): logic error");
      assert( fabs( inner_prod(B0,B2) ) < tiny::working_precision<T>()             || !"compute_obb_obb_sat_axes(): logic error");
      assert( fabs( inner_prod(B1,B2) ) < tiny::working_precision<T>()             || !"compute_obb_obb_sat_axes(): logic error");

      axes.resize(15u);

      axes[0] = A0;
      axes[1] = A1;
      axes[2] = A2;

      axes[3] = B0;
      axes[4] = B1;
      axes[5] = B2;

      axes[6]     = tiny::cross(A0, B0);
      axes[7]     = tiny::cross(A0, B1);
      axes[8]     = tiny::cross(A0, B2);

      axes[9]     = tiny::cross(A1, B0);
      axes[10]    = tiny::cross(A1, B1);
      axes[11]    = tiny::cross(A1, B2);

      axes[12]    = tiny::cross(A2, B0);
      axes[13]    = tiny::cross(A2, B1);
      axes[14]    = tiny::cross(A2, B2);


      T const l6  = tiny::norm( axes[6]  );
      T const l7  = tiny::norm( axes[7]  );
      T const l8  = tiny::norm( axes[8]  );
      T const l9  = tiny::norm( axes[9]  );
      T const l10 = tiny::norm( axes[10] );
      T const l11 = tiny::norm( axes[11] );
      T const l12 = tiny::norm( axes[12] );
      T const l13 = tiny::norm( axes[13] );
      T const l14 = tiny::norm( axes[14] );

      axes[6]     = (l6  > tiny::working_precision<T>() ) ? axes[6]  / l6  : A0;
      axes[7]     = (l7  > tiny::working_precision<T>() ) ? axes[7]  / l7  : A0;
      axes[8]     = (l8  > tiny::working_precision<T>() ) ? axes[8]  / l8  : A0;

      axes[9]     = (l9  > tiny::working_precision<T>() ) ? axes[9]  / l9  : A1;
      axes[10]    = (l10 > tiny::working_precision<T>() ) ? axes[10] / l10 : A1;
      axes[11]    = (l11 > tiny::working_precision<T>() ) ? axes[11] / l11 : A1;

      axes[12]    = (l12 > tiny::working_precision<T>() ) ? axes[12] / l12 : A2;
      axes[13]    = (l13 > tiny::working_precision<T>() ) ? axes[13] / l13 : A2;
      axes[14]    = (l14 > tiny::working_precision<T>() ) ? axes[14] / l14 : A2;

      assert( is_number( axes[0](0) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[0](0) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[0](1) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[0](1) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[0](2) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[0](2) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[0],axes[0]) ) < tiny::working_precision<T>() || !"compute_obb_obb_sat_axes(): logic error");

      assert( is_number( axes[1](0) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[1](0) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[1](1) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[1](1) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[1](2) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[1](2) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[1],axes[1]) ) < tiny::working_precision<T>() || !"compute_obb_obb_sat_axes(): logic error");

      assert( is_number( axes[2](0) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[2](0) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[2](1) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[2](1) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[2](2) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[2](2) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[2],axes[2]) ) < tiny::working_precision<T>() || !"compute_obb_obb_sat_axes(): logic error");

      assert( is_number( axes[3](0) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[3](0) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[3](1) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[3](1) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[3](2) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[3](2) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[3],axes[3]) ) < tiny::working_precision<T>() || !"compute_obb_obb_sat_axes(): logic error");

      assert( is_number( axes[4](0) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[4](0) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[4](1) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[4](1) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[4](2) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[4](2) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[4],axes[4]) ) < tiny::working_precision<T>() || !"compute_obb_obb_sat_axes(): logic error");

      assert( is_number( axes[5](0) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[5](0) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[5](1) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[5](1) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[5](2) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[5](2) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[5],axes[5]) ) < tiny::working_precision<T>() || !"compute_obb_obb_sat_axes(): logic error");

      assert( is_number( axes[6](0) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[6](0) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[6](1) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[6](1) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[6](2) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[6](2) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[6],axes[6]) ) < tiny::working_precision<T>() || !"compute_obb_obb_sat_axes(): logic error");

      assert( is_number( axes[7](0) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[7](0) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[7](1) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[7](1) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[7](2) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[7](2) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[7],axes[7]) ) < tiny::working_precision<T>() || !"compute_obb_obb_sat_axes(): logic error");

      assert( is_number( axes[8](0) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[8](0) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[8](1) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[8](1) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[8](2) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[8](2) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[8],axes[8]) ) < tiny::working_precision<T>() || !"compute_obb_obb_sat_axes(): logic error");

      assert( is_number( axes[9](0) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[9](0) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[9](1) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[9](1) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[9](2) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[9](2) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[9],axes[9]) ) < tiny::working_precision<T>() || !"compute_obb_obb_sat_axes(): logic error");

      assert( is_number( axes[10](0) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[10](0) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[10](1) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[10](1) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[10](2) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[10](2) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[10],axes[10]) ) < tiny::working_precision<T>() || !"compute_obb_obb_sat_axes(): logic error");

      assert( is_number( axes[11](0) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[11](0) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[11](1) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[11](1) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[11](2) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[11](2) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[11],axes[11]) ) < tiny::working_precision<T>() || !"compute_obb_obb_sat_axes(): logic error");

      assert( is_number( axes[12](0) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[12](0) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[12](1) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[12](1) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[12](2) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[12](2) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[12],axes[12]) ) < tiny::working_precision<T>() || !"compute_obb_obb_sat_axes(): logic error");

      assert( is_number( axes[13](0) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[13](0) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[13](1) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[13](1) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[13](2) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[13](2) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[13],axes[13]) ) < tiny::working_precision<T>() || !"compute_obb_obb_sat_axes(): logic error");

      assert( is_number( axes[14](0) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[14](0) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[14](1) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[14](1) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( is_number( axes[14](2) ) || !"compute_obb_obb_sat_axes(): nan");
      assert( is_finite( axes[14](2) ) || !"compute_obb_obb_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[14],axes[14]) ) < tiny::working_precision<T>() || !"compute_obb_obb_sat_axes(): logic error");
    }

  }// end namespace detail

  /**
   * Extended OBB versus OBB Separating Axis(SAT) overlap Test. This version
   * provides geometric feedback information that can be helpful to determine
   * the "best" separation direction (the normal direction of the overlap).
   *
   * @param a   Corner points of OBB A.
   * @param A   OBB A.
   * @param b   Corner points of OBB B.
   * @param B   OOB B.
   * @param n   Upon return, this holds a potential normal for the overlap.
   *
   * @return    If overlap exists then the return value is true.
   */
  template<typename MT>
  inline bool overlap_obb_obb(
                              std::vector<typename MT::vector3_type> const & a
                              , OBB<MT> const & A
                              , std::vector<typename MT::vector3_type> const & b
                              , OBB<MT> const & B
                              , typename MT::vector3_type & n
                              )
  {
    using std::min;
    using std::max;
    using std::fabs;

    typedef typename MT::vector3_type    V;
    typedef typename MT::real_type       T;
    typedef typename MT::value_traits    VT;

    std::vector<V> axes;
    detail::compute_obb_obb_sat_axes( A,B, axes);

    std::vector<T> a_min( 15u, VT::highest() );
    std::vector<T> b_min( 15u, VT::highest() );
    std::vector<T> a_max( 15u, VT::lowest()  );
    std::vector<T> b_max( 15u, VT::lowest()  );

    T min_overlap = VT::lowest();

    for(size_t i=0u;i < 15u; ++i)
    {

      for( typename std::vector<V>::const_iterator p_a = a.begin(); p_a != a.end(); ++p_a)
      {
        T const d = inner_prod( (*p_a), axes[i] );
        a_min[i] = min( a_min[i], d);
        a_max[i] = max( a_max[i], d);
      }
      for( typename std::vector<V>::const_iterator p_b = b.begin(); p_b != b.end(); ++p_b)
      {
        T const d = inner_prod( (*p_b), axes[i] );
        b_min[i] = min( b_min[i], d);
        b_max[i] = max( b_max[i], d);
      }

      assert( is_number( a_min[i] ) || !"overlap_obb_obb(); nan");
      assert( is_finite( a_min[i] ) || !"overlap_obb_obb(): inf");
      assert( is_number( a_max[i] ) || !"overlap_obb_obb(); nan");
      assert( is_finite( a_max[i] ) || !"overlap_obb_obb(): inf");

      assert( is_number( b_min[i] ) || !"overlap_obb_obb(); nan");
      assert( is_finite( b_min[i] ) || !"overlap_obb_obb(): inf");
      assert( is_number( b_max[i] ) || !"overlap_obb_obb(); nan");
      assert( is_finite( b_max[i] ) || !"overlap_obb_obb(): inf");

      if(a_max[i] < b_min[i])
        return false;

      if(b_max[i] < a_min[i])
        return false;

      if(a_min[i] <= b_min[i] &&  b_min[i] <= a_max[i])
      {
        T const overlap = b_min[i] - a_max[i];

        if(overlap > min_overlap)
        {
          min_overlap = overlap;
          n = axes[i];
        }
      }

      if(b_min[i] <= a_min[i] &&  a_min[i] <= b_max[i])
      {
        T const overlap = a_min[i] - b_max[i];

        if(overlap > min_overlap)
        {
          min_overlap = overlap;
          n = -axes[i];
        }
      }

      assert( is_number( min_overlap ) || !"overlap_obb_obb(); nan");
      assert( is_finite( min_overlap ) || !"overlap_obb_obb(): inf");
    }
    
    return (min_overlap <= VT::zero());
  }

  /**
   * This version of the OBB verusus OBB overlap test is faster and does not
   * care about finding a sensible separation direction.
   */
  template<typename MT>
  inline bool overlap_obb_obb(
                                OBB<MT> const & A
                              , OBB<MT> const & B
                              )
  {
    using std::min;
    using std::max;
    using std::fabs;

    typedef typename MT::vector3_type    V;
    typedef typename MT::real_type       T;
    typedef typename MT::value_traits    VT;
    typedef typename MT::matrix3x3_type  M;

    std::vector<V> axes;
    detail::compute_obb_obb_sat_axes( A,B, axes);

    std::vector<T> a_min( 15u, VT::highest() );
    std::vector<T> b_min( 15u, VT::highest() );
    std::vector<T> a_max( 15u, VT::lowest()  );
    std::vector<T> b_max( 15u, VT::lowest()  );

    M const R_a = tiny::make(A.orientation());
    M const R_b = tiny::make(B.orientation());

    V const ai_extent  = R_a.get_column_copy(0) * A.half_extent()(0);
    V const aj_extent  = R_a.get_column_copy(1) * A.half_extent()(1);
    V const ak_extent  = R_a.get_column_copy(2) * A.half_extent()(2);
    V const bi_extent  = R_b.get_column_copy(0) * B.half_extent()(0);
    V const bj_extent  = R_b.get_column_copy(1) * B.half_extent()(1);
    V const bk_extent  = R_b.get_column_copy(2) * B.half_extent()(2);

    for(size_t i=0u;i < 15u; ++i)
    {
      T const ae    = fabs(inner_prod( ai_extent, axes[i] ))
                    + fabs(inner_prod( aj_extent, axes[i] ))
                    + fabs(inner_prod( ak_extent, axes[i] ))
                    ;

      T const ac    = inner_prod( A.center(), axes[i] );
      T const a_min = ac - ae;
      T const a_max = ac + ae;

      T const be    = fabs(inner_prod( bi_extent, axes[i] ))
                    + fabs(inner_prod( bj_extent, axes[i] ))
                    + fabs(inner_prod( bk_extent, axes[i] ))
                    ;

      T const bc    = inner_prod( B.center(), axes[i] );
      T const b_min = bc - be;
      T const b_max = bc + be;

      assert( is_number( a_min ) || !"overlap_obb_obb(); nan");
      assert( is_finite( a_min ) || !"overlap_obb_obb(): inf");
      assert( is_number( a_max ) || !"overlap_obb_obb(); nan");
      assert( is_finite( a_max ) || !"overlap_obb_obb(): inf");

      assert( is_number( b_min ) || !"overlap_obb_obb(); nan");
      assert( is_finite( b_min ) || !"overlap_obb_obb(): inf");
      assert( is_number( b_max ) || !"overlap_obb_obb(); nan");
      assert( is_finite( b_max ) || !"overlap_obb_obb(): inf");

      if(a_max < b_min)
        return false;

      if(b_max < a_min)
        return false;
    }
    
    return true;
  }

} //namespace geometry

  // GEOMETRY_OVERLAP_OBB_OBB
#endif