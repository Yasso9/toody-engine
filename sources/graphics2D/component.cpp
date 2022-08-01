#include "component.hpp"

void Component::update( float deltaTime )
{
    this->update_before( deltaTime );

    for ( Component * component : m_childs )
    {
        component->update( deltaTime );
    }

    this->update_after( deltaTime );
}

void Component::update_before( float /* deltaTime */ ) {}
void Component::update_after( float /* deltaTime */ ) {}

void Component::render_before( Render & /* render */ ) const {}
void Component::render_after( Render & /* render */ ) const {}

void Component::render_all( Render & render ) const
{
    this->render_before( render );

    for ( Component const * component : m_childs )
    {
        component->render_all( render );
    }

    this->render_after( render );
}
