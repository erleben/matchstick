#ifndef BROAD_ACCESSOR_H
#define BROAD_ACCESSOR_H

namespace broad
{
  template< typename T> class System;           ///< Forward declaration of System class

  namespace detail
  {
    template<typename T>
    class Accessor
      {
      public:
        
        static typename System<T>::grid_type & get_grid(System<T> & sys ){ return sys.m_grid; }
        
        static typename System<T>::object_ptr_container & get_objects( System<T>  & S ){ return S.m_object_ptrs; }
        
      };
    
  } //namespace detail
  
} //namespace broad

// BROAD_ACCESSOR_H
#endif 
