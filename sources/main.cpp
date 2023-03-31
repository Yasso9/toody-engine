#include <cstdlib>  // for EXIT_SUCCESS

#include "game/game.hpp"           // for Game
#include "game/settings.hpp"       // for Window
#include "interface/window.hpp"    // for Settings
#include "tools/system/clock.hpp"  // for Clock

int main ()
{
    Window      window { "Toody Engine (Dev)" };
    GameContext game {};
    Clock       clock {};

    float const refreshRate { Settings::get_instance().get_refresh_rate() };

    while ( game.should_run() )
    {
        float const deltaTime { clock.get_elapsed_time() };
        if ( deltaTime > refreshRate )
        {
            game.get_state().update_all( UpdateContext { window, deltaTime } );
            game.get_state().render_all( RenderContext { window } );

            clock.reset();
        }
    }

    return EXIT_SUCCESS;
}