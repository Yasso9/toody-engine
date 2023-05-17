#pragma once

#include "component.hpp"

template< cComponent C >
void Component::add_child( C * component )
{
    m_childs.push_back( component );
}

template< cComponent C >
void Component::add_child( C & component )
{
    this->add_child( &component );
}

template< cComponent C >
void Component::add_child( C & component, View const & view )
{
    component.set_view( view );
    this->add_child( component );
}

template< cComponent C >
void Component::add_childs( std::vector< C > & components )
{
    for ( C & component : components )
    {
        this->add_child( component );
    }
}

template< cComponent C >
void Component::remove_child( C * component )
{
    m_childs.erase( std::remove( m_childs.begin(), m_childs.end(), component ),
                    m_childs.end() );
}

template< cComponent C >
void Component::remove_child( C & component )
{
    this->remove_child( &component );
}
