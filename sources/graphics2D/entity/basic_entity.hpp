#pragma once

#include "graphics2D/collision/collision_list.hpp"  // for CollisionList
#include "graphics2D/entity/static_entity.hpp"      // for StaticEntity2D
#include "graphics2D/view.hpp"                      // for View
#include "maths/geometry/polygon.hpp"               // for PolygonF
#include "maths/vector2.hpp"                        // for Vector2F
#include "states/input.hpp"                         // for S_KeyboardMove

class BasicEntity2D : public StaticEntity2D
{
    View const &   m_view;
    math::Vector2F m_speed;

  public:
    BasicEntity2D( math::PolygonF quadrangle, View const & view );

    math::Vector2F get_speed () const;
    void           set_speed ( float speed );
    void           set_speed ( math::Vector2F speed );

    virtual void update ( UpdateContext context ) override;
};