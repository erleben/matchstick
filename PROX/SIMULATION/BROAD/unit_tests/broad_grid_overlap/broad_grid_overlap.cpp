#include <broad.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

template< typename T>
class MyObject : public broad::Object<T> 
{
protected:
  
  T m_mx;
  T m_my;
  T m_mz;
  T m_Mx;
  T m_My;
  T m_Mz;
  
public:
    
  void set_box(T const & mx,T const & my,T const & mz,T const & Mx,T const & My,T const & Mz)
  {
    this->m_mx = mx;
    this->m_my = my;
    this->m_mz = mz;
    this->m_Mx = Mx;
    this->m_My = My;
    this->m_Mz = Mz;
  }

  void get_box(T & mx,T & my,T & mz,T & Mx,T & My,T & Mz) const 
  {
    mx = this->m_mx;
    my = this->m_my;
    mz = this->m_mz;
    Mx = this->m_Mx;
    My = this->m_My;
    Mz = this->m_Mz;
  }

};

typedef broad::Object<float>                              base_object_type;
typedef MyObject<float>                                   object_type;
typedef broad::System<float>                              system_type;
typedef std::pair< base_object_type*, base_object_type* > overlap_type;
typedef std::vector< overlap_type  >                        overlap_container;

/**
 * This function tests if the specified pair of objects are reported uniquely as an overlap.
 */
inline bool exist_unique_overlap( object_type const & A, object_type const & B, overlap_container const & O)
{
  size_t count = 0;
  
  base_object_type const * a =  &A;
  base_object_type const * b =  &B;
  
  for( overlap_container::const_iterator o = O.begin(); o != O.end(); ++o)
  {
    BOOST_CHECK( o->first < o->second );
    if( o->first == a && o->second == b)
      count++;
    if( o->first == b && o->second == a)
      count++;
  }
  return count==1u;
}

BOOST_AUTO_TEST_SUITE(broad);

BOOST_AUTO_TEST_CASE(all_pair_overlap_test)
{	
  system_type S;

  object_type O1;
  object_type O2;
  object_type O3;
    
  O1.set_box( -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f);
  O2.set_box( -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f);
  O3.set_box( -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f);
  
  
  S.connect( &O1 );
  S.connect( &O2 );
  S.connect( &O3 );
  
  overlap_container overlaps;
  float efficiency = 0.0f;
  broad::find_overlaps( S, overlaps, efficiency, broad::grid_algorithm() );
  
  BOOST_CHECK( overlaps.size() == 3u );

  BOOST_CHECK( exist_unique_overlap( O1, O2, overlaps) );
  BOOST_CHECK( exist_unique_overlap( O1, O3, overlaps) );
  BOOST_CHECK( exist_unique_overlap( O2, O3, overlaps) );
  
  BOOST_CHECK(  !exist_unique_overlap( O1, O1, overlaps) );
  BOOST_CHECK(  !exist_unique_overlap( O2, O2, overlaps) );
  BOOST_CHECK(  !exist_unique_overlap( O3, O3, overlaps) );
  
}

BOOST_AUTO_TEST_CASE(no_overlap_test)
{	
  system_type S;
  
  object_type O1;
  object_type O2;
  object_type O3;
  
  O1.set_box( 
             -10.0f, -1.0f, -1.0f,
              -9.0f, 1.0f, 1.0f
             );
  O2.set_box( 
             -8.0f, -1.0f, -1.0f,
             -7.0f, 1.0f, 1.0f
             );
  O3.set_box( 
             -6.0f, -1.0f, -1.0f,
             -5.0f, 1.0f, 1.0f
             );
  
  
  S.connect( &O1 );
  S.connect( &O2 );
  S.connect( &O3 );
  
  overlap_container overlaps;
  float efficiency = 0.0f;
  broad::find_overlaps( S, overlaps, efficiency, broad::grid_algorithm() );
  
  BOOST_CHECK( overlaps.size() == 0u );

  BOOST_CHECK(  !exist_unique_overlap( O1, O1, overlaps) );
  BOOST_CHECK(  !exist_unique_overlap( O2, O2, overlaps) );
  BOOST_CHECK(  !exist_unique_overlap( O3, O3, overlaps) );
    
  BOOST_CHECK( ! exist_unique_overlap( O1, O2, overlaps) );
  BOOST_CHECK( ! exist_unique_overlap( O1, O3, overlaps) );
  BOOST_CHECK( ! exist_unique_overlap( O2, O3, overlaps) );
}

BOOST_AUTO_TEST_CASE(large_aspect_ratio_test)
{	
  system_type S;
  
  object_type O1;
  object_type O2;
  object_type O3;
  
  O1.set_box( 
             -10.0f, -1.0f, -1.0f,
              10.0f, 1.0f, 1.0f
             );
  O2.set_box( 
             -1.0f, -10.0f, -1.0f,
              1.0f,  10.0f, 1.0f
             );
  
  O3.set_box( 
             10.0f, 10.0f, -1.0f,
             11.0f, 11.0f, 1.0f
             );
  
  S.connect( &O1 );
  S.connect( &O2 );
  S.connect( &O3 );
  
  overlap_container overlaps;
  float efficiency = 0.0f;
  broad::find_overlaps( S, overlaps, efficiency, broad::grid_algorithm() );

  BOOST_CHECK( overlaps.size() == 1u );

  BOOST_CHECK(  !exist_unique_overlap( O1, O1, overlaps) );
  BOOST_CHECK(  !exist_unique_overlap( O2, O2, overlaps) );
  BOOST_CHECK(  !exist_unique_overlap( O3, O3, overlaps) );
  
  BOOST_CHECK(   exist_unique_overlap( O1, O2, overlaps) );
  BOOST_CHECK( ! exist_unique_overlap( O1, O3, overlaps) );
  BOOST_CHECK( ! exist_unique_overlap( O2, O3, overlaps) );
}

BOOST_AUTO_TEST_CASE(touching_contact_test)
{	
  system_type S;
  
  object_type O1;
  object_type O2;
  object_type O3;
  object_type O4;
  
  O1.set_box( 
             -1.0f, -1.0f, -1.0f,
              1.0f, 1.0f, 1.0f
             );
  O2.set_box( 
             1.0f, -1.0f, -1.0f,
             2.0f,  1.0f, 1.0f
             );
  
  O3.set_box( 
             -1.0f,  1.0f, -1.0f,
              1.0f,  2.0f,  1.0f
             );

  O4.set_box( 
             -1.0f, -1.0f, 1.0f,
             1.0f,  1.0f,  2.0f
             );
  
  
  S.connect( &O1 );
  S.connect( &O2 );
  S.connect( &O3 );
  S.connect( &O4 );
  
  overlap_container overlaps;
  float efficiency = 0.0f;
  broad::find_overlaps( S, overlaps, efficiency, broad::grid_algorithm() );

  BOOST_CHECK(  !exist_unique_overlap( O1, O1, overlaps) );
  BOOST_CHECK(  !exist_unique_overlap( O2, O2, overlaps) );
  BOOST_CHECK(  !exist_unique_overlap( O3, O3, overlaps) );
  BOOST_CHECK(  !exist_unique_overlap( O4, O4, overlaps) );

  BOOST_CHECK(  exist_unique_overlap( O1, O2, overlaps) );
  BOOST_CHECK(  exist_unique_overlap( O1, O3, overlaps) );
  BOOST_CHECK(  exist_unique_overlap( O1, O4, overlaps) );
  BOOST_CHECK(  exist_unique_overlap( O2, O3, overlaps) );
  BOOST_CHECK(  exist_unique_overlap( O2, O4, overlaps) );
  BOOST_CHECK(  exist_unique_overlap( O3, O4, overlaps) );
  
  BOOST_CHECK( overlaps.size() == 6u );
}

BOOST_AUTO_TEST_CASE(inclusion_test)
{	
  system_type S;
  
  object_type O1;
  object_type O2;
  object_type O3;
  
  O1.set_box( 
             -1.0f, -1.0f, -1.0f,
             1.0f, 1.0f, 1.0f
             );
  O2.set_box( 
             -2.0f, -2.0f, -2.0f,
              2.0f,  2.0f, 2.0f
             );
  
  O3.set_box( 
             5.0f,  5.0f, -1.0f,
             6.0f,  6.0f,  1.0f
             );
  
  
  
  S.connect( &O1 );
  S.connect( &O2 );
  S.connect( &O3 );
  
  overlap_container overlaps;
  float efficiency = 0.0f;
  broad::find_overlaps( S, overlaps, efficiency, broad::grid_algorithm() );
  
  BOOST_CHECK(  !exist_unique_overlap( O1, O1, overlaps) );
  BOOST_CHECK(  !exist_unique_overlap( O2, O2, overlaps) );
  BOOST_CHECK(  !exist_unique_overlap( O3, O3, overlaps) );
  BOOST_CHECK(   exist_unique_overlap( O1, O2, overlaps) );
  BOOST_CHECK(  !exist_unique_overlap( O1, O3, overlaps) );
  BOOST_CHECK(  !exist_unique_overlap( O2, O3, overlaps) );
  
  BOOST_CHECK( overlaps.size() == 1u );
}

BOOST_AUTO_TEST_SUITE_END();
