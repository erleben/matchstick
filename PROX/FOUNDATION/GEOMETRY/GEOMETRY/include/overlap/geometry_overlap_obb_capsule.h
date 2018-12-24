#ifndef GEOMETRY_OVERLAP_OBB_CAPSULE
#define GEOMETRY_OVERLAP_OBB_CAPSULE

#include <types/geometry_obb.h>
#include <types/geometry_capsule.h>


#include <geometry_transform.h>


#include <tiny.h>

#include <cmath>
#include <vector>

namespace geometry
{
  namespace detail
  {

    template<typename MT>
    inline void compute_obb_capsule_sat_axes(
                                             OBB<MT> const & obb
                                             , Capsule<typename MT::vector3_type> const & capsule
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

      M const R = tiny::make(obb.orientation());

      V const A0  = R.get_column_copy(0);
      V const A1  = R.get_column_copy(1);
      V const A2  = R.get_column_copy(2);
      V const B   = capsule.point1()- capsule.point0();

      assert( fabs( VT::one() - inner_prod(A0,A0) ) < tiny::working_precision<T>() || !"compute_obb_capsule_sat_axes(): logic error");
      assert( fabs( VT::one() - inner_prod(A1,A1) ) < tiny::working_precision<T>() || !"compute_obb_capsule_sat_axes(): logic error");
      assert( fabs( VT::one() - inner_prod(A2,A2) ) < tiny::working_precision<T>() || !"compute_obb_capsule_sat_axes(): logic error");
      assert( fabs( inner_prod(A0,A1) ) < tiny::working_precision<T>()             || !"compute_obb_capsule_sat_axes(): logic error");
      assert( fabs( inner_prod(A0,A2) ) < tiny::working_precision<T>()             || !"compute_obb_capsule_sat_axes(): logic error");
      assert( fabs( inner_prod(A1,A2) ) < tiny::working_precision<T>()             || !"compute_obb_capsule_sat_axes(): logic error");

      axes.resize(7u);

      axes[0] = A0;
      axes[1] = A1;
      axes[2] = A2;
      axes[3] = B;
      axes[4] = tiny::cross(A0, B);
      axes[5] = tiny::cross(A1, B);
      axes[6] = tiny::cross(A2, B);

      T const l3 = tiny::norm( axes[3] );
      T const l4 = tiny::norm( axes[4] );
      T const l5 = tiny::norm( axes[5] );
      T const l6 = tiny::norm( axes[6] );

      axes[3]    = (l3 > tiny::working_precision<T>() ) ? axes[3] / l3 : B;
      axes[4]    = (l4 > tiny::working_precision<T>() ) ? axes[4] / l4 : A0;
      axes[5]    = (l5 > tiny::working_precision<T>() ) ? axes[5] / l5 : A1;
      axes[6]    = (l6 > tiny::working_precision<T>() ) ? axes[6] / l6 : A2;

      assert( is_number( axes[0](0) ) || !"compute_obb_capsule_sat_axes(): nan");
      assert( is_finite( axes[0](0) ) || !"compute_obb_capsule_sat_axes(): inf");
      assert( is_number( axes[0](1) ) || !"compute_obb_capsule_sat_axes(): nan");
      assert( is_finite( axes[0](1) ) || !"compute_obb_capsule_sat_axes(): inf");
      assert( is_number( axes[0](2) ) || !"compute_obb_capsule_sat_axes(): nan");
      assert( is_finite( axes[0](2) ) || !"compute_obb_capsule_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[0],axes[0]) ) < tiny::working_precision<T>() || !"compute_obb_capsule_sat_axes(): logic error");

      assert( is_number( axes[1](0) ) || !"compute_obb_capsule_sat_axes(): nan");
      assert( is_finite( axes[1](0) ) || !"compute_obb_capsule_sat_axes(): inf");
      assert( is_number( axes[1](1) ) || !"compute_obb_capsule_sat_axes(): nan");
      assert( is_finite( axes[1](1) ) || !"compute_obb_capsule_sat_axes(): inf");
      assert( is_number( axes[1](2) ) || !"compute_obb_capsule_sat_axes(): nan");
      assert( is_finite( axes[1](2) ) || !"compute_obb_capsule_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[1],axes[1]) ) < tiny::working_precision<T>() || !"compute_obb_capsule_sat_axes(): logic error");

      assert( is_number( axes[2](0) ) || !"compute_obb_capsule_sat_axes(): nan");
      assert( is_finite( axes[2](0) ) || !"compute_obb_capsule_sat_axes(): inf");
      assert( is_number( axes[2](1) ) || !"compute_obb_capsule_sat_axes(): nan");
      assert( is_finite( axes[2](1) ) || !"compute_obb_capsule_sat_axes(): inf");
      assert( is_number( axes[2](2) ) || !"compute_obb_capsule_sat_axes(): nan");
      assert( is_finite( axes[2](2) ) || !"compute_obb_capsule_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[2],axes[2]) ) < tiny::working_precision<T>() || !"compute_obb_capsule_sat_axes(): logic error");

      assert( is_number( axes[3](0) ) || !"compute_obb_capsule_sat_axes(): nan");
      assert( is_finite( axes[3](0) ) || !"compute_obb_capsule_sat_axes(): inf");
      assert( is_number( axes[3](1) ) || !"compute_obb_capsule_sat_axes(): nan");
      assert( is_finite( axes[3](1) ) || !"compute_obb_capsule_sat_axes(): inf");
      assert( is_number( axes[3](2) ) || !"compute_obb_capsule_sat_axes(): nan");
      assert( is_finite( axes[3](2) ) || !"compute_obb_capsule_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[3],axes[3]) ) < tiny::working_precision<T>() || !"compute_obb_capsule_sat_axes(): logic error");

      assert( is_number( axes[4](0) ) || !"compute_obb_capsule_sat_axes(): nan");
      assert( is_finite( axes[4](0) ) || !"compute_obb_capsule_sat_axes(): inf");
      assert( is_number( axes[4](1) ) || !"compute_obb_capsule_sat_axes(): nan");
      assert( is_finite( axes[4](1) ) || !"compute_obb_capsule_sat_axes(): inf");
      assert( is_number( axes[4](2) ) || !"compute_obb_capsule_sat_axes(): nan");
      assert( is_finite( axes[4](2) ) || !"compute_obb_capsule_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[4],axes[4]) ) < tiny::working_precision<T>() || !"compute_obb_capsule_sat_axes(): logic error");

      assert( is_number( axes[5](0) ) || !"compute_obb_capsule_sat_axes(): nan");
      assert( is_finite( axes[5](0) ) || !"compute_obb_capsule_sat_axes(): inf");
      assert( is_number( axes[5](1) ) || !"compute_obb_capsule_sat_axes(): nan");
      assert( is_finite( axes[5](1) ) || !"compute_obb_capsule_sat_axes(): inf");
      assert( is_number( axes[5](2) ) || !"compute_obb_capsule_sat_axes(): nan");
      assert( is_finite( axes[5](2) ) || !"compute_obb_capsule_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[5],axes[5]) ) < tiny::working_precision<T>() || !"compute_obb_capsule_sat_axes(): logic error");

      assert( is_number( axes[6](0) ) || !"compute_obb_capsule_sat_axes(): nan");
      assert( is_finite( axes[6](0) ) || !"compute_obb_capsule_sat_axes(): inf");
      assert( is_number( axes[6](1) ) || !"compute_obb_capsule_sat_axes(): nan");
      assert( is_finite( axes[6](1) ) || !"compute_obb_capsule_sat_axes(): inf");
      assert( is_number( axes[6](2) ) || !"compute_obb_capsule_sat_axes(): nan");
      assert( is_finite( axes[6](2) ) || !"compute_obb_capsule_sat_axes(): inf");
      assert( fabs( VT::one() - inner_prod(axes[6],axes[6]) ) < tiny::working_precision<T>() || !"compute_obb_capsule_sat_axes(): logic error");
    }
    
  }// end namespace detail

  /**
   *
   */
  template<typename MT>
  inline bool overlap_obb_capsule(
                                OBB<MT> const & obb
                              , Capsule<typename MT::vector3_type> const & capsule
                              )
  {
    using std::min;
    using std::max;
    using std::fabs;

    typedef typename MT::vector3_type    V;
    typedef typename MT::real_type       T;
    typedef typename MT::value_traits    VT;

    std::vector<V> axes;

    detail::compute_obb_capsule_sat_axes( obb, capsule, axes );

    std::vector<V> a(8u, V::zero());
    a[0] = transform_from_obb( get_local_corner(0, obb), obb );
    a[1] = transform_from_obb( get_local_corner(1, obb), obb );
    a[2] = transform_from_obb( get_local_corner(2, obb), obb );
    a[3] = transform_from_obb( get_local_corner(3, obb), obb );
    a[4] = transform_from_obb( get_local_corner(4, obb), obb );
    a[5] = transform_from_obb( get_local_corner(5, obb), obb );
    a[6] = transform_from_obb( get_local_corner(6, obb), obb );
    a[7] = transform_from_obb( get_local_corner(7, obb), obb );

    std::vector<T> a_min( 7u, VT::highest() );
    std::vector<T> b_min( 7u, VT::highest() );
    std::vector<T> a_max( 7u, VT::lowest()  );
    std::vector<T> b_max( 7u, VT::lowest()  );

    T min_overlap = VT::lowest();

    for(size_t i=0u;i < 7u; ++i)
    {

      for( typename std::vector<V>::const_iterator p_a = a.begin(); p_a != a.end(); ++p_a)
      {
        T const d = inner_prod( (*p_a), axes[i] );
        a_min[i] = min( a_min[i], d);
        a_max[i] = max( a_max[i], d);
      }

      {
        T const d0 = inner_prod( capsule.point0(), axes[i] );
        T const d1 = inner_prod( capsule.point1(), axes[i] );

        b_min[i] = min( b_min[i], d0 - capsule.radius());
        b_max[i] = max( b_max[i], d0 + capsule.radius());

        b_min[i] = min( b_min[i], d1 - capsule.radius());
        b_max[i] = max( b_max[i], d1 + capsule.radius());
      }

      assert( is_number( a_min[i] ) || !"overlap_obb_capsule(); nan");
      assert( is_finite( a_min[i] ) || !"overlap_obb_capsule(): inf");
      assert( is_number( a_max[i] ) || !"overlap_obb_capsule(); nan");
      assert( is_finite( a_max[i] ) || !"overlap_obb_capsule(): inf");

      assert( is_number( b_min[i] ) || !"overlap_obb_capsule(); nan");
      assert( is_finite( b_min[i] ) || !"overlap_obb_capsule(): inf");
      assert( is_number( b_max[i] ) || !"overlap_obb_capsule(); nan");
      assert( is_finite( b_max[i] ) || !"overlap_obb_capsule(): inf");

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
//          n = axes[i];
        }
      }
      if(b_min[i] <= a_min[i] &&  a_min[i] <= b_max[i])
      {
        T const overlap = a_min[i] - b_max[i];
        if(overlap > min_overlap)
        {
          min_overlap = overlap;
//          n = -axes[i];
        }
      }
      assert( is_number( min_overlap ) || !"overlap_obb_capsule(); nan");
      assert( is_finite( min_overlap ) || !"overlap_obb_capsule(): inf");
    }
    
    return (min_overlap <= VT::zero());
  }

} //namespace geometry

  // GEOMETRY_OVERLAP_OBB_CAPSULE
#endif