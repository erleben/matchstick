#include <factory/mesh_array_profile_sweep.h>

#include <tiny_math_types.h>

#include <vector>
#include <cassert>

namespace mesh_array
{
  
  template<typename MT>
  void profile_sweep(
                     std::vector<typename MT::vector3_type> const & profile
                     , size_t const & slices
                     , T3Mesh & mesh
                     , VertexAttribute<typename MT::real_type,T3Mesh> & X
                     , VertexAttribute<typename MT::real_type,T3Mesh> & Y
                     , VertexAttribute<typename MT::real_type,T3Mesh> & Z
                     )
  {
    typedef typename MT::real_type       T;
    typedef typename MT::value_traits    VT;
    typedef typename MT::vector3_type    V;
    typedef typename MT::quaternion_type Q;
    
    size_t const N      = profile.size();
    size_t const J      = slices;
    
    assert( N>2u || !"profile_sweep(): Profile must have at least 3 points");
    assert( J>2u || !"profile_sweep(): Sweep must have at least 3 slices to be a proper volume.");
    
    size_t const K      = (N-2u)*J + 2u;  // Total number of vertices
    size_t const bottom = K-1u;           // Index to bottom vertex
    size_t const top    = K-2u;           // Index to top vertex
    size_t const H      = N-2u;           // Number of latitude circles
    size_t const F      = 2*J*(N-1u);     // Total number of triangle faces

    mesh.clear();
    
    mesh.set_capacity(K,F);
    
    X.bind(mesh);
    Y.bind(mesh);
    Z.bind(mesh);
    
      //--- Make a 2D grid of vertices by sweeping profile around y-axis
    T const dtheta = VT::two()*VT::pi() / J;  // The angle of each slice
    
    for(size_t j = 0u; j < J; ++j)
    {
      T const theta = j*dtheta;
      Q const R = Q::Ru( theta, V::j() );
      
      for(size_t i = 0u; i < H; ++i)
      {
        V      const coords = rotate(R, profile[i+1]);
        Vertex const v      = mesh.push_vertex();
        
        X(v) = coords(0);
        Y(v) = coords(1);
        Z(v) = coords(2);
      }
    }
    
      // Now fill in top and bottom vertices
    {
      mesh.push_vertex();
      Vertex const t = mesh.vertex(top);
      
      X(t) = profile[N-1](0);
      Y(t) = profile[N-1](1);
      Z(t) = profile[N-1](2);
      
      mesh.push_vertex();
      Vertex const b = mesh.vertex(bottom);
      
      X(b) = profile[0](0);
      Y(b) = profile[0](1);
      Z(b) = profile[0](2);
    }
      // Make faces for bottom-ring
    for(size_t j=0;j<J;++j)
    {
        //
        //  V  = {c1} {c2} ... {cJ}  b t
        //
        // b c1.0 c2.0 | b c2.0 c3.0| ... | b cJ.0 c1.0
        // b 0 (N-2) | b (N-2) 2*(N-2)
        //
      size_t const left  = j;
      size_t const right = ((j+1)%J);
      
      Vertex const vi = mesh.vertex( bottom  );
      Vertex const vj = mesh.vertex( H*right );
      Vertex const vk = mesh.vertex( H*left  );
      
      mesh.push_triangle( vi, vj, vk );
    }
    
      // Make faces for middle-rings
    for(size_t i = 0u; i < H-1u; ++i)//ring number
    {
      for(size_t j = 0u; j < J; ++j)//slice number
      {
        size_t const left   = j;
        size_t const right  = (j+1) % J;
        size_t const up     = i+1;
        size_t const down   = i;
        
        Vertex const vi = mesh.vertex( left*H + down );
        Vertex const vj = mesh.vertex(right*H + down );
        Vertex const vk = mesh.vertex(right*H + up    );
        Vertex const vm = mesh.vertex( left*H + up    );
      
        mesh.push_triangle( vi, vj, vk );
        mesh.push_triangle( vi, vk, vm );
      }
    }
    
      // Make faces for top-ring
    for(size_t j=0;j<J;++j)
    {
      size_t const offset = (N-3);
      size_t const left   = j;
      size_t const right  = (j+1)%J;
      
      Vertex const vi = mesh.vertex( top              );
      Vertex const vj = mesh.vertex( left*H + offset  );
      Vertex const vk = mesh.vertex( right*H + offset );
      
      mesh.push_triangle( vi, vj, vk );
    }
  }
  
  typedef tiny::MathTypes<float> MTf;
  typedef tiny::MathTypes<double> MTd;
  
  template
  void profile_sweep<MTf>(
                          std::vector<MTf::vector3_type> const & profile
                          , size_t const & slices
                          , T3Mesh & mesh
                          , VertexAttribute<MTf::real_type,T3Mesh> & X
                          , VertexAttribute<MTf::real_type,T3Mesh> & Y
                          , VertexAttribute<MTf::real_type,T3Mesh> & Z
                          );
  
  template
  void profile_sweep<MTd>(
                          std::vector<MTd::vector3_type> const & profile
                          , size_t const & slices
                          , T3Mesh & mesh
                          , VertexAttribute<MTd::real_type,T3Mesh> & X
                          , VertexAttribute<MTd::real_type,T3Mesh> & Y
                          , VertexAttribute<MTd::real_type,T3Mesh> & Z
                          );
  
} //namespace mesh_array
