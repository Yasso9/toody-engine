#include "state.hpp"

#include "main/resources.hpp"

State::State( State::E_List const & stateName )
  : m_stateName( stateName ), m_deltaTime( 0.f )
{}

State::E_List State::get_state_to_print() const
{
    return this->m_stateName;
}

void State::update_inputs( sf::Event const & event )
{
    float constexpr mouseWheelSensibility { 0.8f };

    switch ( event.type )
    {
    case sf::Event::Closed :
        this->m_stateName = State::E_List::Quit;
        break;
    case sf::Event::KeyPressed :
        this->keyboard_pressed( event );
        break;
    case sf::Event::KeyReleased :
        if ( event.key.code == sf::Keyboard::Escape )
        {
            if ( this->m_stateName == State::E_List::MainMenu )
            {
                this->m_stateName = State::E_List::Quit;
                break;
            }

            this->m_stateName = State::E_List::MainMenu;
            break;
        }
        this->keyboard_released( event );
        break;
    case sf::Event::MouseButtonPressed :
        this->mouse_pressed( event );
        break;
    case sf::Event::MouseButtonReleased :
        this->mouse_released( event );
        break;
    case sf::Event::MouseWheelScrolled :
        if ( std::abs( event.mouseWheelScroll.delta ) > mouseWheelSensibility )
        {
            this->mouse_scroll( event.mouseWheelScroll.delta );
        }
        break;
    default :
        break;
    }
}

void State::update_data( float const & deltaTime )
{
    this->m_deltaTime = deltaTime;
    this->update();
}

void State::render_all() const
{
    this->render();

    sf::Text framerateText { std::to_string( 1.f / this->m_deltaTime ),
                             Resources::get_instance().get_font(
                                 Resources::E_FontKey::Arial ) };
    framerateText.setPosition( 50.f, 50.f );
    framerateText.setCharacterSize( 50u );

    Window::get_instance().draw( framerateText );
}

void State::keyboard_pressed( sf::Event /* event */ ) {}
void State::keyboard_released( sf::Event /* event */ ) {}

void State::mouse_pressed( sf::Event /* event */ ) {}
void State::mouse_released( sf::Event /* event */ ) {}

void State::mouse_scroll( float const & /* deltaScroll */ ) {}

void State::extra_events() {}
