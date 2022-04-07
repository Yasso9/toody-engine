#include "window.hpp"

#include "tools/string.hpp"

class Position : public sf::Vector2i
{
  public:
    Position( sf::Vector2i const & vector2i ) : sf::Vector2i( vector2i ) {}
    virtual ~Position() = default;

    template < typename PrimitivType >
    bool is_inside( sf::Rect< PrimitivType > const & rectangle ) const
    {
        sf::Rect< int > integerRectangle { static_cast< sf::Rect< int > >(
            rectangle ) };

        return ( this->x >= integerRectangle.left
                 && this->x < integerRectangle.left + integerRectangle.width
                 && this->y >= integerRectangle.top
                 && this->y < integerRectangle.top + integerRectangle.height );
    }

    template < typename PrimitivType >
    bool is_inside( sf::Vector2< PrimitivType > const & rectanglePosition,
                    sf::Vector2< PrimitivType > const & rectangleSize ) const
    {
        return (
            this->is_inside( sf::Rect< PrimitivType > { rectanglePosition.x,
                                                        rectanglePosition.y,
                                                        rectangleSize.x,
                                                        rectangleSize.y } ) );
    }
};

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

bool Window::has_absolute_focus() const
{
    Position const mousePosition { sf::Mouse::getPosition(
        Window::get_instance() ) };

    return this->hasFocus()
           && mousePosition.is_inside( sf::Vector2u { 0u, 0u },
                                       this->getSize() );
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