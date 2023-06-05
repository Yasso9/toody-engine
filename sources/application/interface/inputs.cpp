#include "inputs.hpp"

#include "graphics2D/view.hpp"  // for View

// Define all the Inputs class
Inputs::Inputs( Window & window )
  : m_window { window },
    m_mouseButtonsPressed {},
    m_mouseButtonsReleased {},
    m_keyPressed {},
    m_keyReleased {},
    m_mouseMovement {},
    m_mouseScroll {},
    m_mouseEnteredWindow {},
    m_mouseLeftWindow {},
    m_focusGained {},
    m_focusLost {},
    m_windowClosed {},
    m_windowResized {},
    m_textEntered {}
{
    this->clear();
}

void Inputs::update( sf::Event event )
{
    switch ( event.type )
    {
    case sf::Event::Closed :
        m_windowClosed = true;
        break;
    case sf::Event::MouseMoved :
        m_mouseMovement = { event.mouseMove.x, event.mouseMove.y };
        break;
    case sf::Event::MouseWheelScrolled :
        m_mouseScroll = { event.mouseWheelScroll.x, event.mouseWheelScroll.y };
        break;
    case sf::Event::MouseEntered :
        m_mouseEnteredWindow = true;
        break;
    case sf::Event::MouseLeft :
        m_mouseLeftWindow = true;
        break;
    case sf::Event::GainedFocus :
        m_focusGained = true;
        break;
    case sf::Event::LostFocus :
        m_focusLost = true;
        break;
    case sf::Event::Resized :
        m_windowResized = true;
        break;
    case sf::Event::TextEntered :
        m_textEntered = static_cast< char >( event.text.unicode );
        break;
    case sf::Event::KeyPressed :
        m_keyPressed.push_back( event.key.code );
        break;
    case sf::Event::KeyReleased :
        m_keyReleased.push_back( event.key.code );
        break;
    case sf::Event::MouseButtonPressed :
        m_mouseButtonsPressed.push_back( event.mouseButton.button );
        break;
    case sf::Event::MouseButtonReleased :
        m_mouseButtonsReleased.push_back( event.mouseButton.button );
        break;
    default :
        break;
    }
}

void Inputs::clear()
{
    m_mouseButtonsPressed.clear();
    m_mouseButtonsReleased.clear();
    m_keyPressed.clear();
    m_keyReleased.clear();
    m_mouseMovement      = { 0, 0 };
    m_mouseScroll        = { 0, 0 };
    m_mouseEnteredWindow = false;
    m_mouseLeftWindow    = false;
    m_focusGained        = false;
    m_focusLost          = false;
    m_windowClosed       = false;
    m_windowResized      = false;
    m_textEntered        = '\0';
}

bool Inputs::is_pressed( sf::Mouse::Button button ) const
{
    return std::find( m_mouseButtonsPressed.begin(),
                      m_mouseButtonsPressed.end(), button )
           != m_mouseButtonsPressed.end();
}

bool Inputs::is_released( sf::Mouse::Button button ) const
{
    return std::find( m_mouseButtonsReleased.begin(),
                      m_mouseButtonsReleased.end(), button )
           != m_mouseButtonsReleased.end();
}

bool Inputs::is_pressed( sf::Keyboard::Key key ) const
{
    return std::find( m_keyPressed.begin(), m_keyPressed.end(), key )
           != m_keyPressed.end();
}

bool Inputs::is_released( sf::Keyboard::Key key ) const
{
    return std::find( m_keyReleased.begin(), m_keyReleased.end(), key )
           != m_keyReleased.end();
}

math::Vector2I Inputs::get_mouse_movement() const
{
    return m_mouseMovement;
}

math::Vector2I Inputs::get_mouse_scroll() const
{
    return m_mouseScroll;
}

bool Inputs::is_mouse_entered() const
{
    return m_mouseEnteredWindow;
}

bool Inputs::is_mouse_left() const
{
    return m_mouseLeftWindow;
}

bool Inputs::is_focus_gained() const
{
    return m_focusGained;
}

bool Inputs::is_focus_lost() const
{
    return m_focusLost;
}

bool Inputs::is_window_closed() const
{
    return m_windowClosed;
}

bool Inputs::is_window_resized() const
{
    return m_windowResized;
}

char Inputs::get_text_entered() const
{
    return m_textEntered;
}

math::Vector2I Inputs::get_mouse_position() const
{
    return sf::Mouse::getPosition( m_window );
}

math::Vector2F Inputs::get_mouse_position( View const & view ) const
{
    return ( this->get_mouse_position().to_float() / view.get_zoom( m_window ) )
           + view.get_position();
}

void Inputs::set_mouse_position( math::Vector2I newMousePosition ) const
{
    sf::Mouse::setPosition( newMousePosition, m_window );
}

math::Vector2F Inputs::get_movement( KeyboardMove movementKey,
                                     bool         invertMovement ) const
{
    math::Vector2F moveDirection { 0.f, 0.f };
    if ( this->is_pressed( movementKey.up ) )
    {
        moveDirection += math::Vector2F { 0.f, -1.f };
    }
    if ( this->is_pressed( movementKey.right ) )
    {
        moveDirection += math::Vector2F { 1.f, 0.f };
    }
    if ( this->is_pressed( movementKey.down ) )
    {
        moveDirection += math::Vector2F { 0.f, 1.f };
    }
    if ( this->is_pressed( movementKey.left ) )
    {
        moveDirection += math::Vector2F { -1.f, 0.f };
    }

    if ( invertMovement )
    {
        moveDirection = -moveDirection;
    }

    return moveDirection;
}
