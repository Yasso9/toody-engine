#include "character_entity.hpp"

#include "graphics2D/control.hpp"

CharacterEntity::CharacterEntity( sf::Texture const & texture, Control control )
  : m_sprite { texture }, m_control { control }, m_speed { 10.f }
{
    this->add_child( m_sprite );
}

void CharacterEntity::update( float deltaTime )
{
    math::Vector2F moveSpeed {
        ( math::Vector2F { m_speed, m_speed }
          / m_control.get_view().get_zoom() ) };

    math::Vector2F movementValue {
        m_control.get_input_movement() * moveSpeed * deltaTime };

    this->move( movementValue );

    /// @todo ne pas revenir complètement, mais revenir à l'endroit juste avant
    /// la collision. Trouver une idée pour avoir cela
    if ( m_control.is_collision_detected( m_sprite.get_polygon() ) )
    {
        this->move( -movementValue );
    }
}

void CharacterEntity::render( Render & /* render */ ) const {}