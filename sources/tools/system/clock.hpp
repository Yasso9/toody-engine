
#include <chrono>

class Clock
{
    std::chrono::high_resolution_clock::time_point m_startTime;

  public:
    Clock();
    virtual ~Clock() = default;

    /// @brief Restart the clock
    void reset ();
    /// @brief Return the elapsed time in seconds
    float get_elapsed_time () const;
};
