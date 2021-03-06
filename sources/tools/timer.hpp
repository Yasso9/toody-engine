#pragma once

#include <chrono>

#include "tools/string.hpp"

/**
 * @brief Create an instance of this class at the begginning
 *        of a function to benchmark it
 */
class Timer final
{
  public:
    Timer( std::string const & indication );
    ~Timer();

  private:
    std::string const m_indication;
    std::chrono::steady_clock::time_point const m_beginTime;
};