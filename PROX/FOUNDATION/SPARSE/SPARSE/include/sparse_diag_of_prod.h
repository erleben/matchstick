#ifndef SPARSE_DIAG_OF_PROD_H
#define SPARSE_DIAG_OF_PROD_H

#include <sparsefwd.h>    
#include <sparse_traits.h> // for zero_block

#include <cassert>

namespace sparse
{
    
    /**
     * Diagonal of Matrix Product.
     * This function computes the  blocked diagonal of a matrix product between a two column matrix and a compressed row matrix.
     *
     *
     *
     * @tparam B1    The block type of the two column matrix.
     * @tparam B2    The block type of the compressed row matrix.
     * @tparam BR    The block type of the resulting block diagonal matrix.
     *
     * @param init   If set to true then the function will initialize the diagonal blocks to zero. Default value is false.
     * @param D      Upon return this diagonal matrix holds the result of the computation. Note that if the dimensions of D does not fit the dimension of the diagonal of the product then D is resized by the function.
     *
     * @warning PRECONDITION: (STRONG!) J(i,j) non-zero <=> JT(j,i) non-zero
     */
    // 2009-06-30 Kenny: Proper ADL for STL dependencies?
    // 2009-06-30 Kenny: Documentation Needed, it is not clear what JT really is. It is not really J transposed, but it will act as J transposed right?
    template <typename B1, typename B2, typename BR>
    inline void diag_of_prod(
                             TwoColumnMatrix<B1> const& J
                             , CompressedRowMatrix<B2> const& JT
                             , DiagonalMatrix<BR>& D
                             , bool init = false
                             )
    {
        assert(J.ncols() == JT.nrows() || !"Number of rows of JT was different then number of columns of J");
        assert(J.nrows() == JT.ncols() || !"Number of rows of J was different then number of columns of JT");
        assert( J.size() == JT.size()  || !"J and JT sizes are not the same"                               );
        
        if (init)
        {
            std::fill(D.begin(), D.end(), zero_block<BR>());
        }
        if (D.nrows() != J.nrows())
        {
            D.resize(J.nrows());
        }
        
        typedef typename CompressedRowMatrix<B2>::accessor C_A;
        
        size_t const JT_size = JT.size();
        
        // 2009-06-30 Kenny: Native new/delete may be problematic
        int * JT_idxs = new int[JT_size];
        std::fill(JT_idxs, JT_idxs+JT_size, -1);
        
        typename C_A::cols_container_type const & JT_cols = C_A::cols(JT);
        int * tmp_idx;
        
        
        // Transpose JT and dot every row of J with every row of JT
        //
        // Observation A: 
        //                  There are two elements in every col of JT
        //
        // Observation B:
        //
        //                  Elements of JT are lex-sorted by row-col
        //
        //
        // Observations A and B implies:
        //
        //        First element in JT_cols with value i will have its
        //        corresponding data element v in {JT[0]..JT[JT_size-1]}
        //        placed at index i*2
        //        next element with col-value i is at index i*2+1
        //
        for (size_t i = 0; i < JT_size; ++i)
        {
            // 2009-30-06 Kenny: More pedagogical explanation... I got lost in this indexing?
            tmp_idx = JT_idxs + JT_cols[i]*2;
            *tmp_idx == -1 ? *tmp_idx = i : *(tmp_idx+1) = i;
        }
        
        //
        // JT_idxs now holds indexes to {JT[0]..JT[JT_size-1]}
        // structurally equivalent to J
        // i.e. row(J[i]) == row(JT_idxs[i]) AND col(J[i]) == col(JT_idxs[i])
        // to dot rows, just iterate through J and JT
        // taking advantage of observation A
        typename DiagonalMatrix<BR>::iterator D_iter = D.begin();
        for (size_t i = 0; i < JT_size; ++i, ++D_iter)
        {
            prod(J[i], JT[JT_idxs[i]], *D_iter);
            ++i;
            prod(J[i], JT[JT_idxs[i]], *D_iter);
        }
        delete[] JT_idxs;
    }
    
    /**
     * Diagonal of Matrix Product.
     * This function computes the  blocked diagonal of a matrix product between two compressed row matrices.
     *
     *
     *
     * @tparam B1    The block type of the compressed row matrix.
     * @tparam B2    The block type of the compressed row matrix.
     * @tparam BR    The block type of the resulting block diagonal matrix.
     *
     * @param init   If set to true then the function will initialize the diagonal blocks to zero. Default value is false.
     * @param D      Upon return this diagonal matrix holds the result of the computation. Note that if the dimensions of D does not fit the dimension of the diagonal of the product then D is resized by the function.
     *
     * @warning PRECONDITION: (STRONG!) J(i,j) non-zero <=> JT(j,i) non-zero
     */
    // 2009-06-30 Kenny: Proper ADL for STL dependencies?
    // 2009-06-30 Kenny: Documentation Needed, it is not clear what JT really is. It is not really J transposed, but it will act as J transposed right?
    template <typename B1, typename B2, typename BR>
    inline void diag_of_prod(
                             CompressedRowMatrix<B1> const& J
                             , CompressedRowMatrix<B2> const& JT
                             , DiagonalMatrix<BR>& D
                             , bool init = false
                             )
    {
        assert( J.ncols() == JT.nrows() || !"Number of rows of JT was different then number of columns of J");
        assert( J.nrows() == JT.ncols() || !"Number of rows of J was different then number of columns of JT");
        assert( J.size() == JT.size()   || !"J and JT sizes are not the same"                               );
        
        if (init)
        {
            std::fill(D.begin(), D.end(), zero_block<BR>());
        }
        if (D.nrows() != J.nrows())
        {
            D.resize(J.nrows());
        }
        
        typedef typename CompressedRowMatrix<B2>::accessor C_A;
        
        size_t const JT_size = JT.size();
        
        // 2009-06-30 Kenny: Native new/delete may be problematic
        int * JT_idxs = new int[JT_size];
        std::fill(JT_idxs, JT_idxs+JT_size, -1);
        
        typename C_A::cols_container_type const & JT_cols = C_A::cols(JT);
        int * tmp_idx;
        
        
        // Transpose JT and dot every row of J with every row of JT
        //
        // Observation A: 
        //                  There are two elements in every col of JT
        //
        // Observation B:
        //
        //                  Elements of JT are lex-sorted by row-col
        //
        //
        // Observations A and B implies:
        //
        //        First element in JT_cols with value i will have its
        //        corresponding data element v in {JT[0]..JT[JT_size-1]}
        //        placed at index i*2
        //        next element with col-value i is at index i*2+1
        //
        for (size_t i = 0; i < JT_size; ++i)
        {
            // 2009-30-06 Kenny: More pedagogical explanation... I got lost in this indexing?
            tmp_idx = JT_idxs + JT_cols[i]*2;
            *tmp_idx == -1 ? *tmp_idx = i : *(tmp_idx+1) = i;
        }
        
        //
        // JT_idxs now holds indexes to {JT[0]..JT[JT_size-1]}
        // structurally equivalent to J
        // i.e. row(J[i]) == row(JT_idxs[i]) AND col(J[i]) == col(JT_idxs[i])
        // to dot rows, just iterate through J and JT
        // taking advantage of observation A
        typename DiagonalMatrix<BR>::iterator D_iter = D.begin();
        for (size_t i = 0; i < JT_size; ++i, ++D_iter)
        {
            prod(J[i], JT[JT_idxs[i]], *D_iter);
            ++i;
            prod(J[i], JT[JT_idxs[i]], *D_iter);
        }
        delete[] JT_idxs;
    }
    
    // 2009-06-30 Kenny: Why is there no support for products of compressed row matrices?
    
} // namespace sparse

// SPARSE_DIAG_OF_PROD_H
#endif 
