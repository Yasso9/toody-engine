#include "character_entity.hpp"

CharacterEntity::CharacterEntity( sf::Texture const & texture )
  : m_texture { texture }, m_sprite { m_texture }
{}

void CharacterEntity::update( float /* deltaTime */ ) {}

void CharacterEntity::render( Render & /* render */ ) const {}