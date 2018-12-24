#include <sparse.h>
#include <sparse_fill.h>
#include <prox_math_policy.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(math_policy_functions);
BOOST_AUTO_TEST_CASE(compute_z_test){
    
    typedef prox::MathPolicy<float>       math_policy;
    
    math_policy::vector4_type x(1),b(1),z;
    math_policy::diagonal4x4_type R(1,1,1);
    math_policy::compressed4x6_type J(1,1,1);
    math_policy::compressed6x4_type WJT(1,1,1);
    
    //filling the vectors
    x(0)[0] = 13;  x(0)[1] = 8;  x(0)[2] = 12;  x(0)[3] = 1;
    b(0)[0] = 16;  b(0)[1] = 5;  b(0)[2] = 9;  b(0)[3] = 4;
    
    //filling the matrices
    sparse::fill(R(0),1);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
    math_policy::block4x6_type & Ji = J(0,0);
    math_policy::real_type d = 3;
    for( size_t i =0; i<24; i++)
    {
        Ji[i] = d;    d++;
    }
    math_policy::block6x4_type & WJTi = WJT(0,0);
    d = 2;
    for( size_t i =0; i<24; i++)
    {
        WJTi[i] = d;    d++;
    }
    
    //computing z = x - R(J*WJ^T *x + b), results checked using matlab
    math_policy::compute_z(x,R,J,WJT,b,z);
    BOOST_CHECK_EQUAL(z(0)[0], -489006);
    BOOST_CHECK_EQUAL(z(0)[1], -1143883);
    BOOST_CHECK_EQUAL(z(0)[2], -1798751);
    BOOST_CHECK_EQUAL(z(0)[3], -2453634);
    
    //changing the input and recalculating, results checked using matlab
    sparse::fill(R(0),2);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
    math_policy::compute_z(x,R,J,WJT,b,z);
    BOOST_CHECK_EQUAL(z(0)[0], -652724);
    BOOST_CHECK_EQUAL(z(0)[1], -1307601);
    BOOST_CHECK_EQUAL(z(0)[2], -1962469);
    BOOST_CHECK_EQUAL(z(0)[3], -2617352);
    
    //changing the input and recalculating, results checked using matlab
    d = 5;
    for( size_t i =0; i<24; i++)
    {
        WJTi[i] = d;
        d++;
    }
    math_policy::compute_z(x,R,J,WJT,b,z);
    BOOST_CHECK_EQUAL(z(0)[0], -795320);
    BOOST_CHECK_EQUAL(z(0)[1], -1592181);
    BOOST_CHECK_EQUAL(z(0)[2], -2389033);
    BOOST_CHECK_EQUAL(z(0)[3], -3185900);
    
    
}

BOOST_AUTO_TEST_CASE(compute_norm_inf_test)
{
    typedef prox::MathPolicy<float> math_policy;
    
    math_policy::vector4_type x(1);
    
    //filling the vector with values 0-3
    sparse::fill(x(0));// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
    math_policy::real_type out = math_policy::compute_norm_inf(x);
    //checking that max value is 3
    BOOST_CHECK_EQUAL(out , math_policy::value_traits::numeric_cast(3));
    
    //resizing to check function works for multiple blocks
    x.resize(3);
    
    //assigning a large value to check that the multiple blocks are traversed
    x(1)(3) = math_policy::value_traits::numeric_cast(59);
    out = math_policy::compute_norm_inf(x);
    BOOST_CHECK_EQUAL(out , math_policy::value_traits::numeric_cast(59));
    
    //assigning a large value to check that the multiple blocks are traversed
    x(2)(3) = math_policy::value_traits::numeric_cast(432);
    out = math_policy::compute_norm_inf(x);
    BOOST_CHECK_EQUAL(out , math_policy::value_traits::numeric_cast(432));
    
    //assigning a large value to check that the multiple blocks are traversed
    //checking that negative values are considered
    x(0)(3) = math_policy::value_traits::numeric_cast(-1432);
    out = math_policy::compute_norm_inf(x);
    BOOST_CHECK_EQUAL(out , math_policy::value_traits::numeric_cast(1432));
}

BOOST_AUTO_TEST_CASE(compute_wjt_test)
{
    typedef prox::MathPolicy<float> math_policy;
    typedef math_policy::diagonal6x6_type   diagonal6x6_type;
    typedef math_policy::compressed4x6_type compressed4x6_type;
    typedef math_policy::compressed6x4_type compressed6x4_type;
    
    diagonal6x6_type   W(1);
    compressed4x6_type J(1,1,1);
    compressed6x4_type JT(1,1,1);
    compressed6x4_type WJT(1,1,1);
    
    sparse::fill(W(0));// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
    sparse::fill(J(0,0),3);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
    sparse::fill(JT(0,0),4);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
    
    math_policy::compute_WJT(W,J,WJT);
    BOOST_CHECK_EQUAL(WJT(0,0)[0],100);
    BOOST_CHECK_EQUAL(WJT(0,0)[1],190);
    BOOST_CHECK_EQUAL(WJT(0,0)[2],280);
    BOOST_CHECK_EQUAL(WJT(0,0)[3],370);
    BOOST_CHECK_EQUAL(WJT(0,0)[4],298);
    BOOST_CHECK_EQUAL(WJT(0,0)[5],604);
    BOOST_CHECK_EQUAL(WJT(0,0)[6],910);
    BOOST_CHECK_EQUAL(WJT(0,0)[7],1216);
    BOOST_CHECK_EQUAL(WJT(0,0)[8],496);
    BOOST_CHECK_EQUAL(WJT(0,0)[9],1018);
    BOOST_CHECK_EQUAL(WJT(0,0)[10],1540);
    BOOST_CHECK_EQUAL(WJT(0,0)[11],2062);
    BOOST_CHECK_EQUAL(WJT(0,0)[12],694);
    BOOST_CHECK_EQUAL(WJT(0,0)[13],1432);
    BOOST_CHECK_EQUAL(WJT(0,0)[14],2170);
    BOOST_CHECK_EQUAL(WJT(0,0)[15],2908);
    BOOST_CHECK_EQUAL(WJT(0,0)[16],892);
    BOOST_CHECK_EQUAL(WJT(0,0)[17],1846);
    BOOST_CHECK_EQUAL(WJT(0,0)[18],2800);
    BOOST_CHECK_EQUAL(WJT(0,0)[19],3754);
    BOOST_CHECK_EQUAL(WJT(0,0)[20],1090);
    BOOST_CHECK_EQUAL(WJT(0,0)[21],2260);
    BOOST_CHECK_EQUAL(WJT(0,0)[22],3430);
    BOOST_CHECK_EQUAL(WJT(0,0)[23],4600);
    
    
    WJT.clear();
    math_policy::compute_WJT(W,JT,WJT);
    BOOST_CHECK_EQUAL(WJT(0,0)[0],280);
    BOOST_CHECK_EQUAL(WJT(0,0)[1],295);
    BOOST_CHECK_EQUAL(WJT(0,0)[2],310);
    BOOST_CHECK_EQUAL(WJT(0,0)[3],325);
    BOOST_CHECK_EQUAL(WJT(0,0)[4],784);
    BOOST_CHECK_EQUAL(WJT(0,0)[5],835);
    BOOST_CHECK_EQUAL(WJT(0,0)[6],886);
    BOOST_CHECK_EQUAL(WJT(0,0)[7],937);
    BOOST_CHECK_EQUAL(WJT(0,0)[8],1288);
    BOOST_CHECK_EQUAL(WJT(0,0)[9],1375);
    BOOST_CHECK_EQUAL(WJT(0,0)[10],1462);
    BOOST_CHECK_EQUAL(WJT(0,0)[11],1549);
    BOOST_CHECK_EQUAL(WJT(0,0)[12],1792);
    BOOST_CHECK_EQUAL(WJT(0,0)[13],1915);
    BOOST_CHECK_EQUAL(WJT(0,0)[14],2038);
    BOOST_CHECK_EQUAL(WJT(0,0)[15],2161);
    BOOST_CHECK_EQUAL(WJT(0,0)[16],2296);
    BOOST_CHECK_EQUAL(WJT(0,0)[17],2455);
    BOOST_CHECK_EQUAL(WJT(0,0)[18],2614);
    BOOST_CHECK_EQUAL(WJT(0,0)[19],2773);
    BOOST_CHECK_EQUAL(WJT(0,0)[20],2800);
    BOOST_CHECK_EQUAL(WJT(0,0)[21],2995);
    BOOST_CHECK_EQUAL(WJT(0,0)[22],3190);
    BOOST_CHECK_EQUAL(WJT(0,0)[23],3385);
    
    
}

BOOST_AUTO_TEST_CASE(compute_prod_test)
{
    typedef prox::MathPolicy<float>         math_policy;
    typedef math_policy::vector6_type       vector6_type;
    typedef math_policy::vector4_type       vector4_type;
    typedef math_policy::compressed6x4_type compressed6x4_type;
    typedef math_policy::diagonal4x4_type   diagonal4x4_type;
    
    diagonal4x4_type A(1), B(1);
    compressed6x4_type C(1,1,1);
    vector4_type D(1);
    vector6_type E(1);
/*    
    sparse::fill(J(0,0),1.0f);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
    sparse::fill(e(0),2.0f);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
    
    BOOST_CHECK_EQUAL(e(0)[0],2.0f);
    BOOST_CHECK_EQUAL(e(0)[1],3.0f);
    BOOST_CHECK_EQUAL(e(0)[2],4.0f);
    BOOST_CHECK_EQUAL(e(0)[3],5.0f);
    
    sparse::fill(Wdth(0),1.0f);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
    sparse::prod(2.0f,e,g);
    sparse::prod(2.0f,Wdth,u);
    
    math_policy::compute_b(J,Wdth,u,e,g,b);
    
    //checked by matlab
    BOOST_CHECK_EQUAL(b(0)[0],641);
    BOOST_CHECK_EQUAL(b(0)[1],1959);
    BOOST_CHECK_EQUAL(b(0)[2],3781);
    BOOST_CHECK_EQUAL(b(0)[3],6107);
    
    // changing the input slightly
    sparse::fill(J(0,0),3.0f);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
    math_policy::compute_b(J,Wdth,u,e,g,b);
    
    BOOST_CHECK_EQUAL(b(0)[0],935);
    BOOST_CHECK_EQUAL(b(0)[1],2337);
    BOOST_CHECK_EQUAL(b(0)[2],4243);
    BOOST_CHECK_EQUAL(b(0)[3],6653);
*/    
}

BOOST_AUTO_TEST_CASE(compute_b_test)
{
    typedef prox::MathPolicy<float>         math_policy;
    typedef math_policy::vector6_type       vector6_type;
    typedef math_policy::vector4_type       vector4_type;
    typedef math_policy::compressed4x6_type compressed4x6_type;
    
    compressed4x6_type J(1,1,1);
    vector4_type e(1), g(1), b(1);
    vector6_type Wdth(1), u(1);
    
    sparse::fill(J(0,0),1.0f);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
    sparse::fill(e(0),2.0f);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
    
    BOOST_CHECK_EQUAL(e(0)[0],2.0f);
    BOOST_CHECK_EQUAL(e(0)[1],3.0f);
    BOOST_CHECK_EQUAL(e(0)[2],4.0f);
    BOOST_CHECK_EQUAL(e(0)[3],5.0f);
    
    sparse::fill(Wdth(0),1.0f);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
    sparse::prod(2.0f,e,g);
    sparse::prod(2.0f,Wdth,u);
    
    math_policy::compute_b(J,Wdth,u,e,g,b);
    
    //checked by matlab
    BOOST_CHECK_EQUAL(b(0)[0],641);
    BOOST_CHECK_EQUAL(b(0)[1],1959);
    BOOST_CHECK_EQUAL(b(0)[2],3781);
    BOOST_CHECK_EQUAL(b(0)[3],6107);
    
    // changing the input slightly
    sparse::fill(J(0,0),3.0f);// 2009-09-20 Kenny:  error:`fill' is not a member of 'sparse'
    math_policy::compute_b(J,Wdth,u,e,g,b);
    
    BOOST_CHECK_EQUAL(b(0)[0],935);
    BOOST_CHECK_EQUAL(b(0)[1],2337);
    BOOST_CHECK_EQUAL(b(0)[2],4243);
    BOOST_CHECK_EQUAL(b(0)[3],6653);
    
}

BOOST_AUTO_TEST_CASE(compute_z_k_test){
    
    // note that the test is not a realistic usage example
    
    typedef prox::MathPolicy<float>       math_policy;
    
    math_policy::vector4_type x(1), b(1), z_k(1);
    math_policy::vector6_type w(1);
    math_policy::diagonal4x4_type R(1,1,1);
    math_policy::compressed4x6_type J(1,1,1);
    math_policy::compressed6x4_type WJT(1,1,1);
    
    //filling the vectors
    x(0)[0] = 13;  x(0)[1] = 8;  x(0)[2] = 12; x(0)[3] = 1;
    b(0)[0] = 16;  b(0)[1] = 5;  b(0)[2] = 9;  b(0)[3] = 4;
    
    //filling the matrices
    sparse::fill(R(0),1);
    sparse::fill(J(0,0),3);
    sparse::fill(WJT(0,0),2);
    
    //computing z_k = x_k - R_kk ( J w + b_k )
    w(0)[0] = 0;   w(0)[1] = 0;  w(0)[2] = 0;  w(0)[3] = 0; w(0)[4] = 0;  w(0)[5] = 0;
    sparse::prod(WJT,x,w);
    math_policy::compute_z_k(x(0), w, R(0), J, b(0), z_k(0), 0);
    math_policy::compute_z(x, R, J, WJT, b, z_k);
    BOOST_CHECK_EQUAL(z_k(0)[0], -489006);
    BOOST_CHECK_EQUAL(z_k(0)[1], -1143883);
    BOOST_CHECK_EQUAL(z_k(0)[2], -1798751);
    BOOST_CHECK_EQUAL(z_k(0)[3], -2453634);
    
    //changing the input and recalculating, results checked using matlab
    sparse::fill(R(0),2);
    w(0)[0] = 0;   w(0)[1] = 0;  w(0)[2] = 0;  w(0)[3] = 0; w(0)[4] = 0;  w(0)[5] = 0;
    sparse::prod(WJT,x,w);
    math_policy::compute_z_k(x(0), w, R(0), J, b(0), z_k(0), 0);
    BOOST_CHECK_EQUAL(z_k(0)[0], -652724);
    BOOST_CHECK_EQUAL(z_k(0)[1], -1307601);
    BOOST_CHECK_EQUAL(z_k(0)[2], -1962469);
    BOOST_CHECK_EQUAL(z_k(0)[3], -2617352);
    
    //changing the input and recalculating, results checked using matlab
    sparse::fill(WJT(0,0),5);
    w(0)[0] = 0;   w(0)[1] = 0;  w(0)[2] = 0;  w(0)[3] = 0; w(0)[4] = 0;  w(0)[5] = 0;
    sparse::prod(WJT,x,w);
    math_policy::compute_z_k(x(0), w, R(0), J, b(0), z_k(0), 0);
    BOOST_CHECK_EQUAL(z_k(0)[0], -795320);
    BOOST_CHECK_EQUAL(z_k(0)[1], -1592181);
    BOOST_CHECK_EQUAL(z_k(0)[2], -2389033);
    BOOST_CHECK_EQUAL(z_k(0)[3], -3185900);
    
    
}
BOOST_AUTO_TEST_SUITE_END();