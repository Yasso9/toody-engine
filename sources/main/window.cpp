#include "window.hpp"

#include <iostream>  // for operator<<, endl, basic_o...
#include <string>    // for allocator, string

#include <SFML/System/Vector2.hpp>          // for Vector2u
#include <SFML/Window/ContextSettings.hpp>  // for ContextSettings
#include <SFML/Window/Mouse.hpp>            // for Mouse
#include <SFML/Window/WindowStyle.hpp>      // for Default

#include "graphics3D/openGL.hpp"     // for clear_window, initialize
#include "main/settings.hpp"         // for Settings
#include "maths/geometry/point.hpp"  // for PointI
#include "maths/geometry/point.tpp"  // for Point::Point<Type>, Point...
#include "maths/vector2.tpp"         // for Vector2::Vector2<Type>
#include "tools/singleton.tpp"       // for Singleton::get_instance

namespace sf
{
    class Color;
}  // namespace sf

Window::Window()
{
    this->creation();
    this->initialize();
}

math::Vector2U Window::get_size() const
{
    return math::Vector2U { this->getSize() };
}

float Window::get_aspect_ratio() const
{
    return static_cast< float >( Window::get_instance().get_size().x
                                 / Window::get_instance().get_size().y );
}

math::Vector2U Window::get_center_position() const
{
    return this->get_size() / 2u;
}

bool Window::is_hovered() const
{
    math::PointI const mousePosition { sf::Mouse::getPosition( *this ) };

    return mousePosition.is_inside( math::PointI { 0, 0 },
                                    math::Vector2I { this->getSize() } );
}

bool Window::has_absolute_focus() const
{
    return this->hasFocus() && this->is_hovered();
}

void Window::clear_all( sf::Color const & backgroundColor )
{
    gl::clear_window( backgroundColor );
    this->clear( backgroundColor );
}

void Window::creation()
{
    std::string const gameTitle { "Toody Engine (In Developpement)" };

    unsigned int const windowStyle { sf::Style::Titlebar | sf::Style::Resize
                                     | sf::Style::Close };

    sf::ContextSettings contextSettings {};
    contextSettings.depthBits         = 24;
    contextSettings.sRgbCapable       = false;
    contextSettings.stencilBits       = 8;
    contextSettings.antialiasingLevel = 4;
    contextSettings.majorVersion      = 4;
    contextSettings.minorVersion      = 6;

    this->sf::RenderWindow::create( Settings::get_instance().get_video_mode(),
                                    gameTitle, windowStyle, contextSettings );
}

void Window::initialize()
{
    this->setVisible( true );
    this->requestFocus();
    this->setKeyRepeatEnabled( false );
    this->setVerticalSyncEnabled(
        Settings::get_instance().get_vertical_sync() );

    if ( ! this->setActive( true ) )
    {
        std::cerr << "Cannot put the window active state for OpenGL calls"
                  << std::endl;
    }

    gl::initialize( this->getSize().x, this->getSize().y );
}