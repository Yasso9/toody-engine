#include "state.hpp"

State::State( std::map<TextureKey, sf::Texture> const & textures,
              std::map<FontKey, sf::Font> const & fonts,
              sf::Vector2u const & windowSize,
              StateName const & stateName )
  : m_textures( textures ),
    m_fonts( fonts ),
    m_keyboard( InputInitialization::keyboard( stateName ) ),
    m_mouseButton( InputInitialization::mouse_button( stateName ) ),
    m_windowSize( windowSize ),
    m_stateName( stateName )
{
}

StateName State::get_next()
{
    return this->m_stateName;
}

void State::update_input( sf::Event const & event )
{
    this->update_overall_input( event );

    this->update_mouse_position( event );

    this->update_mouse_button( event );

    this->update_extra_input( event );
}

void State::update_overall_input( sf::Event const & event )
{
    switch ( event.type )
    {
    case sf::Event::Closed :
        this->m_stateName = StateName::Quit;
        break;
    case sf::Event::KeyPressed :
        this->update_keyboard_pressed( event );
        break;
    case sf::Event::KeyReleased :
        this->update_keyboard_released( event );
        break;
    default :
        break;
    }
}

void State::update_keyboard_pressed( sf::Event const & event )
{
    for ( auto element : this->m_keyboard )
    {
        if ( event.key.code == element.second.first )
        {
            if ( this->m_keyboard.at( element.first ).second == false )
            {
                this->handle_keyboard_press( element.first );
            }

            this->m_keyboard.at( element.first ).second = true;

            // to make this series of if act like else if
            break;
        }
    }
}

void State::update_keyboard_released( sf::Event const & event )
{
    for ( auto element : this->m_keyboard )
    {
        if ( event.key.code == element.second.first )
        {
            this->m_keyboard.at( element.first ).second = false;
            break;
        }
    }
}

void State::update_mouse_button( sf::Event const & event )
{
    if ( event.type == sf::Event::MouseButtonPressed )
    {
        for ( auto element : this->m_mouseButton )
        {
            if ( event.mouseButton.button == element.second.first )
            {
                if ( this->m_mouseButton.at( element.first ).second == false )
                {
                    this->handle_mouse_button_press( element.first );
                }

                this->m_mouseButton.at( element.first ).second = true;

                break;
            }
        }
    }
    else if ( event.type == sf::Event::MouseButtonReleased )
    {
        for ( auto element : this->m_mouseButton )
        {
            if ( event.mouseButton.button == element.second.first )
            {
                this->handle_mouse_button_release( element.first );
                this->m_mouseButton.at( element.first ).second = false;
                break;
            }
        }
    }
    else if ( event.type == sf::Event::MouseWheelScrolled )
    {
        // Sensibility of the mouse wheel
        float constexpr deltaMouseWheel { 0.8f };

        if ( event.mouseWheelScroll.delta > deltaMouseWheel )
        {
            this->handle_mouse_wheel_up();
        }
        else if ( event.mouseWheelScroll.delta < -deltaMouseWheel )
        {
            this->handle_mouse_wheel_down();
        }
    }
}

void State::update_mouse_position( sf::Event const & event )
{
    if ( event.type == sf::Event::MouseButtonPressed
         && event.mouseButton.button == sf::Mouse::Left )
    {
        this->m_mousePosition.set_press(
            sf::Vector2f( static_cast<float>( event.mouseButton.x ),
                          static_cast<float>( event.mouseButton.y ) ) );
    }
    else if ( event.type == sf::Event::MouseButtonReleased
              && event.mouseButton.button == sf::Mouse::Left )
    {
        this->m_mousePosition.set_release(
            sf::Vector2f( static_cast<float>( event.mouseButton.x ),
                          static_cast<float>( event.mouseButton.y ) ) );
    }
    else if ( event.type == sf::Event::MouseMoved )
    {
        this->m_mousePosition.set_overall(
            sf::Vector2f( static_cast<float>( event.mouseMove.x ),
                          static_cast<float>( event.mouseMove.y ) ) );
    }
}

// These functions needs to be overloaded on the childs classes

void State::handle_keyboard_press( std::string const & /* input */ ) {}

void State::handle_mouse_button_press( std::string const & /* input */ ) {}
void State::handle_mouse_button_release( std::string const & /* input */ ) {}

void State::handle_mouse_wheel_up() {}
void State::handle_mouse_wheel_down() {}

void State::update_extra_input( sf::Event const & /* event */ ) {}
void State::handle_global_input() {}