#pragma once

#include "graphics2D/collision_list.hpp"        // for CollisionList
#include "graphics2D/entity/static_entity.hpp"  // for StaticEntity2D
#include "graphics2D/view.hpp"                  // for View
#include "input/input.hpp"                      // for S_KeyboardMove
#include "maths/geometry/polygon.hpp"           // for PolygonF
#include "maths/vector2.hpp"                    // for Vector2F

class MoveableEntity2D : public StaticEntity2D
{
    CollisionList const & m_collisionList;
    View const &          m_view;
    input::S_KeyboardMove m_movementKey;
    math::Vector2F        m_speed;

  public:
    MoveableEntity2D(
        math::PolygonF quadrangle, CollisionList const & collisionList,
        View const & view, input::S_KeyboardMove movementKey );

    math::Vector2F get_speed () const;
    void           set_speed ( float speed );
    void           set_speed ( math::Vector2F speed );

    bool is_collision_detected () const;

    virtual void update_before ( float deltaTime ) override;
};