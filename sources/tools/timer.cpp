#include "timer.hpp"

#include <iostream>  // for operator<<, basic_ostream, endl, basic_ostream<>...

#include "tools/traces.hpp"

Timer::Timer( std::string const & indication )
  : m_indication( indication ), m_beginTime( std::chrono::steady_clock::now() )
{}

Timer::~Timer()
{
    auto const endTime { std::chrono::steady_clock::now() };

    long long const elapsedTime {
        std::chrono::duration_cast< std::chrono::milliseconds >( endTime
                                                                 - m_beginTime )
            .count() };

    Trace::Debug( "Elapsed Time for '" + m_indication + "' - "
                  + std::to_string( elapsedTime ) + " [ms]" );
};