#ifndef UTIL_TIMER_H
#define UTIL_TIMER_H

#include <chrono>

#include <util_log.h>

#include <cassert>
#include <sstream>   // needed for std::stringstream

namespace util 
{

  template <class Clock>
  std::string display_clock_precision()
  {
    std::stringstream text;

    typedef std::chrono::duration<double, std::nano> NS;

    NS const ns = typename Clock::duration(1);
    text << ns.count() << " ns";

    return text.str();
  }

  inline void print_timers_info()
  {
    Log logging;

    logging << "steady_clock precision          : " << display_clock_precision<std::chrono::steady_clock>()          << Log::newline();
    logging << "steady_clock is steady          : " << std::chrono::steady_clock::is_steady                          << Log::newline();
    logging << "system_clock precision          : " << display_clock_precision<std::chrono::system_clock>()          << Log::newline();
    logging << "system_clock is steady          : " << std::chrono::system_clock::is_steady                          << Log::newline();
    logging << "high_resolution_clock precision : " << display_clock_precision<std::chrono::high_resolution_clock>() << Log::newline();
    logging << "high_resolution_clock is steady : " << std::chrono::high_resolution_clock::is_steady                 << Log::newline();
  }

  class Timer
  {
  protected:

    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_end;

    bool m_start_called;

  public:

    Timer()
    : m_start_called(false)
    {}

  public:

    void start()
    {
      m_end = m_start = std::chrono::high_resolution_clock::now();
      m_start_called = true;
    }

    void stop()
    {
      assert( m_start_called || !"stop(): start was never invokved");

      m_end = std::chrono::high_resolution_clock::now();

      assert( m_end >= m_start || !"stop(): std::chrono::high_resolution_clock::now() must have returned a bad-value?");

      m_start_called = false;
    }

    float duration()
    {
      assert( m_start_called || !"stop(): start was never invokved");

      this->stop();

      assert( m_end >= m_start || !"duration(): end time was smaller than start time");

      return std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_start).count();
    }

    /**
     * Get Time.
     *
     * @return     The time between start and stop in milliseconds.
     */
    float operator()()const
    {
      assert( m_end >= m_start || !"operator(): Must call stop before operator()");

      return std::chrono::duration_cast<std::chrono::milliseconds>(m_end - m_start).count();
    }
  };

}  // end of namespace util

// UTIL_TIMER_H
#endif 
