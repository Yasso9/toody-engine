#include "entity.hpp"

#include "tools/string.hpp"

Entity::Entity( sf::Texture const & texture )
  : m_texture( texture ), m_name( "Unkown"s ), m_speed( 30.f )
{
    this->m_sprite.setTexture( this->m_texture );
}

void Entity::draw( sf::RenderTarget & target, sf::RenderStates states ) const
{
    states.transform *= this->getTransform();

    target.draw( this->m_sprite, states );
}