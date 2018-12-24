#ifndef BROAD_GRID_H
#define BROAD_GRID_H

#include <broad_object.h>

#include <tiny_is_finite.h>
#include <tiny_is_number.h>

#include <algorithm>   // needed for std::copy and std::max
#include <vector>      // needed for std::vector
#include <cassert>     // needed for assert
#include <cmath>       // needed for std::pow and std::floor

namespace broad
{
  
  namespace detail
  {
    
    template<typename T>
    class Cell
      {
      public:
        
        typedef Object<T>                               object_type;
        typedef std::vector<object_type*>               object_ptr_container;
        
      protected:
        
        size_t                  m_time_stamp;     ///< Timestamp, used to mark last query when data were stored in the cell.
        size_t                  m_touched_stamp;  ///< Timestamp, used to guard against hash-collisions.
        size_t                  m_size;           ///< The number of objects currently stored in the cell.
        object_ptr_container    m_data;           ///< A reservoir of array entries that can be used for storing objects in the cell.
        
      public:
        
        size_t       & touched()       { return this->m_touched_stamp;    }
        size_t const & touched() const { return this->m_touched_stamp;    }
        
        /**
         * Get the number of objects currently stored in the cell.
         *
         * @param time    The current grid time. This is basically just an unique number identifying the current query that is in progress.
         */
        size_t size(size_t const & time) const 
        { 
          return (this->m_time_stamp == time) ? this->m_size : 0u;             
        }

        /**
         * The size of the cell from last query.
         */
        size_t const & last_size() const { return this->m_size; }

      public:
        
        Cell()
        : m_time_stamp(0u)
        , m_touched_stamp(0u)
        , m_size(0u)
        , m_data()
        {
          this->m_data.resize(8u);  // We assume that at most 8 objects will be mapped to the same hash cell
        }
        
        Cell( Cell const & cell ) { *this = cell; }
        
        Cell & operator=(Cell const & cell )
        {
          if( this != &cell)
          {
            this->m_time_stamp    = cell.m_time_stamp;
            this->m_touched_stamp = cell.m_touched_stamp;
            this->m_size          = cell.m_size;
            this->m_data          = cell.m_data;
          }
          return *this;
        }
        
      public:
        
        /**
         * Add Object to Cell.
         *
         * @param time    The current grid time. This is basically just an unique number identifying the current query that is in progress.
         */
        void add( object_type * obj, size_t const & time )
        {
          using std::copy;
          
          assert( obj  || !"add(): object was null");
          
          // First test if data stored in cell is valid or not
          if( this->m_time_stamp != time )
          {
            this->m_size = 0u;
            this->m_time_stamp = time;
          }
          
          // Second test if we got enough space to add one more object otherwise re-allocate internal storage!
          if(this->m_size >= this->m_data.size())
          {
            object_ptr_container tmp;
            
            tmp = this->m_data;
            this->m_data.resize(2u*this->m_size);
            copy(
                 tmp.begin()
                 , tmp.end()
                 , this->m_data.begin()
                 );
          }
          
          // Finally we can add a pointer to the object
          this->m_data[this->m_size++] = obj;
        }
        
        object_type * get_object_ptr( size_t const & idx )
        {
          assert( idx >= 0u          || !"get_object_ptr(): invalid index");
          assert( idx < this->m_size || !"get_object_ptr(): invalid index");
          return this->m_data[idx];
        }
        
      };
    
    template < typename T >
    class Grid
      {
      public:
        
        typedef Cell<T>                               cell_type;
        typedef typename std::vector< cell_type >     cell_storage;
        typedef typename cell_storage::iterator       cell_iterator;
        
      protected:
        
        size_t                  m_time_stamp;       ///< Query time-stamp. The time stamp of the last valid query. If cells have older time stamps then their data is invalid and can be ignored.
        T                       m_cell_spacing;     ///< Grid cell spacing. 
        size_t                  m_N;                ///< Internal hash function variable, it mimicks the number of cells in a 3D cubic array of cells. The idea is that the number of objects stored is roughly equal to the number of cells needed. Thus O = N*N*N where O is number of objects and N is the number of cells along a side in the cubic cell array.
        cell_storage            m_cells;            ///< Hash table cells.
        
      public:
        
        size_t const & get_time() const { return this->m_time_stamp;   }
        size_t const   size()     const { return this->m_cells.size(); }

        cell_iterator begin() { return m_cells.begin(); }
        cell_iterator end()   { return m_cells.end();   }


      public:
        
        Grid( )
        : m_time_stamp(0u)
        , m_cell_spacing(5.0)
        , m_N(0)
        {
          this->resize(1000u);
        }
        
      public:
        
        /**
         * Resize Grid.
         *
         * @param wanted_size       The number of cells wanted in the grid. Observe that the grid is based
         *                          on a 1D hash tabel of cells. Thus, the actual size of cells used in the
         *                          grid may be much lower than the wanted cell size.
         */
        void resize( size_t const & wanted_size )
        {        
          using std::floor;
          using std::pow;
          using std::max;
          
          size_t const cur_size = this->m_cells.size(); 
          
          if( wanted_size > cur_size )
          {
            // We compute how many hash cells we need if objects are assumed
            // uniformly scattered in space and densely packed in a 3D cubic
            // array of cells.
            // 
            // Let the number of cells along each side of the cubic array be
            // N, let the number of objects be O and assume that we store 1
            // object per cell on average.
            //
            // Then by our assumption we must have the relation
            //
            //        O = N*N*N
            //
            float const O = static_cast<float>( wanted_size );
            float const N = pow( O , 1.0f/3.0f);
            
            this->m_N  = static_cast<int>( floor(N + 0.5f) );  // Make sure we have a proper conversion from float-value to an integer value
            size_t const four = 4u;
            this->m_N  = max( four, this->m_N);                // Clamp the cell array side to have at least 64 cells, the number 64 is just a random choice
            
            size_t const new_size = (this->m_N)*(this->m_N)*(this->m_N);
            
            this->m_cells.resize( new_size, cell_type() );        
          }
        }
        
        void set_spacing( T const & value )
        {
          assert( value > 0 || !"set_spacing(): spacing must be a positive value");

          this->m_cell_spacing = value;
        }
        
        T get_spacing( ) const { return this->m_cell_spacing; }
        
        cell_type & get_cell(int i, int j, int k)
        {
          // The model of the hash grid is a cubic cell grid of N*N*N cells
          int const N = this->m_N;
          int const C = this->m_cells.size();
          
          // First we simply map the (i,j,k) world cell position into the N*N*N dense grid cell position
          // This is done using a compenent-wise independent periodic mapping
          while(i<0)
            i += N;
          while(j<0)
            j += N;
          while(k<0)
            k += N;

          while(i>=N)
            i -= N;
          while(j>=N)
            j -= N;
          while(k>=N)
            k -= N;
                  
          // Next the cubic dense cell position is transformed from a 3D index
          // into a 1D index, assuming the N*N*N cell array are stored in a
          // traditional row-major fashion. Afterwards the 1D flat index is
          // taken modulo to the number of hash cells.
          int hash_key = (  (k*N + j)*N + i ) % C;
          
          return this->m_cells[ hash_key ];
        }
        
        void get_cell_indices(T const & x, T const & y, T const & z, int & i,int & j,int & k) const
        {
          using std::floor;
          
          T const & dx = this->m_cell_spacing;

          assert( dx > 0 || !"broad::Grid::get_cell_indices(): spacing was non-positive");

          assert( is_finite(x) || !"broad::Grid::get_cell_indices(): Inf encountered");
          assert( is_finite(y) || !"broad::Grid::get_cell_indices(): Inf encountered" );
          assert( is_finite(z) || !"broad::Grid::get_cell_indices(): Inf encountered" );

          assert( is_number(x) || !"broad::Grid::get_cell_indices(): Nan encountered" );
          assert( is_number(y) || !"broad::Grid::get_cell_indices(): Nan encountered" );
          assert( is_number(z) || !"broad::Grid::get_cell_indices(): Nan encountered" );

          i = static_cast<int>( floor( x / dx ) );
          j = static_cast<int>( floor( y / dx ) );
          k = static_cast<int>( floor( z / dx ) );

        }
        
        void clear()
        {
          ++(this->m_time_stamp); /* Lazy deallocation */
        }
        
      };
    
  } // namespace detail
} // namespace broad

// BROAD_GRID_H
#endif
