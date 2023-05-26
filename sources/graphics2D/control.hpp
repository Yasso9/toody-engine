#pragma once

#include "graphics2D/collision/collision_list.hpp"  // for CollisionList
#include "graphics2D/view.hpp"                      // for View
#include "interface/keyboard_move.hpp"
#include "maths/geometry/polygon.hpp"  // for PolygonF
#include "maths/vector2.hpp"           // for Vector2F

class Control
{
    CollisionList const & m_collisionList;
    View const &          m_view;
    KeyboardMove          m_movementKey;

  public:
    Control( CollisionList const & collisionList, View const & view,
             KeyboardMove movementKey );

    CollisionList const & get_collisions () const;
    View const &          get_view () const;
    math::Vector2F get_input_movement ( UpdateContext const & context ) const;

    bool is_collision_detected ( math::PolygonF entityPolygon ) const;
};
