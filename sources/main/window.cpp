#include "window.hpp"

#include <iostream>

#include "graphics2D/component.hpp"
#include "graphics3D/openGL.hpp"
#include "main/render.hpp"
#include "main/settings.hpp"
#include "maths/maths.hpp"
#include "tools/string.hpp"

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

void Window::draw( Component3D const & component, sf::RenderTarget & target,
                   sf::RenderStates const & states ) const
{
    /// @todo imporve that
    Render render { *this, target, states };
    component.render_all( render );
}

void Window::creation()
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