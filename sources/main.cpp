#include <chrono>
#include <cstdlib>  // for EXIT_SUCCESS

#include "main/game.hpp"      // for Game
#include "main/settings.hpp"  // for Window
#include "main/window.hpp"    // for Settings

// #include <SFML/System/Clock.hpp>  // for Clock
// #include <SFML/System/Time.hpp>   // for Time

int main ()
{
    Game game {};

    /// @todo create a time class to handle time
    auto start_time = std::chrono::high_resolution_clock::now();

    // sf::Clock clock {};
    // // Reset the clock juste before the game run
    // clock.restart();

    double const refreshRate { Settings::get_instance().get_refresh_rate() };

    while ( game.should_run() )
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime =
            std::chrono::duration_cast< std::chrono::milliseconds >(
                currentTime - start_time );

        float deltaTime = static_cast< float >( elapsedTime.count() ) / 1000.f;

        // sf::Time const deltaTime { clock.getElapsedTime() };
        // if ( deltaTime.asSeconds() > refreshRate )

        if ( deltaTime > refreshRate )
        {
            // game.update( deltaTime.asSeconds() );
            game.update( deltaTime );

            /// @todo améliorer ça : window ne doit plus etre un singleton + on
            /// doit passer directement window a la fonction render
            Render render { Window::get_instance() };
            game.render( render );

            // // reset the counter
            // clock.restart();
            start_time = std::chrono::high_resolution_clock::now();
        }
    }

    Window::get_instance().close();

    return EXIT_SUCCESS;
}