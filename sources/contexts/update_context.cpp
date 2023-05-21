#include "update_context.hpp"

#include "states/editor.hpp"     // for EditorState
#include "states/graphic.hpp"    // for GraphicState
#include "states/main_menu.hpp"  // for MainMenuState
#include "states/state.hpp"      // for State::E_List
#include "states/test.hpp"       // for TestState

UpdateContext::UpdateContext()
  : BaseContext {},
    m_state { nullptr },
    inputs { window },
    deltaTime { 0.f },
    shouldRun { true }
{
    // this->transition_to( State::MainMenu );
    this->transition_to( std::make_shared< MainMenuState >() );
}

void UpdateContext::transition_to( std::shared_ptr< State > state )
{
    m_state = state;
}

void UpdateContext::transition_to( int state )
{
    switch ( static_cast< State::E_List >( state ) )
    {
    case State::MainMenu :
        this->transition_to( std::make_shared< MainMenuState >() );
        break;
    case State::Editor :
        this->transition_to( std::make_shared< EditorState >() );
        break;
    case State::Graphics :
        this->transition_to( std::make_shared< GraphicState >() );
        break;
    case State::Test :
        this->transition_to( std::make_shared< TestState >() );
        break;
    case State::Quit :
        this->transition_to( std::make_shared< MainMenuState >() );
        shouldRun = false;
        break;
    default :
        Trace::Error( "Context - Transition to unknown state." );
        break;
    }
}

void UpdateContext::clear()
{
    inputs.clear();
}
