#include "state.hpp"

#include "graphics2D/view.hpp"
#include "input/input.hpp"
#include "main/resources.hpp"

State::State( State::E_List const & stateName )
  : m_stateName { stateName },
    m_view { Window::get_instance().getDefaultView() }
{}

State::E_List State::get_state_to_print() const
{
    return m_stateName;
}

void State::update_inputs( sf::Event const & event )
{
    switch ( event.type )
    {
    case sf::Event::Closed :
        m_stateName = State::E_List::Quit;
        break;
    case sf::Event::KeyPressed :
        this->keyboard_pressed( event );
        break;
    case sf::Event::KeyReleased :
        if ( event.key.code == sf::Keyboard::Escape )
        {
            if ( m_stateName == State::E_List::MainMenu )
            {
                m_stateName = State::E_List::Quit;
                break;
            }

            m_stateName = State::E_List::MainMenu;
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
    case sf::Event::MouseMoved :
        input::set_mouse_movement(
            math::Vector2I { event.mouseMove.x, event.mouseMove.y }
                .to_float() );
        this->mouse_moved( event );
        break;
    case sf::Event::MouseWheelScrolled :
        input::set_mouse_scroll( event.mouseWheelScroll.delta );
        this->mouse_scroll( event.mouseWheelScroll.delta );
        break;
    default :
        break;
    }
}

void State::render_before( Render & render ) const
{
    /// @todo create method set view
    render.get_target().setView( m_view );
}
void State::render_after( Render & render ) const
{
    /// @todo create method reset view
    render.get_target().setView( render.get_target().getDefaultView() );
}

void State::keyboard_pressed( sf::Event /* event */ ) {}
void State::keyboard_released( sf::Event /* event */ ) {}

void State::mouse_pressed( sf::Event /* event */ ) {}
void State::mouse_released( sf::Event /* event */ ) {}
void State::mouse_moved( sf::Event /* event */ ) {}

void State::mouse_scroll( float const & /* deltaScroll */ ) {}
