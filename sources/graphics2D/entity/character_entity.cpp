#include "character_entity.hpp"

#include "graphics2D/control.hpp"
#include "libraries/imgui.hpp"

CharacterEntity::CharacterEntity( sf::Texture const & texture, Control control )
  : m_sprite { texture, { 30.f, 30.f } },
    m_control { control },
    m_speed { 100.f },
    m_isWindowShowed { true },
    m_animation { { { "normal",
                      { { E_Direction::Up, { 6u } },
                        { E_Direction::Down, { 0u } },
                        { E_Direction::Right, { 3u } },
                        { E_Direction::Left, { 9u } } } },
                    { "walking",
                      { { E_Direction::Up, { 6u, 7u, 6u, 8u } },
                        { E_Direction::Down, { 0u, 1u, 0u, 2u } },
                        { E_Direction::Right, { 3u, 4u, 3u, 5u } },
                        { E_Direction::Left, { 9u, 10u, 9u, 11u } } } } },
                  "normal",
                  E_Direction::Down }
{
    this->add_child( m_sprite );
    this->add_child( m_animation );
}

void CharacterEntity::update( UpdateContext context )
{
    math::Vector2F movement { this->get_movement( context ) };

    this->move( movement );

    m_animation.set_direction( movement );
    if ( movement == math::Vector2F { 0.f, 0.f } )
    {
        m_animation.set_state( "normal" );
    }
    else
    {
        m_animation.set_state( "walking" );
    }

    m_sprite.select_animation( m_animation.get_current_sprite() );

    bool isCollisionDetected { false };
    /// @todo ne pas revenir complètement, mais revenir à l'endroit juste
    /// avant la collision. Trouver une idée pour avoir cela
    if ( m_control.is_collision_detected( this->get_polygon() ) )
    {
        this->move( -movement );
        isCollisionDetected = true;
    }

    /// @todo What's the use of this ? Try to use it on the render method or add
    /// comments
    this->set_next_transformation( this->getTransform() );

    ImGui::P_Show( "Character", &m_isWindowShowed, [=, *this] () {
        std::ostringstream stream {};
        stream << "Entity Position : " << this->getPosition() << "\n";
        stream << "Sprite Position : " << m_sprite.get_position() << "\n";
        stream << "Size : " << m_sprite.get_size() << "\n";
        stream << "Entity Polygon : " << this->get_polygon() << "\n";
        stream << "Sprite Polygon : " << m_sprite.get_polygon() << "\n";
        stream << "Movement : " << movement << "\n";
        stream << "Current Direction : "
               << to_string( m_animation.get_direction() ) << "\n";
        stream << "Current State : " << m_animation.get_state() << "\n";
        stream << "Current Sprite : " << m_animation.get_current_sprite()
               << "\n";
        stream << "Number of collisions : "
               << m_control.get_collisions().get_entities().size() << "\n";
        stream << "Is Collision Detected : " << std::boolalpha
               << isCollisionDetected << "\n";
        ImGui_P::Text( stream );
    } );
}

math::PolygonF CharacterEntity::get_polygon() const
{
    return m_sprite.get_polygon() + this->getPosition();
}

math::Vector2F CharacterEntity::get_movement( UpdateContext context ) const
{
    math::Vector2F const moveSpeed {
        ( math::Vector2F { m_speed, m_speed }
          / m_control.get_view().get_zoom( context.window ) ) };

    math::Vector2F const movementValue {
        m_control.get_input_movement( context.window ) * moveSpeed
        * context.deltaTime };

    return movementValue.get_norm();
}