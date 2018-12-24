#ifndef SPARSE_IO_MATLAB_WRITERS_H
#define SPARSE_IO_MATLAB_WRITERS_H

#include <sparsefwd.h>
#include <io/sparse_file_stream_helpers.h> // for open_file_stream, close_file_stream

#include <fstream>
#include <string>

namespace sparse
{
	namespace io
	{
    
    namespace detail
    {
      
      template <typename OS, typename I>
      inline void write_matlab_vector(OS& os, I iter, I last)
      {
        os << "[ ";
        for(; iter != last; ++iter)
        {
          os << *iter << " ";
        }
        os << "]";
      }
            
      template <typename OS, typename I1, typename I2, typename N>
      inline void write_matlab_sparse(OS& os, I1 row_idxs, I1 col_idxs, I2 data, N m, N n, N nnz)
      {
        // matlab has one-based indexing so we need to adjust our zero-based indexing
        I1 r = row_idxs;
        I1 c = col_idxs;
        for(size_t cnt=0;cnt<nnz;++cnt)
        {
          *r += 1; *c += 1;
          ++r;++c;
        }
        
        os << "sparse(";
        write_matlab_vector(os, row_idxs, row_idxs+nnz);
        os << ",";
        write_matlab_vector(os, col_idxs, col_idxs+nnz);
        os << ",";
        write_matlab_vector(os,     data,     data+nnz);
        os << "," << m << "," << n << "," << nnz << ")";
      }
      
      template <typename block_type>
      inline void block2array(
                              block_type const& b
                              , size_t const & /*b_idx*/
                              , typename block_type::value_type* const data
                              , size_t& n
                              )
      {
        size_t const n_data = block_type::size();
        for (size_t i = 0; i < n_data; ++i)
        {
          data[n] = b[i];
          ++n;
        }
      }
      
      template <typename block_type>
      inline void block2array(
                              block_type const& b
                              , size_t  const b_row
                              , size_t  const b_col
                              , size_t * const r_idxs
                              , size_t * const c_idxs
                              , typename block_type::value_type * const data
                              , size_t& n
                              , bool const is_trans
                              )
      {
        assert(block_type::size() == block_type::nrows()*block_type::ncols() || !"Invalid Arguments");
        
        // Number of rows and cols in the block
        size_t const n_rows = block_type::nrows();
        size_t const n_cols = block_type::ncols();
        
        // global row/col indexes to first entry in the block
        size_t const    row = b_row*(is_trans ? n_cols : n_rows);
        size_t const    col = b_col*(is_trans ? n_rows : n_cols);
        
        size_t dr = 0; // local row offset in b.data
        for (size_t r = 0; r < n_rows; ++r)
        {
          for (size_t c = 0; c < n_cols; ++c)
          {
            r_idxs[n] = row + (is_trans ? c : r);
            c_idxs[n] = col + (is_trans ? r : c);
            data[  n] = b[dr + c];
            ++n;
          }
          dr += n_cols;
        }
      }
      
      template<typename matrix_type>
      inline void  write_sparse(
                                 std::ostream & os
                                , matrix_type const& A
                                , bool const is_trans = false
                                )
      {
        typedef typename matrix_type::block_type   block_type;
        typedef typename block_type::value_type    value_type;
        
        assert(A.size() > 0 || !"Invalid Argument");
        
        size_t  const n_blocks = A.size();
        
        size_t       const n_data   = n_blocks*block_type::nrows()*block_type::ncols();
        // 2009-07-01 Kenny: new/delete unsafe?
        size_t     * const row_idxs = new size_t[n_data];
        size_t     * const col_idxs = new size_t[n_data];
        value_type * const data     = new value_type[n_data];
        
        size_t n = 0;
        for (size_t i = 0; i < n_blocks ; ++i)
        {
          block_type const & b = A[i];
          block2array(b, row(i,A), col(i,A), row_idxs, col_idxs, data, n, is_trans);
        }
        
        write_matlab_sparse(os, row_idxs, col_idxs, data,
                            A.nrows()*(is_trans ? block_type::ncols() : block_type::nrows()),
                            A.ncols()*(is_trans ? block_type::nrows()	: block_type::ncols()),
                            n_data
                            );
        // 2009-07-01 Kenny: new/delete unsafe?
        delete[] row_idxs;
        delete[] col_idxs;
        delete[] data;
      }
      
      template <typename block_type>
      inline void write_vector(
                                std::ostream & os
                               , Vector<block_type> const & u
                               , bool const is_trans = false
                               )
      {
        assert(u.size() > 0 || !"Invalid Argument");
        
        typedef typename block_type::value_type value_type;
        
        size_t       const n_blocks = u.size();
        size_t       const n_data = n_blocks * block_type::size();
        // 2009-07-01 Kenny: new/delete unsafe?
        value_type * const data = new value_type[n_data];  
        size_t n = 0;
        for (size_t i = 0; i < n_blocks; ++i)
        {
          block2array(u(i), i, data, n);
        }
        write_matlab_vector( os, data, data + n_data );
        
        if (!is_trans)
        {
          os << "'";
        }
        // 2009-07-01 Kenny: new/delete unsafe?
        delete[] data;
      }
      
    } // namespace detail
    
    /**
     *
     * @warning Do not support fake-transposed matrices.
     */
    template<typename B>
    inline std::ostream & operator << (
                                       std::ostream & os
                                       , DiagonalMatrix<B> const & A
                                       )
    {
      detail::write_sparse( os, A, false);
      return os;
    }

    template<typename B>
    inline std::ostream & operator << (
                                       std::ostream & os
                                       , CompressedRowMatrix<B> const & A
                                       )
    {
      detail::write_sparse( os, A, false);
      return os;
    }
    
    template<typename B>
    inline std::ostream & operator << (
                                       std::ostream & os
                                       , TwoColumnMatrix<B> const & A
                                       )
    {
      detail::write_sparse( os, A, false);
      return os;
    }
    
    /**
     * @warning Do not support fake-transposed vectors.
     */
    template<typename block_type>
    inline std::ostream & operator << (
                                       std::ostream & os
                                       , Vector<block_type> const & u
                                       )
    {
      detail::write_vector( os, u, false );
      return os;
    }
    
		template<typename matrix_type>
    inline void  write_matlab_file(
                                    std::string const & file_path
                                   , matrix_type const& A
                                   , bool const is_trans = false
                                   )
		{
      std::ofstream os;
      open_file_stream(file_path, os);
		  detail::write_sparse( os, A, is_trans );
			close_file_stream(os);
		}
    
		template <typename block_type>
    inline void write_matlab_file(
                                  std::string const & file_path
                                  , Vector<block_type> const & u
                                  , bool const is_trans = false
                                  )
		{
			std::ofstream os;
			open_file_stream(file_path, os);
      detail::write_vector( os, u, is_trans);
			close_file_stream(os);
		}
    
	} // namespace io
}   // namespace sparse

// SPARSE_IO_MATLAB_WRITERS_H
#endif
