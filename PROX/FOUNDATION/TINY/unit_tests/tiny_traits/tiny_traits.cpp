#include <tiny_type_traits.h>
#include <tiny_constants.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

template<size_t stride, typename type_traits, typename op_type, typename real_type>
void Verify (op_type _result, real_type value)
{
	real_type result[stride];
	type_traits::store_op_type(result,_result);
	real_type tol = real_type(0.1); // 1/10 percent
	for (size_t i=0; i<stride;++i)
	  {	
	    BOOST_CHECK_CLOSE(result[i],value,tol); 
	  }
}


template <typename type_traits>
void test_traits()
{	
  using namespace tiny;
  
	typedef typename type_traits::real_type	real_type;
	typedef typename type_traits::op_type 	op_type;
	enum {stride = type_traits::stride};

	BOOST_REQUIRE(stride>0); // Types needs to contain at least one element.
	
	real_type const zero 	= detail::zero<real_type>();
	real_type const one  	= detail::one<real_type>();
	real_type const two  	= detail::two<real_type>();
	real_type const four 	= detail::four<real_type>();
  //real_type const eight 	= detail::eight<real_type>();
  //real_type const sixteen	= eight+eight;
	real_type const quarter = one/four;
	real_type const half 	= one/two;
	
	real_type const _true  	= detail::one<real_type>();
	real_type const _false 	= detail::zero<real_type>();
	
	op_type const _zero 	= type_traits::set_op_type(zero); 
	op_type const _one  	= type_traits::set_op_type(one); 
	op_type const _two  	= type_traits::set_op_type(two);
	op_type const _four 	= type_traits::set_op_type(four);
	op_type const _quarter 	= type_traits::set_op_type(quarter);
	op_type const _half 	= type_traits::set_op_type(half);

	op_type const _mone 	= type_traits::set_op_type(-one);
	op_type const _mtwo 	= type_traits::set_op_type(-two);
	op_type const _mfour 	= type_traits::set_op_type(-four);
	op_type const  _mquarter	= type_traits::set_op_type(-quarter);
	op_type const  _mhalf 	= type_traits::set_op_type(-half);
	
	// Verify correctness of load & store
	Verify<stride,type_traits>(_one,one);
	
	// Verify correctness of assign
	op_type a = _one; 
	Verify<stride,type_traits>(a,one);

  op_type x;
	// Verify correctness of set_pad_value
	// TODO
	
	// Verify correctness of equal
	Verify<stride,type_traits>(type_traits::equal(_one,_one),_true);
	Verify<stride,type_traits>(type_traits::equal(_one,_zero),_false);
	
	// Verify correctness of not_equal
	Verify<stride,type_traits>(type_traits::not_equal(_one,_zero),_true);
	Verify<stride,type_traits>(type_traits::not_equal(_one,_one),_false);
	
	// Verify correctness of less_than
	Verify<stride,type_traits>(type_traits::less_than(_one,_zero),_false);
	Verify<stride,type_traits>(type_traits::less_than(_one,_one),_false);
	Verify<stride,type_traits>(type_traits::less_than(_zero,_one),_true);

	// Verify correctness of greater_than
	Verify<stride,type_traits>(type_traits::greater_than(_one,_zero),_true);
	Verify<stride,type_traits>(type_traits::greater_than(_one,_one),_false);
	Verify<stride,type_traits>(type_traits::greater_than(_zero,_one),_false);

	// Verify correctness of less_than_or_equal
	Verify<stride,type_traits>(type_traits::less_than_or_equal(_one,_zero),_false);
	Verify<stride,type_traits>(type_traits::less_than_or_equal(_one,_one),_true);
	Verify<stride,type_traits>(type_traits::less_than_or_equal(_zero,_one),_true);

	// Verify correctness of greater_than_or_equal
	Verify<stride,type_traits>(type_traits::greater_than_or_equal(_one,_zero),_true);
	Verify<stride,type_traits>(type_traits::greater_than_or_equal(_one,_one),_true);
	Verify<stride,type_traits>(type_traits::greater_than_or_equal(_zero,_one),_false);

	// Verify correctness of add_assign
	x = _one;
	Verify<stride,type_traits>(type_traits::add_assign(x,_one),two);
	Verify<stride,type_traits>(x,two); // Verify that x has been updated

	// Verify correctness of sub_assign
	x = _two;
	Verify<stride,type_traits>(type_traits::sub_assign(x,_one),one);
	Verify<stride,type_traits>(x,one);// Verify that x has been updated

	// Verify correctness of mul_assign
	x = _two;
	Verify<stride,type_traits>(type_traits::mul_assign(x,_two),four);
	Verify<stride,type_traits>(x,four);// Verify that x has been updated

	// Verify correctness of div_assign
	x = _four;
	Verify<stride,type_traits>(type_traits::div_assign(x,_two),two);
	Verify<stride,type_traits>(x,two);// Verify that x has been updated
	
	// Verify correctness of add
	Verify<stride,type_traits>(type_traits::add(_one,_zero),one);
	
	// Verify correctness of sub
	Verify<stride,type_traits>(type_traits::sub(_one,_one),zero);

	// Verify correctness of mul
	Verify<stride,type_traits>(type_traits::mul(_two,_two),four);

	// Verify correctness of div
	Verify<stride,type_traits>(type_traits::div(_four,_two),two);

	// Verify correctness of negate
	Verify<stride,type_traits>(type_traits::negate(_one),-one);
	Verify<stride,type_traits>(type_traits::negate(_mone),one);
	
	// Verify correctness of sign
	Verify<stride,type_traits>(type_traits::sign(_mtwo),-one);
	Verify<stride,type_traits>(type_traits::sign(_zero),zero);
	Verify<stride,type_traits>(type_traits::sign(_two),one);

	// Verify correctness of round
	Verify<stride,type_traits>(type_traits::round(_half),one);
	Verify<stride,type_traits>(type_traits::round(_quarter),zero);
	Verify<stride,type_traits>(type_traits::round(_mhalf),-one);
	Verify<stride,type_traits>(type_traits::round(_mquarter),zero);
	
	// Verify correctness of max_assign
	x = _zero;
	Verify<stride,type_traits>(type_traits::max_assign(x,_half),half);
	Verify<stride,type_traits>(x,half);
	x = _mfour;
	Verify<stride,type_traits>(type_traits::max_assign(x,_mtwo),-two);
	Verify<stride,type_traits>(x,-two);
	// Verify correctness of min_assign
	x = _zero;
	Verify<stride,type_traits>(type_traits::min_assign(x,_half),zero);
	Verify<stride,type_traits>(x,zero);
	x = _mfour;
	Verify<stride,type_traits>(type_traits::min_assign(x,_mtwo),-four);
	Verify<stride,type_traits>(x,-four);
	
	// Verify correctness of max
	Verify<stride,type_traits>(type_traits::max(_half,_zero),half);
	Verify<stride,type_traits>(type_traits::max(_mtwo,_mfour),-two);

	// Verify correctness of min
	Verify<stride,type_traits>(type_traits::min(_half,_zero),zero);
	Verify<stride,type_traits>(type_traits::min(_mtwo,_mfour),-four);

	// Verify correctness of abs
	Verify<stride,type_traits>(type_traits::abs(_one),one);
	Verify<stride,type_traits>(type_traits::abs(_mone),one);

	// Verify correctness of floor
	Verify<stride,type_traits>(type_traits::floor(_half),zero);
	Verify<stride,type_traits>(type_traits::floor(_mhalf),-one);

	// Verify correctness of ceil
	Verify<stride,type_traits>(type_traits::ceil(_half),one);
	Verify<stride,type_traits>(type_traits::ceil(_mhalf),zero);

	// Verify correctness of sqrt
	Verify<stride,type_traits>(type_traits::sqrt(_four),two);

	// Verify correctness of rsqrt
	Verify<stride,type_traits>(type_traits::rsqrt(_four),half);

	
	// TODO: For horizontal functions - consider cases where padding has been used

	real_type const tol = real_type(0.1);

	// Verify correctness of horizontal sum
	switch (stride)
	{
//	case 4:  // 2009-07-14 Kenny:  warning: case label value exceeds maximum value for type (for ScalarTrait) 
//	  BOOST_CHECK_CLOSE(type_traits::sum(_one),four,tol); 
//	  break;
	case 1:
	  BOOST_CHECK_CLOSE(type_traits::sum(_one),one,tol); 
	  break;
	default: break;
	}
	// Verify correctness of horizontal mul
	switch (stride)
	{
//	case 4:
//	  BOOST_CHECK_CLOSE(type_traits::mul(_two),sixteen,tol); 
//	  break;
	case 1:
	  BOOST_CHECK_CLOSE(type_traits::mul(_two),two,tol); 
	  break;
	default: break;
	}

	// Verify correctness of horizontal min and max
	// Prepare variables

	size_t const size = stride;
	real_type scalars[size];
	for (size_t i = 0 ; i<size;++i)
	{ 
		scalars[i]=tiny::detail::numeric_cast<real_type>(i+1);
	}
		
	x = type_traits::load_op_type(scalars);

	BOOST_CHECK(type_traits::min(x)==one);
	BOOST_CHECK(type_traits::max(x)==tiny::detail::numeric_cast<real_type>(size));
}


BOOST_AUTO_TEST_SUITE(tiny_traits);

BOOST_AUTO_TEST_CASE(default_traits_testing)
{
	
	typedef tiny::ScalarTraits<float>  float_traits;
	typedef tiny::ScalarTraits<double> double_traits;

	test_traits<float_traits>();
	test_traits<double_traits>();

}

BOOST_AUTO_TEST_SUITE_END();
