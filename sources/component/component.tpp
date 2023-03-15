#pragma once

#include "component.hpp"

template< C_IsComponent ComponentClass >
void Component::add_child( ComponentClass * component )
{
    m_childs.push_back( component );
}

template< C_IsComponent ComponentClass >
void Component::add_child( ComponentClass & component )
{
    this->add_child( &component );
}

template< C_IsComponent ComponentClass >
void Component::add_child( ComponentClass & component, View const & view )
{
    component.set_view( view );
    this->add_child( component );
}

template< C_IsComponent ComponentClass >
void Component::add_childs( std::vector< ComponentClass > & components )
{
    for ( ComponentClass & component : components )
    {
        this->add_child( component );
    }
}

template< C_IsComponent ComponentClass >
void Component::remove_child( ComponentClass * component )
{
    // 
    m_childs.erase( std::remove( m_childs.begin(), m_childs.end(), component ),
                    m_childs.end() );
}

template< C_IsComponent ComponentClass >
void Component::remove_child( ComponentClass & component )
{
    this->remove_child( &component );
}
