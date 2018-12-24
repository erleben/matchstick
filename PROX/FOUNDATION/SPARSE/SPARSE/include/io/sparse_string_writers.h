#ifndef SPARSE_IO_STRING_WRITERS_H
#define SPARSE_IO_STRING_WRITERS_H

#include <sparsefwd.h>
#include <string>
#include <sstream>

/**
* @file sparse_string_writers.h
* The SPARSE string writers contain functions to write SPARSE vectors and matrices to strings while
* retaining the blocked format. This differs from the Matlab writers that write the vectors
* and matrices in Matlab sparse format, which obscure the block format. The primary application
* is debugging.
*/

namespace sparse
{
  /**
  * @namespace sparse::io
  * @brief The io namespace handles input/output for SPARSE, with support for outputting SPARSE vectors and matrices
  * in Matlab sparse format or an easy readable string for debugging.
  */
	namespace io
	{ 
    /**
    * This writes the contents of a block to a string. Comma is used to separate elements in
    * the same row, and semicolon to separate rows. "[" and "]" act as delimiters of the block.
    * @param B The block to write to string
    * @return The std::string representing the block B
    */
    template<size_t M, size_t N, typename T>
    inline std::string to_string( Block<M,N,T> const & B )
    {
      std::ostringstream stream;
      stream << "[ ";
      size_t const MN = M*N;
      for(size_t i = 0u ; i < MN; ++i )
      {
          stream << B[i];
        if( (i%N == N-1u) && (i != MN - 1u) )
            stream << " ; ";
          else if ( i != MN - 1u )
            stream << ",";
      }
      stream << " ] ";
      return stream.str();
    }

    /**
    * This writes the contents of a CompressedRowMatrix to a string in verbose or compressed format.
    * The verbose format starts by "Block at row i, column j => " and then list the content of that 
    * block. The compressed format write "(i,j): " followed by the content of the block.
    * @param A The CompressedRowMatrix to write to a string
    * @param compress_output boolean flag that allows the user to compress the output
    * @return The std::string representing the matrix A
    */
    template<typename B>
    inline std::string to_string( CompressedRowMatrix<B> const & A, bool compress_output = false )
    {

      std::ostringstream stream;
      typename CompressedRowMatrix<B>::const_iterator iter = A.begin();
      typename CompressedRowMatrix<B>::const_iterator last = A.end();
      for( ; iter != last; ++iter )
      {
        size_t const row_idx = row(iter);
        size_t const col_idx = col(iter);
        if( compress_output )
          stream << "(" << row_idx << "," << col_idx << "): ";
        else
          stream << "Block at row: " << row_idx << ", column: " << col_idx << " => ";
        stream << to_string(*iter);
        if( !compress_output )
          stream << std::endl;
      }
      return stream.str();
    }

    /**
    * This writes the contents of a DiagonalMatrix to a string in verbose or compressed format.
    * The verbose format starts by "Block at row i, column j => " and then list the content of that 
    * block. The compressed format write "(i,j): " followed by the content of the block.
    * @param A The DiagonalMatrix to write to a string
    * @param compress_output boolean flag that allows the user to compress the output
    * @return The std::string representing the matrix A
    */
    template<typename B>
    inline std::string to_string( DiagonalMatrix<B> const & A, bool compress_output = false )
    {

      std::ostringstream stream;
      typename DiagonalMatrix<B>::const_iterator iter = A.begin();
      typename DiagonalMatrix<B>::const_iterator last = A.end();
      for( ; iter != last; ++iter )
      {
        size_t const row_idx = row(iter);
        size_t const col_idx = col(iter);
        if( compress_output )
          stream << "(" << row_idx << "," << col_idx << "): ";
        else
          stream << "Block at row: " << row_idx << ", column: " << col_idx << " => ";
        stream << to_string(*iter);
        if( !compress_output )
          stream << std::endl;
      }
      return stream.str();
    }

    /**
    * This writes the contents of a Vector to a string in verbose or compressed format.
    * The verbose format starts by "Block at row i => " and then list the content of that 
    * block. The compressed format write "i: " followed by the content of the block.
    * @param v The Vector to write to a string
    * @param compress_output boolean flag that allows the user to compress the output
    * @return The std::string representing the vector v
    */
    template<typename B>
    inline std::string to_string( Vector<B> const & v, bool compress_output = false )
    {
      std::ostringstream stream;
      for( size_t i = 0u ; i < v.size(); ++i )
      {
        if( !compress_output )
          stream << "Block at row: " << i << " => ";
        stream << to_string(v[i]);
        if( !compress_output )
          stream << std::endl;
      }
      return stream.str();
    }

    /**
    * This writes the contents of a CompressedVector to a string in verbose or compressed format.
    * The verbose format starts by "Block at row i => " and then list the content of that 
    * block. The compressed format write "i: " followed by the content of the block.
    * @param v The CompressedVector to write to a string
    * @param compress_output boolean flag that allows the user to compress the output
    * @return The std::string representing the vector v
    */
    template<typename B,typename C>
    inline std::string to_string( CompressedVector<B,C> const & v, bool compress_output = false )
    {
      std::ostringstream stream;
      typename CompressedVector<B,C>::const_iterator iter = v.begin();
      typename CompressedVector<B,C>::const_iterator last = v.end();
      for( ; iter != last; ++iter )
      {
        size_t const row_idx = row(iter);
        if( compress_output )
          stream << row_idx << ": ";
        else
          stream << "Block at row: " << row_idx << " => ";
        stream << to_string(*iter);
        if( !compress_output )
          stream << std::endl;
      }
      return stream.str();
    }


	} // namespace io
} // namespace sparse

// SPARSE_IO_STRING_WRITERS_H
#endif
