#include <tiny_prime_numbers.h>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

BOOST_AUTO_TEST_SUITE(tiny_prime_numbers);

BOOST_AUTO_TEST_CASE(simple_test)
{
  // Taken from http://en.wikipedia.org/wiki/List_of_prime_numbers
  int prime_table[] = { 3, 5, 7, 11, 13, 17, 19, 23, 31, 43, 61, 79, 101, 127, 167, 191, 199, 313, 347, 701, 1709, 2617, 3539, 5807, 10501, 10691, 11279, 12391, 14479, 42737, 83339, 95369, 117239, 127031, 138937, 141079, 267017, 269987, 374321 };
  
  for (int i=0;i<39;++i)
  {
    BOOST_CHECK( tiny::trial_division( prime_table[i] ) == true );
//    BOOST_CHECK( tiny::pseudo_prime( prime_table[i] ) == true );        // 2009-07-05 Kenny: Not always guaranteed to find a solution, hard to test in unit-test?
//    BOOST_CHECK( tiny::miller_rabin( prime_table[i], 1000 ) == true );  // 2009-07-05 Kenny: Not always guaranteed to find a solution, hard to test in unit-test?
  }
  
  int number = 4;
  for (int i=0;i<8;++i)
  {
    BOOST_CHECK( tiny::trial_division( number ) == false );
    BOOST_CHECK( tiny::pseudo_prime( number ) == false );
    BOOST_CHECK( tiny::miller_rabin( number, 10 ) == false );
    number = number << 1;
  }
  
  int a_prime = tiny::prime_search( 1024 );
  BOOST_CHECK( tiny::trial_division( a_prime ) == true );
  
  BOOST_CHECK( tiny::gcd_euclid_algorithm(12,24) == 12 );
  BOOST_CHECK( tiny::gcd_euclid_algorithm(prime_table[5],prime_table[10]) == 1 );
  BOOST_CHECK( tiny::is_relative_prime(prime_table[5],prime_table[10]) == true );
  BOOST_CHECK( tiny::is_relative_prime(12,24) == false );
  
}

BOOST_AUTO_TEST_SUITE_END();

