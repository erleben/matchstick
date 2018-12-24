#include <mass.h>
#include <cmath>

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>


BOOST_AUTO_TEST_SUITE(mass);

BOOST_AUTO_TEST_CASE(cone_bounding_test)
{
  // Values for outer bounding cylinder -- given by bottom radius
  mass::Properties<float> O;

  // Values for inner bounding cylinder -- given by bottom radius
  ;

  // Values for conical solid
  ;
  
  float const rho = 1.0f;  // density
  float       a   = 2.0f;  // bottom radius
  float const b   = 1.0f;  // top radius
  float const h   = 1.0f;  // height
  
  // Compute inner bounding cylinder -- Never changes!!! 
	mass::Properties<float> I = compute_cylinder(rho, b, h/2.0f);
  I = translate_to_model_frame(0.0f, h/2.0f, 0.0f, I);
  
  float old_Cy = 0.0f;
  
  for ( int i = 0 ; i < 9 ;++i)
  {
    a -= 0.1f;  // We are schrinking bottom radius of conical solid
    
    // Compute outer bounding cylinder 
    mass::Properties<float> O = compute_cylinder(rho, a, h/2.0f);
    O = translate_to_model_frame(0.0f, h/2.0f, 0.0f, O);
        
    mass::Properties<float> C = compute_conical_solid(rho, a, b, h );
        
    
    BOOST_CHECK( !C.is_body_space() );  
    BOOST_CHECK(  C.is_model_space() );  

    // Verify that the mass are bounded by the cylinders
    BOOST_CHECK( I.m_m < C.m_m  );
    BOOST_CHECK( C.m_m < O.m_m  );

    // Verify that the xx inertia products are bounded
    BOOST_CHECK( I.m_Ixx < C.m_Ixx );
    BOOST_CHECK( C.m_Ixx < O.m_Ixx );
    
    // Verify that the yy inertia products are bounded
    BOOST_CHECK( I.m_Iyy < C.m_Iyy );
    BOOST_CHECK( C.m_Iyy < O.m_Iyy );

    // Verify that the zz inertia products are the same as teh xx values
    BOOST_CHECK_CLOSE(C.m_Ixx, C.m_Izz, 0.01f);
    
    // Test if center of mass in x-z plane are correct
    BOOST_CHECK_CLOSE(C.m_x, 0.0f, 0.01f);
    BOOST_CHECK_CLOSE(C.m_z, 0.0f, 0.01f);

    // Test if center of mass of conical is lower than the cylinders
    BOOST_CHECK(C.m_y < O.m_y);

    // As bottom radius is increas center of mass should rise up moving closer and closer towards center of mass of the boudning cylinders.
    BOOST_CHECK(C.m_y > old_Cy);
    old_Cy = C.m_y;
  }
  
}

BOOST_AUTO_TEST_SUITE_END();
