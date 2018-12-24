#ifndef BROAD_FIND_OVERLAPS_H
#define BROAD_FIND_OVERLAPS_H

#include "broad_system.h"
#include "broad_object.h"
#include "broad_accessor.h"
#include "broad_grid.h"

#include <util_log.h>

#include <cassert>    // needed for assert
#include <algorithm>  // needed for std::pair and std::sort

namespace broad
{  

  /**
   * Algorithm tags, can be used by end-user for making specific algorithm
   * choices. The tags essentially picks the correct overloaded version of
   * the find_overlaps implmentation.
   */
  struct grid_algorithm {};
  struct all_pair_algorithm {};
    
  namespace detail
  {
    template<typename T>
    inline std::pair<Object<T>*,Object<T>*>  make_overlap( Object<T> const * A, Object<T> const * B )
    {          
      assert( A || !"make_overlap(): A object pointer was null");
      assert( B || !"make_overlap(): B object pointer was null");
      
      Object<T> * obj1 = const_cast<Object<T> *> ( A < B ? A : B );
      Object<T> * obj2 = const_cast<Object<T> *> ( A < B ? B : A );
      
      return std::make_pair( obj1, obj2 );
    }
    
  }// namespace detail
  
  /**
   * This function implements a simple ground truth algorithm for comparison purposes.
   *
   * @param efficiency    Upon return this argument gives the ratio of number of found
   *                      overlaps divided by the actual overlap tests done. A ratio of
   *                      close to 1 is optimal whereas a ratio close to zero is very bad.
   */
  template<typename T, typename overlap_container>
  inline bool find_overlaps( 
                              System<T> & sys
                            , overlap_container & overlaps
                            , float & efficiency
                            , all_pair_algorithm const & /*tag*/
                            )
  {
    typedef detail::Accessor<T>                                accessor;
    typedef Object<T>                                          object_type;
    typedef typename System<T>::object_ptr_container           object_ptr_container;
    typedef typename object_ptr_container::iterator            object_ptr_iterator;
    
    // Clean up any potential old left over information
    overlaps.clear();
    
    // Determine box overlaps
    object_ptr_container & objects = accessor::get_objects( sys );
    object_ptr_iterator end   = objects.end();
    object_ptr_iterator iterA = objects.begin();
    
    size_t cnt_tests = 0u;
    
    for(;iterA!=end;)
    {     
      object_type * A = (*iterA);
            
      // Get axis aligned bounding box of object A
      T min_x_A;
      T min_y_A;
      T min_z_A;
      T max_x_A;
      T max_y_A;
      T max_z_A;
      A->get_box( min_x_A, min_y_A, min_z_A, max_x_A, max_y_A, max_z_A);
      
      object_ptr_iterator iterB = ++iterA; 
      for(;iterB!=end;++iterB)
      {
        object_type * B = (*iterB);
                
        // Get axis aligned bounding box of object B
        T min_x_B;
        T min_y_B;
        T min_z_B;
        T max_x_B;
        T max_y_B;
        T max_z_B;
        B->get_box( min_x_B, min_y_B, min_z_B, max_x_B, max_y_B, max_z_B);
        
        ++cnt_tests;
        
        // Test if bounding boxes of A and B are overlapping
        if(max_x_B  < min_x_A) continue;
        if(max_x_A  < min_x_B) continue;        
        if(max_y_B  < min_y_A) continue;
        if(max_y_A  < min_y_B) continue;        
        if(max_z_B  < min_z_A) continue;
        if(max_z_A  < min_z_B) continue;
        
        // Report that we have found an overlap between the bounding boxes of object A and B.
        overlaps.push_back( detail::make_overlap(A, B) );
      }
    }

    // Lexiographic storting of overlaps
    std::sort( overlaps.begin(), overlaps.end() );

    efficiency = 1.0f*overlaps.size() / cnt_tests;

    {
      size_t cnt_obj     = objects.size();
      size_t upper_bound = (cnt_obj*(cnt_obj - 1u))/ 2;

      util::Log logging;
      logging << "broad::find_overlaps(..., all_pair_algorithm): efficiency       = " << efficiency          << util::Log::newline();
      logging << "broad::find_overlaps(..., all_pair_algorithm): #objects         = " << objects.size()      << util::Log::newline();
      logging << "broad::find_overlaps(..., all_pair_algorithm): #bound           = " << upper_bound         << util::Log::newline();
      logging << "broad::find_overlaps(..., all_pair_algorithm): #overlaps        = " << overlaps.size()     << util::Log::newline();
      logging << "broad::find_overlaps(..., all_pair_algorithm): #tests           = " << cnt_tests           << util::Log::newline();
    }

    // Return a status flag indicating whether we have seen an overlap or not
    return (overlaps.size()>0);
  }    
  

  /**
   * This function implements a simple grid based algorithm for broad phase collision detection.
   *
   * @param efficiency    Upon return this argument gives the ratio of number of found
   *                      overlaps divided by the actual overlap tests done. A ratio of
   *                      close to 1 is optimal whereas a ratio close to zero is very bad.
   */
  template<typename T, typename overlap_container>
  inline bool find_overlaps( 
                            System<T> & sys
                            , overlap_container & overlaps
                            , float & efficiency
                            , grid_algorithm const & /*tag*/
                            )
  {
    typedef detail::Accessor<T>                                accessor;
    typedef Object<T>                                          object_type;
    typedef typename System<T>::object_ptr_container           object_ptr_container;
    typedef typename object_ptr_container::iterator            object_ptr_iterator;
    
    typedef detail::Grid<T>                    grid_type;
    typedef detail::Cell<T>                    cell_type;
    typedef typename grid_type::cell_iterator  cell_iterator;
    
    // Clean up any potential old left over information
    overlaps.clear();
    
    // Prepare grid for new collision query
    grid_type & grid = accessor::get_grid( sys );
    grid.clear();
    
    // Reset touched time stamp on all grid cells so we can guard against hash
    // collisions in the grid
    {
      cell_iterator cell       = grid.begin();
      cell_iterator cell_end   = grid.end();
      for( ; cell != cell_end; ++cell)
        cell->touched() = 0u;
    }
    
    // Next we iterate over all objects and map each object into the cells
    // of grids. If a cell already contains any objects then we report overlaps
    // between the already added objects and the newly added object.
    object_ptr_container & objects = accessor::get_objects( sys );
    object_ptr_iterator end   = objects.end();
    object_ptr_iterator iter  = objects.begin();
  
    size_t touched_time        = 0u;
    
    size_t cnt_tests           = 0u;   // Total number of pair wise object tests done
    size_t cnt_hash_collisions = 0u;   // Total number of times we encountered a cell hash collision
    size_t cnt_skipped_tests   = 0u;   // Total number of times we skipped a redundant pair-wise test

    for(; iter!=end; ++iter)
    {
      ++touched_time;
      
      object_type * A = *iter;
      
      // Get axis aligned bounding box of object A
      T min_x_A;
      T min_y_A;
      T min_z_A;
      T max_x_A;
      T max_y_A;
      T max_z_A;
      A->get_box( min_x_A, min_y_A, min_z_A, max_x_A, max_y_A, max_z_A);

      assert(is_number(max_x_A) || !"broad::find_overlaps(): Nan");
      assert(is_number(max_y_A) || !"broad::find_overlaps(): Nan");
      assert(is_number(max_z_A) || !"broad::find_overlaps(): Nan");
      assert(is_number(min_x_A) || !"broad::find_overlaps(): Nan");
      assert(is_number(min_y_A) || !"broad::find_overlaps(): Nan");
      assert(is_number(min_z_A) || !"broad::find_overlaps(): Nan");

      assert(is_finite(max_x_A) || !"broad::find_overlaps(): Inf");
      assert(is_finite(max_y_A) || !"broad::find_overlaps(): Inf");
      assert(is_finite(max_z_A) || !"broad::find_overlaps(): Inf");
      assert(is_finite(min_x_A) || !"broad::find_overlaps(): Inf");
      assert(is_finite(min_y_A) || !"broad::find_overlaps(): Inf");
      assert(is_finite(min_z_A) || !"broad::find_overlaps(): Inf");

      // Get span of cell indices of all cells that overlap with bounding box of object A
      int min_i;
      int min_j;
      int min_k;
      int max_i;
      int max_j;
      int max_k;
      grid.get_cell_indices( min_x_A, min_y_A, min_z_A, min_i, min_j, min_k );
      grid.get_cell_indices( max_x_A, max_y_A, max_z_A, max_i, max_j, max_k );


      int const number_of_cells_to_touch = (max_i-min_i+1)*(max_j-min_j+1)*(max_k-min_k+1);

      if(number_of_cells_to_touch > grid.size())
      {
        util::Log logging;

        logging << "broad::find_overlaps(grid_algorithm): WARNING object spans more cells than grid size" << util::Log::newline();
        logging << "broad::find_overlaps(grid_algorithm): WARNING this suggests bad ratio of object sizes" << util::Log::newline();
        logging << "broad::find_overlaps(grid_algorithm): WARNING or too few cells in grid" << util::Log::newline();
      }

      // Iterate over all grid cells spanned by the bounding box of object A
      for ( int i = min_i; i <= max_i; ++i)
        for ( int j = min_j; j <= max_j; ++j)
          for ( int k = min_k; k <= max_k; ++k)
          {
            cell_type & cell = grid.get_cell(i,j,k);

            // guard against hash collisions
            if(cell.touched() == touched_time)
            {
              ++cnt_hash_collisions;
              continue;
            }
            
            // Iterate over all objects already stored in the grid cell
            size_t const number_of_objs = cell.size( grid.get_time() );
//            {
//              util::Log logging;
//              logging << "broad DEBUG "
//                      << number_of_objs
//                      << " objects in cell("
//                      << i << ","
//                      << j << ","
//                      << k << ")"
//                      << util::Log::newline();
//            }

            for( size_t idx=0u; idx < number_of_objs; ++idx)
            {              
              object_type * B = cell.get_object_ptr( idx );

              // Test if we have already seen B versus A in this query
              if( B->m_seen_by == A )
              {
                ++cnt_skipped_tests;
                continue;
              }

              B->m_seen_by = A;   // Make sure we remember that we have seen (A,B)
               
              // Get axis aligned bounding box of object B
              T min_x_B;
              T min_y_B;
              T min_z_B;
              T max_x_B;
              T max_y_B;
              T max_z_B;
              B->get_box( min_x_B, min_y_B, min_z_B, max_x_B, max_y_B, max_z_B);

              assert(is_number(max_x_B) || !"broad::find_overlaps(): Nan");
              assert(is_number(max_y_B) || !"broad::find_overlaps(): Nan");
              assert(is_number(max_z_B) || !"broad::find_overlaps(): Nan");
              assert(is_number(min_x_B) || !"broad::find_overlaps(): Nan");
              assert(is_number(min_y_B) || !"broad::find_overlaps(): Nan");
              assert(is_number(min_z_B) || !"broad::find_overlaps(): Nan");

              assert(is_finite(max_x_B) || !"broad::find_overlaps(): Inf");
              assert(is_finite(max_y_B) || !"broad::find_overlaps(): Inf");
              assert(is_finite(max_z_B) || !"broad::find_overlaps(): Inf");
              assert(is_finite(min_x_B) || !"broad::find_overlaps(): Inf");
              assert(is_finite(min_y_B) || !"broad::find_overlaps(): Inf");
              assert(is_finite(min_z_B) || !"broad::find_overlaps(): Inf");

              ++cnt_tests;
              
              // Test if bounding boxes of A and B are overlapping
              if(max_x_B  < min_x_A) continue;
              if(max_x_A  < min_x_B) continue;
              
              if(max_y_B  < min_y_A) continue;
              if(max_y_A  < min_y_B) continue;
              
              if(max_z_B  < min_z_A) continue;
              if(max_z_A  < min_z_B) continue;
              
              // Report that we have found an overlap between the bounding boxes of object A and B.
              overlaps.push_back( detail::make_overlap(A, B) );
            }
            
            // Finally we mark the cell as touched by object A
            cell.touched() = touched_time;
            
            // and add object A to the cell
            cell.add( A, grid.get_time() );
            
            A->m_seen_by = 0; // Make sure that there are no left overs when we later query against A
          }
    }
    
    // Lexiographic storting of overlaps, this is to ensure deterministic behaviour
    std::sort( overlaps.begin(), overlaps.end() );

    efficiency = 1.0f*overlaps.size() / cnt_tests;

    {
      size_t cnt_obj     = objects.size();
      size_t upper_bound = (cnt_obj*(cnt_obj - 1u))/ 2;

      util::Log logging;

      logging << "broad::find_overlaps(..., grid_algorithm): efficiency       = " << efficiency          << util::Log::newline();
      logging << "broad::find_overlaps(..., grid_algorithm): #objects         = " << objects.size()      << util::Log::newline();
      logging << "broad::find_overlaps(..., grid_algorithm): #bound           = " << upper_bound         << util::Log::newline();
      logging << "broad::find_overlaps(..., grid_algorithm): #overlaps        = " << overlaps.size()     << util::Log::newline();
      logging << "broad::find_overlaps(..., grid_algorithm): #tests           = " << cnt_tests           << util::Log::newline();
      logging << "broad::find_overlaps(..., grid_algorithm): #skipped tests   = " << cnt_skipped_tests   << util::Log::newline();
      logging << "broad::find_overlaps(..., grid_algorithm): #hash collisions = " << cnt_hash_collisions << util::Log::newline();

      //--- Raw data for making histogram
      //      logging << " H = [";
      //      cell_iterator cell       = grid.begin();
      //      cell_iterator cell_end   = grid.end();
      //      for( ; cell != cell_end; ++cell)
      //        logging << cell->last_size() << ",";
      //      logging << "];"  << util::Log::newline();
    }
    
    // Return a status flag indicating whether we have seen an overlap or not
    return (overlaps.size()>0);
  }
    
  /**
   * Default version of the find-overlaps function.
   *
   * @param efficiency    Upon return this argument gives the ratio of number of found
   *                      overlaps divided by the actual overlap tests done. A ratio of
   *                      close to 1 is optimal whereas a ratio close to zero is very bad.
   */
  template<typename T, typename overlap_container>
  inline bool find_overlaps( System<T> & sys, overlap_container & overlaps, float & efficiency )
  {
    return find_overlaps( sys, overlaps, efficiency, all_pair_algorithm() );
  }
  
} //namespace broad

// BROAD_FIND_OVERLAPS_H
#endif 
