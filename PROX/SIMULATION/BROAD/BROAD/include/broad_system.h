#ifndef BROAD_SYSTEM_H
#define BROAD_SYSTEM_H

#include "broad_object.h"
#include "broad_accessor.h"
#include "broad_grid.h"

#include <util_log.h>

#include <vector>
#include <algorithm>  // needed for std::find and std::pair and std::min and std::max
#include <limits>     // needed for std::numeric_limits

namespace broad
{
  
  template<typename T>
  class System
    {
    public:
      
      friend class detail::Accessor<T>;
      
      typedef detail::Accessor<T>                   accessor;
      typedef Object<T>                             object_type;
      typedef std::vector<object_type*>             object_ptr_container;
      typedef std::pair<object_type*,object_type*>  overlap_type;
      
    protected:
      
      typedef detail::Grid<T>                       grid_type;
      
    protected:
      
      object_ptr_container      m_object_ptrs;
      grid_type                 m_grid;
      
      T m_min_span_x;        ///< Minimum span of object bounding boxes
      T m_min_span_y;
      T m_min_span_z;
            
    protected:
      
      System( System const & sys);
      System& operator=( System const & sys);
      
    public:
      
      System()
      : m_object_ptrs()
      , m_min_span_x( std::numeric_limits<T>::max()  )
      , m_min_span_y( std::numeric_limits<T>::max()  )
      , m_min_span_z( std::numeric_limits<T>::max()  )
      {}
      
      ~System()
      {
        this->clear();
      }

    protected:

      /**
       * Find the median of the sorted array of values.
       *
       * @param values   An array of sorted values.
       *
       * @return         The median value.
       */
      T median(std::vector<T> const & values)
      {
        size_t const N = values.size();

        assert(N>0u || !"broad::System()::median(): Can not find median of empty vector");

        if (N  % 2 == 0)
        {
          return (values[N / 2 - 1] + values[N / 2]) / 2;
        }
        return values[N / 2];
      }

    public:

      /**
       * This method analyses the object sizes and tries to pick a
       * cell size that is the best compromise.
       *
       * The method assumes that all objects have been "connected"
       * to the system.
       */
      void compute_optimal_cell_spacing()
      {
        using std::max;
        using std::ceil;

        size_t const N = this->m_object_ptrs.size();

        if(N==0)
        {
          util::Log logging;

          logging << "broad::System::compute_optimal_cell_spacing(): No data in system,"
                  << "can not determine spacing, using default value of 1.0"
                  << util::Log::newline();

          this->m_grid.set_spacing( 1.0f );
          return;
        }

        std::vector<T> span_x;
        std::vector<T> span_y;
        std::vector<T> span_z;

        span_x.resize(N);
        span_y.resize(N);
        span_z.resize(N);

        typename std::vector<object_type*>::const_iterator obj_ptr = this->m_object_ptrs.begin();
        typename std::vector<object_type*>::const_iterator end     = this->m_object_ptrs.end();

        for(size_t i = 0u;obj_ptr!=end; ++obj_ptr,++i)
        {
          T min_x;
          T min_y;
          T min_z;
          T max_x;
          T max_y;
          T max_z;
          (*obj_ptr)->get_box( min_x, min_y, min_z, max_x, max_y, max_z);

          span_x[i]  = max_x - min_x;
          span_y[i]  = max_y - min_y;
          span_z[i]  = max_z - min_z;
        }

        std::sort( span_x.begin(), span_x.end() );
        std::sort( span_y.begin(), span_y.end() );
        std::sort( span_z.begin(), span_z.end() );

        T const median_x = median(span_x);
        T const median_y = median(span_y);
        T const median_z = median(span_z);

        T const optimal_spacing = max(median_x,max(median_y,median_z))*2;

        {
          util::Log logging;
          logging << "broad::System::compute_optimal_cell_spacing(): optimal spacing found to be = " << optimal_spacing << util::Log::newline();
        }

        this->m_grid.set_spacing( optimal_spacing );

        //--- Now make sure to allocate sufficiently many cells to cover the
        //--- whole scene
        T const width  = span_x[N-1u] - span_x[0u];
        T const height = span_y[N-1u] - span_y[0u];
        T const depth  = span_z[N-1u] - span_z[0u];

        size_t const cnt_x = ceil(width  / optimal_spacing );
        size_t const cnt_y = ceil(height / optimal_spacing );
        size_t const cnt_z = ceil(depth  / optimal_spacing );

        size_t const M = max(cnt_x,max(cnt_y,cnt_z));

        this->m_grid.resize(M*M*M);
      }

      void connect( object_type * obj )
      {
        using std::min;
        using std::max;
        
        assert(obj || !"connect() obj was null");
        assert( std::find( this->m_object_ptrs.begin(), this->m_object_ptrs.end(), obj) == this->m_object_ptrs.end() || !"connect() obj already connected");
        
        // Compute some statistics in order to pick a "good" cell spacing of the grid
        // Here we just do something simple, we pick two times the average box size as the cell size!
        {
          // Get bounding box of object
          T min_x;
          T min_y;
          T min_z;
          T max_x;
          T max_y;
          T max_z;
          obj->get_box( min_x, min_y, min_z, max_x, max_y, max_z);
          
          // Compute span of bounding box
          T const span_x  = max_x - min_x;
          T const span_y  = max_y - min_y;
          T const span_z  = max_z - min_z;
          
          // Find minimum spans. Determine simple first order statistics about all boxes that have been added to the system so far.
          this->m_min_span_x = min( this->m_min_span_x, span_x );
          this->m_min_span_y = min( this->m_min_span_y, span_y );
          this->m_min_span_z = min( this->m_min_span_z, span_z );
          
          // Now we use the statistics to set a cell size of the grid.
          T const new_spacing = max( this->m_min_span_x, max( this->m_min_span_y, this->m_min_span_z ) ) * 2;
          
          this->m_grid.set_spacing( new_spacing );
        }
        
        this->m_object_ptrs.push_back( obj );
        this->m_grid.resize ( this->m_object_ptrs.size() );  // Remember to resize grid so it got space for the objects.
      }
      
      void disconnect( object_type * obj )
      {
        assert(obj || !"disconnect() obj was null");        
        assert( std::find( this->m_object_ptrs.begin(), this->m_object_ptrs.end(), obj) != this->m_object_ptrs.end() || !"disconnect() obj was not connected");
        
        this->m_object_ptrs.remove( obj );
      }
      
      void clear()
      {
        this->m_object_ptrs.clear();
      }
      
    };
  
  /**
   *
   * Factory function for creating a broad phase collision detection system.
   */
  template<typename configuration_type, typename T>
  inline void make( configuration_type /*const*/ & data, System<T> & sys)
  {
    // Cleanup any leftovers
    sys.clear();
    
    // Initialize scene by filling it with the boxes of the objects
    for( typename configuration_type::iterator obj = data.begin(); obj!=data.end();++obj)
      sys.connect( &(*obj) );
  }
  
} //namespace broad

// BROAD_SYSTEM_H
#endif 
