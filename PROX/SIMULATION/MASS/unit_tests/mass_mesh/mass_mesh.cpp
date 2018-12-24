#include <mass.h>

#include <algorithm> // needed for std::min and std::max
#include <cmath>     // needed for std::fabs

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_tools.hpp>

template<typename T>
class BoxMesh 
: public mass::FaceCallbackInterface<T>
{
public:
  
  T m_x1[4];  T m_y1[4];  T m_z1[4];
  T m_x2[4];  T m_y2[4];  T m_z2[4];
  T m_x3[4];  T m_y3[4];  T m_z3[4];
  T m_x4[4];  T m_y4[4];  T m_z4[4];
  T m_x5[4];  T m_y5[4];  T m_z5[4];
  T m_x6[4];  T m_y6[4];  T m_z6[4];
  
  T * m_x[6];  T * m_y[6];  T * m_z[6];
  
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
  
  BoxMesh()
  {
    this->set_box( 2.0, 2.0, 2.0, 2.0, 2.0  );
  }    
  
  BoxMesh(
          T const & top_width
          , T const & top_depth
          , T const & bottom_width
          , T const & bottom_depth
          , T const & height
          )
  {          
    this->set_box( top_width, top_depth, bottom_width, bottom_depth, height  );
  }
  
protected:
  
  void set_box(
               T const & top_width
               , T const & top_depth
               , T const & bottom_width
               , T const & bottom_depth
               , T const & height)
  {          
    T const H     = height       * 0.5;
    T const top_W = top_width    * 0.5;
    T const top_D = top_depth    * 0.5;
    T const bot_W = bottom_width * 0.5;
    T const bot_D = bottom_depth * 0.5;
    
    // front face	       
    m_x1[0] = -bot_W; m_y1[0] = -H; m_z1[0] =  bot_D;
    m_x1[1] =  bot_W; m_y1[1] = -H; m_z1[1] =  bot_D;
    m_x1[2] =  top_W; m_y1[2] =  H; m_z1[2] =  top_D;   
    m_x1[3] = -top_W; m_y1[3] =  H; m_z1[3] =  top_D;   
    
    // back face	       
    m_x2[0] = -top_W; m_y2[0] =  H; m_z2[0] = -top_D;
    m_x2[1] =  top_W; m_y2[1] =  H; m_z2[1] = -top_D;   
    m_x2[2] =  bot_W; m_y2[2] = -H; m_z2[2] = -bot_D;      
    m_x2[3] = -bot_W; m_y2[3] = -H; m_z2[3] = -bot_D;
    
    // top face
    m_x3[0] = -top_W; m_y3[0] =  H; m_z3[0] =  top_D;   
    m_x3[1] =  top_W; m_y3[1] =  H; m_z3[1] =  top_D;   
    m_x3[2] =  top_W; m_y3[2] =  H; m_z3[2] = -top_D;   
    m_x3[3] = -top_W; m_y3[3] =  H; m_z3[3] = -top_D;   
    
    // bottom face	       
    m_x4[0] = -bot_W; m_y4[0] = -H; m_z4[0] = -bot_D;
    m_x4[1] =  bot_W; m_y4[1] = -H; m_z4[1] = -bot_D;
    m_x4[2] =  bot_W; m_y4[2] = -H; m_z4[2] =  bot_D;
    m_x4[3] = -bot_W; m_y4[3] = -H; m_z4[3] =  bot_D;
    
    // left face	       
    m_x5[0] = -bot_W; m_y5[0] = -H; m_z5[0] = -bot_D;
    m_x5[1] = -bot_W; m_y5[1] = -H; m_z5[1] =  bot_D;
    m_x5[2] = -top_W; m_y5[2] =  H; m_z5[2] =  top_D;   
    m_x5[3] = -top_W; m_y5[3] =  H; m_z5[3] = -top_D;   
    
    // right face	       
    m_x6[0] =  bot_W; m_y6[0] = -H; m_z6[0] =  bot_D;
    m_x6[1] =  bot_W; m_y6[1] = -H; m_z6[1] = -bot_D;
    m_x6[2] =  top_W; m_y6[2] =  H; m_z6[2] = -top_D;   
    m_x6[3] =  top_W; m_y6[3] =  H; m_z6[3] =  top_D;   
    
    m_x[0] = &m_x1[0]; m_x[1] = &m_x2[0]; m_x[2] = &m_x3[0]; m_x[3] = &m_x4[0]; m_x[4] = &m_x5[0]; m_x[5] = &m_x6[0];
    m_y[0] = &m_y1[0]; m_y[1] = &m_y2[0]; m_y[2] = &m_y3[0]; m_y[3] = &m_y4[0]; m_y[4] = &m_y5[0]; m_y[5] = &m_y6[0];
    m_z[0] = &m_z1[0]; m_z[1] = &m_z2[0]; m_z[2] = &m_z3[0]; m_z[3] = &m_z4[0]; m_z[4] = &m_z5[0]; m_z[5] = &m_z6[0];
  }
  
};

BOOST_AUTO_TEST_SUITE(mass);

BOOST_AUTO_TEST_CASE(box_mesh)
{
  BoxMesh<double> const callback;
  
  double const rho = 1.0;
  
  mass::Properties<double> Ibox = compute_box( rho, 1.0, 1.0, 1.0 );  // Compute ground truth value using analytical solution 
    
  // Compute numerical solution
  mass::Properties<double> Imesh = compute_mesh( rho, 6, &callback);
  
  // Compare numerical solution to ground truth solution -- they should be the same
  BOOST_CHECK( Imesh.m_m   > 0.0 ); 
  BOOST_CHECK( Imesh.m_Ixx > 0.0 ); 
  BOOST_CHECK( Imesh.m_Iyy > 0.0 ); 
  BOOST_CHECK( Imesh.m_Izz > 0.0 );   
  BOOST_CHECK_CLOSE( Imesh.m_Ixx, Imesh.m_Iyy, 0.01 );
  BOOST_CHECK_CLOSE( Imesh.m_Iyy, Imesh.m_Izz, 0.01 );
  BOOST_CHECK_CLOSE(   Imesh.m_x, 0.0, 0.01 );
  BOOST_CHECK_CLOSE(   Imesh.m_y, 0.0, 0.01 );
  BOOST_CHECK_CLOSE(   Imesh.m_z, 0.0, 0.01 );
  BOOST_CHECK_CLOSE(   Imesh.m_m, Ibox.m_m, 0.01 );
  BOOST_CHECK_CLOSE( Imesh.m_Ixx, Ibox.m_Ixx, 0.01 );
  BOOST_CHECK_CLOSE( Imesh.m_Iyy, Ibox.m_Iyy, 0.01 );
  BOOST_CHECK_CLOSE( Imesh.m_Izz, Ibox.m_Izz, 0.01 );
  BOOST_CHECK_CLOSE( Imesh.m_Ixy, 0.0, 0.01 );
  BOOST_CHECK_CLOSE( Imesh.m_Ixz, 0.0, 0.01 );
  BOOST_CHECK_CLOSE( Imesh.m_Iyz, 0.0, 0.01 );
  
  BOOST_CHECK( Imesh.is_body_space() );  
  BOOST_CHECK( !Imesh.is_model_space() );  
}


BOOST_AUTO_TEST_CASE(cuboid_mesh)
{
  using std::min;
  using std::max;
  using std::fabs;
  
  double const rho = 1.0;
  
  double tw = 1.0;
  double td = 1.0;
  double bw = 1.0;
  double bd = 1.0;
  double h  = 1.0;
  
  double old_y = 0.0;
  
  for(size_t i=0;i<10u;++i)
  {
    BoxMesh<double> const callback = BoxMesh<double>(tw,td,bw,bd,h);
    
    tw *= 0.1;
    td *= 0.1;
    
    double iw = min(tw,bw)*0.5;
    double id = min(td,bd)*0.5;  
    mass::Properties<double> I = compute_box( rho, iw, h, id ); 
    
    double ow = max(tw,bw)*0.5;
    double od = max(td,bd)*0.5;
    mass::Properties<double> O = compute_box( rho, ow, h, od );  
        
    // Compute numerical solution
    mass::Properties<double> M = compute_mesh( rho, 6, &callback);
    
    BOOST_CHECK( M.m_m   >= I.m_m );
    BOOST_CHECK( M.m_m   <= O.m_m );

    
    BOOST_CHECK( M.m_Ixx   >= I.m_Ixx );
    BOOST_CHECK( M.m_Ixx   <= O.m_Ixx );

    BOOST_CHECK( M.m_Iyy   >= I.m_Iyy );
    BOOST_CHECK( M.m_Iyy   <= O.m_Iyy );
    
    BOOST_CHECK( M.m_Izz   >= I.m_Izz );
    BOOST_CHECK( M.m_Izz   <= O.m_Izz );
    
    BOOST_CHECK( fabs( M.m_Ixy ) < 10e-10 );
    BOOST_CHECK( fabs( M.m_Ixz ) < 10e-10 );
    BOOST_CHECK( fabs( M.m_Iyz ) < 10e-10 );
    
    BOOST_CHECK( fabs( M.m_x ) < 10e-10 );
    BOOST_CHECK( fabs( M.m_z ) < 10e-10 );
    
    BOOST_CHECK( M.m_y   <= old_y );

    old_y = M.m_y;
  }
}








BOOST_AUTO_TEST_SUITE_END();
