#include "component.hpp"

#include "main/render.hpp"
#include "main/window.hpp"

void Component::update( float deltaTime )
{
    this->update_extra( deltaTime );

    for ( std::shared_ptr< Component > component : m_childs )
    {
        component->update( deltaTime );
    }
}
void Component::update_extra( float /* deltaTime */ ) {}

void Component::render_before( Render & /* render */ ) const {}
void Component::render( Render & /* render */ ) const {}
void Component::render_after( Render & /* render */ ) const {}

void Component::render_all( Render & render ) const
{
    this->render_before( render );

    this->render( render );

    for ( std::shared_ptr< Component > component : m_childs )
    {
        component->render_all( render );
        // switch ( component->get_type() )
        // {
        // case E_Type::OpenGL :
        //     render.draw(
        //         *( dynamic_cast< Component3D * >( component.get() ) ) );
        //     break;
        // case E_Type::SFML :
        //     render.draw(
        //         *( dynamic_cast< Component2D * >( component.get() ) ) );
        //     break;
        // }
    }

    this->render_after( render );
}

Component::E_Type Component::get_type() const
{
    return m_type;
}

void Component2D::draw( sf::RenderTarget & /* target */,
                        sf::RenderStates /* states */ ) const
{
    // Render render { Window::get_instance(), target, states };
    // this->render_all( render );
}
