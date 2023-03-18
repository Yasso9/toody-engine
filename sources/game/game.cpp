#include "game.hpp"

#include "states/editor.hpp"     // for EditorState
#include "states/graphic.hpp"    // for GraphicState
#include "states/main_menu.hpp"  // for MainMenuState
#include "states/test.hpp"       // for TestState
#include "tools/traces.hpp"      // for Trace::Info, Trace::Error, Trace::Debug

GameContext::GameContext() : m_state { nullptr }, m_shouldRun { true }
{
    Trace::Info( "Game Started" );
    this->transition_to( std::make_shared< MainMenuState >( *this ) );
}

GameContext::~GameContext()
{
    Trace::Info( "Game Ended" );
}

void GameContext::transition_to( std::shared_ptr< State > state )
{
    m_state = state;
}

void GameContext::transition_to( State::E_List state )
{
    switch ( state )
    {
    case State::MainMenu :
        this->transition_to( std::make_shared< MainMenuState >( *this ) );
        break;
    case State::Editor :
        this->transition_to( std::make_shared< EditorState >( *this ) );
        break;
    case State::Graphics :
        this->transition_to( std::make_shared< GraphicState >( *this ) );
        break;
    case State::Test :
        this->transition_to( std::make_shared< TestState >( *this ) );
        break;
    case State::Quit :
        this->transition_to( std::make_shared< MainMenuState >( *this ) );
        this->should_run( false );
        break;
    default :
        Trace::Error( "Context - Transition to unknown state." );
        break;
    }
}

bool GameContext::should_run() const
{
    return m_shouldRun;
}

void GameContext::should_run( bool shouldRun )
{
    m_shouldRun = shouldRun;
}

State & GameContext::get_state()
{
    return *m_state;
}

State const & GameContext::get_state() const
{
    return *m_state;
}