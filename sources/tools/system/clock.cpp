#include "clock.hpp"

Clock::Clock() : m_startTime { std::chrono::high_resolution_clock::now() } {}

/// @brief Restart the clock
void Clock::reset()
{
    m_startTime = std::chrono::high_resolution_clock::now();
}

/// @brief Return the elapsed time in seconds
float Clock::get_elapsed_time() const
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration_cast< std::chrono::milliseconds >(
        currentTime - m_startTime );

    return static_cast< float >( elapsedTime.count() ) / 1000.f;
}
