#include "component.hpp"

#include "contexts/render_context.hpp"  // for RenderContext
#include "contexts/update_context.hpp"  // for UpdateContext

Component::Component() : m_childs {}, m_debugWindows {}, m_view { nullptr } {};

Component::Component( Component const & component ) noexcept
  : m_childs { component.m_childs },
    m_debugWindows { component.m_debugWindows },
    m_view { component.m_view }
{}

Component::Component( Component && component ) noexcept
  : m_childs { std::exchange( component.m_childs, {} ) },
    m_debugWindows { std::exchange( component.m_debugWindows, {} ) },
    m_view { std::exchange( component.m_view, {} ) }
{}

Component & Component::operator= ( Component const & component ) noexcept
{
    return *this = Component { component };
}

Component & Component::operator= ( Component && component ) noexcept
{
    std::swap( m_childs, component.m_childs );
    std::swap( m_debugWindows, component.m_debugWindows );
    std::swap( m_view, component.m_view );
    return *this;
}

std::vector< Component const * > Component::get_childs() const
{
    std::vector< Component const * > childs {};
    for ( Component * component : m_childs )
    {
        childs.push_back( component );
    }
    return childs;
}

std::vector< Component * > Component::get_childs()
{
    std::vector< Component * > childs {};
    for ( Component * component : m_childs )
    {
        childs.push_back( component );
    }
    return childs;
}

void Component::update_all( UpdateContext & context )
{
    this->update_before( context );

    this->update( context );

    for ( DebugWindow * debugWindow : m_debugWindows )
    {
        // TODO : voir si on fait cette vérification ici ou dans la classe
        // if ( ! debugWindow->is_showed() )
        // {
        //     continue;
        // }
        debugWindow->update_debug();
    }

    for ( Component * component : this->get_childs() )
    {
        component->update_all( context );
    }

    this->update_after( context );
}

void Component::render_all( RenderContext & context ) const
{
    if ( m_view != nullptr )
    {
        context.window.setView( *m_view );
    }

    this->render_before( context );

    this->render( context );

    for ( Component const * component : this->get_childs() )
    {
        component->render_all( context );
    }

    this->render_after( context );

    if ( m_view != nullptr )
    {
        context.window.reset_view();
    }
}

void Component::set_view( View const & view )
{
    m_view = &view;
}

void Component::update_before( UpdateContext & /* context */ ) {}

void Component::update( UpdateContext & /* context */ ) {}

void Component::update_after( UpdateContext & /* context */ ) {}

void Component::render_before( RenderContext & /* context */ ) const {}

void Component::render( RenderContext & /* context */ ) const {}

void Component::render_after( RenderContext & /* context */ ) const {}
