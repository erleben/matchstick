#ifndef PROX_MATH_POLICY_H
#define PROX_MATH_POLICY_H

#include <prox_mass_block.h>    

#include <tiny_math_types.h>
#include <sparse.h>

#include <cmath> // for std::abs


namespace prox
{
        
    template< typename T >
    class MathPolicy
    : public tiny::MathTypes<T>
    {
    public:
        
        typedef tiny::MathTypes<T>                            base_type;
        typedef tiny::MathTypes<T>                            tiny_types;

    public:
        
        typedef T										                          real_type;
        
        typedef MassBlock<real_type>		                      mass_block_type;
        
        typedef sparse::Block<4,1,real_type>                  block4x1_type;
        typedef sparse::Block<6,1,real_type>                  block6x1_type;
        typedef sparse::Block<7,1,real_type>                  block7x1_type;
        typedef sparse::Block<4,4,real_type>                  block4x4_type;
        typedef sparse::Block<4,6,real_type>                  block4x6_type;
        typedef sparse::Block<6,4,real_type>                  block6x4_type;
        typedef sparse::Block<6,6,real_type>                  block6x6_type;
        
      // 2009-08-04 Kenny: style problem, vector3_type is not a blocked vector
      // like vector4_type. So how can we tell the two different types apart by
      // looking at their names only?

      
        typedef sparse::Vector< block4x1_type >		       	    vector4_type;
        typedef sparse::Vector< block6x1_type >               vector6_type;
        typedef sparse::Vector< block7x1_type >               vector7_type;
        
        typedef sparse::DiagonalMatrix< block4x4_type >       diagonal4x4_type;
        typedef sparse::DiagonalMatrix< block6x6_type >       diagonal6x6_type;
        typedef sparse::DiagonalMatrix< mass_block_type >     diagonal_mass_type;
        typedef sparse::CompressedRowMatrix< block6x4_type >  compressed6x4_type;
        typedef sparse::CompressedRowMatrix< block4x6_type >  compressed4x6_type;
        
        typedef typename compressed4x6_type::const_iterator      compressed4x6_type_const_iterator;
        typedef typename compressed4x6_type::const_row_iterator  compressed4x6_type_const_row_iterator;
        typedef typename compressed6x4_type::accessor            compressed6x4_type_accessor;
        typedef typename compressed6x4_type::const_row_iterator  compressed6x4_type_const_row_iterator;
        
    public:
        
        // 2009-08-13 Kenny: We should add unit-test for this method, it is a core method and we need to make sure it works correctly.
        // compute b = (I+E)J u + J W (dt h)
        static void compute_b(
                              compressed4x6_type const& J,
                              vector6_type const& Wdth,
                              vector6_type const& u,
                              vector4_type const& e,
                              vector4_type const& g,
                              vector4_type & b
                              )
        {

            b.resize( g.size() );
            
            vector4_type v_minus(g.size());
            vector4_type v_plus( g.size());
            
            sparse::prod(J, u, v_minus);             // v_minus = J*u
            v_plus = v_minus;
            sparse::prod(J, Wdth, v_plus, false);    // v_plus  += J*Wdth
            sparse::element_prod(e, v_minus, b);     // b = (e#v_minus)      
            sparse::add( g, b);                      // b += g : correction/stabilization term
            sparse::add( v_plus, b);                 // b += v_plus
        }
        
        static void compute_WJT(
                                diagonal6x6_type const& W,     // 2009-08-13 Kenny: Optimization replace with diagonal_mass_type, maybe wait to optimize until all it working
                                compressed4x6_type const& J,
                                compressed6x4_type & WJT
                                )
        {
            compressed6x4_type JT;
            
            // TODO: can probably be optimised by doing W x J -> WJ^T on the fly
            sparse::transpose(J, JT);
            WJT.resize(W.nrows(), JT.ncols(), JT.size());
            sparse::prod(W, JT, WJT);
        }
        
        static void compute_WJT(
                                diagonal6x6_type const& W     // 2009-08-13 Kenny: Optimization replace with diagonal_mass_type, maybe wait to optimize until all it working
                                , compressed6x4_type const& JT
                                , compressed6x4_type & WJT
                                )
        {
            WJT.resize(W.nrows(), JT.ncols(), JT.size());
            sparse::prod(W, JT, WJT);
        }
        
        
        static void compute_Wdth(
                                 diagonal6x6_type const& W    // 2009-08-13 Kenny: Optimization replace with diagonal_mass_type, maybe wait to optimize until all it working
                                 , real_type const& dt
                                 , vector6_type & h
                                 , vector6_type & Wdth
                                 )
        {            // 2011-06-07 Sarah: kill when fixed in prox_bullet.cpp, already removed from prox_solver.h

            sparse::prod(dt, h);
            sparse::prod(W, h, Wdth);
        }
        
        //compute z = x - R(J W J^T x  + b)  = x - R( A x  + b)
        static void compute_z(
                              vector4_type       const & x
                              , diagonal4x4_type   const & R
                              , compressed4x6_type const & J
                              , compressed6x4_type const & WJT
                              , vector4_type       const & b
                              , vector4_type             & z
                              )
        {
            z.resize(x.size());
            
            vector6_type t1(WJT.nrows());
            vector4_type t2(x.nrows());
            vector4_type t3(x.nrows()); // 2009-08-25 Sarah: Had to add extra 'keeper' variable, prod(R, t2, t2) didn't give the desired t2 = R*t2 and I would prefer not to mess with the sparse lib 
            
            sparse::prod(WJT, x, t1);       // t1 = WJT*x
            sparse::prod(J, t1, t2, false); // t2 = J*t1    // 2009-08-04 Kenny: We should probably extend SPARSE with axpy_prod for compressed row matrices
            sparse::add(b,t2,t2);           // t2 += b
            sparse::prod(R, t2, t3);        // t3 = R*t2
            sparse::sub(x, t3, z);          // z  = x-t3
        }
        
        static void compute_z_k(
                                block4x1_type        const & x_k
                                , vector6_type       const & w
                                , block4x4_type      const & R_k
                                , compressed4x6_type const & J
                                , block4x1_type      const & b_k
                                , block4x1_type            & z_k
                                , size_t             const & k
                                )
        {
            // z_k = x_k - R_kk ( J w + b_k )
            z_k.clear_data();
            block4x1_type temp(0);
            sparse::row_prod(J, w, z_k, k);       // z_k += J_k w
            sparse::add(b_k, z_k);                // z_k += b_k
            sparse::prod(R_k, z_k, temp);         // temp += R_kk z_k
            sparse::sub(x_k, temp, z_k);          // z_k = x_k - temp
        }
        
        static void update_w(
                             compressed6x4_type    const & WJT
                             , compressed4x6_type  const & J
                             , block4x1_type       const & delta_x
                             , size_t              const & k
                             , vector6_type              & w
                             )
        {
            // 2011-06-07 Sarah: kill when parallel solvers work, already removed from prox_solver.h
            // consider: move this into the function to save a function call? Or inline the call?
            sparse::column_prod( WJT, J, delta_x, w, k);
        }
        
        /** update w knowing the new and old values of x_k */
        template<typename w_vector_type>
        static void update_w(
                             compressed6x4_type        const & WJT
                             , compressed4x6_type      const & J
                             , block4x1_type           const & x_k_in
                             , block4x1_type           const & x_k_out
                             , size_t                  const & k
                             , w_vector_type                 & w
                             )
        {
            block4x1_type delta_x(0); // performance: is it reasonable to reallocate delta_x?
            sparse::sub(x_k_out, x_k_in, delta_x);
            sparse::column_prod( WJT, J, delta_x, w, k);
        }
        
        static void compute_w_m(
                                compressed6x4_type    const & WJT
                                , vector4_type        const & x
                                , size_t              const   m
                                , vector6_type              & w
                                )
        {
            w[m].clear_data();
            sparse::row_prod( WJT, x, w[m], m );
        }
        
        static void update_R_k( real_type const nu, block4x4_type & R_k)// 2011-06-07 Sarah: kill when parallel solvers work
        {
            sparse::scalar_prod(nu, R_k);
        }
        
        
        static real_type compute_norm_inf( vector4_type const& x )
        {
            using std::abs;
            
            // 2009-08-04 Kenny: Argh, this is ugly ugly ugly code, if block4_type were a tiny::Vector<4,T> type then this could be made much more pretty
            
            assert(x.size() > 0  || !"compute_norm_inf() zero sized vector");
            typename vector4_type::const_iterator iter = x.begin();
            typename vector4_type::const_iterator last = x.end();
            
            real_type max = base_type::value_traits::zero();
            real_type candidate;
            for (; iter != last; ++iter)
            {
                block4x1_type const & block = *iter;
                for (size_t i = 0;i<4;++i)
                {
                    candidate = abs( block(i) );
                    if (candidate > max)
                    {
                        max = candidate;
                    }
                }
            }
            return max;
        }
        
        /** Given the kth block of x_i and x_{i+1}, the residual norm of the entire block
         * is returned */
        static real_type compute_norm_inf_residual_k(
                                                     block4x1_type const & x_k_in
                                                     , block4x1_type const & x_k_out
                                                     )
        {
            // the norm inf is the largest absolute value in the residual
            //compute_residual_k(x_k_in, x_k_out, r_k);
            real_type current_max = base_type::value_traits::zero();
            real_type candidate = base_type::value_traits::zero();
            for( size_t i = 0u; i < 4u; ++i )
            {
                candidate = fabs(x_k_in[i] - x_k_out[i]);
                if( candidate > current_max )
                {
                    current_max = candidate;
                }
            }
            return current_max;
        }
        
        static void compute_prod(
                                 diagonal4x4_type const& A
                                 , diagonal4x4_type & B
                                 )
        {
            // compute B = A * B
            sparse::prod(A, B);
        }
        
        static void compute_prod(
                                 compressed6x4_type const& WJT
                                 , vector4_type const & lambda
                                 , vector6_type & fc
                                 )
        {
            if(fc.size() != WJT.nrows() )
                fc.resize( WJT.nrows() );
            sparse::prod(WJT, lambda, fc, true);
        }
        
        
        
        static void compute_sum( 
                                vector6_type const& A,
                                vector6_type const& B,
                                vector6_type const& C,
                                vector6_type & out)
        {
            out.resize(A.nrows());
            sparse::add(A, B, C, out);
        }
        
        static void compute_sum( 
                                vector6_type const& A,
                                vector6_type const& B,
                                vector6_type & out )
        {
            out.resize(A.nrows());
            sparse::add(A, B, out);
        }
        
        // 2009-08-13 Kenny: We should add unit-test for this method, it is a core method and we need to make sure it works correctly.
        static void compute_A_diagonal(
                                       compressed4x6_type const& J,
                                       compressed6x4_type const& WJT,
                                       diagonal4x4_type & diagA
                                       )
        {
            diagA.resize(J.nrows());
            sparse::diag_of_prod(J, WJT, diagA);     // 2009-08-13 Kenny: I think this only exist in SPARSE in a TwoColumnMatrix version?
        }
        
        /// This function compute the number of nonzero blocks in the specified row in WJT
        static size_t get_row_length( compressed6x4_type const & WJT, size_t const row )
        {
            return compressed6x4_type_accessor::row_ptrs(WJT)[row+1u] - compressed6x4_type_accessor::row_ptrs(WJT)[row] - 1u;
        }

      static void make_zero(
                            vector4_type & v
                            , unsigned int const & K
                            )
      {
        v.resize( K );
        v.clear_data();
      }

    };
} // namespace prox

// PROX_MATH_POLICY_H
#endif 
