#ifndef GEOMETRY_DIRECTION_TABLE_H
#define GEOMETRY_DIRECTION_TABLE_H

#include <cstddef> // Needed for size_t
#include <cassert> 

namespace geometry
{
  
  template<typename V, size_t N>
  class DirectionTable
  {
  protected:
    
    V m_directions[N];
    
  public:
    
    V const & operator()(size_t const & idx) const
    {
      assert( idx < N || !"DirectionTable::operator(): idx was out of range");
      return this->m_directions[idx];
    }
    
    V       & operator()(size_t const & idx)
    {
      assert( idx < N || !"DirectionTable::operator(): idx was out of range");
      return this->m_directions[idx];
    }
    
    size_t size() const { return N; }
    
  public:
    
    DirectionTable()
    {}
    
  };
  
  template<typename V, size_t N1, size_t N2>
  inline DirectionTable<V,N1+N2> make_union( DirectionTable<V,N1> const & D1, DirectionTable<V,N2> const & D2)
  {
    DirectionTable<V,N1+N2> D;
    
    for(size_t n = 0u; n < N1; ++n)
      D(n)    = D1(n);
    for(size_t n = 0u; n < N2; ++n)
      D(n+N1) = D2(n);
    
    return D;
  }

  template<typename V>
  inline DirectionTable<V,3>  make3()
  {
    typedef typename V::value_traits VT;
    
    DirectionTable<V,3> D;
    
    D(0) =  V::make( VT::one(),  VT::zero(), VT::zero() );
    D(1) =  V::make( VT::zero(), VT::one(),  VT::zero() );
    D(2) =  V::make( VT::zero(), VT::zero(), VT::one()  );
    
    return D;
  }
  
  template<typename V>
  inline DirectionTable<V,4>  make4()
  {
    typedef typename V::value_traits VT;

    DirectionTable<V,4> D;
    
    D(0) = unit( V::make(  VT::one(),  VT::one(),  VT::one() ) );
    D(1) = unit( V::make(  VT::one(),  VT::one(), -VT::one() ) );
    D(2) = unit( V::make(  VT::one(), -VT::one(),  VT::one() ) );
    D(3) = unit( V::make(  VT::one(), -VT::one(), -VT::one() ) );
    
    return D;
  }
  
  template<typename V>
  inline DirectionTable<V,6> make6()
  {
    typedef typename V::value_traits VT;

    DirectionTable<V,6> D;
    
    D(0) = unit( V::make(  VT::one(),   VT::one(),  VT::zero()) );
    D(1) = unit( V::make(  VT::one(),  -VT::one(),  VT::zero()) );
    D(2) = unit( V::make(  VT::one(),   VT::zero(), VT::one() ) );
    D(3) = unit( V::make(  VT::zero(),  VT::one(),  VT::one() ) );
    D(4) = unit( V::make( -VT::one(),   VT::zero(), VT::one() ) );
    D(5) = unit( V::make(  VT::zero(), -VT::one(),  VT::one() ) );
    
    return D;
  }
  
  template<typename V>
  inline DirectionTable<V,7>  make7()
  {
    return make_union( make3<V>(), make4<V>() );
  }
  
  template<typename V>
  inline DirectionTable<V,9>  make9()
  {
    return make_union( make3<V>(), make6<V>() );
  }
  
  template<typename V>
  inline DirectionTable<V,10>  make10()
  {
    return make_union( make4<V>(), make6<V>() );
  }
  
  template<typename V>
  inline DirectionTable<V,13>  make13()
  {
    return make_union( make7<V>(), make6<V>() );
  }
  
  template<typename V, size_t N> class DirectionTableHelper;
  
  template<typename V>
  class DirectionTableHelper<V,3>
  {
  public:
    static DirectionTable<V,3> make() { return make3<V>(); }
  };

  template<typename V>
  class DirectionTableHelper<V,4>
  {
  public:
    static DirectionTable<V,4> make() { return make4<V>(); }
  };
  
  template<typename V>
  class DirectionTableHelper<V,6>
  {
  public:
    static DirectionTable<V,6> make() { return make6<V>(); }
  };
  
  template<typename V>
  class  DirectionTableHelper<V,7>
  {
  public:
    static DirectionTable<V,7> make() { return make7<V>(); }
  };
  
  template<typename V>
  class  DirectionTableHelper<V,9>
  {
  public:
    static DirectionTable<V,9> make() { return make9<V>(); }
  };
  
  template<typename V>
  class  DirectionTableHelper<V,10>
  {
  public:
    static DirectionTable<V,10> make() { return make10<V>(); }
  };
  
  template<typename V>
  class  DirectionTableHelper<V,13>
  {
  public:
    static DirectionTable<V,13> make() { return make13<V>(); }
  };

}// namespace geometry

// GEOMETRY_DIRECTION_TABLE_H
#endif