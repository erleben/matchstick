#include <mass.h>
#include <tiny.h>

#include <algorithm> // needed for std::min and std::max
#include <cmath>     // needed for std::fabs

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

typedef tiny::MathTypes<double> math_types;
typedef math_types::vector3_type    V;
typedef math_types::quaternion_type Q;

class CuboidMesh 
: public mass::FaceCallbackInterface<double>
{
public:
  
  double m_x1[4];  double m_y1[4];  double m_z1[4];
  double m_x2[4];  double m_y2[4];  double m_z2[4];
  double m_x3[4];  double m_y3[4];  double m_z3[4];
  double m_x4[4];  double m_y4[4];  double m_z4[4];
  double m_x5[4];  double m_y5[4];  double m_z5[4];
  double m_x6[4];  double m_y6[4];  double m_z6[4];
  
  double * m_x[6];  double * m_y[6];  double * m_z[6];
  
public:
  
  size_t get_number_of_vertices(size_t const & ) const
  {
    return 4u;
  }
  
  void get_x_coords(size_t const & face_no, double * coords) const
  {
    coords[0] = m_x[face_no][0];
    coords[1] = m_x[face_no][1];
    coords[2] = m_x[face_no][2];
    coords[3] = m_x[face_no][3];
  }
  
  void get_y_coords(size_t const & face_no, double * coords) const
  {
    coords[0] = m_y[face_no][0];
    coords[1] = m_y[face_no][1];
    coords[2] = m_y[face_no][2];
    coords[3] = m_y[face_no][3];
  }
  
  void get_z_coords(size_t const & face_no, double * coords) const
  {
    coords[0] = m_z[face_no][0];
    coords[1] = m_z[face_no][1];
    coords[2] = m_z[face_no][2];
    coords[3] = m_z[face_no][3];
  }
  
public:
    
  CuboidMesh(
         V const & v1
          , V const & v2
          , V const & v3
          , V const & v4
          , V const & v5
          , V const & v6
          , V const & v7
          , V const & v8
          )
  {    
    // front face	       
    m_x1[0] = v1(0); m_y1[0] = v1(1); m_z1[0] = v1(2);
    m_x1[1] = v2(0); m_y1[1] = v2(1); m_z1[1] = v2(2);
    m_x1[2] = v3(0); m_y1[2] = v3(1); m_z1[2] = v3(2);   
    m_x1[3] = v4(0); m_y1[3] = v4(1); m_z1[3] = v4(2);   
    
    // back face	       
    m_x2[0] = v8(0); m_y2[0] = v8(1); m_z2[0] = v8(2);
    m_x2[1] = v7(0); m_y2[1] = v7(1); m_z2[1] = v7(2);   
    m_x2[2] = v6(0); m_y2[2] = v6(1); m_z2[2] = v6(2);      
    m_x2[3] = v5(0); m_y2[3] = v5(1); m_z2[3] = v5(2);
    
    // top face
    m_x3[0] = v4(0); m_y3[0] = v4(1); m_z3[0] = v4(2);
    m_x3[1] = v3(0); m_y3[1] = v3(1); m_z3[1] = v3(2);   
    m_x3[2] = v7(0); m_y3[2] = v7(1); m_z3[2] = v7(2);      
    m_x3[3] = v8(0); m_y3[3] = v8(1); m_z3[3] = v8(2);
    
    // bottom face	       
    m_x4[0] = v2(0); m_y4[0] = v2(1); m_z4[0] = v2(2);
    m_x4[1] = v1(0); m_y4[1] = v1(1); m_z4[1] = v1(2);   
    m_x4[2] = v5(0); m_y4[2] = v5(1); m_z4[2] = v5(2);      
    m_x4[3] = v6(0); m_y4[3] = v6(1); m_z4[3] = v6(2);
    
    // left face	       
    m_x5[0] = v1(0); m_y5[0] = v1(1); m_z5[0] = v1(2);
    m_x5[1] = v4(0); m_y5[1] = v4(1); m_z5[1] = v4(2);   
    m_x5[2] = v8(0); m_y5[2] = v8(1); m_z5[2] = v8(2);      
    m_x5[3] = v5(0); m_y5[3] = v5(1); m_z5[3] = v5(2);
    
    // right face	       
    m_x6[0] = v2(0); m_y6[0] = v2(1); m_z6[0] = v2(2);
    m_x6[1] = v6(0); m_y6[1] = v6(1); m_z6[1] = v6(2);   
    m_x6[2] = v7(0); m_y6[2] = v7(1); m_z6[2] = v7(2);      
    m_x6[3] = v3(0); m_y6[3] = v3(1); m_z6[3] = v3(2);
    
    m_x[0] = &m_x1[0]; m_x[1] = &m_x2[0]; m_x[2] = &m_x3[0]; m_x[3] = &m_x4[0]; m_x[4] = &m_x5[0]; m_x[5] = &m_x6[0];
    m_y[0] = &m_y1[0]; m_y[1] = &m_y2[0]; m_y[2] = &m_y3[0]; m_y[3] = &m_y4[0]; m_y[4] = &m_y5[0]; m_y[5] = &m_y6[0];
    m_z[0] = &m_z1[0]; m_z[1] = &m_z2[0]; m_z[2] = &m_z3[0]; m_z[3] = &m_z4[0]; m_z[4] = &m_z5[0]; m_z[5] = &m_z6[0];
  }
  
};

BOOST_AUTO_TEST_SUITE(mass);

BOOST_AUTO_TEST_CASE(test_transform)
{
  double const rho = 1.0;
  
  double const W = 1.0;
  double const H = 2.0;
  double const D = 3.0;
  
  mass::Properties<double> Bbody = compute_box( rho, W, H, D );
  
  for(size_t i = 0u; i < 20u; ++i)
  {
    // Make random rotation and translation
    Q const q = tiny::unit( Q::random() );
    V const T = V::random(-10,10);
    
    // Apply transform to a 2x2x2 box 
    V const v1 = tiny::rotate(q, V::make(-W, -H,  D) ) + T;
    V const v2 = tiny::rotate(q, V::make( W, -H,  D) ) + T;
    V const v3 = tiny::rotate(q, V::make( W,  H,  D) ) + T;
    V const v4 = tiny::rotate(q, V::make(-W,  H,  D) ) + T;
    
    V const v5 = tiny::rotate(q, V::make(-W, -H, -D) ) + T;
    V const v6 = tiny::rotate(q, V::make( W, -H, -D) ) + T;
    V const v7 = tiny::rotate(q, V::make( W,  H, -D) ) + T;
    V const v8 = tiny::rotate(q, V::make(-W,  H, -D) ) + T;
    
    // Compute mass 
    CuboidMesh const callback = CuboidMesh(v1,v2,v3,v4,v5,v6,v7,v8);
    
    mass::Properties<double> Cmodel = compute_mesh( rho, 6, &callback);
    
    // Transform body frame ground truth box values into same
    // model frame as compute_mesh is using
    mass::Properties<double> Binter = mass::rotate( q.real(), q.imag()(0), q.imag()(1), q.imag()(2),  Bbody );
    mass::Properties<double> Bmodel = mass::translate_to_model_frame( T(0), T(1), T(2), Binter );

    // Make sure compute_mesh has computed the correct values!
    BOOST_CHECK_CLOSE( Bmodel.m_m,   Cmodel.m_m,   0.1 );
    BOOST_CHECK_CLOSE( Bmodel.m_Ixx, Cmodel.m_Ixx, 0.1 );
    BOOST_CHECK_CLOSE( Bmodel.m_Iyy, Cmodel.m_Iyy, 0.1 );
    BOOST_CHECK_CLOSE( Bmodel.m_Izz, Cmodel.m_Izz, 0.1 );
    BOOST_CHECK_CLOSE( Bmodel.m_Ixy, Cmodel.m_Ixy, 0.1 );
    BOOST_CHECK_CLOSE( Bmodel.m_Ixz, Cmodel.m_Ixz, 0.1 );
    BOOST_CHECK_CLOSE( Bmodel.m_Iyz, Cmodel.m_Iyz, 0.1 );
    BOOST_CHECK_CLOSE( Bmodel.m_x,   Cmodel.m_x,   0.1 );
    BOOST_CHECK_CLOSE( Bmodel.m_y,   Cmodel.m_y,   0.1 );
    BOOST_CHECK_CLOSE( Bmodel.m_z,   Cmodel.m_z,   0.1 );
    BOOST_CHECK_CLOSE( Bmodel.m_Qs,  Cmodel.m_Qs,  0.1 );
    BOOST_CHECK_CLOSE( Bmodel.m_Qx,  Cmodel.m_Qx,  0.1 );
    BOOST_CHECK_CLOSE( Bmodel.m_Qy,  Cmodel.m_Qy,  0.1 );
    BOOST_CHECK_CLOSE( Bmodel.m_Qz,  Cmodel.m_Qz,  0.1 );

    // Now we wish to test if we can transform the compute_mesh
    // values into their body frame
    mass::Properties<double> Cinter = mass::translate_to_body_frame( Cmodel );
    mass::Properties<double> Cbody  = mass::rotate_to_body_frame( Cinter );

    BOOST_CHECK_CLOSE( Bbody.m_m,   Cbody.m_m,   0.1 );
    BOOST_CHECK_CLOSE( Bbody.m_Ixx, Cbody.m_Ixx, 0.1 );
    BOOST_CHECK_CLOSE( Bbody.m_Iyy, Cbody.m_Iyy, 0.1 );
    BOOST_CHECK_CLOSE( Bbody.m_Izz, Cbody.m_Izz, 0.1 );
    BOOST_CHECK_CLOSE( Bbody.m_Ixy, Cbody.m_Ixy, 0.1 );
    BOOST_CHECK_CLOSE( Bbody.m_Ixz, Cbody.m_Ixz, 0.1 );
    BOOST_CHECK_CLOSE( Bbody.m_Iyz, Cbody.m_Iyz, 0.1 );
    BOOST_CHECK_CLOSE( Bbody.m_x,   Cbody.m_x,   0.1 );
    BOOST_CHECK_CLOSE( Bbody.m_y,   Cbody.m_y,   0.1 );
    BOOST_CHECK_CLOSE( Bbody.m_z,   Cbody.m_z,   0.1 );
    
    // Finally we wish to extract the body to model frame
    // transformation and test it against the known transformation.
    
    // Test if mesh has computed the correct center of mass value
    BOOST_CHECK_CLOSE( Cmodel.m_x, T(0), 0.1 );
    BOOST_CHECK_CLOSE( Cmodel.m_y, T(1), 0.1 );
    BOOST_CHECK_CLOSE( Cmodel.m_z, T(2), 0.1 );
    
    // 2011-01-30 Kenny:  We can not just compare orientations... they
    // are not really unique. There could be upto three pi rotations (one
    // pi around x, one around y and one around z) permuations.
//    BOOST_CHECK_CLOSE( Cbody.m_Qs,  q.real(),     0.1 );
//    BOOST_CHECK_CLOSE( Cbody.m_Qx,  q.imag()[0],  0.1 );
//    BOOST_CHECK_CLOSE( Cbody.m_Qy,  q.imag()[1],  0.1 );
//    BOOST_CHECK_CLOSE( Cbody.m_Qz,  q.imag()[2],  0.1 );
  }
}

BOOST_AUTO_TEST_SUITE_END();
