#include "window.hpp"

#include <iostream>

#include "graphics3D/openGL.hpp"
#include "main/settings.hpp"
#include "maths/maths.hpp"
#include "tools/string.hpp"

Window::Window()
{
    this->create();
    this->initialize();
}

sf::Vector2f Window::get_size_f() const
{
    return static_cast< sf::Vector2f >( this->getSize() );
}

sf::Vector2i Window::get_size_i() const
{
    return static_cast< sf::Vector2i >( this->getSize() );
}

float Window::get_aspect_ratio() const
{
    return ( Window::get_instance().get_size_f().x
             / Window::get_instance().get_size_f().y );
}

sf::Vector2u Window::get_center_position_u() const
{
    return ( this->getSize() / 2u );
}

sf::Vector2f Window::get_center_position_f() const
{
    return static_cast< sf::Vector2f >( this->get_center_position_u() );
}

math::Vector2F Window::get_mouse_position() const
{
    return math::Vector2I { sf::Mouse::getPosition( *this ) };
}

bool Window::has_absolute_focus() const
{
    math::Vector2F const mousePosition { sf::Mouse::getPosition(
        Window::get_instance() ) };

    return this->hasFocus()
           && mousePosition.is_inside( math::Vector2F { 0.f, 0.f },
                                       math::Vector2F { this->getSize() } );
}

void Window::clear_all( sf::Color const & backgroundColor )
{
    gl::clear_window( backgroundColor );
    this->clear( backgroundColor );

    // Reset the view
    Window::get_instance().setView( Window::get_instance().getDefaultView() );
}

void Window::gl_draw_elements( unsigned int const & vertexArrayObject,
                               GLenum const & primitiveType,
                               GLenum const & dataType,
                               std::size_t const & elementsSize ) const
{
    glBindVertexArray( vertexArrayObject );
    // enable openGL Z buffer
    glEnable( GL_DEPTH_TEST );

    glDrawElements( primitiveType,
                    static_cast< int >( elementsSize ),
                    dataType,
                    0 );

    glDisable( GL_DEPTH_TEST );
    glBindVertexArray( 0 );
}
void Window::gl_draw_arrays( unsigned int const & vertexArrayObject,
                             GLenum const & primitiveType,
                             unsigned int const & arraySize ) const
{
    glBindVertexArray( vertexArrayObject );
    glEnable( GL_DEPTH_TEST );

    int const verticesBeginPosition { 0 };
    glDrawArrays( primitiveType,
                  verticesBeginPosition,
                  static_cast< int >( arraySize ) );

    glDisable( GL_DEPTH_TEST );
    glBindVertexArray( 0 );
}

void Window::create()
{
    std::string const gameTitle { "Toody Engine (In Developpement)"s };

    unsigned int const windowStyle { sf::Style::Default };

    sf::ContextSettings contextSettings {};
    contextSettings.depthBits         = 24;
    contextSettings.sRgbCapable       = false;
    contextSettings.stencilBits       = 8;
    contextSettings.antialiasingLevel = 4;
    contextSettings.majorVersion      = 4;
    contextSettings.minorVersion      = 6;

    this->sf::RenderWindow::create( Settings::get_instance().get_video_mode(),
                                    gameTitle,
                                    windowStyle,
                                    contextSettings );
}
void Window::initialize()
{
    this->setVisible( true );
    // this->requestFocus();
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