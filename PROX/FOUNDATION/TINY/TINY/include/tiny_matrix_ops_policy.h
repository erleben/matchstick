#ifndef TINY_MATRIX_OPS_POLICY_H
#define TINY_MATRIX_OPS_POLICY_H

#include <tiny_matrix.h>
#include <tiny_vector_ops_policy.h>

namespace tiny
{
  namespace detail
  {
    template <typename M>
    class MatrixOpsPolicy
      {
      protected:
        
        typedef typename M::value_traits   	      value_traits;
        typedef typename M::type_traits   	      type_traits;
        typedef typename type_traits::op_type     op_type;
        
      public:
        
        typedef typename type_traits::real_type             real_type;
                
        template <typename matrix_left,  typename matrix_right>
        static M mul_matrix (matrix_left const & lhs, matrix_right const & rhs)
        {
          typedef typename matrix_left::row_type              left_row_type;
          typedef typename matrix_right::column_type          right_column_type;
          typedef          VectorOpsPolicy< left_row_type >   vector_ops_policy;
          
          M result;
          
          size_t const I = lhs.size1();
          size_t const K = rhs.size2();
          
          for (size_t k=0 ; k<K ; ++k)
          {
            right_column_type const column( rhs.get_column_copy(k) );
            for (size_t i=0 ; i<I ; ++i)
            { 
              left_row_type const row( lhs.get_row_copy(i) );
              result(i,k) = vector_ops_policy::inner_prod(row,column);
            }
          }
          return result;
        }
        
        static M& mul_assign (M & lhs, M const & rhs)
        {
          lhs = mult(lhs,rhs);
          return lhs;
        }
        
        static typename M::column_type  mul_vector (M const & lhs, typename M::row_type const & rhs)
        {
          typedef VectorOpsPolicy< typename M::row_type > vector_ops_policy;
          
          size_t const I = lhs.size1();
          
          typename M::column_type  result;
          
          for (size_t i=0; i<I ; ++i)
          {
            typename M::row_type const row( lhs.get_row_copy(i) );
            result[i] = vector_ops_policy::inner_prod(row,rhs);
          }
          return result;
        }
                
        template <typename untransposed_M>
        static M trans(untransposed_M const & matrix)
        {
          size_t const I = matrix.size1();
          size_t const J = matrix.size2();
          
          M transposed;
          
          for (size_t i = 0 ; i<I; ++i)
            for (size_t j = 0 ; j<J; ++j)
              transposed(j,i) = matrix(i,j);
          
          return transposed;
        }
      };
    
  } // namespace detail
} // namespace tiny


// TINY_MATRIX_OPS_POLICY_H
#endif 

