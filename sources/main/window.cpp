#include "window.hpp"

#include "tools/string.hpp"

Window::Window()
{
    this->create();
    this->initialize();
}

void Window::create()
{
    sf::Vector2u const windowSize { 900u, 900u };

    std::string const gameTitle { "Toody Engine (In Developpement)"s };

    unsigned int const windowStyle { sf::Style::Default };

    sf::ContextSettings contextSettings {};
    contextSettings.depthBits         = 24;
    contextSettings.sRgbCapable       = false;
    contextSettings.stencilBits       = 8;
    contextSettings.antialiasingLevel = 4;
    contextSettings.majorVersion      = 4;
    contextSettings.minorVersion      = 6;

    this->sf::RenderWindow::create( sfpp::to_video_mode( windowSize ),
                                    gameTitle,
                                    windowStyle,
                                    contextSettings );

    if ( ! this->setActive( true ) )
    {
        throw std::runtime_error {
            "Cannot set the windows as active state for OpenGL"s
        };
    }
}
void Window::initialize()
{
    // TYPO à mettre autre part
    this->setVisible( true );
    this->requestFocus();
    this->setKeyRepeatEnabled( false );
    this->setVerticalSyncEnabled( true );
}