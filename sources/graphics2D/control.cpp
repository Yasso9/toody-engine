#include "control.hpp"

Control::Control( CollisionList const & collisionList, View const & view,
                  KeyboardMove movementKey )
  : m_collisionList { collisionList },
    m_view { view },
    m_movementKey { movementKey }
{}

CollisionList const & Control::get_collisions() const
{
    return m_collisionList;
}

View const & Control::get_view() const
{
    return m_view;
}

math::Vector2F Control::get_input_movement(
    UpdateContext const & context ) const
{
    return context.inputs.get_movement( m_movementKey );
}

bool Control::is_collision_detected( math::PolygonF entityPolygon ) const
{
    for ( StaticEntity2D const & entity : m_collisionList.get_entities() )
    {
        if ( math::is_intersection( entityPolygon, entity.get_polygon() ) )
        {
            return true;
        }
    }
    return false;
}
