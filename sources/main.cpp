#include <cstdlib>  // for EXIT_SUCCESS

#include "application/contexts/game_context.hpp"  // for Game
#include "application/configuration.hpp"          // for Window
#include "application/interface/inputs.hpp"       // for Inputs
#include "application/interface/window.hpp"       // for Settings
#include "tools/system/clock.hpp"     // for Clock

int main ()
{
    GameContext game {};
    Clock       clock {};
    clock.reset();

    float const refreshRate { Config::get_instance().get_refresh_rate() };

    while ( game.shouldRun )
    {
        float const deltaTime { clock.get_elapsed_time() };
        if ( deltaTime > refreshRate )
        {
            game.update( deltaTime );

            clock.reset();
        }
    }

    return EXIT_SUCCESS;
}
