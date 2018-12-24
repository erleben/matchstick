#ifndef PROX_R_STRATEGY_H
#define PROX_R_STRATEGY_H

namespace prox
{

  /**
   * A R-factor strategy functor.
   */  
  template<typename M>
  class RStrategy
  {    
  public:
    
    virtual void operator()(
                      typename M::compressed4x6_type const & J
                    , typename M::compressed6x4_type const & WJT
                    , typename M::diagonal4x4_type & R
                    , typename M::diagonal4x4_type & nu
                    ) const = 0;
    
  };
    
} //namespace prox

// PROX_R_STRATEGY_H
#endif
