#include "component.hpp"

#include "main/render.hpp"  // for Render

Component::Component( Component const & component ) noexcept
  : m_childs { component.m_childs }, m_view { component.m_view }
{}

Component::Component( Component && component ) noexcept
  : m_childs( std::exchange( component.m_childs, {} ) ),
    m_view( std::exchange( component.m_view, {} ) )
{}

Component & Component::operator= ( Component const & component ) noexcept
{
    return *this = Component { component };
}

Component & Component::operator= ( Component && component ) noexcept
{
    std::swap( m_childs, component.m_childs );
    std::swap( m_view, component.m_view );
    return *this;
}

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
    if ( m_view != nullptr )
    {
        render.set_view( *m_view );
    }

    this->render_before( render );

    for ( Component const * component : m_childs )
    {
        component->render_all( render );
    }

    this->render_after( render );

    if ( m_view != nullptr )
    {
        render.reset_view();
    }
}

void Component::set_view( View const & view )
{
    m_view = &view;
}