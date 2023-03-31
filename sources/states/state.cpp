#include "state.hpp"

#include <IMGUI/imgui-SFML.h>     // for Update, ProcessEvent
#include <SFML/Window/Event.hpp>  // for Event, Event::Closed, Event::...

#include "game/game.hpp"          // for GameContext
#include "graphics3D/openGL.hpp"  // for check_error

/* static */ State::E_List State::get_enum_state( std::string enumString )
{
    State::E_List enumValue {};
    boost::mp11::mp_for_each<
        boost::describe::describe_enumerators< State::E_List > >(
        [&] ( auto D ) {
            if ( enumString == D.name )
            {
                enumValue = D.value;
                return;
            }
        } );
    return enumValue;
}

/* static */ std::vector< std::string > State::get_state_list()
{
    std::vector< std::string > stateList {};

    boost::mp11::mp_for_each<
        boost::describe::describe_enumerators< State::E_List > >(
        [&] ( auto D ) { stateList.push_back( D.name ); } );

    return stateList;
}

State::State( GameContext & gameContext )
  : m_gameContext { gameContext }, m_inputs {}
{}

void State::update_all( UpdateContext context )
{
    ImGui::SFML::Update( sf::Mouse::getPosition( context.window ),
                         context.window.get_size().to_float(),
                         sf::seconds( context.deltaTime ) );

    m_inputs.clear();

    sf::Event event {};
    // The event loop must always be part of the main loop,
    // otherwise bug and crash could happen
    while ( context.window.pollEvent( event ) )
    {
        ImGui::SFML::ProcessEvent( context.window, event );
        m_inputs.update( event );
    }

    if ( m_inputs.is_window_closed() )
    {
        m_gameContext.should_run( false );
    }

    this->Component::update_all( context );
}

void State::render_all( RenderContext context ) const
{
    context.window.clear();

    this->Component::render_all( context );
    // We render after our state render, so the imGui's windows
    // will be drawn if we have a background
    ImGui::SFML::Render( context.window );

    // Each draw is a chance that we have an error with OpenGL
    gl::check_error();

    context.window.display();
}
