#include "state.hpp"

#include <SFML/Graphics/RenderTarget.hpp>  // for RenderTarget
#include <SFML/Window/Event.hpp>           // for Event, Event::(anonymous)
#include <SFML/Window/Keyboard.hpp>        // for Keyboard, Keyboard::Escape

#include "graphics2D/view.hpp"  // for View
#include "input/input.hpp"      // for set_mouse_movement, set_mo...
#include "main/window.hpp"      // for Window
#include "maths/vector2.hpp"    // for Vector2I
#include "maths/vector2.tpp"    // for Vector2::to_float
#include "tools/array/vector.hpp"
#include "tools/singleton.tpp"  // for Singleton::get_instance

State::State( State::E_List const & stateName )
  : m_stateName { stateName },
    m_mouseButtonsPressed {},
    m_mouseButtonsReleased {}
{}

State::E_List State::get_enum_state( std::string enumString )
{
    State::E_List enumValue {};
    boost::mp11::mp_for_each<
        boost::describe::describe_enumerators< State::E_List > >(
        [&] ( auto D ) {
            if ( enumString == D.name )
            {
                enumValue = State::E_List { D.value };
                return;
            }
        } );
    return enumValue;
}

std::vector< std::string > State::get_state_list()
{
    std::vector< std::string > stateList {};

    boost::mp11::mp_for_each<
        boost::describe::describe_enumerators< State::E_List > >(
        [&] ( auto D ) { stateList.push_back( D.name ); } );

    return stateList;
}

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
        m_mouseButtonsPressed.push_back( event.mouseButton.button );
        this->mouse_pressed( event );
        break;
    case sf::Event::MouseButtonReleased :
        m_mouseButtonsReleased.push_back( event.mouseButton.button );
        this->mouse_released( event );
        break;
    case sf::Event::MouseMoved :
        input::set_mouse_movement(
            math::Vector2I { event.mouseMove.x, event.mouseMove.y }
                .to_float() );
        this->mouse_moved( event );
        break;
    case sf::Event::MouseWheelScrolled :
        if ( ! ImGui::IsWindowHovered( ImGuiHoveredFlags_AnyWindow ) )
        {  // No ImGui Window should be hovered for the scroll to be taken
            input::set_mouse_scroll( event.mouseWheelScroll.delta );
            this->mouse_scroll( event.mouseWheelScroll.delta );
        }
        break;
    default :
        break;
    }
}

void State::clear_buttons()
{
    m_mouseButtonsPressed.clear();
    m_mouseButtonsReleased.clear();
}

bool State::is_pressed( sf::Mouse::Button mouseButton ) const
{
    return vector::contains( m_mouseButtonsPressed, mouseButton );
}

bool State::is_released( sf::Mouse::Button mouseButton ) const
{
    return vector::contains( m_mouseButtonsReleased, mouseButton );
}

void State::set_new_state( State::E_List newState )
{
    m_stateName = newState;
}

void State::keyboard_pressed( sf::Event /* event */ ) {}

void State::keyboard_released( sf::Event /* event */ ) {}

void State::mouse_pressed( sf::Event /* event */ ) {}

void State::mouse_released( sf::Event /* event */ ) {}

void State::mouse_moved( sf::Event /* event */ ) {}

void State::mouse_scroll( float const & /* deltaScroll */ ) {}
