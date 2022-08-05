#include "timer.hpp"

#include <iostream>  // for operator<<, basic_ostream, endl, basic_ostream<>...

Timer::Timer( std::string const & indication )
  : m_indication( indication ), m_beginTime( std::chrono::steady_clock::now() )
{}

Timer::~Timer()
{
    auto const endTime { std::chrono::steady_clock::now() };

    long long const elapsedTime {
        std::chrono::duration_cast< std::chrono::milliseconds >(
            endTime - this->m_beginTime )
            .count() };

    std::cout << "Elapsed Time for '" << this->m_indication
              << "' : " << elapsedTime << " [ms]" << std::endl;
};