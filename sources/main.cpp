#include <cstdlib>  // for EXIT_SUCCESS

#include "main/game.hpp"           // for Game
#include "main/settings.hpp"       // for Window
#include "main/window.hpp"         // for Settings
#include "tools/system/clock.hpp"  // for Clock

int main ()
{
    Window window { "Toody Engine (Developpement)" };
    Game   game {};
    Clock  clock {};

    float const refreshRate { Settings::get_instance().get_refresh_rate() };

    while ( game.should_run() )
    {
        float const deltaTime { clock.get_elapsed_time() };

        if ( deltaTime > refreshRate )
        {
            game.update_inputs( window );

            game.update_all( UpdateContext { window, deltaTime } );
            /// @todo voir si c'est utile de faire un renderContext & et pas
            /// seulement un renderContext
            RenderContext renderContext { window };
            game.render_all( renderContext );

            clock.reset();
        }
    }

    return EXIT_SUCCESS;
}