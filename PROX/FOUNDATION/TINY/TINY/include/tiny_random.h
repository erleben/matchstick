#ifndef TINY_RANDOM_H
#define TINY_RANDOM_H

#include <tiny_constants.h> // for zero and one type traits

#include <ctime>           // for std::time()
#include <cstdlib>         // for std::srand() and std::rand()

namespace tiny
{
  
  template <typename T>
  class Random
    {
    protected:
            
      T m_lower;
      T m_upper;
      
    private:
      
      // Make Random class non-assignable and non-copyconstructable
      
      Random(Random const & rnd){}
      
      Random & operator=(Random const & rnd) { return *this; }
      
    protected:
      
      static bool & is_initialized()
      {
        static bool initialized = false;
        return initialized;
      }
      
      void seed()
      {
        using std::time;
        using std::srand;
        if(!is_initialized())
        {
          srand(   static_cast<unsigned int>( time(0) )  );
          is_initialized() = true;
        }
      }
      
    public:
      
      Random() 
      : m_lower( tiny::detail::zero<T>() ) 
      , m_upper( tiny::detail::one<T>()  )
      {
        this->seed();
      }
      
      Random(T const & lower,T const & upper) 
      : m_lower(lower) 
      , m_upper(upper)
      { 
        this->seed();
      }
      
    public:
      
      T operator()() const
      {        
        double rnd = rand() / (1.0*RAND_MAX);
        
        return tiny::detail::numeric_cast<T>(  m_lower + (m_upper-m_lower)*rnd );
      }
      
      bool operator==(Random const & rnd) const { return (m_lower==rnd.m_lower && m_upper==rnd.m_upper);  }
      
    };
  
} // namespace tiny

//TINY_RANDOM_H
#endif
