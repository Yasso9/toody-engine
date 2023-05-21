#include "game_context.hpp"

#include "states/main_menu.hpp"  // for MainMenu
#include "tools/traces.hpp"      // for Trace::Info, Trace::Error, Trace::Debug

GameContext::GameContext() : UpdateContext {}, RenderContext {}
{
    Trace::Info( "Game Started" );
}

GameContext::~GameContext()
{
    Trace::Info( "Game Ended" );
}

void GameContext::update( float aDeltaTime )
{
    UpdateContext::clear();

    deltaTime = aDeltaTime;
    m_state->update_all( *this );
    m_state->render_all( *this );
}
