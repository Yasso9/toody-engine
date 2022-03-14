#include "state.hpp"

State::State( std::shared_ptr<sf::RenderWindow> window,
              Ressources const & ressources, State::E_List const & stateName )
  : m_window( window ),
    m_ressources( ressources ),
    m_stateName( stateName ),
    m_deltaTime( 0.f )
{}

State::E_List State::get_state_to_print() const
{
    return this->m_stateName;
}

void State::update_inputs( sf::Event const & event )
{
    float constexpr deltaMouseWheel { 0.8f };

    switch ( event.type )
    {
    case sf::Event::Closed :
        this->m_stateName = State::E_List::Quit;
        break;
    case sf::Event::KeyPressed :
        this->keyboard_pressed( event );
        break;
    case sf::Event::KeyReleased :
        this->keyboard_released( event );
        break;
    case sf::Event::MouseButtonPressed :
        this->mouse_pressed( event );
        break;
    case sf::Event::MouseButtonReleased :
        this->mouse_released( event );
        break;
    case sf::Event::MouseWheelScrolled :
        if ( std::abs( event.mouseWheelScroll.delta ) > deltaMouseWheel )
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

void State::keyboard_pressed( sf::Event /* event */ ) {}
void State::keyboard_released( sf::Event /* event */ ) {}

void State::mouse_pressed( sf::Event /* event */ ) {}
void State::mouse_released( sf::Event /* event */ ) {}

void State::mouse_scroll( float const & /* deltaScroll */ ) {}
