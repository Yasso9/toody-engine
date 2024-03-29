#include "state.hpp"

#include <SFML/Window/Event.hpp>  // for Event, Event::Closed, Event::...
#include <imgui/imgui-SFML.h>     // for Update, ProcessEvent

#include "graphics3D/openGL.hpp"  // for check_error

State::State() {}

void State::update_all( UpdateContext & context )
{
    ImGui::SFML::Update( sf::Mouse::getPosition( context.window ),
                         context.window.get_size().to_float(),
                         sf::seconds( context.deltaTime ) );

    sf::Event event {};
    // The event loop must always be part of the main loop,
    // otherwise bug and crash could happen
    while ( context.window.pollEvent( event ) )
    {
        ImGui::SFML::ProcessEvent( context.window, event );
        context.inputs.update( event );
    }

    if ( context.inputs.is_window_closed() )
    {
        context.shouldRun = false;
    }

    this->Component::update_all( context );
}

void State::render_all( RenderContext & context ) const
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
