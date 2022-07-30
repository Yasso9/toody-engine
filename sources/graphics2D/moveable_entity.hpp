#pragma once

#include "collision2D/collision_list.hpp"
#include "graphics2D/static_entity.hpp"

class MoveableEntity2D : public StaticEntity2D
{
    CollisionList const & m_collisionList;
    View const & m_view;
    input::S_KeyboardMove m_movementKey;
    math::Vector2F m_speed;

  public:
    MoveableEntity2D( math::PolygonF quadrangle,
                      CollisionList const & collisionList, View const & view,
                      input::S_KeyboardMove movementKey );

    math::Vector2F get_speed() const;
    void set_speed( float speed );
    void set_speed( math::Vector2F speed );

    bool is_collision_detected() const;

    virtual void update_before( float deltaTime ) override;
};