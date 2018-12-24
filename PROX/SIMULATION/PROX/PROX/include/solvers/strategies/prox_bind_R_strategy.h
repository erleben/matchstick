#ifndef PROX_BIND_R_STRATEGY_H
#define PROX_BIND_R_STRATEGY_H

#include <prox_enums.h>

#include <solvers/strategies/prox_R_strategy.h>
#include <solvers/strategies/prox_block_R_strategy.h>
#include <solvers/strategies/prox_local_R_strategy.h>
#include <solvers/strategies/prox_global_R_strategy.h>

#include <util_log.h>

#include <cassert>

namespace prox
{
  
  /**
   * R-factor strategy function pointer binder.
   */  
  template<typename M>
  class RStrategyBinder
  : public RStrategy<M>
  {
  public:
    
    typedef void func_type(  typename M::compressed4x6_type const & J
                                 , typename M::compressed6x4_type const & WJT
                                 , typename M::diagonal4x4_type & R
                                 , typename M::diagonal4x4_type & nu
                                 , M const & tag );
    
    func_type * m_strategy;
    
  public:
    
    RStrategyBinder()
    : m_strategy(0)
    {
    }
    
    RStrategyBinder(func_type * strategy)
    : m_strategy(strategy)
    {
    }
    
  public:
    
    void operator()(
                    typename M::compressed4x6_type const & J
                    , typename M::compressed6x4_type const & WJT
                    , typename M::diagonal4x4_type & R
                    , typename M::diagonal4x4_type & nu
                    ) const
    {
      assert( this->m_strategy || !"RStrategyBinder(): strategy was null");
      
      this->m_strategy( J, WJT, R, nu, M() );
    }
    
  };
    
  /**
   *
   */     
  template<typename M>
  inline RStrategyBinder<M> bind_strategy( strategy_type const & type )
  {
    util::Log logging;

    switch( type )
    {
      case local_strategy:
        logging << "bind_strategy(): using local r-factor strategy"<< util::Log::newline();
        return RStrategyBinder<M>( &detail::local_R_strategy<M> );

      case global_strategy:
        logging << "bind_strategy(): using global r-factor strategy"<< util::Log::newline();
        return RStrategyBinder<M>( &detail::global_R_strategy<M> );

      case blocked_strategy:
        logging << "bind_strategy(): using blocked r-factor strategy"<< util::Log::newline();
        return RStrategyBinder<M>( &detail::block_R_strategy<M> );

      default:
        assert(!"bind_strategy(): unknown strategy type");
        break;
    };
    
    return RStrategyBinder<M>();
  }
  
} //namespace prox

// PROX_BIND_R_STRATEGY_H
#endif
