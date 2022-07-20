#include "component.hpp"

void Component::update( float deltaTime )
{
    this->update_extra( deltaTime );

    for ( auto component : m_childs )
    {
        component->update( deltaTime );
    }
}

void Component::update_extra( float /* deltaTime */ ) {}

void Component::render_before( sf::RenderTarget & /* target */,
                               sf::RenderStates /* states */ ) const
{}
void Component::render( sf::RenderTarget & /* target */,
                        sf::RenderStates /* states */ ) const
{}
void Component::render_after( sf::RenderTarget & /* target */,
                              sf::RenderStates /* states */ ) const
{}

void Component::draw( sf::RenderTarget & target, sf::RenderStates states ) const
{
    this->render_before( target, states );

    this->render( target, states );

    for ( std::shared_ptr< Component > component : m_childs )
    {
        target.draw( *component, states );
    }

    this->render_after( target, states );
}