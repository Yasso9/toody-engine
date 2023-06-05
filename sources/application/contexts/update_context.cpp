#include "update_context.hpp"

#include "application/configuration.hpp"     // for Settings
#include "states/editor.hpp"     // for EditorState
#include "states/graphic.hpp"    // for GraphicState
#include "states/main_menu.hpp"  // for MainMenuState
#include "states/state.hpp"      // for StateList
#include "states/test.hpp"       // for TestState

UpdateContext::UpdateContext()
  : BaseContext {},
    m_state { nullptr },
    inputs { window },
    deltaTime { 0.f },
    shouldRun { true }
{
    this->transition_to( Config::get_instance().get_startup_state() );
}

void UpdateContext::transition_to( std::shared_ptr< State > state )
{
    m_state = state;
}

void UpdateContext::transition_to( StateList state )
{
    switch ( static_cast< StateList >( state ) )
    {
    case StateList::MainMenu :
        this->transition_to( std::make_shared< MainMenuState >() );
        break;
    case StateList::Editor :
        this->transition_to( std::make_shared< EditorState >() );
        break;
    case StateList::Graphics :
        this->transition_to( std::make_shared< GraphicState >() );
        break;
    case StateList::Test :
        this->transition_to( std::make_shared< TestState >() );
        break;
    case StateList::Quit :
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
