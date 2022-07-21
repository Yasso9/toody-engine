#include "component.hpp"

#include "main/render.hpp"
#include "main/window.hpp"

void BaseComponent::update( float deltaTime )
{
    this->update_extra( deltaTime );

    for ( std::shared_ptr< Component2D > component : m_childs2D )
    {
        component->update( deltaTime );
    }
    for ( std::shared_ptr< Component3D > component : m_childs3D )
    {
        component->update( deltaTime );
    }
}
void BaseComponent::update_extra( float /* deltaTime */ ) {}

void BaseComponent::render_before( Render & /* render */ ) const {}
void BaseComponent::render( Render & /* render */ ) const {}
void BaseComponent::render_after( Render & /* render */ ) const {}

void BaseComponent::render_all( Render & render ) const
{
    this->render_before( render );

    this->render( render );

    for ( std::shared_ptr< Component3D > component : m_childs3D )
    {
        render.draw( *component );
    }
    for ( std::shared_ptr< Component2D > component : m_childs2D )
    {
        render.draw( *component );
    }

    this->render_after( render );
}

void Component2D::draw( sf::RenderTarget & /* target */,
                        sf::RenderStates /* states */ ) const
{
    /// @todo à définir
    std::cout << "We draw nothing" << std::endl;
    // BaseComponent::render_all { {}, target, states };
}