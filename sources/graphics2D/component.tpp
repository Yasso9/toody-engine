#pragma once

#include "component.hpp"

template < C_Component ComponentClass >
void Component::add_child( ComponentClass & component )
{
    m_childs.push_back( std::make_shared< ComponentClass >( component ) );
}
template < C_Component ComponentClass >
void Component::add_childs( std::vector< ComponentClass > & components )
{
    for ( ComponentClass & component : components )
    {
        this->add_child( component );
    }
}