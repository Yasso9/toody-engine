#pragma once

#include "graphics2D/collision/collision_list.hpp"  // for CollisionList
#include "graphics2D/control.hpp"
#include "graphics2D/entity/static_entity.hpp"  // for StaticEntity2D
#include "graphics2D/view.hpp"                  // for View
#include "maths/geometry/polygon.hpp"           // for PolygonF
#include "maths/vector2.hpp"                    // for Vector2F

class ControllableEntity2D : public StaticEntity2D
{
    Control        m_control;
    math::Vector2F m_speed;

  public:
    ControllableEntity2D( math::PolygonF quadrangle, Control m_control );

    math::Vector2F get_speed () const;
    void           set_speed ( float speed );
    void           set_speed ( math::Vector2F speed );

    virtual void update ( UpdateContext & context ) override;
};
