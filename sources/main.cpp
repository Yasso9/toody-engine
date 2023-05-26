#include <cstdlib>  // for EXIT_SUCCESS

#include "contexts/game_context.hpp"  // for Game
#include "game/settings.hpp"          // for Window
#include "interface/inputs.hpp"       // for Inputs
#include "interface/window.hpp"       // for Settings
#include "tools/system/clock.hpp"     // for Clock

int main ()
{
    GameContext game {};
    Clock       clock {};
    clock.reset();

    float const refreshRate { Settings::get_instance().get_refresh_rate() };

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
