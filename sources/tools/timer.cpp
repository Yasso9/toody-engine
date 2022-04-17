#include "timer.hpp"

#include <iostream>

Timer::Timer( std::string const & indication )
  : m_indication( indication ), m_beginTime( std::chrono::steady_clock::now() )
{}

Timer::~Timer()
{
    auto const endTime { std::chrono::steady_clock::now() };

    long long const elapsedTime {
        std::chrono::duration_cast< std::chrono::milliseconds >(
            endTime - this->m_beginTime )
            .count()
    };

    std::cout << "Elapsed Time for '"s << this->m_indication << "' : "s
              << elapsedTime << " [ms]" << std::endl;
};