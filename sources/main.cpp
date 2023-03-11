#include <cstdlib>  // for EXIT_SUCCESS

#include "main/game.hpp"           // for Game
#include "main/settings.hpp"       // for Window
#include "main/window.hpp"         // for Settings
#include "tools/system/clock.hpp"  // for Clock

int main ()
{
    Game  game {};
    Clock clock {};

    /// @todo revoir settings pour qu'il soit utilisable partout sans avoir un
    /// singleton
    float const refreshRate { Settings {}.get_refresh_rate() };

    while ( game.should_run() )
    {
        // 
        float const deltaTime { clock.get_elapsed_time() };

        if ( deltaTime > refreshRate )
        {
            game.update_all( deltaTime );

            /// @todo améliorer ça : window ne doit plus etre un singleton + on
            /// doit passer directement window a la fonction render
            Render render { Window::get_instance() };
            game.render_all( render );

            clock.reset();
        }
    }

    /// @todo améliorer ça : window ne doit plus etre un singleton
    Window::get_instance().close();

    return EXIT_SUCCESS;
}