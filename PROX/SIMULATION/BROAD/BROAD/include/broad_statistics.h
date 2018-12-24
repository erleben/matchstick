#ifndef BROAD_STATISTICS_H
#define BROAD_STATISTICS_H

#include "broad_system.h"
#include "broad_object.h"
#include "broad_accessor.h"
#include "broad_grid.h"

#include <cmath>
#include <sstream>

namespace broad
{  
  
  
  /**
   * Data Container for Grid based broad phase collision detection.
   */
  class Statistics
  {
  public:
    
    size_t m_total_cells;
    size_t m_nonempty_cells;
    float  m_fill_ratio;
    size_t m_min_obj;
    size_t m_max_obj;
    float  m_avg_obj;
    float  m_std_obj;
    float  m_min_x;
    float  m_min_y;
    float  m_min_z;
    float  m_max_x;
    float  m_max_y;
    float  m_max_z;
    float  m_avg_x;
    float  m_avg_y;
    float  m_avg_z;
    float  m_std_x;
    float  m_std_y;
    float  m_std_z;
    size_t m_total_objects;
    float  m_cell_x;
    float  m_cell_y;
    float  m_cell_z;
    
  };
  
  
  /**
   * Make Statistics for Grid based Broad Phase Collision Detection.
   *
   * @param sys     The broad phase collision detection system.
   *
   * @return        A data container holding all the statistics that is computed.
   */
  template<typename T>
  inline Statistics make_statistics(System<T> & sys )
  {    
    using std::min;
    using std::max;
    using std::sqrt;
    
    typedef detail::Accessor<T>                                accessor;
    typedef Object<T>                                          object_type;
    typedef typename System<T>::object_ptr_container           object_ptr_container;
    typedef typename object_ptr_container::iterator            object_ptr_iterator;
    
    typedef detail::Grid<T>                    grid_type;
    typedef typename grid_type::cell_iterator  cell_iterator;
    
    Statistics stats;
    
    grid_type & grid = accessor::get_grid( sys );
    
    size_t K       = 0u;        // Total number of nonempty cells
    size_t N       = 0u;        // Total number of cells    
    size_t min_val = 0xFFFFu;   // Minimum number of objects in a nonempty cell
    size_t max_val = 0u;        // Maximum number of objects in a nonempty cell 
    float avg_val  = 0.0f;      // Average number of objects in a nonempty cell
    float std_val  = 0.0f;      // Standard deviation of number of objects in a nonempty cell
    
    {
      cell_iterator cell       = grid.begin();
      cell_iterator cell_end   = grid.end();
      for( ; cell != cell_end; ++cell)
      {
        size_t val = cell->size( grid.get_time() );
        
        if(val>0u)
        {
          min_val = min(min_val, val);
          max_val = max(max_val, val);
          avg_val = avg_val + val;        
          K++;
        }
        else
        {
        }
        N++;
      }
      avg_val /= N;
    }
    float fill_ratio = 1.0f*K / N;   // The fill ratio of the grid -- if all cells got something in them this should be close to 1.
    {
      cell_iterator cell       = grid.begin();
      cell_iterator cell_end   = grid.end();
      for( ; cell != cell_end; ++cell)
      {
        float val = cell->size( grid.get_time() );
        if(val>0.0f)
        {
          val = (val - avg_val)*(val - avg_val);        
          std_val = std_val + val;
        }
      }
      std_val = sqrt(std_val / N);
    }
    
    stats.m_total_cells    = N;
    stats.m_nonempty_cells = K;
    stats.m_fill_ratio     = fill_ratio;
    stats.m_min_obj = min_val;
    stats.m_max_obj = max_val;
    stats.m_avg_obj = avg_val;
    stats.m_std_obj = std_val;
    
    stats.m_cell_x = grid.get_spacing();
    stats.m_cell_y = grid.get_spacing();
    stats.m_cell_z = grid.get_spacing();
    
    float min_x = 10e15f;
    float min_y = 10e15f;
    float min_z = 10e15f;
    float max_x = 0.0f;
    float max_y = 0.0f;
    float max_z = 0.0f;
    float avg_x = 0.0f;
    float avg_y = 0.0f;
    float avg_z = 0.0f;
    float std_x = 0.0f;
    float std_y = 0.0f;
    float std_z = 0.0f;
    size_t B = 0u;   // Total number of objects
    
    {
      object_ptr_container & objects = accessor::get_objects( sys );
      object_ptr_iterator end   = objects.end();
      object_ptr_iterator iter  = objects.begin();
      for(; iter!=end; ++iter)
      {
        object_type * obj = *iter;
        
        T mx;
        T my;
        T mz;
        T Mx;
        T My;
        T Mz;
        obj->get_box( mx, my, mz, Mx, My, Mz);
        
        float sx = Mx - mx;
        float sy = My - my;
        float sz = Mz - mz;
        
        min_x = min( min_x, sx );
        min_y = min( min_y, sy );
        min_z = min( min_z, sz );
        max_x = max( max_x, sx );
        max_y = max( max_y, sy );
        max_z = max( max_z, sz );
        
        avg_x += sx;
        avg_y += sy;
        avg_z += sz;
        
        B++;        
      }
    }
    avg_x /= B;
    avg_y /= B;
    avg_z /= B;
    {
      object_ptr_container & objects = accessor::get_objects( sys );
      object_ptr_iterator end   = objects.end();
      object_ptr_iterator iter  = objects.begin();
      for(; iter!=end; ++iter)
      {
        object_type * obj = *iter;
        
        T mx;
        T my;
        T mz;
        T Mx;
        T My;
        T Mz;
        obj->get_box( mx, my, mz, Mx, My, Mz);
        
        float sx = (Mx - mx) - avg_val;
        float sy = (My - my) - avg_val;
        float sz = (Mz - mz) - avg_val;
        
        std_x += sx*sx;
        std_y += sy*sy;
        std_z += sz*sy;
      }
      std_x = sqrt( std_x / B );
      std_y = sqrt( std_y / B );
      std_z = sqrt( std_z / B );
    }
    
    stats.m_min_x = min_x;
    stats.m_min_y = min_y;
    stats.m_min_z = min_z;
    stats.m_max_x = max_x;
    stats.m_max_y = max_y;
    stats.m_max_z = max_z;
    stats.m_avg_x = avg_x;
    stats.m_avg_y = avg_y;
    stats.m_avg_z = avg_z;
    stats.m_std_x = std_x;
    stats.m_std_y = std_y;
    stats.m_std_z = std_z;
    
    stats.m_total_objects = B;
    
    return stats;
  }
  
  
  /**
   * Converts broad phase grid statistics into a human readable string.
   *
   * @param stats   The statistics data from a grid broad phase collision detection algorithm.
   *
   * @return        The resulting string value.
   */
  inline std::string stats2str( Statistics const & stats )
  {
    std::ostringstream s;
    
    s << "cells = " << stats.m_total_cells << std::endl;
    s << "\tnon empty cells   = " << stats.m_nonempty_cells << std::endl;
    s << "\tfill ratio        = " << stats.m_fill_ratio << std::endl;
    s << "\t(min,max,avg,std) = " << "(" << stats.m_min_obj << "," << stats.m_max_obj << "," << stats.m_avg_obj << "," << stats.m_std_obj << ")" << std::endl;
    s << "\tcell = " << "(" << stats.m_cell_x << "," << stats.m_cell_y << "," << stats.m_cell_z << ")" << std::endl;
    s << "objects = " << stats.m_total_objects << std::endl;
    s << "\tmin box = " << "(" << stats.m_min_x << "," << stats.m_min_y << "," << stats.m_min_z << ")" << std::endl;
    s << "\tmax box = " << "(" << stats.m_max_x << "," << stats.m_max_y << "," << stats.m_max_z << ")" << std::endl;
    s << "\tavg box = " << "(" << stats.m_avg_x << "," << stats.m_avg_y << "," << stats.m_avg_z << ")" << std::endl;
    s << "\tstd box = " << "(" << stats.m_std_x << "," << stats.m_std_y << "," << stats.m_std_z << ")" << std::endl;
    
    return s.str();    
  }
    
} //namespace broad

// BROAD_STATISTICS_H
#endif 
