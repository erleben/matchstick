#include <sparse.h>
#include <prox_math_policy.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(mass_block_jac);
BOOST_AUTO_TEST_CASE(mass_block_test_case)
{
    typedef prox::MathPolicy<float>       math_policy;
    typedef math_policy::mass_block_type	 Mass_block;
    typedef math_policy::value_traits     value_traits;
    typedef math_policy::real_type        real_type;
        
    Mass_block b(0); // zero-init
    // size checks
    BOOST_CHECK_EQUAL( b.nrows() , 6u);
    BOOST_CHECK_EQUAL( b.ncols() , 6u); 
    BOOST_CHECK_EQUAL( b.size()  , 7u);

    // is data initialised to float(0)
    BOOST_CHECK_EQUAL(std::find_if(b.begin(), b.end(),bind2nd(std::not_equal_to<real_type>(), 0)) , b.end());

    b(0,0) = value_traits::half(); // should make m = b[0] == 0.5
    BOOST_CHECK_EQUAL(b[0] , value_traits::half());

    b(1,1) = value_traits::numeric_cast(1.5); // should make m = b[0] == 1.5
    BOOST_CHECK_EQUAL(b[0] , value_traits::numeric_cast(1.5));
    BOOST_CHECK_EQUAL(b(2,2) , b(0,0));//should be same as b(0,0) && b(1,1)
    BOOST_CHECK_EQUAL(b(2,2) , b(1,1));//should be same as b(0,0) && b(1,1)

    Mass_block b_copy(b);
    BOOST_CHECK_EQUAL(b[0] , b_copy[0]);
    BOOST_CHECK_EQUAL(b[1] , b_copy[1]);
    BOOST_CHECK_EQUAL(b[2] , b_copy[2]);
    BOOST_CHECK_EQUAL(b[3] , b_copy[3]);
    BOOST_CHECK_EQUAL(b[4] , b_copy[4]);
    BOOST_CHECK_EQUAL(b[5] , b_copy[5]);
    BOOST_CHECK_EQUAL(b[6] , b_copy[6]);

    b_copy(3,3) = value_traits::numeric_cast(0.5);
    b_copy(4,4) = value_traits::numeric_cast(2.5);
    b_copy(4,5) = value_traits::numeric_cast(3.5);
    b_copy(3,4) = value_traits::numeric_cast(4.5);
    b_copy(5,4) = value_traits::numeric_cast(5.5);
    b_copy(3,5) = value_traits::numeric_cast(6.5);
    b_copy(5,5) = value_traits::numeric_cast(7.5);

    BOOST_CHECK_EQUAL(b_copy(4,3), b_copy(3,4));// by symmetry
    BOOST_CHECK_EQUAL(b_copy(4,3), value_traits::numeric_cast(4.5)); 
    BOOST_CHECK_EQUAL(b_copy(4,5), b_copy(5,4)); // by symmetry
    BOOST_CHECK_EQUAL(b_copy(4,5), value_traits::numeric_cast(5.5)); 
    BOOST_CHECK_EQUAL(b_copy(5,3), b_copy(3,5)); // by symmetry
    BOOST_CHECK_EQUAL(b_copy(3,5), value_traits::numeric_cast(6.5)); 

    b = b_copy;

    BOOST_CHECK_EQUAL(b[0] , value_traits::numeric_cast(1.5));
    BOOST_CHECK_EQUAL(b[1] , value_traits::numeric_cast(0.5));
    BOOST_CHECK_EQUAL(b[2] , value_traits::numeric_cast(4.5));
    BOOST_CHECK_EQUAL(b[3] , value_traits::numeric_cast(6.5));
    BOOST_CHECK_EQUAL(b[4] , value_traits::numeric_cast(2.5));
    BOOST_CHECK_EQUAL(b[5] , value_traits::numeric_cast(5.5));
    BOOST_CHECK_EQUAL(b[6] , value_traits::numeric_cast(7.5));
 
}
BOOST_AUTO_TEST_SUITE_END();
