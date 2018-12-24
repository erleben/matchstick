#include <geometry.h>
#include <tiny.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>


template<typename V, size_t N>
void test_direction_table(geometry::DirectionTable<V,N> const & DT )
{
  BOOST_CHECK_EQUAL(DT.size(), N);
  
  for(size_t i=0u; i < N; ++i)
  {
    V const di = DT(i);
    
    BOOST_CHECK_CLOSE( inner_prod(di,di), 1.0f, 0.01f);
    
    for(size_t j=i+1u; j < N; ++j)
    {
      V const dj = DT(j);

      V diff = di-dj;
      
      BOOST_CHECK_GT( inner_prod(diff,diff) , 0.01f );
      
    }
  }
}


BOOST_AUTO_TEST_SUITE(geometry);

BOOST_AUTO_TEST_CASE(make_direction_tables)
{
  typedef tiny::MathTypes<float> MT;
  typedef MT::vector3_type       V;
  
  {
    geometry::DirectionTable<V,3> A = geometry::make3<V>();
    test_direction_table(A);
  }
  {
    geometry::DirectionTable<V,4> A = geometry::make4<V>();
    test_direction_table(A);
  }
  {
    geometry::DirectionTable<V,6> A = geometry::make6<V>();
    test_direction_table(A);
  }
  {
    geometry::DirectionTable<V,7> A = geometry::make7<V>();
    test_direction_table(A);
  }
  {
    geometry::DirectionTable<V,9> A = geometry::make9<V>();
    test_direction_table(A);
  }
  {
    geometry::DirectionTable<V,10> A = geometry::make10<V>();
    test_direction_table(A);
  }
  {
    geometry::DirectionTable<V,13> A = geometry::make13<V>();
    test_direction_table(A);
  }
}

BOOST_AUTO_TEST_SUITE_END();
