#pragma once

#include "component.hpp"

template < C_IsComponent2D ComponentClass >
void BaseComponent::add_child( ComponentClass & component )
{
    m_childs2D.push_back( std::make_shared< ComponentClass >( component ) );
}
template < C_IsComponent3D ComponentClass >
void BaseComponent::add_child( ComponentClass & component )
{
    m_childs3D.push_back( std::make_shared< ComponentClass >( component ) );
}

template < C_IsComponent ComponentClass >
void BaseComponent::add_childs( std::vector< ComponentClass > & components )
{
    for ( ComponentClass & component : components )
    {
        this->add_child( component );
    }
}