#ifndef UTIL_PROFILING_H
#define UTIL_PROFILING_H

#include <util_timer.h>
#include <util_string_helper.h>

#include <map>
#include <vector>
#include <cmath>
#include <sstream>
#include <cassert>

#ifdef USE_PROFILING

#define START_TIMER(NAME)         util::Profiling::get_timer_monitor(NAME)->start()
#define PAUSE_TIMER(NAME)         util::Profiling::get_timer_monitor(NAME)->pause()
#define RESUME_TIMER(NAME)        util::Profiling::get_timer_monitor(NAME)->resume()
#define STOP_TIMER(NAME)          util::Profiling::get_timer_monitor(NAME)->stop()
#define RECORD_TIME(NAME,VALUE)   util::Profiling::get_timer_monitor(NAME)->record(VALUE)
#define RECORD(NAME,VALUE)        util::Profiling::get_monitor(NAME)->record(VALUE)
#define NEW_UNIQUE_NAME(NAME)     util::Profiling::generate_unique_name(NAME)
#define RECORD_VECTOR(NAME,VALUE) util::Profiling::get_vector_monitor(NAME)->record(VALUE)


#define RECORD_VECTOR_NEW(NAME) util::Profiling::get_vector_monitor(NAME)->record_new()
#define RECORD_VECTOR_PUSH(NAME,VALUE) util::Profiling::get_vector_monitor(NAME)->record_push(VALUE)


#else

#define START_TIMER(NAME)
#define PAUSE_TIMER(NAME)
#define RESUME_TIMER(NAME)
#define STOP_TIMER(NAME)
#define RECORD_TIME(NAME,VALUE)
#define RECORD(NAME,VALUE)
#define NEW_UNIQUE_NAME(NAME)  ""

#endif

/**
 * Macro for settig prefix string values for names.
 * When profiling sub-routines used for solving different problems
 * (like a Gauss-Seidel sovler being used for either dynamics or
 * post-stabilization) then one wish to be able to distinguish data comming
 * from different problems.
 *
 * In order to not obscure sub-routines with profiling-caller information
 * one may use this PREFIX macro to "push" a prefix string value onto all
 * names that will be subsequently used.
 *
 * One must be careful to clear the prefix value to the empty string to insure
 * that "naming" behaviour returns to "normal-mode" onece there is no need
 * for this "override" behavior.
 */
#define PREFIX(VALUE)             util::Profiling::prefix() = VALUE


namespace util
{

  /**
   * Profiling Class.
   * This class provides functionality for monitoring/handling multiple
   * timers and recording profiling data.
   */
  class Profiling
  {
  public:

    class Monitor
    {
    protected:

      float       m_total;
      float       m_min;
      float       m_max;
      float       m_avg;
      size_t      m_N;
      std::vector<float> m_values;

    public:

      float get_avg() const    { return m_avg;    }
      float get_min() const    { return m_min;    }
      float get_max() const    { return m_max;    }
      float get_total() const  { return m_total;  }
      std::vector<float> const & get_values() const { return m_values; }
      size_t get_number_of_entries() const  { return m_N;  }

    public:

      Monitor()
      : m_total( 0.0f )
      , m_min( 0.0f )
      , m_max( 0.0f )
      , m_N( 0u )
      , m_values()
      {}

    public:

      void record( float const & value )
      {
        using std::min;
        using std::max;

        m_values.push_back( value );
        m_min   = min( m_min, value );
        m_max   = max( m_max, value );
        m_avg   =  (m_N*m_avg + value)/ (m_N+1);
        m_N     = m_N + 1;
        m_total = m_total + value;
      }

      void clear()
      {
        using std::min;
        using std::max;

        m_values.clear();
        m_min   = 0.0f;
        m_max   = 0.0f;
        m_avg   = 0.0f;
        m_N     = 0u;
        m_total = 0.0f;
      }
    };

    class TimerMonitor
    : public Monitor
    {
    protected:

      util::Timer m_timer;
      float       m_time;

    public:

      TimerMonitor()
      : Monitor()
      , m_timer()
      , m_time(0.0f)
      {}

    public:

      void start()
      {
        m_timer.start();
      }

      void pause()
      {
        m_timer.stop();
        m_time += m_timer();
      }

      void resume()
      {
        start();
      }

      void stop()
      {
        pause();
        Monitor::record( m_time );
        m_time = 0.0f;
      }

      void set_time(float const & time)
      {
        m_time = time;
      }

      void add_time(float const & time)
      {
        m_time += time;
      }

      float get_time() const
      {
        return m_time;
      }

    };

    class VectorMonitor
    {
    protected:

      std::vector< std::vector<float>  > m_values;

    public:

      std::vector<float> const & get_values(size_t const & idx) const { return m_values[idx]; }
      size_t get_number_of_entries() const  { return m_values.size();  }

    public:

      VectorMonitor()
      : m_values()
      {}

    public:

      void record( std::vector<float> const & values )
      {
        m_values.push_back( values );
      }

      void record_new( )
      {
        m_values.push_back( std::vector<float>() );
      }

      void record_push( float const & value )
      {
        m_values.back().push_back( value );
      }

      void clear()
      {
        using std::min;
        using std::max;

        m_values.clear();
      }
    };

    typedef std::map<std::string, Monitor >               monitors_container;
    typedef std::map<std::string, TimerMonitor>           timer_monitors_container;
    typedef std::map<std::string, VectorMonitor >         vector_monitors_container;
    typedef std::map<std::string, size_t >                counter_container;

  private:

    static counter_container & counters()
    {
      static counter_container data;

      return data;
    }

  public:

    static std::string & prefix()
    {
      static std::string prefix_value = "";

      return prefix_value;
    }

    static std::string generate_unique_name(std::string const & name)
    {
      std::string const decorated_name = prefix() + name;

      assert( !util::contains(decorated_name, " ") || !"util::Profiling::get_last_monitor_name(): spaces not allowed in names");

      size_t const value = counters()[decorated_name];

      ++counters()[decorated_name];

      std::ostringstream s;

      s << decorated_name << "_" << value;

      return s.str();
    }

    static std::string get_last_monitor_name(std::string const & name)
    {
      std::string const decorated_name = prefix() + name;

      assert( !util::contains(decorated_name, " ") || !"util::Profiling::get_last_monitor_name(): spaces not allowed in names");

      size_t const value = counters()[decorated_name]-1; // 20XX-YY-ZZ: Sarah FIX THIS: -1 is because we are off by
                                                         // one when engine::get_convergence is called, this is because
                                                         // there is no collisions detected in frame 0. Does this
                                                         // mean we are 'behind' on our collision information and
                                                         // what issues are involved in that case???

      std::ostringstream s;

      s << decorated_name << "_" << value;

      return s.str();
    }

  public:

    static monitors_container * get_monitors_instance()
    {
      static monitors_container  monitors;

      return &monitors;
    }

    static timer_monitors_container * get_timer_monitors_instance()
    {
      static timer_monitors_container  monitors;

      return &monitors;
    }

    static vector_monitors_container * get_vector_monitors_instance()
    {
      static vector_monitors_container  monitors;

      return &monitors;
    }

  public:
    
    static Monitor * get_monitor(std::string const & name)
    {
      std::string const decorated_name = prefix() + name;

      assert( !util::contains(decorated_name, " ") || !"util::Profiling::get_monitor(): spaces not allowed in names");

      return &(*Profiling::get_monitors_instance())[decorated_name];
    }

    
    static TimerMonitor * get_timer_monitor(std::string const & name)
    {
      std::string const decorated_name = prefix() + name;

      assert( !util::contains(decorated_name, " ") || !"util::Profiling::get_timer_monitor(): spaces not allowed in names");

      return &(*Profiling::get_timer_monitors_instance())[decorated_name];
    }


    static VectorMonitor * get_vector_monitor(std::string const & name)
    {
      std::string const decorated_name = prefix() + name;

      assert( !util::contains(decorated_name, " ") || !"util::Profiling::get_vector_monitor(): spaces not allowed in names");

      return &(*Profiling::get_vector_monitors_instance())[decorated_name];
    }

  public:

    /**
     * Reset Profiling Data.
     * Clears all collected data sofar.
     */
    static void reset()
    {
      get_monitors_instance()->clear();

      get_timer_monitors_instance()->clear();

      get_vector_monitors_instance()->clear();

      counters().clear();
    }
    
  };
  
}//namespace util

// UTIL_PROFILING_H
#endif
